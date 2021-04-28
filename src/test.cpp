#include <Arduino.h>
#include <GenerateMenu.h>
#include <LcdMenu.h>

void setup() {
    Serial.begin(9600);
    while (!Serial) continue;

    char input[] =
        "0020\n0320Settings\n1320WiFi\n0701Backlight\n1601Contrast\n0501User\n1"
        "321Android\n0502Password\n1402Connect\n";

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