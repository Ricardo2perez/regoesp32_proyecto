#include <Arduino.h>
//-----------------------------------
// Lectura del botón pulsado
//---------------------------------

// -----------------------------------------------------------------------
// Valores del teclado settings.json
//-------------------------------------------------------------------------
/* BS_ANALOG_VALUE_PULS1 = 0;    // valor de lectura analógica del pulsador 1
 BS_ANALOG_VALUE_PULS2 = 1855; // valor de lectura analógica del pulsador 2
 BS_ANALOG_VALUE_PULS3 = 2525; // valor de lectura analógica del pulsador 3
 BS_ANALOG_VALUE_PULS4 = 2872; // valor de lectura analógica del pulsador 4
 BS_ANALOG_VALUE_PULS5 = 3280; // valor de lectura analógica del pulsador 5
 BS_ANALOG_PULLUP_VALUE = 4095; // valor de lectura analógica sin pulsar ninguna tecla
*/

//-------------------------------------------
// Declaración de variables
//-------------------------------------------
BUTTON _last_button;
uint32_t _button_statuses[6];
long _current_millis ;
long ultimaLecturaTeclado = 0 ;


//----------------------------------------------
// Incicialización de variables para el teclado
//----------------------------------------------
void setupKeyboard()
{

    _last_button = BUTTON_NONE;
    _current_millis = millis();
}

//----------------------------------------------
// Tiempo que lleva pulsado el botón
//----------------------------------------------
void released(BUTTON button)
{
    button--;

    if (_button_statuses[button] != 0)
    {
        uint32_t pressed_millis = _current_millis - _button_statuses[button];

        if (pressed_millis > BUTTON_MIN_PRESSED)
        {
            if (pressed_millis < BUTTON_LONG_PRESSED)
            {
                // EVENTS->indicateButtonReleased(button + 1);
                Serial.print("[INFO] Botón soltado: ");
                Serial.print((button + 1));
                Serial.print("  tiempo pulsado: ");
                Serial.println(pressed_millis);
                setOnOffSingle(button+1);
            }

            _button_statuses[button] = 0;
        }
    }
}

//----------------------------------------------------
// Guardar la última vez que se pulsó un botón
//----------------------------------------------------
void pressed(BUTTON button)
{
    
    button--;
    if (_button_statuses[button] == 0)
    {
        _button_statuses[button] = _current_millis;
        // EVENTS->indicateButtonPressed(button + 1);
        Serial.print("[INFO] Botón pulsado: ");
        Serial.println((button + 1));
    }
}

//--------------------------------------------------
// Escaneo del teclado para ver que botón se pulsó
//--------------------------------------------------
void scanButtons()
{
    long value = analogRead(36);
    Serial.print("valor de la lectura analógica ");
    Serial.println(value);
    double diff, minor_diff;

    minor_diff = abs(value - BS_ANALOG_PULLUP_VALUE);
    BUTTON button = BUTTON_NONE;

    diff = abs(value - BS_ANALOG_VALUE_PULS1);
    if (diff < minor_diff)
    {
        minor_diff = diff;
        button = BUTTON_PULS1;
    }

    diff = abs(value - BS_ANALOG_VALUE_PULS2);
    if (diff < minor_diff)
    {
        minor_diff = diff;
        button = BUTTON_PULS2;
    }

    diff = abs(value - BS_ANALOG_VALUE_PULS3);
    if (diff < minor_diff)
    {
        minor_diff = diff;
        button = BUTTON_PULS3;
    }

    diff = abs(value - BS_ANALOG_VALUE_PULS4);
    if (diff < minor_diff)
    {
        minor_diff = diff;
        button = BUTTON_PULS4;
    }

    diff = abs(value - BS_ANALOG_VALUE_PULS5);
    if (diff < minor_diff)
    {
        minor_diff = diff;
        button = BUTTON_PULS5;
    }
    if (button != _last_button)
    {
        if (_last_button != BUTTON_NONE)
            released(_last_button);
        if (button != BUTTON_NONE)
            pressed(button);
        _last_button = button;
    }
}


//---------------------------------------------
// Verificación de larga pulsación botón y
// llamada a la función de escaneo del teclado
//---------------------------------------------
void tick()
{
    //_current_millis = micros / 1000;
    _current_millis =millis();

    // raise long-pressed events
    for (int b = 0; b < 6; b++)
    {
        if (_button_statuses[b] != 0)
        {
            uint32_t pressed_millis = _current_millis - _button_statuses[b];
            if (pressed_millis > BUTTON_LONG_PRESSED)
            {
                // EVENTS->indicateButtonLongReleased(b + 1);
                Serial.print("[INFO] Larga pulsación del botón: ");
                Serial.print(b + 1);
                Serial.print("  tiempo pulsado: ");
                Serial.println(pressed_millis);

                _button_statuses[b] = 0;
            }
        }
    }

    scanButtons();
   
}
