#ifndef QOpenHDLink_H
#define QOpenHDLink_H

#include <QObject>
#include <QtQuick>

#include "constants.h"
#include <lib/json.hpp>


#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

#include <qmdnsengine/hostname.h>
#include <qmdnsengine/message.h>
#include <qmdnsengine/provider.h>
#include <qmdnsengine/browser.h>
#include <qmdnsengine/cache.h>
#include <qmdnsengine/resolver.h>

class QUdpSocket;

class QOpenHDLink: public QObject {
    Q_OBJECT

public:
    explicit QOpenHDLink(QObject *parent = nullptr);

    Q_INVOKABLE void setWidgetLocation(QString widgetName, int alignment, int xOffset, int yOffset, bool hCenter, bool vCenter);
    Q_INVOKABLE void setWidgetEnabled(QString widgetName, bool enabled);

    Q_INVOKABLE void setGroundIP(QString address);


    Q_INVOKABLE void handshake();

    Q_PROPERTY(QString link_peer MEMBER m_link_peer NOTIFY link_peer_changed)

signals:
    void widgetLocation(QString widgetName, int alignment, int xOffset, int yOffset, bool hCenter, bool vCenter);
    void widgetEnabled(QString widgetName, bool enabled);

    void link_peer_changed(QString link_peer);

private slots:
    void readyRead();

    void mdnsHostnameChanged(const QByteArray &hostname);
    void mdnsMessageReceived(const QMdnsEngine::Message &message);


    void onServiceAdded(const QMdnsEngine::Service &service);
    void onServiceUpdated(const QMdnsEngine::Service &service);
    void onServiceRemoved(const QMdnsEngine::Service &service);

    void updateService();

private:
    void init();
    int findService(const QByteArray &name);

    QString groundAddress = "192.168.2.1";

    void processCommand(QByteArray buffer);
    void processSetWidgetLocation(nlohmann::json command);
    void processSetWidgetEnabled(nlohmann::json commandData);

    void processHandshake(nlohmann::json commandData);

    QMdnsEngine::Server mServer;
    QMdnsEngine::Hostname mHostname;
    QMdnsEngine::Provider mProvider;
    QMdnsEngine::Service service;

    QMdnsEngine::Cache cache;
    QMdnsEngine::Browser mBrowser;
    QList<QMdnsEngine::Service> mServices;
    QMdnsEngine::Resolver *mResolver;

    QUdpSocket *linkSocket = nullptr;

    QHostAddress *peer = nullptr;

    QString m_link_peer;
};

#endif
