#include <Arduino.h>
#include <ArduinoJson.h>
#include <LcdMenu.h>

struct Pair {
    char* key;
    JsonVariant value;
};

struct MenuPair {
    MenuItem* menu;
    struct Pair next;
    uint8_t step;
};

Pair getPair(JsonObject obj);
MenuPair read(Pair tmp, uint8_t step);

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

    struct Pair answer0 = getPair(arr);

    for (uint8_t i = 0; i < answer0.value.size(); i++) {
        struct Pair p = getPair(answer0.value[i]);

        // Serial.print("-> ");
        // Serial.println(p.key);

        uint8_t ending = p.value.size() + 2;
        struct MenuPair a = read(p, 0);

        for (uint8_t j = 0; j < ending; j++) {
            Serial.print(a.menu[j].getText());
            Serial.print(F(","));
        }
        Serial.println();

        if (strlen(a.next.key) != 1) {

            // Serial.print("-> ");
            // Serial.println(a.next.key);

            uint8_t ending1 = a.next.value.size() + 2;
            struct MenuPair a1 = read(a.next, 0);

            for (uint8_t j = 0; j < ending1; j++) {
                Serial.print(a1.menu[j].getText());
                Serial.print(F(","));
            }
            Serial.println();
        }
    }
}

MenuPair read(Pair tmp, uint8_t step) {
    struct MenuPair data;

    MenuItem* tempItemsA = new MenuItem[tmp.value.size() + 2];

    for (uint8_t i = 0; i < tmp.value.size(); i++) {
        if (tmp.value[i].is<JsonObject>()) {
            Pair tmp1 = getPair(tmp.value[i]);

            data.next = tmp1;

            MenuItem headA = ItemHeader();
            if (step == 0) headA = ItemHeader(NULL);

            tempItemsA[0] = headA;

            if (strlen(tmp1.key) != 1)
                tempItemsA[i + 1] = MenuItem((String)tmp1.key);
            else {
                char* val = tmp1.value[0];

                MenuItem titA;

                switch (tmp1.key[0]) {
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
                tempItemsA[i + 1] = titA;
            }
        }
    }

    if (tempItemsA[0].getType() == MENU_ITEM_MAIN_MENU_HEADER ||
        tempItemsA[0].getType() == MENU_ITEM_SUB_MENU_HEADER)
        tempItemsA[tmp.value.size() + 1] = ItemFooter();

    data.menu = tempItemsA;
    return data;
}

Pair getPair(JsonObject obj) {
    struct Pair data;
    for (JsonObject::iterator it = obj.begin(); it != obj.end(); ++it) {
        data.key = (char*)(it->key()).c_str();
        data.value = it->value();
    }
    return data;
}

void loop() {}