// vue32_LedBlink.hpp

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

bool ioBlink = false;
unsigned long milOld;
int rndTemp = 0;
bool cambioEstado = false;
long ultimoEstado = 0;

// -------------------------------------------------------------------
// Off un Led/Relay/Actuador
// -------------------------------------------------------------------
void setOffSingle(int _pin)
{
    digitalWrite(_pin, LOW); // 0
}
// -------------------------------------------------------------------
// On un Led/Relay/Actuador
// -------------------------------------------------------------------
void setOnSingle(int _pin)
{
    digitalWrite(_pin, HIGH); // 1
}
// -------------------------------------------------------------------
// On/Off un Led/Relay/Actuador segun Estados
// -------------------------------------------------------------------
void setOnOffSingle(int _pin)
{
    int _salida = 0;
    switch (_pin)
    {
    case 1:
        _salida = SAL_1;
        break;
    case 2:
        _salida = SAL_2;
        break;
    case 3:
        _salida = SAL_3;
        break;
    case 4:
        _salida = SAL_4;
        break;
    default:
        // statements
        break;
    }

    bool status = digitalRead(_salida);
    if (!status)
    {
        digitalWrite(_salida, HIGH);
    }
    else
    {
        digitalWrite(_salida, LOW);
    }
    cambioEstado = true;
}
// -------------------------------------------------------------------
// Simple blinking function - Pestañeo para Alarmas tiempo variable
// -------------------------------------------------------------------
void blinkSingle(int speed, int _pin)
{
    if ((milOld + speed) < millis())
    {
        milOld = millis();
        if (ioBlink == false)
        {
            digitalWrite(_pin, HIGH);
            ioBlink = true;
        }
        else
        {
            digitalWrite(_pin, LOW);
            ioBlink = false;
        }
    }
}
// -------------------------------------------------------------------
// Parpadeo Asincrónico Tiempo On y Tiempo Off tiempo variable
// -------------------------------------------------------------------
void blinkSingleAsy(int timeHigh, int timeLow, int _pin)
{
    if (ioBlink == false)
    {
        if ((milOld + timeHigh) < millis())
        {
            milOld = millis();
            digitalWrite(_pin, LOW);
            ioBlink = true;
        }
    }
    else
    {
        if ((milOld + timeLow) < millis())
        {
            milOld = millis();
            digitalWrite(_pin, HIGH);
            ioBlink = false;
        }
    }
}
// -------------------------------------------------------------------
// Blinking with randomised delay como para TX/RX de Datos
// -------------------------------------------------------------------
void blinkRandomSingle(int minTime, int maxTime, int _pin)
{
    if ((milOld + rndTemp) < millis())
    {
        milOld = millis();
        rndTemp = random(minTime, maxTime);
        if (ioBlink == false)
        {
            digitalWrite(_pin, HIGH);
            ioBlink = true;
        }
        else
        {
            digitalWrite(_pin, LOW);
            ioBlink = false;
        }
    }
}

bool getCambioEstado()
{
    return cambioEstado;
}

void resetCambioEstado()
{
    cambioEstado = false;
}

void enciendeLedAuto()
{
    setOffSingle(RED);
    setOnSingle(GREEN);
    cambioEstado = true;
}

void enciendeLedManual()
{
    setOffSingle(GREEN);
    setOnSingle(RED);
    cambioEstado = true;
}