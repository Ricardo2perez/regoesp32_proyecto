// vue32_header.hpp

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
// Definiciones
// -------------------------------------------------------------------
// #define WIFILED 12                                      // GPIO12 LED WIFI
// #define MQTTLED 13                                      // GPIO13 LED MQTT

//*** Entradas ***
// #define KEY A0        //SPV Entrada teclado
#define KEY 36  //A0  // SPV Entrada teclado
#define DHTPIN 17 // GPIO17 SENSOR TEMPERATURA DHT11

// sensores humidade
// La GPIO 26  coincide con el ADC2 que entra en conflicto con el wifi, no se puede usar
#define IN_1 39 // Entrada sensor 1
#define IN_2 35 // Entrada sensor 2
#define IN_3 33 // Entrada sensor 3
#define IN_4 34 // Entrada sensor 4


// finais de carrera
#define IL_1 18 // Entrada Activo baixo 1
#define IL_2 19 // Entrada Activo baixo 2
#define IH_1 23 // Entrada Activo alto 1
#define IH_2 14 // Entrada Activo alto 2

// RTC PCF8523 soldado PCF8563!
#define INT 4 // Interrupción RTC

// Electrovalvúlas
#define SAL_1 27         // Electrovalvúla 1 
#define SAL_2 25         // Electrovalvúla 2
#define SAL_3 12  //TD1  // Electrovalvúla 3
#define SAL_4  0         // Electrovalvúla 4


// Motores
#define ACT_1 2       	 // Pechar
#define ACT_2 15 //TD02  // Abrir

// LED Auto/Manual
#define RED 10  // RED   (RED = 1 GREEN = 0)
#define GREEN 9 // GREEN (RED = 0 GREEN = 1)

// Pin Reset
#define PIN_RESET 5

// -------------------------------------------------------------------
// CALCULAR LA CAPACIDAD DEL JSON
// Asistente ArduinoJson: https://arduinojson.org/v6/assistant/
// Documentación: https://arduinojson.org/v6/api/json/deserializejson/
// -------------------------------------------------------------------
const size_t capacitySettings = 2300;
// const size_t capacitySettings = 1836;
// const size_t capacitySettings = 1463;
//  -------------------------------------------------------------------
//  Versión de Firmware desde las variables de entorno platformio.ini
//  -------------------------------------------------------------------
#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)
String device_fw_version = ESCAPEQUOTE(BUILD_TAG);
// -------------------------------------------------------------------
// Version de Hardware y Fabricante
// -------------------------------------------------------------------
#define device_hw_version "ADMINVUE32 v1 00000000" // Versión del hardware
#define device_manufacturer "IOTHOST"              // Fabricante
// -------------------------------------------------------------------
// Zona configuración Dispositivo
// -------------------------------------------------------------------
boolean device_config_file;    // Identificador para archivo de configuración
char device_config_serial[30]; // Numero de serie de cada Archivo configuración unico
char device_id[30];            // ID del dispositivo
int device_restart;            // Número de reinicios
char device_old_user[15];      // Usuario para acceso al servidor Web
char device_old_password[15];  // Contraseña del usuario servidor Web
// -------------------------------------------------------------------
// Zona configuración WIFI modo Cliente
// -------------------------------------------------------------------
int wifi_mode = WIFI_STA;

boolean wifi_ip_static;      // Uso de IP Estática DHCP
char wifi_ssid[30];          // Nombre de la red WiFi
char wifi_password[30];      // Contraseña de la Red WiFi
char wifi_ipv4[15];          // Dir IPv4 Estático
char wifi_gateway[15];       // Dir IPv4 Gateway
char wifi_subnet[15];        // Dir IPv4 Subred
char wifi_dns_primary[15];   // Dir IPv4 DNS primario
char wifi_dns_secondary[15]; // Dir IPv4 DNS secundario
// -------------------------------------------------------------------
// Zona configuración WIFI modo AP
// -------------------------------------------------------------------
boolean ap_mode;      // Uso de Modo AP
char ap_ssid[31];     // Nombre del SSID AP
char ap_password[63]; // Contraseña del AP
int ap_chanel;        // Canal AP
int ap_visibility;    // Es visible o no el AP  (0 - Visible  1 - Oculto)
int ap_connect;       // Número de conexiones en el AP máx 8 conexiones ESP32
// -------------------------------------------------------------------
// Zona configuración MQTT
// -------------------------------------------------------------------
boolean mqtt_cloud_enable; // Habilitar MQTT Broker
char mqtt_cloud_id[30];    // Cliente ID MQTT Broker
char mqtt_user[30];        // Usuario MQTT Broker
char mqtt_password[39];    // Contraseña del MQTT Broker
char mqtt_server[39];      // Servidor del MQTT Broker
int mqtt_port;             // Puerto servidor MQTT Broker
boolean mqtt_retain;       // Habilitar mensajes retenidos
int mqtt_qos;              // Calidad del servicio
boolean mqtt_time_send;    // Habilitar en envio de datos
int mqtt_time_interval;    // Tiempo de envio por MQTT en segundos
boolean mqtt_status_send;  // Habilitar en envio de estados
// -------------------------------------------------------------------
// Zona Firmware Update
// -------------------------------------------------------------------
size_t content_len;
#define U_PART U_SPIFFS
// -------------------------------------------------------------------
// Zona EEPROM para contador de reinicios
// -------------------------------------------------------------------
#define Start_Address 0
#define Restart_Address Start_Address + sizeof(int)
// -------------------------------------------------------------------
// Zona Time
// -------------------------------------------------------------------
ESP32Time rtc; // Clase ESP32Time
RTC_PCF8523 rtc8523;
int time_ajuste;     // 2- PCF8523 1- Manual  0- Automático internet SRV NTP
char time_date[18];   // 2023-09-07T23:47
long time_z_horaria;  // Zona Horaria GMT 0 = 0 - GMT +1/4 = 3600*4 - GMT -1 = -3600
char time_server[39]; // Servidor NTP Ejemplo: time.nist.gov

int time_hr; // Hora 0 - 23
int time_mn; // Minutos 0 - 59
int time_sc; // Segundos 0 - 59
int time_dy; // Días 1 - 31
int time_mt; // Meses 1 - 12
int time_yr; // Año 2023

// NTP Server
WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP);

long lastTime = 0; // Variable para tiempo de envio por WS

// ---------------------------------------------
// DHT11
// ---------------------------------------------

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11
// #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)
float TEMP_AMB;      // valor de la temperatura ambiente
float HUM_AMB;       // valor de la humedad ambiente
int HUM_SUE;         // valor del sensor 1 de humedad del suelo
int HUM_SUE2;        // valor del sensor 2 de humedad del suelo
int HUM_SUE3;        // valor del sensor 3 de humedad del suelo
int HUM_SUE4;        // valor del sensor 4 de humedad del suelo
float TEMP_AMB_OFFS; // valor de corrección de la temperatura ambiente
float HUM_AMB_OFFS;  // valor de corrección de la humedad ambiente
int HUM_SUE_OFFS;    // valor de corrección del valor del sensor 1 de humedad del suelo
int HUM_SUE2_OFFS;   // valor de corrección del valor del sensor 2 de humedad del suelo
int HUM_SUE3_OFFS;   // valor de corrección del valor del sensor 3 de humedad del suelo
int HUM_SUE4_OFFS;   // valor de corrección del valor del sensor 4 de humedad del suelo
bool DHT_STATUS;     // estado del dht

// ---------------------------------------------
// Teclado
// ---------------------------------------------
int BS_ANALOG_VALUE_PULS1;  // valor de lectura analógica del pulsador 1
int BS_ANALOG_VALUE_PULS2;  // valor de lectura analógica del pulsador 2
int BS_ANALOG_VALUE_PULS3;  // valor de lectura analógica del pulsador 3
int BS_ANALOG_VALUE_PULS4;  // valor de lectura analógica del pulsador 4
int BS_ANALOG_VALUE_PULS5;  // valor de lectura analógica del pulsador 5
int BS_ANALOG_PULLUP_VALUE; // valor de lectura analógica sin ningún pulsador metido
int BUTTON_MIN_PRESSED;     // Tiempo mínimo de pulsación del botón
int BUTTON_LONG_PRESSED;    // Tiempo pulsado para que sea larga pulsación

// -----------------------------------------------
// botones
// -----------------------------------------------
enum
{
    BUTTON_NONE = 0,
    BUTTON_PULS1 = 1,
    BUTTON_PULS2 = 2,
    BUTTON_PULS3 = 3,
    BUTTON_PULS4 = 4,
    BUTTON_PULS5 = 5,
};
typedef uint8_t BUTTON;

// --------------------------------------------
// Automatico / Manual
// --------------------------------------------
boolean ciclo = true; // false = Manual true = Automatico

// -----------------------------------------------
// Accionadores Apertura/Cierre Ventanas
// -----------------------------------------------
enum
{
    APAGADO = 0,
    ABRIR = 1,
    CERRAR = 2,
};
typedef uint8_t ACCIONADOR;
//-----------------------------------------------------------------------
// Inicializar Accionadores
//-----------------------------------------------------------------------
ACCIONADOR acciona = APAGADO;

// --------------------------------------
// Valores ajustes regulación
//---------------------------------------
int TEMP_CONEXION;       // valor de temperatura para apertura trampilla
int TEMP_DESCONEXION;    // valor de temperatura para cierre de la trampilla
int HUMEDAD_CONEXION;    // valor humedad para conexión electroválvula
int HUMEDAD_DESCONEXION; // valor humedad para conexión electroválvula
