#include <Arduino.h>
#include <ArduinoJson.h>
#include <LcdMenu.h>

const JsonVariant getPair(JsonObject obj, char*& outKey);
const JsonVariant read(JsonVariant obj, char*& outKey, MenuItem*& outMenu);

void setup() {
    Serial.begin(9600);
    while (!Serial) continue;

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

    char* ansKey;
    JsonVariant ansValue = getPair(doc.as<JsonVariant>(), ansKey);

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
            Serial.print((String)menu[j].getType());
            Serial.print(F(","));
        }
        Serial.println();
        // delete[] menu;

        if (strlen(nextKey) != 1) {
            // Serial.print("-> ");
            // Serial.println(a.next.key);

            uint8_t ending1 = nextValue.size() + 2;

            char* key1;
            MenuItem* menu1 = new MenuItem[ending1];
            read(nextValue, key1, menu1);

            for (uint8_t j = 0; j < ending1; j++) {
                Serial.print(menu1[j].getText());
                Serial.print((String)menu1[j].getType());
                Serial.print(F(","));
            }
            Serial.println();
            // delete[] menu1;
        }
    }
}

MenuItem getMenuItem(char key, JsonVariant json) {
    char* val = json[0];
    MenuItem item;
    switch (key) {
        case 'C':
            item = ItemCommand(val, NULL);
            break;
        case 'T':
            item = ItemToggle(val, NULL);
            break;
        case 'I':
            item = ItemInput(val, "", NULL);
            break;
        default:
            item = MenuItem(val);
            break;
    }
    return item;
}

const JsonVariant read(JsonVariant value, char*& outKey, MenuItem*& outMenu) {
    uint8_t size = value.size();
    JsonVariant nextValue;

    for (uint8_t i = 0; i < size; i++)
        if (value[i].is<JsonObject>()) {
            nextValue = getPair(value[i], outKey);

            if (strlen(outKey) != 1)
                outMenu[i + 1] = ItemSubMenu(outKey, NULL);
            else
                outMenu[i + 1] = getMenuItem(outKey[0], nextValue);
        }

    outMenu[0] = ItemHeader(NULL);
    outMenu[size + 1] = ItemFooter();
    return nextValue;
}

const JsonVariant getPair(JsonObject obj, char*& outKey) {
    outKey = (char*)obj.begin()->key().c_str();
    return obj.begin()->value();
}

void loop() {}