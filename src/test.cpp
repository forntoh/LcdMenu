#include <Arduino.h>
#include <GenerateMenu.h>
#include <LcdMenu.h>

void setup() {
    Serial.begin(9600);
    while (!Serial) continue;

    char input[] =
        "0200\n0203Settings\n1203WiFi\n0017Backlight\n1015Contrast\n0015User\n1"
        "213Android\n0025Password\n1024Connect\n";

    MenuItem* currMenu = generateMenu(input);

    for (uint8_t i = 0; i < 4; i++) {
        MenuItem item1 = currMenu[i];
        Serial.println(item1.getText());

        if (item1.getType() == MENU_ITEM_SUB_MENU) {
            for (uint8_t j = 0; j < 4; j++) {
                MenuItem item2 = item1.getSubMenu()[j];
                Serial.print(' ');
                Serial.println(item2.getText());

                if (item2.getType() == MENU_ITEM_SUB_MENU) {
                    for (uint8_t k = 0; k < 4; k++) {
                        MenuItem item3 = item2.getSubMenu()[k];
                        Serial.print(F("  "));
                        Serial.println(item3.getText());
                    }
                }
            }
        }
    }
}

void loop() {}