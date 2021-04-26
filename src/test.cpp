#include <Arduino.h>
#include <ArduinoJson.h>
#include <LcdMenu.h>

const JsonVariant getPair(JsonObject obj, char*& outKey);
const JsonVariant read(JsonVariant obj, char*& outKey, MenuItem*& outMenu,
                       MenuItem*& parent);
void readRecursive(char* key, JsonVariant value, MenuItem*& parent);
MenuItem getMenuItem(char key, JsonVariant json);

void setup() {
    Serial.begin(9600);
    while (!Serial) continue;

    Serial.println();

    // Allocate the JSON document
    //
    // Inside the brackets, 200 is the capacity of the memory pool in bytes.
    // Don't forget to change this value to match your JSON document.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<272> doc;

    char json[] =
        "[{\"Hh\":[{\"C\":[\"A\",\"a\"]},{\"T\":[\"B\",\"b\"]}]},{\"Ii\":[{"
        "\"C\":[\"C\",\"c\"]},{\"T\":[\"D\",\"d\"]},{\"I\":[\"E\",\"e\"]},{"
        "\"Wo\":[{\"M\":[\"F\",\"f\"]},{\"I\":[\"G\",\"g\"]}]}]}]";

    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    JsonArray arr = doc.as<JsonArray>();

    uint8_t i = 0;

    MenuItem* mainMenu = new MenuItem[arr.size() + 2];
    mainMenu[0] = ItemHeader();
    mainMenu[arr.size() + 1] = ItemFooter();

    for (JsonArray::iterator it = arr.begin(); it != arr.end(); ++it) {
        char* key;
        JsonVariant value = getPair(*it, key);

        MenuItem subMenu = ItemSubMenu(key, mainMenu);
        MenuItem* sm = &subMenu;

        mainMenu[i + 1] = subMenu;

        readRecursive(key, value, sm);

        i++;
    }

    for (uint8_t j = 0; j < arr.size() + 2; j++) {
        Serial.print(mainMenu[j].getText());
        Serial.print(F(","));

        if (mainMenu[j].getType() == MENU_ITEM_SUB_MENU) {
            MenuItem* sm = mainMenu[j].getSubMenu();
            
            for (uint8_t k = 0; k < 2 + 2; k++) {
                Serial.print(sm[k].getText());
                Serial.print(F(","));

                if (mainMenu[j].getType() == MENU_ITEM_SUB_MENU) {
                }
            }
        }
    }
}

void readRecursive(char* key, JsonVariant value, MenuItem*& parent) {
    uint8_t size = value.size() + 2;
    if (strlen(key) > 1) {
        char* nextKey;
        MenuItem* child = new MenuItem[size];

        JsonVariant nextValue = read(value, nextKey, child, parent);

        // for (uint8_t j = 0; j < size; j++) {
        //     Serial.print(child[j].getText());
        //     Serial.print(F(","));
        // }
        // Serial.println();

        // delete[] child;

        readRecursive(nextKey, nextValue, child);
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

const JsonVariant read(JsonVariant value, char*& outKey, MenuItem*& outMenu,
                       MenuItem*& parent) {
    uint8_t size = value.size();
    JsonVariant nextValue;

    for (uint8_t i = 0; i < size; i++)
        if (value[i].is<JsonObject>()) {
            nextValue = getPair(value[i], outKey);

            if (strlen(outKey) != 1)
                outMenu[i + 1] = ItemSubMenu(outKey, parent);
            else
                outMenu[i + 1] = getMenuItem(outKey[0], nextValue);
        }

    outMenu[0] = ItemHeader(parent);
    outMenu[size + 1] = ItemFooter();

    return nextValue;
}

const JsonVariant getPair(JsonObject obj, char*& outKey) {
    outKey = (char*)obj.begin()->key().c_str();
    return obj.begin()->value();
}

void loop() {}