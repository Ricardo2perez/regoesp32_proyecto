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
// Declaración de funciones
// -------------------------------------------------------------------
void ProcessRequest(AsyncWebSocketClient * client, String request);

// -------------------------------------------------------------------
// Variable para tiempo de envio por WS
// -------------------------------------------------------------------
long lastWsSend = 0;    

// -------------------------------------------------------------------
// Eventos del Websocket
// -------------------------------------------------------------------
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){ 
	if(type == WS_EVT_CONNECT){
		Serial.printf("[ INFO ] ws[%s][%u] connect\n", server->url(), client->id());
		client->printf("{\"ClientID\": %u }", client->id());
		client->ping();
	} else if(type == WS_EVT_DISCONNECT){
		//Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
	} else if(type == WS_EVT_ERROR){
		//Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
	} else if(type == WS_EVT_PONG){
		//Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
	} else if(type == WS_EVT_DATA){
		AwsFrameInfo * info = (AwsFrameInfo*)arg;
		String msg = "";
		if(info->final && info->index == 0 && info->len == len){
			if(info->opcode == WS_TEXT){
				for(size_t i=0; i < info->len; i++) {
					msg += (char) data[i];
				}
			} else {
				char buff[3];
				for(size_t i=0; i < info->len; i++) {
					sprintf(buff, "%02x ", (uint8_t) data[i]);
					msg += buff ;
				}
			}
			if(info->opcode == WS_TEXT)
			   ProcessRequest(client, msg);			
		} else {
			//message is comprised of multiple frames or the frame is split into multiple packets
			if(info->opcode == WS_TEXT){
				for(size_t i=0; i < len; i++) {
					msg += (char) data[i];
				}
			} else {
				char buff[3];
				for(size_t i=0; i < len; i++) {
					sprintf(buff, "%02x ", (uint8_t) data[i]);
					msg += buff ;
				}
			}
			Serial.printf("%s\n",msg.c_str());

			if((info->index + len) == info->len){
				if(info->final){
					if(info->message_opcode == WS_TEXT)
					   ProcessRequest(client, msg);
				}
			}
		}
	}
}
// -------------------------------------------------------------------
// Inicializar el Websocket
// -------------------------------------------------------------------
void InitWebSockets(){
	ws.onEvent(onWsEvent);
	server.addHandler(&ws);
    log("[ INFO ] Servidor WebSocket iniciado");
}

// -------------------------------------------------------------------
// Manejador de ordenes enviada por Websocket
// -------------------------------------------------------------------
void ProcessRequest(AsyncWebSocketClient * client, String request){    
	String command = request;
    command.trim();
    
	if (strcmp(command.c_str(), "restore") == 0){
		log("[ INFO ] Commando por WS => " + command);
		settingsReset();
		if(settingsSave()){
			WsMessage("¡Equipo restablecido correctamente!","undo-alt","info");
			log("[ INFO ] ¡Equipo restablecido correctamente!");
			Serial.flush(); 
			ESP.restart();
		}
	}else if(strcmp(command.c_str(), "restart") == 0){
		log("[ INFO ] Commando por WS => " + command);	
		WsMessage("¡Equipo reiniciado correctamente!","redo-alt","info");
		log("[ INFO ] ¡Equipo reiniciado correctamente!");
		Serial.flush(); 
		ESP.restart();
	}
	else{
		OnOffRelays(command);
		log("[INFO ] Commando por WS => " + command);	
		//WsMessage("¡Salida actualizada!","redo-alt","info");
	}
	
}
// -------------------------------------------------------------------
// Función enviar JSON por Websocket 
// -------------------------------------------------------------------
void WsMessage(String msg, String icon, String Type){

	if(strcmp(Type.c_str(), "info") == 0){
		String response;
		StaticJsonDocument<300> doc;
		doc["type"] = Type;
		doc["msg"] = msg;
		doc["icon"] = icon;
		serializeJson(doc, response);
		ws.textAll(response);
	}else{
		ws.textAll(msg);
	}
	
}
// -------------------------------------------------------------------
// Empaquetar el JSON para enviar por WS Datos para Index cada 1s
// -------------------------------------------------------------------
String getJsonIndex(){
	String response = "";
	DynamicJsonDocument jsonDoc(3000);
	jsonDoc["type"] = "data";
	jsonDoc["device_time_active"] = longTimeStr(millis() / 1000);
	jsonDoc["device_ram_available"] = ESP.getFreeHeap();
	jsonDoc["mqtt_online"] = mqttClient.connected() ? true : false;
	jsonDoc["mqtt_server"] = mqttClient.connected() ? F(mqtt_server) : F("server not connected");
	jsonDoc["wifi_online"] = WiFi.status() == WL_CONNECTED ? true : false;
	jsonDoc["wifi_rssi"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
	jsonDoc["wifi_signal"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0;
	serializeJson(jsonDoc, response);
	return response;
}

// ------------------------------------------------------------------------------
// Empaquetar el JSON para enviar por WS Datos para Temperatura y Humedad cada 1s
// ------------------------------------------------------------------------------
String getJsonTemperature(){
	String response = "";
	DynamicJsonDocument jsonDoc(3000);
	jsonDoc["type"] = "temperature";
	jsonDoc["temperatura_ambiente"] = TEMP_AMB;
	jsonDoc["humedad_ambiente"] = HUM_AMB;
	jsonDoc["humedad_suelo"] = HUM_SUE;
	jsonDoc["humedad_suelo2"] = HUM_SUE2;
	jsonDoc["humedad_suelo3"] = HUM_SUE3;
	jsonDoc["humedad_suelo4"] = HUM_SUE4;
	jsonDoc["temp_amb_offs"] = TEMP_AMB_OFFS;
	jsonDoc["hum_amb_offs"] = HUM_SUE_OFFS;
	jsonDoc["hum_sue_offs"] = HUM_SUE_OFFS;
	jsonDoc["hum_sue2_offs"] = HUM_SUE2_OFFS;
	jsonDoc["hum_sue3_offs"] = HUM_SUE3_OFFS;
	jsonDoc["hum_sue4_offs"] = HUM_SUE4_OFFS;
	serializeJson(jsonDoc, response);
	return response;
}

// ------------------------------------------------------------------------------
// Empaquetar el JSON para enviar por WS Datos de las salidas cuando se actualiza en manual
// ------------------------------------------------------------------------------
String getJsonSalidas(){
	String response = "";
	DynamicJsonDocument jsonDoc(3000);
	jsonDoc["type"] = "salidas";
	jsonDoc["SALIDA1_STATUS"] = digitalRead(SAL_1);
	jsonDoc["SALIDA2_STATUS"] = digitalRead(SAL_2);
	jsonDoc["SALIDA3_STATUS"] = digitalRead(SAL_3);
	jsonDoc["SALIDA4_STATUS"] = digitalRead(SAL_4);
	jsonDoc["ACTUADOR1_STATUS"] = digitalRead(ACT_1);
	jsonDoc["ACTUADOR2_STATUS"] = digitalRead(ACT_2);
	jsonDoc["LEDROJO_STATUS"] = digitalRead(RED);
	jsonDoc["LEDVERDE_STATUS"] = digitalRead(GREEN);
	serializeJson(jsonDoc, response);
	return response;
}
 
/* SALIDA1_STATUS: false,
            SALIDA2_STATUS: false,
            SALIDA3_STATUS: false,
            SALIDA4_STATUS: false,
            ACTUADOR1_STATUS: false,
            ACTUADOR2_STATUS: false,
            LEDVERDE_STATUS: false,
            LEDROJO_STATUS: false,
            RELAY_STATUS: false,*/