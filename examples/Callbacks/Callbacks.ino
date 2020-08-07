#define LCD_ROWS 2
#define LCD_COLS 16

#define ENNE        0xEE      // la letra ñ es char (0xEE) en el LCD
#define MICRA       0xE4      // el símbolo micra u es char(0xE4) en el LCD
#define GRADO       0xDF      // el símbologrado º es char(0xDF) en el LCD

#define KEY_IN      A0
#define DEBOUNCE_DELAY  70    // 70 milisegundos
 
#define NADA             0    // Ningún movimiento de Joystick
#define MAS              1    // derecha
#define MASMAS           2    // derecha rápido
#define MENOS            3    // izquierda
#define MENOSMENOS       4    // izquierda rápido
#define ARRIBA           5    // arriba
#define ARRIBAARRIBA     6    // arriba rápido
#define ABAJO            7    // abajo
#define ABAJOABAJO       8    // abajo rápido
#define BOTON            9    // botón pulsado

#include <LcdNOI2CMenu.h>



//Variables Joystick
byte joyRead, joyPos, lastJoyRead, lastJoyPos;    
boolean joyPosValido;           // si la posición de joystick o botón es válida        
long lastDebounceTime = 0;      // Tiempo que el joystick está en esa posición

// Declare the call back function
void toggleBacklight();

// Define the main menu
extern MenuItem mainMenu[];
extern MenuItem settingsMenu[];

// Initialize the main menu items
MenuItem mainMenu[] = {ItemHeader(),
                       MenuItem("Start service"),
                       MenuItem("Connect to WiFi"),
                       ItemSubMenu("Settings", settingsMenu),
                       MenuItem("Blink SOS"),
                       MenuItem("Blink random"),
                       ItemFooter()};
/**
 * Create submenu and precise its parent
 */
MenuItem settingsMenu[] = {ItemSubHeader(mainMenu),
                           //
                           // Include callback in ItemToggle
                           //
                           ItemToggle("Backlight", toggleBacklight),
                           MenuItem("Contrast"),
                           ItemFooter()};

// Construct the LcdMenu
LcdMenu menu(LCD_ROWS, LCD_COLS);


void setup()
{
  menu.setupLcdWithMenu( 8, 9, 4, 5, 6, 7, mainMenu);
//settingsMenu[1].isOn = true;
}

void loop()
{
  leeKeyPad();
  if (joyPosValido)
  {
    joyPosValido = false;     

    switch (joyPos)
    {
        case ARRIBA:
        case ARRIBAARRIBA:
            menu.up();
            break;
        case ABAJO:
        case ABAJOABAJO:
            menu.down();
            break;
        case BOTON:
        case MAS:
        case MASMAS:
            menu.enter();
            break;
        case MENOS:
        case MENOSMENOS:
            menu.back();
            break;
    }
  }
}

void leeKeyPad()  // para keypad
{
  int x = analogRead(KEY_IN);  // se deben ajustar los valores a un keypad determinado

    if (x < 60) joyRead = MAS;     
    else if (x < 200) joyRead = ARRIBA;
    else if (x < 400) joyRead = ABAJO;
    else if (x < 600) joyRead = MENOS;
    else if (x < 800) joyRead = BOTON; // botón SELECT
    else joyRead = NADA;

  if (joyRead != lastJoyRead) lastDebounceTime = millis();  
  else if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY)
  {
     if (lastJoyPos != joyRead)
     {
       lastJoyPos = joyPos = joyRead;
       if (!joyPosValido) joyPosValido = true;           
     }  
     else if (((millis() - lastDebounceTime) > (7 * DEBOUNCE_DELAY)) && (joyRead != BOTON))  // por si está pulsado el mismo botón de movimiento mucho tiempo
     {
       if ((millis() - lastDebounceTime) > (10 * DEBOUNCE_DELAY))                            // activa la opción doble sólo para movimiento
         switch (joyRead)
         {
           case MAS: joyPos = MASMAS; break;
           case MENOS: joyPos = MENOSMENOS; break;
           case ARRIBA: joyPos = ARRIBAARRIBA; break;
           case ABAJO: joyPos = ABAJOABAJO; break;
         }                    
         if (!joyPosValido) joyPosValido = true;           
    }     
  }         
  lastJoyRead = joyRead;  // guarda posición actual
}  
/**
 * Define callback
 */
void toggleBacklight()
{
  if (settingsMenu[1].isOn) menu.lcd->display();
  else menu.lcd->noDisplay(); 
}
