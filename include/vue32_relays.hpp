#include <Arduino.h>

// -------------------------------------------------------------------
// Estructura de un Relay
/*
@PIN            ->  Se define el pin del ESP32 Ejemplo: GPIO2 - 2
@LOGICA         ->  Se define la logica del PIN / 0 - Normal, 1 - Invertida
    Normal      - On(1) -> cerrar, Off(0) -> abrir
    Invertida   - On(1) -> abrir,  Off(0) -> cerrar
@MODE           ->  Se define el modo de trabajo del PIN si es de Entrada(INPUT) o Salida(OUTPUT)
@NAME           ->  Se define NOMBRE DEL PIN
@DESCRIPTION    ->  Se define LA DESCRIPCIÓN / USO DEL PIN
*/
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// Control de los Relay desde MQTT & WS
// -------------------------------------------------------------------
void OnOffRelays(String command)
{

    DynamicJsonDocument JsonDoc(1024);

    deserializeJson(JsonDoc, command);

    if (JsonDoc["protocol"] == "WS")
    {
        log("[ INFO ] Commando por WS => " + command);
    }
    else
    {
        log("[ INFO ] Commando por MQTT => " + command);
    }
    int pin = 99;
    if (JsonDoc["output"] == "LR" && JsonDoc["value"])
    {
        settingPines(); // apaga todas las salidas
        ciclo = false;
        enciendeLedManual();
        settingsSave();
    }
    if (!ciclo)
    {
        if (JsonDoc["output"] == "S1")
        {
            pin = SAL_1;
        }
        else if (JsonDoc["output"] == "S2")
        {
            pin = SAL_2;
        }
        else if (JsonDoc["output"] == "S3")
        {
            pin = SAL_3;
        }
        else if (JsonDoc["output"] == "S4")
        {
            pin = SAL_4;
        }
        else if (JsonDoc["output"] == "A1")
        {
            pin = ACT_1;
        }
        else if (JsonDoc["output"] == "A2")
        {
            pin = ACT_2;
        }
        if (JsonDoc["value"])
        {
            setOnSingle(pin);
        }
        else
        {
            setOffSingle(pin);
        }
    }
    if (JsonDoc["output"] == "LR" && !JsonDoc["value"])
    {
        ciclo = true;
        settingsSave();
        enciendeLedAuto();
    }
    // guardar status en memoria spiffs
    // settingsSave();
}
