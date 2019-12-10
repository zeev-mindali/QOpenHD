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

#define LINK_PORT 6000


QOpenHDLink::QOpenHDLink(QObject *parent):
    QObject(parent)
    {
    qDebug() << "QOpenHDLink::QOpenHDLink()";
    groundAddress = "192.168.40.53";

    init();
}



void QOpenHDLink::init() {
#if defined(ENABLE_LINK)
    qDebug() << "QOpenHDLink::init()";

    linkSocket = new QUdpSocket(this);
    linkSocket->bind(LINK_PORT);

    connect(linkSocket, &QUdpSocket::readyRead, this, &QOpenHDLink::readyRead);
#endif
}


void QOpenHDLink::setGroundIP(QString address) {
    groundAddress = address;
}


VMap QOpenHDLink::getAllSyncedSettings() {
    return m_allSettings;
}


void QOpenHDLink::readyRead() {
#if defined(ENABLE_LINK)
    QByteArray datagram;

    while (linkSocket->hasPendingDatagrams()) {
        datagram.resize(int(linkSocket->pendingDatagramSize()));

        linkSocket->readDatagram(datagram.data(), datagram.size());
        processCommand(datagram);
    }
#endif
}


void QOpenHDLink::setWidgetLocation(QString widgetName, int alignment, int xOffset, int yOffset, bool hCenter, bool vCenter) {
#if defined(ENABLE_LINK)
#if !defined(__rasp_pi__)
    qDebug() << "QOpenHDLink::setWidgetLocation";

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

void QOpenHDLink::setSettingBool(QString setting, bool value) {
#if defined(ENABLE_LINK)
    qDebug() << "QOpenHDLink::setSettingBool";

    nlohmann::json j = {
      {"cmd", "setSettingBool"},
      {"setting", setting.toStdString()},
      {"value", value}
    };

    std::string serialized_string = j.dump();
    auto buf = QByteArray(serialized_string.c_str());
    linkSocket->writeDatagram(buf, QHostAddress(groundAddress), LINK_PORT);
#endif
}

void QOpenHDLink::setSettingNumber(QString setting, qint32 value) {
#if defined(ENABLE_LINK)
    qDebug() << "QOpenHDLink::setSettingNumber";

    nlohmann::json j = {
      {"cmd", "setSettingNumber"},
      {"setting", setting.toStdString()},
      {"value", value}
    };

    std::string serialized_string = j.dump();
    auto buf = QByteArray(serialized_string.c_str());
    linkSocket->writeDatagram(buf, QHostAddress(groundAddress), LINK_PORT);
#endif
}

void QOpenHDLink::processCommand(QByteArray buffer) {
#if defined(ENABLE_LINK)
    qDebug() << "QOpenHDLink::processCommand";

    try {
        auto commandData = nlohmann::json::parse(buffer);
        if (commandData.count("cmd") == 1) {
            std::string cmd = commandData["cmd"];

            if (cmd == "setWidgetLocation") {
                processSetWidgetLocation(commandData);
            }

            if (cmd == "setSettingBool") {
                processSetSettingBool(commandData);
            }

            if (cmd == "setSettingNumber") {
                processSetSettingNumber(commandData);
            }

            if (cmd == "syncSettings") {
                processSyncSettings(commandData);
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
    qDebug() << "QOpenHDLink::processSetWidgetLocation";

    std::string widgetName = commandData["widgetName"];

    int alignment = commandData["alignment"];
    int xOffset = commandData["xOffset"];
    int yOffset = commandData["yOffset"];
    bool hCenter = commandData["hCenter"];
    bool vCenter = commandData["vCenter"];

    emit widgetLocation(QString(widgetName.c_str()), alignment, xOffset, yOffset, hCenter, vCenter);
#endif
}

void QOpenHDLink::processSetSettingBool(nlohmann::json commandData) {
#if defined(ENABLE_LINK)
    qDebug() << "QOpenHDLink::processSetSettingBool";

    std::string setting = commandData["setting"];

    bool value = commandData["value"];

    emit settingChangedBool(QString(setting.c_str()), value);
#endif
}

void QOpenHDLink::processSetSettingNumber(nlohmann::json commandData) {
#if defined(ENABLE_LINK)
    qDebug() << "QOpenHDLink::processSetSettingNumber";

    std::string setting = commandData["setting"];

    qint32 value = commandData["value"];

    emit settingChangedNumber(QString(setting.c_str()), value);
#endif
}

void QOpenHDLink::syncSettings() {
    qDebug() << "QOpenHDLink::syncSettings()";

    // run the real network calls in the background. needs some minor changes to avoid threading related
    // errors
    //QFuture<void> future = QtConcurrent::run(this, &OpenHDSettings::_saveSettings, remoteSettings);
    _syncSettings();
}

void QOpenHDLink::_syncSettings() {
    qDebug() << "QOpenHDLink::_syncSettings";

    nlohmann::json sync = {
      {"cmd", "syncSettings"}
    };
    QSettings settings;

    // main settings
    sync["battery_cells"] = settings.value("battery_cells", 3).toInt();
    sync["enable_imperial"] = settings.value("enable_imperial", false).toBool();

    // never sync these
    //sync["enable_software_video_decoder"] = settings.value("enable_software_video_decoder", false).toBool();
    //sync["enable_speech"] = settings.value("enable_speech", false).toBool();
    //sync["enable_rc"] = settings.value("enable_rc", false).toBool();

    // widgets
    sync["show_pip_video"] = settings.value("show_pip_video", false).toBool();
    sync["show_downlink_rssi"] = settings.value("show_downlink_rssi", true).toBool();
    sync["show_uplink_rssi"] = settings.value("show_uplink_rssi", true).toBool();
    sync["show_bitrate"] = settings.value("show_bitrate", true).toBool();
    sync["show_air_battery"] = settings.value("show_air_battery", true).toBool();
    sync["show_gps"] = settings.value("show_gps", true).toBool();
    sync["show_home_distance"] = settings.value("show_home_distance", true).toBool();
    sync["show_flight_timer"] = settings.value("show_flight_timer", true).toBool();
    sync["show_flight_mode"] = settings.value("show_flight_mode", true).toBool();
    sync["show_ground_status"] = settings.value("show_ground_status", true).toBool();
    sync["show_air_status"] = settings.value("show_air_status", true).toBool();
    sync["show_message_hud"] = settings.value("show_message_hud", true).toBool();
    sync["show_horizon"] = settings.value("show_horizon", true).toBool();
    sync["show_fpv"] = settings.value("show_fpv", true).toBool();
    sync["show_altitude"] = settings.value("show_altitude", true).toBool();
    sync["show_speed"] = settings.value("show_speed", true).toBool();
    sync["show_heading"] = settings.value("show_heading", true).toBool();
    sync["show_altitude_second"] = settings.value("show_altitude_second", true).toBool();
    sync["show_arrow"] = settings.value("show_arrow", true).toBool();
    sync["show_map"] = settings.value("show_map", true).toBool();
    sync["show_throttle"] = settings.value("show_throttle", true).toBool();


    std::string serialized_string = sync.dump();
    auto buf = QByteArray(serialized_string.c_str());
    linkSocket->writeDatagram(buf, QHostAddress(groundAddress), LINK_PORT);
}

void QOpenHDLink::processSyncSettings(nlohmann::json commandData) {
    qDebug() << "QOpenHDLink::processSyncSettings";

    VMap settingsModel;

    for (auto& el : commandData.items()) {
        if (el.key() == "cmd") continue;
        std::string key = el.key();
        auto value = el.value();

        if (value.is_null()) continue;

        if (value.is_boolean()) {
            auto _value = el.value().get<bool>();
            settingsModel[QString(key.c_str())] = QVariant(_value);
        }

        if (value.is_number()) {
            auto _value = el.value().get<int>();
            settingsModel[QString(key.c_str())] = QVariant(_value);
        }
    }
    m_allSettings = settingsModel;
    emit allSettingsChanged(m_allSettings);
}

