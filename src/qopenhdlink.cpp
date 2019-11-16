#include "qopenhdlink.h"

#include <QtNetwork>
#include <QThread>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QFuture>

#include "constants.h"

#if defined(ENABLE_LINK)
#include <lib/json.hpp>
#endif

#include <qmdnsengine/dns.h>
#include <qmdnsengine/query.h>
#include <qmdnsengine/service.h>


#define LINK_PORT 6000

#define SERVICE_TYPE "_openhdlink._udp.local."

QOpenHDLink::QOpenHDLink(QObject *parent):
    QObject(parent),
    mHostname(&mServer),
    mProvider(&mServer, &mHostname, this),
    mBrowser(&mServer, SERVICE_TYPE, &cache),
    mResolver(nullptr)
    {
    qDebug() << "QOpenHDLink::QOpenHDLink()";

#if defined(__rasp_pi__)
    connect(&mHostname, &QMdnsEngine::Hostname::hostnameChanged, this, &QOpenHDLink::mdnsHostnameChanged);
    connect(&mServer, &QMdnsEngine::Server::messageReceived, this, &QOpenHDLink::mdnsMessageReceived);
#else
    connect(&mBrowser, &QMdnsEngine::Browser::serviceAdded, this, &QOpenHDLink::onServiceAdded);
    connect(&mBrowser, &QMdnsEngine::Browser::serviceUpdated, this, &QOpenHDLink::onServiceUpdated);
    connect(&mBrowser, &QMdnsEngine::Browser::serviceRemoved, this, &QOpenHDLink::onServiceRemoved);
#endif
    init();
}


void QOpenHDLink::mdnsHostnameChanged(const QByteArray &hostname) {
    auto s = hostname.toStdString();
    qDebug() << "QOpenHDLink::mdnsHostnameChanged(" << QString(s.c_str()) << ")";
}


void QOpenHDLink::mdnsMessageReceived(const QMdnsEngine::Message &message) {

}


int QOpenHDLink::findService(const QByteArray &name) {
    for (auto i = mServices.constBegin(); i != mServices.constEnd(); ++i) {
        if ((*i).name() == name) {
            return i - mServices.constBegin();
        }
    }
    return -1;
}


void QOpenHDLink::onServiceAdded(const QMdnsEngine::Service &service) {
    mServices.append(service);

    mResolver = new QMdnsEngine::Resolver(&mServer, service.hostname(), nullptr, this);
    connect(mResolver, &QMdnsEngine::Resolver::resolved, [this](const QHostAddress &address) {
        auto protocol = address.protocol();
        switch (protocol) {
            case QAbstractSocket::NetworkLayerProtocol::IPv4Protocol:
            if (peer != nullptr) {
                delete peer;
            }
            peer = new QHostAddress(address);
            m_link_peer = address.toString();
            emit link_peer_changed(m_link_peer);
            handshake();
            break;
            case QAbstractSocket::NetworkLayerProtocol::IPv6Protocol:
            break;
            default:
            break;
        }

        qDebug() << "Found service address: " << address.toString();
    });
}

void QOpenHDLink::onServiceUpdated(const QMdnsEngine::Service &service) {
    int i = findService(service.name());
    if (i != -1) {
        mServices.replace(i, service);
        //emit dataChanged(index(i), index(i));
    }
}


void QOpenHDLink::onServiceRemoved(const QMdnsEngine::Service &service) {
    int i = findService(service.name());
    if (i != -1) {
        mServices.removeAt(i);
    }
    m_link_peer = tr("none");
    emit link_peer_changed(m_link_peer);
}


void QOpenHDLink::init() {
#if defined(ENABLE_LINK)
    qDebug() << "QOpenHDLink::init()";

    m_link_peer = tr("none");
    emit link_peer_changed(m_link_peer);

    linkSocket = new QUdpSocket(this);
    linkSocket->bind(LINK_PORT);

    connect(linkSocket, &QUdpSocket::readyRead, this, &QOpenHDLink::readyRead);

    auto timer = new QTimer(this);

#if defined(__rasp_pi__)
    service.setName("QOpenHD");
    service.setType(SERVICE_TYPE);
    service.setPort(LINK_PORT);

    mProvider.update(service);

    QObject::connect(timer, &QTimer::timeout, this, &QOpenHDLink::updateService);
#else
    QObject::connect(timer, &QTimer::timeout, this, &QOpenHDLink::handshake);
#endif

    timer->start(3000);
#endif
}


void QOpenHDLink::setGroundIP(QString address) {
    groundAddress = address;
}


void QOpenHDLink::updateService() {
    mProvider.update(service);
}


void QOpenHDLink::readyRead() {
#if defined(ENABLE_LINK)
    QByteArray datagram;

    while (linkSocket->hasPendingDatagrams()) {
        datagram.resize(int(linkSocket->pendingDatagramSize()));

        linkSocket->readDatagram(datagram.data(), datagram.size(), peer);
        processCommand(datagram);
    }
#endif
}


/*
 * Commands
 *
 *
 */

void QOpenHDLink::handshake() {
    if (peer == nullptr) {
        return;
    }

    // this is going to be wrong but we aren't using the address for anything on the other
    // side so it doesn't matter, it's really just to ensure some real piece of information was
    // transferred, indicating that the link works
    QString localAddress;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (int i = 0; i < list.count(); i++) {
        if (!list[i].isLoopback()) {
            if (list[i].protocol() == QAbstractSocket::IPv4Protocol) {
                localAddress = list[i].toString();
            }
        }
    }

    nlohmann::json j = {
      {"cmd", "handshake"},
      {"address", localAddress.toStdString()}
    };

    std::string serialized_string = j.dump();
    auto buf = QByteArray(serialized_string.c_str());
    if (linkSocket->state() != QUdpSocket::ConnectedState) {
        linkSocket->connectToHost(*peer, LINK_PORT);
    }
    linkSocket->writeDatagram(buf);
}


void QOpenHDLink::setWidgetLocation(QString widgetName, int alignment, int xOffset, int yOffset, bool hCenter, bool vCenter) {
#if defined(ENABLE_LINK)
#if !defined(__rasp_pi__)

    if (peer == nullptr) {
        return;
    }

    nlohmann::json j = {
      {"cmd", "setWidgetLocation"},
      {"widgetName", widgetName.toStdString()},
      {"alignment", alignment},
      {"xOffset", xOffset},
      {"yOffset", yOffset},
      {"hCenter", hCenter},
      {"vCenter", vCenter}
    };

    std::string serialized_string = j.dump();
    auto buf = QByteArray(serialized_string.c_str());
    linkSocket->writeDatagram(buf, QHostAddress(groundAddress), LINK_PORT);
#endif
#endif
}


void QOpenHDLink::setWidgetEnabled(QString widgetName, bool enabled) {
#if defined(ENABLE_LINK)
    nlohmann::json j = {
      {"cmd", "setWidgetEnabled"},
      {"widgetName", widgetName.toStdString()},
      {"enabled", enabled}
    };

    std::string serialized_string = j.dump();
    auto buf = QByteArray(serialized_string.c_str());
    linkSocket->writeDatagram(buf, QHostAddress(groundAddress), LINK_PORT);
#endif
}


void QOpenHDLink::processCommand(QByteArray buffer) {
#if defined(ENABLE_LINK)
    try {
        auto commandData = nlohmann::json::parse(buffer);
        if (commandData.count("cmd") == 1) {
            std::string cmd = commandData["cmd"];

            if (cmd == "setWidgetLocation") {
                processSetWidgetLocation(commandData);
            } else if (cmd == "handshake") {
                processHandshake(commandData);
            }

            if (cmd == "setWidgetEnabled") {
                processSetWidgetEnabled(commandData);
            }
        }
    } catch (std::exception &e) {
        // not much we can do about it but we definitely don't want a crash here,
        // we may consider show warning messages in the local message panel though
        qDebug() << "exception: " << e.what();
    }
#endif
}


void QOpenHDLink::processSetWidgetLocation(nlohmann::json commandData) {
#if defined(ENABLE_LINK)
    std::string widgetName = commandData["widgetName"];

    int alignment = commandData["alignment"];
    int xOffset = commandData["xOffset"];
    int yOffset = commandData["yOffset"];
    bool hCenter = commandData["hCenter"];
    bool vCenter = commandData["vCenter"];

    emit widgetLocation(QString(widgetName.c_str()), alignment, xOffset, yOffset, hCenter, vCenter);
#endif
}

void QOpenHDLink::processSetWidgetEnabled(nlohmann::json commandData) {
#if defined(ENABLE_LINK)
    std::string widgetName = commandData["widgetName"];

    bool enabled = commandData["enabled"];

    emit widgetEnabled(QString(widgetName.c_str()), enabled);
#endif
}


void QOpenHDLink::processHandshake(nlohmann::json commandData) {
    std::string peerAddress = commandData["address"];

    peer = new QHostAddress(peerAddress.c_str());
    m_link_peer = peer->toString();
    emit link_peer_changed(m_link_peer);
}
