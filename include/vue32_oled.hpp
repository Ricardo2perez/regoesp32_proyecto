#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define NUMFLAKES 10     // Number of snowflakes in the animation example

#define LOGO_HEIGHT 22
#define LOGO_WIDTH 27

//-----------------------
#define XPOS 0 // Indexes into the 'icons' array in function below
#define YPOS 1
#define DELTAY 2
//--------------------

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 'xdesig', 27x22px
// Creado con https://javl.github.io/image2cpp/
const unsigned char xdesig5[] PROGMEM = {
    0xf8, 0xff, 0xf1, 0xe0, 0xfc, 0x7f, 0xe3, 0xe0, 0xfe, 0x3f, 0xe7, 0xe0, 0xff, 0xff, 0xe7, 0xe0,
    0xe0, 0x3f, 0xc0, 0xe0, 0xe7, 0x9f, 0x3e, 0x60, 0xef, 0xef, 0x7f, 0x60, 0xef, 0xe6, 0x7f, 0xe0,
    0xef, 0xe1, 0x3f, 0xe0, 0xef, 0xe4, 0x8f, 0xe0, 0xef, 0xf4, 0xe0, 0xe0, 0xef, 0xf4, 0xfc, 0x60,
    0xef, 0xe1, 0xff, 0x60, 0xef, 0xe6, 0x7f, 0x60, 0xef, 0xef, 0x7f, 0x60, 0xef, 0x8f, 0x1e, 0x60,
    0xe0, 0x3f, 0x80, 0xe0, 0xff, 0xbf, 0xbf, 0xe0, 0xfe, 0x3f, 0xc7, 0xe0, 0xfc, 0x7f, 0xe3, 0xe0,
    0xf8, 0xff, 0xe1, 0xe0, 0xf0, 0xff, 0xf1, 0xe0};
static const unsigned char PROGMEM grado[] = {0b00111, 0b00101, 0b00111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
static const unsigned char PROGMEM porcentaje[] = {
    0b01001,
    0b10101,
    0b01010,
    0b00100,
    0b01010,
    0b10101,
    0b10010,
    0b00000};

//-------------------------------------------------
// Inicializa la pantalla y muestra el splash
// ------------------------------------------------
void settingOled()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        log("SSD1306 allocation failed");
        // for(;;); // Don't proceed, loop forever 
        
    }
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    log("[INFO] Inicializando pantalla");
    // delay(2000); // Pause for 2 seconds
    //  Clear the buffer
    display.clearDisplay();
    log("[INFO] Borrando pantalla");

    // Draw a single pixel in white
    // display.drawPixel(10, 10, SSD1306_WHITE);
    display.display();
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(40, 0);            // Posicion del cursor para mostrar los datos
    display.println(F("RegoEsp32"));
    delay(2000);
    display.drawBitmap(
        (display.width() - LOGO_WIDTH) / 2,
        (display.height() - LOGO_HEIGHT) / 2 + 8,
        xdesig5, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(1000);
}

//-------------------------------------------------------
// Mostrar datos de temperatura y humedades por pantalla
//-------------------------------------------------------
void oledMuestraDatos()
{   
    int tamb = (int)TEMP_AMB; // Convertir float a entero
    int hamb = (int)HUM_AMB;  // Convertir floas a entero
    int offset = 0;
    if (hamb >= 100)
    {
        offset = 1;
    }
    display.clearDisplay();
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);
    display.print(F("TA: "));
    display.print(tamb);
    display.drawBitmap(34, 0, grado, 8, 8, 1);
    display.println(F(" C"));
    display.print(F("HA: "));
    display.println(hamb);
    display.drawBitmap(36 + offset, 8, porcentaje, 8, 8, 1);
    display.print(F("HS1:"));
    offset = 0;
    if (HUM_SUE < 10)
    {
        offset = 12;
    }
    if (HUM_SUE > 9 && HUM_SUE < 100)
    {
        offset = 6;
    }
    display.println(HUM_SUE);
    display.drawBitmap(42 - offset, 16, porcentaje, 8, 8, 1);
    display.setCursor(54, 16);
    display.print(F("HS2:"));
    offset = 0;
    if (HUM_SUE2 < 10)
    {
        offset = 12;
    }
    if (HUM_SUE2 > 9 && HUM_SUE2 < 100)
    {
        offset = 6;
    }
    display.println(HUM_SUE2);
    display.drawBitmap(94 - offset, 16, porcentaje, 8, 8, 1);
    display.print(F("HS3:"));
    offset = 0;
    if (HUM_SUE3 < 10)
    {
        offset = 12;
    }
    if (HUM_SUE3 > 9 && HUM_SUE3 < 100)
    {
        offset = 6;
    }
    display.println(HUM_SUE3);
    display.drawBitmap(41 - offset, 24, porcentaje, 8, 8, 1);
    display.setCursor(54, 24);
    display.print(F("HS4:"));
    offset = 0;
    if (HUM_SUE4 < 10)
    {
        offset = 12;
    }
    if (HUM_SUE4 > 9 && HUM_SUE4 < 100)
    {
        offset = 6;
    }
    display.println(HUM_SUE4);
    display.drawBitmap(94 - offset, 24, porcentaje, 8, 8, 1);

    display.display();
}
