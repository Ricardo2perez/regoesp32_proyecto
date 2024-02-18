// main.cpp

/* -------------------------------------------------------------------
   ReGo V4 
     Programado por: ricardo2perez@gmail.com
     Proyecto hardware: https://github.com/xdesig/ReGo
 --------------------------------------------------------------------

  Proyecto basado en el trabajo realizado por:
    AdminESP - ElectronicIOT 2022
    Sitio WEB: https://electroniciot.com
    Correo: admin@electroniciot.com
    Plataforma: ESP32
    Framework:  Arduino
    Proyecto: Panel Administrativo para el ESP32 con Vue.js
    Nombre: VUE32 Admin Tool
    Autor: Ing. Yamir Hidalgo Peña
-------------------------------------------------------------------


 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 USA.

  See LICENSE.txt for details
  */





// -------------------------------------------------------------------
// Librerías
// -------------------------------------------------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include <spi.h>
// Libreria para el RTC del ESP32
#include <ESP32Time.h>
// Librería para NTP
#include <NTPClient.h>
// Libreria DHT
#include <Adafruit_Sensor.h>
// Librerías para pantalla OLED
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// Librería para el PCF8523 RTC de Adafruit
#include "RTClib.h"
// -------------------------------------------------------------------
// Archivos *.hpp - Fragmentar el Código
// -------------------------------------------------------------------
#include "vue32_header.hpp"
#include "vue32_functions.hpp"
#include "vue32_settings.hpp"
#include "vue32_wifi.hpp"
#include "vue32_mqtt.hpp"
#include "vue32_server.hpp"
#include "vue32_websockets.hpp"
#include "vue32_relays.hpp"
#include "vue32_reset.hpp"
#include "vue32_dht.hpp"
#include "vue32_oled.hpp"
#include "vue32_keyboard.hpp"
#include "vue32_regulacion.hpp"

// instance

// -------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  setCpuFrequencyMhz(240);
  // Memoria EEPROM init
  EEPROM.begin(256);
  // Leer el valor de la memoria
  EEPROM.get(Restart_Address, device_restart);
  device_restart++;
  // Guardar el valor a la memoria
  EEPROM.put(Restart_Address, device_restart);
  EEPROM.commit();
  EEPROM.end();
  log("\n[ INFO ] Iniciando Setup");
  log("[ INFO ] Reinicios " + String(device_restart));
  log("[ INFO ] Setup corriendo en el Core " + String(xPortGetCoreID()));
  // Iniciar el SPIFFS
  if (!SPIFFS.begin(true))
  {
    log("[ ERROR ] Falló la inicialización del SPIFFS");
    while (true)
      ;
  }

  // Leer el Archivo settings.json
  if (!settingsRead())
  {
    settingsSave();
  }
  // Setup oled
  settingOled();
  // Configuración de los LEDs
  settingPines();
  // Setup WIFI
  wifi_setup();
  // setup del Time
  timeSetup();

  // Inicializacion del Servidor WEB
  InitServer();
  // Inicializamos el WS
  InitWebSockets();
  // Init pin restore
  setupPinRestore();
  // Init dht
  setupDht();
  // Init teclado
  setupKeyboard();

  
  // Fin del Setup

  log("[ INFO ] Setup completado");
}
// -------------------------------------------------------------------
// Loop Principal
// -------------------------------------------------------------------
void loop()
{
  // -----------------------------------------------------------------
  // WIFI
  // -----------------------------------------------------------------
  if (wifi_mode == WIFI_STA)
  {
    wifiLoop();
  }
  else if (wifi_mode == WIFI_AP)
  {
    wifiAPLoop();
  }
  // -----------------------------------------------------------------
  // MQTT
  // -----------------------------------------------------------------
  if ((WiFi.status() == WL_CONNECTED) && (wifi_mode == WIFI_STA))
  {
    if (mqtt_server != 0)
    {
      // Función para el Loop principla de MQTT
      mqttLoop();
      if (mqttClient.connected() && mqtt_time_send)
      {
        // Funcion para enviar JSON por MQTT
        if (millis() - lastMsg > mqtt_time_interval)
        {
          lastMsg = millis();
          mqtt_publish();
        }
      }
    }
  }
  // -------------------------------------------------------------------
  // Enviar JSON por WS cada 1s
  // -------------------------------------------------------------------
  if (millis() - lastWsSend > 1000)
  {
    lastWsSend = millis();
    WsMessage(getJsonIndex(), "", "");
  }
  // -------------------------------------------------------------------
  // RTC & NTP
  // -------------------------------------------------------------------
  if ((WiFi.status() == WL_CONNECTED) && (wifi_mode == WIFI_STA))
  {
    ntpClient.update();
  }
  if (millis() - lastTime > 1000)
  {
    lastTime = millis();
    WsMessage(getSendJson(getDateTime(), "time"), "", "");
  }
  // -------------------------------------------------------------------
  // Temperatura y Humedad
  // -------------------------------------------------------------------
  
  if (millis() - ultimaLecturaDht > 5000)
  {
    ultimaLecturaDht = millis();
    getJsonDht();
    log(getJsonTemperature());
    WsMessage(getJsonTemperature(), "", "");
    oledMuestraDatos();
  }
 
  //------------------------------------------------------------------
  // Cuando se produce un cambio de estado de una salida lo pasa
  // por websocket a la aplicación web
  //-----------------------------------------------------------------
  if (millis() - ultimoEstado > 5000)
  {
    //WsMessage(getJsonSalidas(), "", "");
    ultimoEstado = millis();
    if (getCambioEstado())
    {
      WsMessage(getJsonSalidas(), "", "");
      resetCambioEstado();
    }
  }

  //
  // -------------------------------------------------------------------
  // Monitorear la Interrupción del Pin 33
  // -------------------------------------------------------------------
   //resetIntLoop();

  //----------------------------------------------------------------
  // Monitorea el teclado cada 1s
  //------------------------------------------------------------------
  if (millis() - ultimaLecturaTeclado > 1000)
  {
    ultimaLecturaTeclado = millis();
    tick();
  }
  //----------------------------------------------------------------
  // Monitorea el teclado cada 1s
  //------------------------------------------------------------------
  if (ciclo){
  if (millis() - tickRegulacion > 1000)
  {
    tickRegulacion = millis();
    controlActuadores();
    controlElectrovavulas();
  }
  }
}