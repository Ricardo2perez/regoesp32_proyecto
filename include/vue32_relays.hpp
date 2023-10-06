// vue32_relays.hpp

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

#include <Arduino.h>

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
    
}
