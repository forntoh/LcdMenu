#include <Arduino.h>
#include <ArduinoJson.h>
#include <LcdMenu.h>

const JsonVariant getPair(JsonObject obj, char*& outKey);
const JsonVariant read(JsonVariant obj, char*& outKey, MenuItem*& outMenu);

void setup() {
    Serial.begin(9600);
    while (!Serial) continue;

    Serial.println();
    Serial.println();
    Serial.println();

    // Allocate the JSON document
    //
    // Inside the brackets, 200 is the capacity of the memory pool in bytes.
    // Don't forget to change this value to match your JSON document.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<280> doc;

    char json[] =
        "{\"\":[{\"Settings\":[{\"C\":[\"Selp\",\"Selp\"]},{\"T\":[\"Sl\","
        "\"Sl\"]}]},{\"Wifi\":[{\"C\":[\"Welp\",\"Welp\"]},{\"T\":[\"Wl\","
        "\"Wl\"]},{\"I\":[\"Wan\",\"Han\"]},{\"WoonLight\":[{\"M\":[\"West\","
        "\"kkk\"]},{\"I\":[\"Win\",\"Han\"]}]}]}]}";

    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    JsonVariant arr = doc.as<JsonVariant>();

    char* ansKey;
    JsonVariant ansValue = getPair(arr, ansKey);

    for (uint8_t i = 0; i < ansValue.size(); i++) {
        char* pKey;
        JsonVariant pValue = getPair(ansValue[i], pKey);

        // Serial.print("-> ");
        // Serial.println(p.key);

        uint8_t ending = pValue.size() + 2;

        char* nextKey;
        MenuItem* menu = new MenuItem[ending];
        JsonVariant nextValue = read(pValue, nextKey, menu);

        for (uint8_t j = 0; j < ending; j++) {
            Serial.print(menu[j].getText());
            Serial.print(F(","));
        }
        Serial.println();
        delete[] menu;

        if (strlen(nextKey) != 1) {
            // Serial.print("-> ");
            // Serial.println(a.next.key);

            uint8_t ending1 = nextValue.size() + 2;

            char* key1;
            MenuItem* menu1 = new MenuItem[ending1];
            read(nextValue, key1, menu1);

            for (uint8_t j = 0; j < ending1; j++) {
                Serial.print(menu1[j].getText());
                Serial.print(F(","));
            }
            Serial.println();
            delete[] menu1;
        }
    }
}

const JsonVariant read(JsonVariant value, char*& outKey, MenuItem*& outMenu) {
    uint8_t size = value.size();
    JsonVariant nextValue;

    for (uint8_t i = 0; i < size; i++)
        if (value[i].is<JsonObject>()) {
            nextValue = getPair(value[i], outKey);

            MenuItem headA = ItemHeader(NULL);

            outMenu[0] = headA;

            if (strlen(outKey) != 1)
                outMenu[i + 1] = MenuItem((String)outKey);
            else {
                char* val = nextValue[0];

                MenuItem titA;

                switch (outKey[0]) {
                    case 'C':
                        titA = ItemCommand(val, NULL);
                        break;
                    case 'T':
                        titA = ItemToggle(val, NULL);
                        break;
                    case 'I':
                        titA = ItemInput(val, "", NULL);
                        break;
                    default:
                        titA = MenuItem(val);
                        break;
                }
                outMenu[i + 1] = titA;
            }
        }

    if (outMenu[0].getType() == MENU_ITEM_MAIN_MENU_HEADER ||
        outMenu[0].getType() == MENU_ITEM_SUB_MENU_HEADER)
        outMenu[size + 1] = ItemFooter();

    return nextValue;
}

const JsonVariant getPair(JsonObject obj, char*& outKey) {
    outKey = (char*)obj.begin()->key().c_str();
    return obj.begin()->value();
}

void loop() {}