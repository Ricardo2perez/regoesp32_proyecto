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


/*
Interrupciones de hardware: ocurren en respuesta a un evento externo. Por ejemplo,
Interrupción GPIO (cuando se presiona una tecla) o Interrupción táctil (cuando se detecta un toque).

Interrupciones de software: ocurren en respuesta a una instrucción de software. 
Por ejemplo, una interrupción de temporizador simple o una interrupción de temporizador de vigilancia
(cuando el temporizador se agota)
*/

#include <Arduino.h>

// -------------------------------------------------------------------
// Estructura Restore PIN
// -------------------------------------------------------------------
struct Restore {
  uint8_t PIN;
  bool active;
};


Restore reset_pin;

int pin_active = 0;

// -------------------------------------------------------------------
// IRAM_ATTR In1
// -------------------------------------------------------------------
void IRAM_ATTR int_restore() {
    reset_pin.active = true;
}

// -------------------------------------------------------------------
// setup del PIN
// -------------------------------------------------------------------
void setupPinRestore(){
    reset_pin = {PIN_RESET, false};                                  // Definir el pin
    pinMode(reset_pin.PIN, INPUT_PULLUP);                     // Activar las resistencias PullUp
    attachInterrupt(reset_pin.PIN, int_restore, FALLING);     // Activar la Interrupción por cambio de Alto a Bajo
}

/*
LOW	    Los disparadores se interrumpen cuando el pin está BAJO
HIGH	Los disparadores interrumpen cada vez que el pin está ALTO
CHANGE	Los disparadores se interrumpen cada vez que el pin cambia de valor, de ALTO a BAJO o de BAJO a ALTO
FALLING	Los disparadores se interrumpen cuando el pin pasa de ALTO a BAJO
RISING	Los disparadores se interrumpen cuando el pin pasa de BAJO a ALTO
*/

byte conteo = 0; 

void resetIntLoop(){
    if (reset_pin.active) {
        static uint32_t lastMillis = 0;
        // incrementar conteo en 1 cada segundo
        if (millis() - lastMillis > 1000) {
            lastMillis = millis();
            conteo++;
            Serial.println(conteo); 
        }

        pin_active = digitalRead(reset_pin.PIN); // En la interrupcion capturamos el estado del PIN

        if (pin_active == LOW && conteo == 10) {   
            // reseteo a fabrica
            settingsReset();
            // guardamos en spiffs
            if(settingsSave()){
                conteo = 0;      
                reset_pin.active = false; 
                log("[ INFO ] Todos los datos a fábrica por interrupción");  
                // Esperar la Transmisión de los datos seriales
                Serial.flush(); 
                ESP.restart();
            }  
        }else if(pin_active == HIGH && conteo > 1){
            // llamada a reiniciar
            conteo = 0;    
            reset_pin.active = false;
            log("[ INFO ] Se reinicio el Dispositivo por interrupción");   
            // Esperar la Transmisión de los datos seriales
            Serial.flush(); 
            ESP.restart();
        }

        vTaskDelay(200); //Antirrebote.

    }
}
