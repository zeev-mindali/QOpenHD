import QtQuick 2.12

/*
 * These are mappings for the raw parameter key/value pairs from the drone. We
 * give certain parameters full readable titles, type information, and limits or
 * ranges in order to make them more visible and easier to deal with. We also allow
 * specific settings to place constraints on other settings so that conflicting or
 * invalid configurations can be prevented.
 *
 */
Item {
    id: ardupilotParameterMap

    property var generalParameterMap: ({

    })

    property var serialBaudRates: [
        {title: "1200", value: 1},
        {title: "2400", value: 2},
        {title: "4800", value: 4},
        {title: "9600", value: 9},
        {title: "19200", value: 19},
        {title: "38400", value: 38},
        {title: "57600", value: 57},
        {title: "111,100", value: 111},
        {title: "115200", value: 115},
        {title: "500,000", value: 500},
        {title: "921,600", value: 921},
        {title: "1,500,000", value: 1500},
    ];

    property var serialPortTypes: [
        {title: "None", value: -1},
        {title: "Mavlink1", value: 1},
        {title: "Mavlink2", value: 2},
        {title: "FrSky", value: 3},
        {title: "FrSky SPort", value: 4},
        {title: "GPS", value: 5},
        {title: "Alexmos Gimbal Serial", value: 7},
        {title: "SToRM32 Gimbal Serial", value: 8},
        {title: "Rangefinder", value: 9},
        {title: "FrSky SPort Passthrough (OpenTX)", value: 10},
        {title: "Lidar360", value: 11},
        {title: "Beacon", value: 13},
        {title: "Volz Servo Out", value: 14},
        {title: "SBus Servo Out", value: 15},
        {title: "ESC Telemetry", value: 16},
        {title: "Devo Telemetry", value: 17},
        {title: "Optical Flow", value: 18},
    ];

    property var portParameterMap: ({
        "SERIAL0_BAUD": {title: "Serial 0 Baud Rate",
                         info: "Baud rate for serial port 0",
                         itemType: "choice",
                         choiceValues: serialBaudRates},
        "SERIAL0_PROTOCOL": {title: "Serial 0 Protocol",
                         info: "Protocol for serial port 0",
                         itemType: "choice",
                         choiceValues: serialPortTypes},

        "SERIAL1_BAUD": {title: "Serial 1 Baud Rate",
                         info: "Baud rate for serial port 1",
                         itemType: "choice",
                         choiceValues: serialBaudRates},
        "SERIAL1_PROTOCOL": {title: "Serial 1 Protocol",
                         info: "Protocol for serial port 1",
                         itemType: "choice",
                         choiceValues: serialPortTypes},

        "SERIAL2_BAUD": {title: "Serial 2 Baud Rate",
                         info: "Baud rate for serial port 2",
                         itemType: "choice",
                         choiceValues: serialBaudRates},
        "SERIAL2_PROTOCOL": {title: "Serial 2 Protocol",
                         info: "Protocol for serial port 2",
                         itemType: "choice",
                         choiceValues: serialPortTypes},

        "SERIAL3_BAUD": {title: "Serial 3 Baud Rate",
                         info: "Baud rate for serial port 3",
                         itemType: "choice",
                         choiceValues: serialBaudRates},
        "SERIAL3_PROTOCOL": {title: "Serial 3 Protocol",
                         info: "Protocol for serial port 3",
                         itemType: "choice",
                         choiceValues: serialPortTypes},

        "SERIAL4_BAUD": {title: "Serial 4 Baud Rate",
                         info: "Baud rate for serial port 4",
                         itemType: "choice",
                         choiceValues: serialBaudRates},
        "SERIAL4_PROTOCOL": {title: "Serial 4 Protocol",
                         info: "Protocol for serial port 4",
                         itemType: "choice",
                         choiceValues: serialPortTypes},

        "SERIAL5_BAUD": {title: "Serial 5 Baud Rate",
                         info: "Baud rate for serial port 5",
                         itemType: "choice",
                         choiceValues: serialBaudRates},
        "SERIAL5_PROTOCOL": {title: "Serial 5 Protocol",
                         info: "Protocol for serial port 5",
                         itemType: "choice",
                         choiceValues: serialPortTypes},

        "SERIAL6_BAUD": {title: "Serial 6 Baud Rate",
                         info: "Baud rate for serial port 6",
                         itemType: "choice",
                         choiceValues: serialBaudRates},
        "SERIAL6_PROTOCOL": {title: "Serial 6 Protocol",
                         info: "Protocol for serial port 6",
                         itemType: "choice",
                         choiceValues: serialPortTypes},

    })

    property var rcParameterMap: ({

    })

    // these parameters wont show up at all
    property var blacklistMap: ({

    })

    // these parameters will simply be disabled and uneditable in the UI
    property var disabledMap: ({

    })
}
