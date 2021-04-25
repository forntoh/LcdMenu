// #include <Arduino.h>
// #include <ArduinoJson.h>
// #include <LcdMenu.h>

// struct Pair {
//     char* key;
//     JsonVariant value;
// };

// Pair getPair(JsonObject obj);
// MenuItem* read(Pair tmp, uint8_t step);

// void setup() {
//     Serial.begin(9600);
//     while (!Serial) continue;

//     Serial.println();
//     Serial.println();
//     Serial.println();

//     // Allocate the JSON document
//     //
//     // Inside the brackets, 200 is the capacity of the memory pool in bytes.
//     // Don't forget to change this value to match your JSON document.
//     // Use arduinojson.org/v6/assistant to compute the capacity.
//     DynamicJsonDocument doc(256);

//     char json[] =
//         "[{\"Settings\":[{\"C\":[\"Selp\",\"Selp\"]},{\"T\":[\"Sl\",\"Sl\"]}]},"
//         "{\"Wifi\":[{\"C\":[\"Welp\",\"Walp\"]},{\"T\":[\"Wl\",\"Wl\"]},{\"I\":"
//         "[\"Wan\"]},{\"WoonLight\":[{\"M\":[\"West\"]},{\"I\":[\"Win\",\"Han\"]"
//         "}]}]}]";

//     DeserializationError error = deserializeJson(doc, json);

//     if (error) {
//         Serial.print(F("deserializeJson() failed: "));
//         Serial.println(error.f_str());
//         return;
//     }

//     JsonVariant arr = doc.as<JsonVariant>();

//     Pair tmp0;
//     tmp0.value = arr;

//     MenuItem* answer = read(tmp0, 0);

//     for (uint8_t i = 0; i < (tmp0.value.size() + 2); i++) {
//         Serial.print(answer[i].getText());
//     }
// }

// MenuItem* read(Pair tmp, uint8_t step) {
//     bool a = false;
//     bool b = false;

//     MenuItem* tempItemsA = new MenuItem[tmp.value.size() + 2];

//     for (uint8_t i = 0; i < tmp.value.size(); i++) {
//         if (tmp.value[i].is<JsonObject>()) {
//             Pair tmp1 = getPair(tmp.value[i]);

//             if (!b) {
//                 step++;
//                 b = true;
//             }

//             // read(tmp1, step);

//             MenuItem headA = ItemHeader();
//             if (step == 1) headA = ItemHeader(NULL);

//             tempItemsA[0] = headA;
//             tempItemsA[i + 1] = MenuItem((String)tmp1.key);
//         } else {
//             char* val = tmp.value[i];

//             if (!a) {
//                 step++;
//                 a = true;

//                 MenuItem titA;

//                 switch (tmp.key[0]) {
//                     case 'C':
//                         titA = ItemCommand(val, NULL);
//                         break;
//                     case 'T':
//                         titA = ItemToggle(val, NULL);
//                         break;
//                     case 'I':
//                         titA = ItemInput(val, "", NULL);
//                         break;
//                     default:
//                         titA = MenuItem(val);
//                         break;
//                 }
//                 tempItemsA[i] = titA;
//             }
//         }
//     }

//     if (tempItemsA[0].getType() == MENU_ITEM_MAIN_MENU_HEADER ||
//         tempItemsA[0].getType() == MENU_ITEM_SUB_MENU_HEADER)
//         tempItemsA[tmp.value.size() + 1] = ItemFooter();

//     return tempItemsA;
// }

// Pair getPair(JsonObject obj) {
//     struct Pair data;
//     for (JsonObject::iterator it = obj.begin(); it != obj.end(); ++it) {
//         data.key = (char*)(it->key()).c_str();
//         data.value = it->value();
//     }
//     return data;
// }

// void loop() {}