// vue32_wifi.hpp

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

const byte DNSSERVER_PORT = 53;
DNSServer dnsServer;

IPAddress ap_IPv4(192,168,4,1);
IPAddress ap_subnet(255,255,255,0);

// WiFi.mode(WIFI_STA)      - station mode: the ESP32 connects to an access point
// WiFi.mode(WIFI_AP)       - access point mode: stations can connect to the ESP32
// WiFi.mode(WIFI_AP_STA)   - access point and a station connected to another access point

bool wifi_change = false;

unsigned long previousMillisWIFI = 0;
unsigned long previousMillisAP = 0;
unsigned long intervalWIFI = 30000; // 30 Segundos

// http://adminesp32.local
const char *esp_hostname = device_id;

// -------------------------------------------------------------------
// Iniciar WIFI Modo AP
// -------------------------------------------------------------------
void startAP(){
    log("[ INFO ] Iniciando Modo AP");
    WiFi.disconnect(true);
    WiFi.softAPConfig(ap_IPv4, ap_IPv4, ap_subnet);
    WiFi.hostname(esp_hostname);
    WiFi.softAP(ap_ssid, ap_password, ap_chanel, ap_visibility, ap_connect);
    log("[ INFO ] WiFi AP " + String(ap_ssid) + " - IP " + ipStr(WiFi.softAPIP()));
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(DNSSERVER_PORT, "*", ap_IPv4);
    wifi_mode = WIFI_AP;
}
// -------------------------------------------------------------------
// Iniciar WIFI Modo Estación
// -------------------------------------------------------------------
void startClient(){
    log("[ INFO ] Iniciando Modo Estación");
    WiFi.mode(WIFI_STA);
    if(wifi_ip_static){
        if(!WiFi.config(CharToIP(wifi_ipv4), CharToIP(wifi_gateway), CharToIP(wifi_subnet), CharToIP(wifi_dns_primary), CharToIP(wifi_dns_secondary))){
            log("[ ERROR ] Falló la configuración en Modo Estación");
        }
    }
    WiFi.hostname(esp_hostname);
    WiFi.begin(wifi_ssid, wifi_password);
    log("[ INFO ] Conectando al SSID " + String(wifi_ssid));
    byte b = 0;
    while( WiFi.status() != WL_CONNECTED && b < 60){
        b++;
        log("[ WARNING ] Intentando conexión WiFi ...");
        vTaskDelay(500);
        //blinkSingle(100, WIFILED);        
    }
    if(WiFi.status() == WL_CONNECTED){
        log("[ INFO ] WiFi conectado (" + String(WiFi.RSSI()) + ") dBm IPv4 " + ipStr(WiFi.localIP()));
        //blinkRandomSingle(10, 100, WIFILED);
        wifi_mode = WIFI_STA;
        wifi_change = true;
    }else{
        log("[ ERROR ] WiFi no conectado");        
        //blinkRandomSingle(10, 100, WIFILED);
        wifi_change = true;
        startAP();
    }
}
// -------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------
void wifi_setup(){
    WiFi.disconnect(true);
    // 1) Si esta activo el Modo AP
    if(ap_mode){
        startAP();
        log("[ INFO ] WiFi en Modo AP");
    }else{
    // 2) Caso contrario en Modo Estación
        startClient();
        if(WiFi.status() == WL_CONNECTED){
            log("[ INFO ] WiFI Modo Estación");
        }
    }
    // Iniciar hostname broadcast en modo STA o AP
    if(wifi_mode == WIFI_STA || wifi_mode == WIFI_AP){
        if(MDNS.begin(esp_hostname)){
            MDNS.addService("http", "tcp", 80);
        }
    }
}
// -------------------------------------------------------------------
// Loop Modo Estación
// -------------------------------------------------------------------
byte w = 0;
void wifiLoop(){
    unsigned long currentMillis = millis();
    if(WiFi.status() != WL_CONNECTED && (currentMillis - previousMillisWIFI >= intervalWIFI)){
        w++;
        //blinkSingle(100, WIFILED);
        WiFi.disconnect(true);
        WiFi.reconnect();
        previousMillisWIFI = currentMillis;
        // 2 = 1 minuto
        if(w == 2){
            log("[ INFO ] Cambiando a Modo AP");
            wifi_change = true;
            w = 0;
            startAP(); 
        }else{
            log("[ WARNING ] SSID " + String(wifi_ssid) + " desconectado ");
        }
    }else{
       //blinkSingleAsy(10, 500, WIFILED); 
    }
}
// -------------------------------------------------------------------
// Loop Modo AP
// -------------------------------------------------------------------
byte a = 0;
void wifiAPLoop(){
    //blinkSingleAsy(5, 100, WIFILED);
    dnsServer.processNextRequest(); // Portal captivo DNS
    unsigned long currentMillis = millis();
    if((currentMillis - previousMillisAP >= intervalWIFI) && wifi_change){
        a++;
        previousMillisAP = currentMillis;
        // 20 es igual a 10 minuto
        if(a == 20){
            log("[ INFO ] Cambiando a Modo Estación");
            wifi_change = false;
            a = 0;
            startClient();
        }
    }
}

