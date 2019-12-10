#ifndef QOpenHDLink_H
#define QOpenHDLink_H

#include <QObject>
#include <QtQuick>

#include "constants.h"
#include <lib/json.hpp>

#include "util.h"

class QUdpSocket;


class QOpenHDLink: public QObject {
    Q_OBJECT

public:
    explicit QOpenHDLink(QObject *parent = nullptr);

    Q_PROPERTY(VMap allSettings MEMBER m_allSettings NOTIFY allSettingsChanged)

    Q_INVOKABLE void setWidgetLocation(QString widgetName, int alignment, int xOffset, int yOffset, bool hCenter, bool vCenter);

    Q_INVOKABLE void setGroundIP(QString address);


    Q_INVOKABLE void setSettingBool(QString setting, bool value);
    Q_INVOKABLE void setSettingNumber(QString setting, qint32 value);

    Q_INVOKABLE void syncSettings();

    Q_INVOKABLE VMap getAllSyncedSettings();

signals:
    void widgetLocation(QString widgetName, int alignment, int xOffset, int yOffset, bool hCenter, bool vCenter);

    void settingChangedBool(QString setting, bool value);
    void settingChangedNumber(QString setting, qint32 value);

    void allSettingsChanged(VMap allSettings);

private slots:
    void readyRead();

private:
    void init();
    void _syncSettings();

    QString groundAddress = "192.168.2.1";

    void processCommand(QByteArray buffer);
    void processSetWidgetLocation(nlohmann::json command);
    void processSetSettingBool(nlohmann::json commandData);
    void processSetSettingNumber(nlohmann::json commandData);

    void processSyncSettings(nlohmann::json commandData);

    QUdpSocket *linkSocket = nullptr;

    VMap m_allSettings;
};

#endif
