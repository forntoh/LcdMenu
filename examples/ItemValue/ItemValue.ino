// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ItemValue.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

#define DHTPIN 12      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22  // DHT 22 (AM2302)

float temp = 0.0;

// Initialize the main menu items
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_VALUE("Temp", temp, "%.1fC"),
    ITEM_BASIC("Placeholder 1"),
    ITEM_BASIC("Placeholder 2"),
    ITEM_BASIC("Placeholder 3"));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
CharacterDisplayRenderer renderer(new LiquidCrystal_I2CAdapter(&lcd), LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);
DHT_Unified dht(DHTPIN, DHTTYPE);

uint16_t dhtReadInterval;

void setup() {
    Serial.begin(9600);
    dht.begin();
    renderer.begin();
    menu.setScreen(mainScreen);

    // Get the sensor details
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dhtReadInterval = sensor.min_delay / 1000;

    Serial.print(F("DHT22 read interval: "));
    Serial.println(dhtReadInterval);
}

void readTemperature() {
    // Get temperature after the read interval
    static unsigned long lastReadTime = 0;
    if (millis() - lastReadTime > dhtReadInterval) {
        sensors_event_t event;
        dht.temperature().getEvent(&event);
        if (event.temperature) {
            temp = event.temperature;

            Serial.print(F("Temperature: "));
            Serial.println(temp);
        }
        lastReadTime = millis();
    }
}

void loop() {
    keyboard.observe();
    menu.poll(dhtReadInterval - 100);  // Polling is required for ITEM_VALUE
    readTemperature();
}
