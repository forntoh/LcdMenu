#include <Arduino.h>
#include <ArduinoJson.h>

struct Pair {
    char* key;
    JsonVariant value;
};

Pair getPair(JsonObject obj);
void read(Pair tmp, uint8_t step);

void setup() {
    Serial.begin(9600);
    while (!Serial) continue;

    Serial.println();

    // Allocate the JSON document
    //
    // Inside the brackets, 200 is the capacity of the memory pool in bytes.
    // Don't forget to change this value to match your JSON document.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<256> doc;

    char json[] =
        "[{\"Settings\":[{\"C\":[\"Selp\",\"Selp\"]},{\"T\":[\"Sl\",\"Sl\"]}]},"
        "{\"Wifi\":[{\"C\":[\"Welp\",\"Walp\"]},{\"T\":[\"Wl\",\"Wl\"]},{\"I\":"
        "[\"Wan\"]},{\"WoonLight\":[{\"M\":[\"West\"]},{\"I\":[\"Win\",\"Han\"]"
        "}]}]}]";

    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    JsonVariant arr = doc.as<JsonVariant>();

    Pair tmp0;
    tmp0.value = arr;

    read(tmp0, 0);
}

void read(Pair tmp, uint8_t step) {
    bool a = false;
    bool b = false;

    /////////////////////// TestMenu Start/////////////////////////////////
    String* tempItems = new String[tmp.value.size() + 2];
    /////////////////////// TestMenu END///////////////////////////////////

    for (uint8_t i = 0; i < tmp.value.size(); i++) {
        if (tmp.value[i].is<JsonObject>()) {
            Pair tmp1 = getPair(tmp.value[i]);

            if (!b) {
                step++;
                b = true;
            }

            // Serial.print((String)step);
            // Serial.print(" ");

            // if (strlen(tmp1.key) != 1) {
            //     Serial.print((String)tmp1.value.size());
            // } else {
            //     Serial.print(" ");
            // }

            // Serial.print(" |");
            // for (uint8_t t = 0; t <= step; t++) Serial.print(" ");

            // Serial.println(tmp1.key);

            read(tmp1, step);

            ////////////// TestMenu Start ///////////////////////////////////
            String head = "SUB_HEAD";
            if (step == 1) head = "HEAD";

            tempItems[0] = head;
            tempItems[i + 1] = tmp1.key;
            ////////////// TestMenu END /////////////////////////////////////
        } else {
            char* val = tmp.value[i];

            ////////////// TestMenu Start////////////////////////////////////
            tempItems[i + 1] = val;
            ////////////// TestMenu END//////////////////////////////////////

            if (!a) {
                step++;
                a = true;

                ///////////// TestMenu Start////////////////////////////////
                String tit;

                switch (tmp.key[0]) {
                    case 'C':
                        tit = "  ItemCommand[";
                        break;
                    case 'T':
                        tit = "  ItemToggle[";
                        break;
                    case 'I':
                        tit = "  ItemInput[";
                        break;
                    default:
                        tit = "  MenuItem[";
                        break;
                }

                tempItems[i] = tit;
                tempItems[i + 1] = val;
                ////////////// TestMenu END////////////////////////////////
            }

            // Serial.print("    |");
            // for (uint8_t t = 0; t <= step; t++) Serial.print(" ");
            // Serial.print(val);
        }
    }

    ////////////////// TestMenu Start//////////////////////////////////////
    if (tempItems[0] == "HEAD" || tempItems[0] == "SUB_HEAD")
        tempItems[tmp.value.size() + 1] = "FOOT";

    for (uint8_t i = 0; i < (tmp.value.size() + 2); i++) {
        Serial.print(tempItems[i]);
        Serial.print(",");
    }
    /////////////////////// TestMenu END///////////////////////////////////

    Serial.println();
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