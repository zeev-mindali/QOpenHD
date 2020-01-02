import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Qt.labs.settings 1.0

import OpenHD 1.0

DroneSettingsPanelForm {

    ListModel {
        dynamicRoles: true
        id: generalParametersModel
    }

    ListModel {
        dynamicRoles: true
        id: rcParametersModel
    }

    ListModel {
        dynamicRoles: true
        id: portParametersModel
    }

    ListModel {
        dynamicRoles: true
        id: otherParametersModel
    }

    ArdupilotParameterMap {
        id: ardupilotParameterMap
    }

    Connections {
        target: mavlinkTelemetry

        onAllParametersChanged: {
            localMessage("Received drone parameters", 2);

             /*
             * Clear the local ListModels for each tab, the ListView in each tab uses these to decide
             * what to draw.
             *
             */
            generalParametersModel.clear();
            rcParametersModel.clear();
            portParametersModel.clear();
            otherParametersModel.clear();

             /*
             * Helper to retrieve and map parameters to a normal representation for display.
             *
             *
             * When the parameters are saved again, we map these values back to the representation
             * expected by the drone.
             *
             */
            function _process(parameter, initialValue, model, mapping, disabled) {
                var itemTitle = mapping[parameter]["title"];
                var itemInfo = mapping[parameter]["info"];
                if (itemInfo === undefined) {
                    itemInfo = "N/A";
                }
                var itemType  = mapping[parameter]["itemType"];

                // not all of these are used for each setting, they don't need to be defined in the
                // mapping if they aren't needed as the QML component will simply not attempt to use them
                var trueValue    = mapping[parameter]["trueValue"];
                var falseValue   = mapping[parameter]["falseValue"];
                var choiceValues = mapping[parameter]["choiceValues"];
                var lowerLimit   = mapping[parameter]["lowerLimit"];
                var upperLimit   = mapping[parameter]["upperLimit"];
                var interval     = mapping[parameter]["interval"];
                var unit         = mapping[parameter]["unit"];

                var finalValue;

                // these all need to be mapped because the values coming from the C++ side
                // are of type 'QVariant', not actual types that can be worked with
                if (itemType === "bool") {
                    finalValue = (initialValue == trueValue) ? true : false;
                } else if (itemType === "choice") {
                    finalValue = String(initialValue);
                } else if (itemType === "range") {
                    finalValue = Number(initialValue);
                } else if (itemType === "number") {
                    finalValue = Number(initialValue);
                } else if (itemType === "string") {
                    finalValue = String(initialValue);
                } else {
                    finalValue = initialValue;
                }

                //configureWithSetting(setting, finalValue);

                model.append({"title": itemTitle,
                              "setting": parameter,
                              "choiceValues": choiceValues,
                              "lowerLimit": lowerLimit,
                              "upperLimit": upperLimit,
                              "interval": interval,
                              "itemType": itemType,
                              "value": finalValue,
                              "unit": unit,
                              "modified": false,
                              "disabled": disabled,
                              "info": itemInfo});
            }

            /*
             * Process all of the ground station settings received over UDP, which happens on the C++
             * side (in openhdsettings.cpp)
             *
             */
            var allParameters = mavlinkTelemetry.getAllParameters();

            for (var parameter in allParameters) {
                /*
                 * Here we distribute the incoming parameter key/value pairs to the ListModel for each tab.
                 *
                 * If a particular parameter is found in one of the mappings, we give it special treatment
                 * by adding a nicer title, handling any value mapping that may be required, and place it
                 * in a particular tab on the settings panel for organization and ease of use.
                 *
                 */

                if (ardupilotParameterMap.blacklistMap[parameter] !== undefined) {
                    continue;
                }

                var disabled = false;
                if (ardupilotParameterMap.disabledMap[parameter] !== undefined) {
                    disabled = true;
                }

                var initialValue = allParameters[parameter];
                if (ardupilotParameterMap.generalParameterMap[parameter] !== undefined) {
                    _process(parameter, initialValue, generalParametersModel, ardupilotParameterMap.generalParameterMap, disabled);
                } else if (ardupilotParameterMap.rcParameterMap[parameter] !== undefined) {
                    _process(parameter, initialValue, rcParametersModel, ardupilotParameterMap.rcParameterMap, disabled);
                } else if (ardupilotParameterMap.portParameterMap[parameter] !== undefined) {
                    _process(parameter, initialValue, portParametersModel, ardupilotParameterMap.portParameterMap, disabled);
                } else {
                    // setting not found in any mapping so add it to the "other" tab as-is, no processing
                    // of any kind. This guarantees that newly added settings are never missing from the app.
                    otherParametersModel.append({"title": parameter,
                                               "setting": parameter,
                                               "itemType": "string",
                                               "value": String(allParameters[parameter]),
                                               "disabled": disabled,
                                               "info": "No additional information available, check the Open.HD wiki"});
                }
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
