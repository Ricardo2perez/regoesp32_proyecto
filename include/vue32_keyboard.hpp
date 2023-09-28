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
long _current_millis;
long ultimaLecturaTeclado = 0;
void singlePressed(BUTTON button);
void longPressed(BUTTON button);
void cambiaAccionador();

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
                singlePressed(button + 1);
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
    // Serial.print("valor de la lectura analógica ");
    // Serial.println(value);
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
    _current_millis = millis();

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
                longPressed(b + 1);
            }
        }
    }

    scanButtons();
}

//---------------------------------------------
// Pulsación Simple, activa salidas
//---------------------------------------------
void singlePressed(BUTTON button)
{
    int pulsador = button;
    /* BUTTON_PULS1 = 1,
    BUTTON_PULS2 = 2,
    BUTTON_PULS3 = 3,
    BUTTON_PULS4 = 4,
    BUTTON_PULS5 = 5,*/
    if (!ciclo)
    {
        switch (pulsador)
        {
        case BUTTON_PULS1:
            setOnOffSingle(BUTTON_PULS1);
            break;
        case BUTTON_PULS2:
            setOnOffSingle(BUTTON_PULS2);
            break;
        case BUTTON_PULS3:
            setOnOffSingle(BUTTON_PULS3);
            break;
        case BUTTON_PULS4:
            setOnOffSingle(BUTTON_PULS4);
            break;
        case BUTTON_PULS5:
            cambiaAccionador();
            break;

        default:
            cambioEstado = true;
            break;
        }
    }
}

//---------------------------------------------
// Pulsación larga, activa salidas
//---------------------------------------------
void longPressed(BUTTON button)
{
    int pulsador = button;
    switch (pulsador)
    {
    case BUTTON_PULS1:
        // Pendiente implementar
        break;
    case BUTTON_PULS2:
        // pendiente implementar
        break;
    case BUTTON_PULS3:
        // pendiente implementar
        break;
    case BUTTON_PULS4:
        // pendiente implementar
        break;
    case BUTTON_PULS5:
        if (ciclo)
        {

            ciclo = false;  // poner en manual
            settingPines(); // apagar salidas
            settingsSave();
            enciendeLedManual();
            cambioEstado = true;
        }
        else
        {
            ciclo = true; // poner en automatico
            settingsSave();
            enciendeLedAuto();
            cambioEstado = true;
        }
        break;

    default:
        break;
    }
}
//---------------------------------------------
// Pulsación Simple, Cambia el estado de los actuadores
//---------------------------------------------

void cambiaAccionador()
{
    switch (acciona)
    {
    case APAGADO:
        setOffSingle(ACT_2);
        setOnSingle(ACT_1);
        acciona = ABRIR;
        cambioEstado = true;
        break;
    case ABRIR:
        setOffSingle(ACT_1);
        setOnSingle(ACT_2);
        acciona = CERRAR;
        cambioEstado = true;
        break;
    case CERRAR:
        setOffSingle(ACT_1);
        setOffSingle(ACT_2);
        acciona = APAGADO;
        cambioEstado = true;
        break;
    default:
        break;
    }
}