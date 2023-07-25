/* -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2022
 * Sitio WEB: https://electroniciot.com
 * Correo: admin@electroniciot.com
 * Cel_WSP: +591 71243395
 * Plataforma: ESP32
 * Framework:  Arduino
 * Proyecto: Panel Administrativo para el ESP32 con Vue.js
 * Nombre: VUE32 Admin Tool
 * Autor: Ing. Yamir Hidalgo Peña
 * -------------------------------------------------------------------
 */

// -------------------------------------------------------------------
// Definiciones
// -------------------------------------------------------------------

#include <Arduino.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>

long ultimaLecturaDht = 0;
DHT dht(DHTPIN, DHTTYPE);
void setupDht()
{

    dht.begin();
}

String getJsonDht()
{
    int hs1 = 0; // variable para guardar lecturas sensores humedad suelo
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    h = round(h);
    // mete la humedad en una variable global
    HUM_AMB = h + HUM_AMB_OFFS;
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    t = round(t);
    // mete la temperatura en una variable global
    TEMP_AMB = t + TEMP_AMB_OFFS;
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
    f = round(f);
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        return ("Error Lectura");
    }
    else
    {
        //------------------------------------
        //    Medida de humedad del suelo
        //------------------------------------
        hs1 = analogRead(IN_2);

        HUM_SUE = map(hs1, 1780, 1300, 0, 100);
        // Evitar valores negativos y por encima de 100
        if (HUM_SUE < 0)
        {
            HUM_SUE = 0;
        }
        if (HUM_SUE > 100)
        {
            HUM_SUE = 100;
        }
        HUM_SUE = HUM_SUE + HUM_SUE_OFFS; // añadir correción de offset al valor de la temperatura
        //------------------------------------
        //    Medida de humedad del suelo 2
        //------------------------------------
        hs1 = analogRead(IN_2);

        HUM_SUE2 = map(hs1, 1780, 1300, 0, 100);
        // Evitar valores negativos y por encima de 100
        if (HUM_SUE2 < 0)
        {
            HUM_SUE2 = 0;
        }
        if (HUM_SUE2 > 100)
        {
            HUM_SUE2 = 100;
        }
        HUM_SUE2 = HUM_SUE2 + HUM_SUE2_OFFS; // añadir correción de offset al valor de la temperatura
        //------------------------------------
        //    Medida de humedad del suelo 3
        //------------------------------------
        hs1 = analogRead(IN_3);

        HUM_SUE3 = map(hs1, 1780, 1300, 0, 100);
        // Evitar valores negativos y por encima de 100
        if (HUM_SUE3 < 0)
        {
            HUM_SUE3 = 0;
        }
        if (HUM_SUE3 > 100)
        {
            HUM_SUE3 = 100;
        }
        HUM_SUE3 = HUM_SUE3 + HUM_SUE3_OFFS; // añadir correción de offset al valor de la temperatura
        //------------------------------------
        //    Medida de humedad del suelo 4
        //------------------------------------
        hs1 = analogRead(IN_4);

        HUM_SUE4 = map(hs1, 1780, 1300, 0, 100);
        // Evitar valores negativos y por encima de 100
        if (HUM_SUE4 < 0)
        {
            HUM_SUE4 = 0;
        }
        if (HUM_SUE4 > 100)
        {
            HUM_SUE4 = 100;
        }
        HUM_SUE4 = HUM_SUE4 + HUM_SUE4_OFFS; // añadir correción de offset al valor de la temperatura

        String response = "";
        DynamicJsonDocument jsonDoc(500);
        jsonDoc["type"] = "data";
        jsonDoc["temperatura C"] = TEMP_AMB;
        jsonDoc["humedad ambiente"] = HUM_AMB;
        jsonDoc["Humedad Suelo 1"] = HUM_SUE;
        jsonDoc["temperatura F"] = f;
        serializeJson(jsonDoc, response);
        return response;
    }
}
