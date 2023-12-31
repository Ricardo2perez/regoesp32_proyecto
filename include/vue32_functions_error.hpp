// vue32_functions_error.hpp

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


#include "vue32_LedBlink.hpp"

// -------------------------------------------------------------------
// Llamada de funciones de otros archivos
// -------------------------------------------------------------------
void WsMessage(String msg, String icon, String Type);
String getSendJson(String msg, String type);
void setDyMsYr();
// -------------------------------------------------------------------
// Genera un log en el puerto Serial
// -------------------------------------------------------------------
void log(String s)
{
    Serial.println(s);
}
// -------------------------------------------------------------------
// Definir la Plataforma
// -------------------------------------------------------------------
String platform()
{
// Optiene la plataforma de hardware
#ifdef ARDUINO_ESP32_DEV
    return "ESP32";
#endif
}
// -------------------------------------------------------------------
// De HEX a String
// -------------------------------------------------------------------
String hexStr(const unsigned long &h, const byte &l = 8)
{
    String s;
    s = String(h, HEX);
    s.toUpperCase();
    s = ("00000000" + s).substring(s.length() + 8 - l);
    return s;
}
// -------------------------------------------------------------------
// Crear un ID unico desde la direccion MAC
// -------------------------------------------------------------------
String idUnique()
{
    // Retorna los ultimos 4 Bytes del MAC rotados
    char idunique[15];
    uint64_t chipid = ESP.getEfuseMac();
    uint16_t chip = (uint16_t)(chipid >> 32);
    snprintf(idunique, 15, "%04X", chip);
    return idunique;
}
// -------------------------------------------------------------------
// Número de serie del Dispositivo único
// -------------------------------------------------------------------
String deviceID()
{
    return String(platform()) + hexStr(ESP.getEfuseMac()) + String(idUnique());
}
// -------------------------------------------------------------------
// Configurar los Pines de Salida WIFI - MQTT
// -------------------------------------------------------------------
void settingPines()
{
    // -------------------------------------------------------------------
    // Configurar los Pines de Salida WIFI - MQTT
    // -------------------------------------------------------------------
    pinMode(SAL_1, OUTPUT);
    pinMode(SAL_2, OUTPUT);
    pinMode(SAL_3, OUTPUT);
    pinMode(SAL_4, OUTPUT);
    pinMode(ACT_1, OUTPUT);
    pinMode(ACT_2, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    digitalWrite(SAL_1, LOW);
    digitalWrite(SAL_2, LOW);
    digitalWrite(SAL_3, LOW);
    digitalWrite(SAL_4, LOW);
    digitalWrite(ACT_1, LOW);
    digitalWrite(ACT_2, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    log("[ INFO ] Salidas inicializadas");

    // pinMode(WIFILED, OUTPUT);
    // pinMode(MQTTLED, OUTPUT);
    // setOffSingle(WIFILED);
    // setOffSingle(MQTTLED);
}
// -------------------------------------------------------------------
// Convierte un char a IP
// -------------------------------------------------------------------
uint8_t ip[4]; // Variable función convertir string a IP
IPAddress CharToIP(const char *str)
{
    sscanf(str, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
    return IPAddress(ip[0], ip[1], ip[2], ip[3]);
}
// -------------------------------------------------------------------
// Retorna IPAddress en formato "n.n.n.n" de IP a String
// -------------------------------------------------------------------
String ipStr(const IPAddress &ip)
{
    String sFn = "";
    for (byte bFn = 0; bFn < 3; bFn++)
    {
        sFn += String((ip >> (8 * bFn)) & 0xFF) + ".";
    }
    sFn += String(((ip >> 8 * 3)) & 0xFF);
    return sFn;
}
// -------------------------------------------------------------------
// Crear un path para los Topicos en MQTT
// v1/devices/vue32_admin/ESP329B1C52100C3D
// -------------------------------------------------------------------
String pathMqtt()
{
    return String("v1/devices/" + String(mqtt_user) + "/" + String(mqtt_cloud_id));
}
// -------------------------------------------------------------------
// Parpadeo LED MQTT Recepción
// -------------------------------------------------------------------
void mqttRX()
{
    for (int16_t i = 0; i < 1; i++)
    {
        WsMessage(getSendJson("Recepción", "mqtt"), "", "");
        // blinkRandomSingle(5,50, MQTTLED);
        // vTaskDelay(10);
        // setOffSingle(MQTTLED);
    }
}
// -------------------------------------------------------------------
// Parpadeo LED MQTT Transmisión
// -------------------------------------------------------------------
void mqttTX()
{
    for (int16_t i = 0; i < 6; i++)
    {
        WsMessage(getSendJson("Transmisión", "mqtt"), "", "");
        // setOnSingle(MQTTLED);
        // vTaskDelay(50);
        // setOffSingle(MQTTLED);
        // vTaskDelay(10);
    }
}
// -------------------------------------------------------------------
// Retorna segundos como "d:hh:mm:ss"
// -------------------------------------------------------------------
String longTimeStr(const time_t &t)
{
    String s = String(t / SECS_PER_DAY) + ':';
    if (hour(t) < 10)
    {
        s += '0';
    }
    s += String(hour(t)) + ':';
    if (minute(t) < 10)
    {
        s += '0';
    }
    s += String(minute(t)) + ':';
    if (second(t) < 10)
    {
        s += '0';
    }
    s += String(second(t));
    return s;
}
// -------------------------------------------------------------------
// Retorna la calidad de señal WIFI en %
// -------------------------------------------------------------------
int getRSSIasQuality(int RSSI)
{
    int quality = 0;
    if (RSSI <= -100)
    {
        quality = 0;
    }
    else if (RSSI >= -50)
    {
        quality = 100;
    }
    else
    {
        quality = 2 * (RSSI + 100);
    }
    return quality;
}
// -------------------------------------------------------------------
// Retorna el contenido del Body Enviado como JSON metodo POST/PUT
// -------------------------------------------------------------------
String GetBodyContent(uint8_t *data, size_t len)
{
    String content = "";
    for (size_t i = 0; i < len; i++)
    {
        content.concat((char)data[i]);
    }
    return content;
}
// -------------------------------------------------------------------
// Retorna el Tipo de Encriptación según el codigo (0-1-2-3-4-5)
// -------------------------------------------------------------------
String EncryptionType(int encryptionType)
{
    switch (encryptionType)
    {
    case (0):
        return "Open";
    case (1):
        return "WEP";
    case (2):
        return "WPA_PSK";
    case (3):
        return "WPA2_PSK";
    case (4):
        return "WPA_WPA2_PSK";
    case (5):
        return "WPA2_ENTERPRISE";
    default:
        return "UNKOWN";
    }
}
// -------------------------------------------------------------------
// Empaquetar el JSON para enviar por WS ( progress en % y Actividad MQTT )
// -------------------------------------------------------------------
String getSendJson(String msg, String type)
{
    String response = "";
    StaticJsonDocument<300> doc;
    doc["type"] = type;
    doc["msg"] = msg;
    serializeJson(doc, response);
    return response;
}
// -------------------------------------------------------------------
// Print Progress Firmware or SPIFFS Update
// -------------------------------------------------------------------
int c = 0;
void printProgress(size_t prog, size_t sz)
{
    int progress = (prog * 100) / content_len;
    switch (progress)
    {
    case 10:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(progress), "update"), "", "");
        }
        break;
    case 20:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(progress), "update"), "", "");
        }
        break;
    case 30:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(progress), "update"), "", "");
        }
        break;
    case 40:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(progress), "update"), "", "");
        }
        break;
    case 50:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(progress), "update"), "", "");
        }
        break;
    case 60:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(progress), "update"), "", "");
        }
        break;
    case 70:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(progress), "update"), "", "");
        }
        break;
    case 80:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(progress), "update"), "", "");
        }
        break;
    case 90:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(progress), "update"), "", "");
        }
        break;
    case 98:
        c++;
        if (c >= 2)
            c = 1;
        if (c == 1)
        {
            WsMessage(getSendJson(String(100), "update"), "", "");
        }
        break;
    }
    Serial.printf("[ INFO ] Progreso de la Actualización al : %d%%\n", progress);
}
// -------------------------------------------------------------------
// Fecha y Hora del Sistema
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// Setup de Fecha y Hora  Auto / Manual
// -------------------------------------------------------------------
void timeSetup()
{

    setDyMsYr();

    if (time_ajuste)
    {
        // rtc.setTime(time_sc, time_mn, time_hr, time_dy, time_mt, time_yr);
        //  rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
        rtc.adjust(DateTime(time_yr, time_mt, time_dy, time_hr, time_mn, time_sc));
        log("[ INFO ] RTC set OK");
        // datos desde el Internet
    }
    else
    {
        if ((WiFi.status() == WL_CONNECTED) && (wifi_mode == WIFI_STA))
        {
            /* WiFi Conectada */
            ntpClient.begin();
            ntpClient.setPoolServerName(time_server);
            ntpClient.setTimeOffset(time_z_horaria);
            ntpClient.update();
            log("[ INFO ] NTP set OK");
        }
        else
        {
            /* Si no hay conexión a WiFi - No Internet */
            // rtc.setTime(time_sc, time_mn, time_hr, time_dy, time_mt, time_yr);
            //  rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
            rtc.adjust(DateTime(time_yr, time_mt, time_dy, time_hr, time_mn, time_sc));
            log("[ INFO ] RTC set OK");
        }
    }
}

// -------------------------------------------------------------------
// Función para seteo de Día, Mes y Año a las variables
// -------------------------------------------------------------------
void setDyMsYr()
{
    // 2022-09-07T23:47
    String str_date = time_date;
    time_sc = 0;
    time_mn = str_date.substring(14, 16).toInt();
    time_hr = str_date.substring(11, 13).toInt();
    time_dy = str_date.substring(8, 10).toInt();
    time_mt = str_date.substring(5, 7).toInt();
    time_yr = str_date.substring(0, 4).toInt();
}

// -------------------------------------------------------------------
// Fecha y Hora del Sistema
// -------------------------------------------------------------------
String getDateTime()
{

    char fecha[20];
    int dia = 0;
    int mes = 0;
    int anio = 0;
    int hora = 0;
    int minuto = 0;
    int segundo = 0;

    if (time_ajuste)
    { // Manual
        /* RTC */

        /* dia = rtc.getDay();
         mes = rtc.getMonth() + 1;
         anio = rtc.getYear();
         hora = rtc.getHour(true);
         minuto = rtc.getMinute();
         segundo = rtc.getSecond();*/
        DateTime now = rtc.now();
        dia = now.day();
        mes = now.month();
        anio = now.year();
        hora = now.hour();
        minuto = now.minute();
        segundo = now.second();
    }
    else
    { // Automatico
        if ((WiFi.status() == WL_CONNECTED) && (wifi_mode == WIFI_STA))
        {
            /* NTP */
            if (ntpClient.isTimeSet())
            {
                String formattedTime = ntpClient.getFormattedTime();
                // FORMAR FECHA DD-MM-YYYY DESDE EPOCH
                time_t epochTime = ntpClient.getEpochTime();
                struct tm *now = gmtime((time_t *)&epochTime);
                anio = now->tm_year + 1900;
                mes = now->tm_mon + 1;
                dia = now->tm_mday;
                // 12:00:00
                hora = ntpClient.getHours();
                minuto = ntpClient.getMinutes();
                segundo = ntpClient.getSeconds();
            }
        }
        else
        {
            /* RTC */
            /*
            dia = rtc.getDay();
            mes = rtc.getMonth() + 1;
            anio = rtc.getYear();
            hora = rtc.getHour(true);
            minuto = rtc.getMinute();
            segundo = rtc.getSecond();*/
            DateTime now = rtc.now();
            dia = now.day();
            mes = now.month();
            anio = now.year();
            hora = now.hour();
            minuto = now.minute();
            segundo = now.second();
        }
    }
    sprintf(fecha, "%.2d-%.2d-%.4d %.2d:%.2d:%.2d", dia, mes, anio, hora, minuto, segundo);
    return String(fecha);
}
