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

boolean settingsRead();
void settingsReset();
boolean settingsSave();
// -------------------------------------------------------------------
// Leer settings.json
// -------------------------------------------------------------------
boolean settingsRead()
{

    DynamicJsonDocument jsonSettings(capacitySettings);

    File file = SPIFFS.open("/settings.json", "r");

    if (deserializeJson(jsonSettings, file))
    {
        // Tomar los valores de Fábrica
        settingsReset();
        DeserializationError error = deserializeJson(jsonSettings, file);
        log("[ ERROR ] Falló la lectura de las configuraciones, tomando valores por defecto");
        if (error)
        {
            log("[ ERROR ] " + String(error.c_str()));
        }
        return false;
    }
    else
    {
        // -------------------------------------------------------------------
        // Dispositivo settings.json
        // -------------------------------------------------------------------
        device_config_file = jsonSettings["device_config_file"];
        strlcpy(device_config_serial, jsonSettings["device_config_serial"], sizeof(device_config_serial));
        strlcpy(device_id, jsonSettings["device_id"], sizeof(device_id));
        strlcpy(device_old_user, jsonSettings["device_old_user"], sizeof(device_old_user));
        strlcpy(device_old_password, jsonSettings["device_old_password"], sizeof(device_old_password));
        // -------------------------------------------------------------------
        // WIFI Cliente settings.json
        // -------------------------------------------------------------------
        wifi_ip_static = jsonSettings["wifi_ip_static"];
        strlcpy(wifi_ssid, jsonSettings["wifi_ssid"], sizeof(wifi_ssid));
        strlcpy(wifi_password, jsonSettings["wifi_password"], sizeof(wifi_password));
        strlcpy(wifi_ipv4, jsonSettings["wifi_ipv4"], sizeof(wifi_ipv4));
        strlcpy(wifi_subnet, jsonSettings["wifi_subnet"], sizeof(wifi_subnet));
        strlcpy(wifi_gateway, jsonSettings["wifi_gateway"], sizeof(wifi_gateway));
        strlcpy(wifi_dns_primary, jsonSettings["wifi_dns_primary"], sizeof(wifi_dns_primary));
        strlcpy(wifi_dns_secondary, jsonSettings["wifi_dns_secondary"], sizeof(wifi_dns_secondary));
        // -------------------------------------------------------------------
        // WIFI AP settings.json
        // -------------------------------------------------------------------
        ap_mode = jsonSettings["ap_mode"];
        strlcpy(ap_ssid, jsonSettings["ap_ssid"], sizeof(ap_ssid));
        strlcpy(ap_password, jsonSettings["ap_password"], sizeof(ap_password));
        ap_visibility = jsonSettings["ap_visibility"];
        ap_chanel = jsonSettings["ap_chanel"];
        ap_connect = jsonSettings["ap_connect"];
        // -------------------------------------------------------------------
        // Cloud settings.json
        // -------------------------------------------------------------------
        mqtt_cloud_enable = jsonSettings["mqtt_cloud_enable"];
        strlcpy(mqtt_user, jsonSettings["mqtt_user"], sizeof(mqtt_user));
        strlcpy(mqtt_password, jsonSettings["mqtt_password"], sizeof(mqtt_password));
        strlcpy(mqtt_server, jsonSettings["mqtt_server"], sizeof(mqtt_server));
        strlcpy(mqtt_cloud_id, jsonSettings["mqtt_cloud_id"], sizeof(mqtt_cloud_id));
        mqtt_port = jsonSettings["mqtt_port"];
        mqtt_retain = jsonSettings["mqtt_retain"];
        mqtt_qos = jsonSettings["mqtt_qos"];
        mqtt_time_send = jsonSettings["mqtt_time_send"];
        mqtt_time_interval = jsonSettings["mqtt_time_interval"];
        mqtt_status_send = jsonSettings["mqtt_status_send"];

        // -------------------------------------------------------------------
        // Time settings.json
        // -------------------------------------------------------------------
        time_ajuste = jsonSettings["time_ajuste"];
        strlcpy(time_date, jsonSettings["time_date"], sizeof(time_date));
        time_z_horaria = jsonSettings["time_z_horaria"];
        strlcpy(time_server, jsonSettings["time_server"], sizeof(time_server));

        // -------------------------------------------------------------------
        // Dht temperatura settings.json
        // -------------------------------------------------------------------
        TEMP_AMB = jsonSettings["TEMP_AMB"].as<int>();
        HUM_AMB = jsonSettings["HUM_AMB"].as<int>();
        HUM_SUE = jsonSettings["HUM_SUE"].as<int>();
        HUM_SUE2 = jsonSettings["HUM_SUE2"].as<int>();
        HUM_SUE3 = jsonSettings["HUM_SUE3"].as<int>();
        HUM_SUE4 = jsonSettings["HUM_SUE4"].as<int>();
        TEMP_AMB_OFFS = jsonSettings["TEMP_AMB_OFFS"].as<int>();
        HUM_AMB_OFFS = jsonSettings["HUM_AMB_OFFS"].as<int>();
        HUM_SUE_OFFS = jsonSettings["HUM_SUE_OFFS"].as<int>();
        HUM_SUE2_OFFS = jsonSettings["HUM_SUE2_OFFS"].as<int>();
        HUM_SUE3_OFFS = jsonSettings["HUM_SUE3_OFFS"].as<int>();
        HUM_SUE4_OFFS = jsonSettings["HUM_SUE4_OFFS"].as<int>();

        // -------------------------------------------------------------------
        // Valores de consigna de temperatura y humedad settings.json
        // -------------------------------------------------------------------

        TEMP_CONEXION = jsonSettings["TEMP_CONEXION"].as<int>();
        TEMP_DESCONEXION = jsonSettings["TEMP_DESCONEXION"].as<int>();
        HUMEDAD_CONEXION = jsonSettings["HUMEDAD_CONEXION"].as<int>();
        HUMEDAD_DESCONEXION = jsonSettings["HUMEDAD_DESCONEXION"].as<int>();


        // -------------------------------------------------------------------
        // Teclado settings.json
        // -------------------------------------------------------------------

        BS_ANALOG_VALUE_PULS1 = jsonSettings["BS_ANALOG_VALUE_PULS1"].as<int>();
        BS_ANALOG_VALUE_PULS2 = jsonSettings["BS_ANALOG_VALUE_PULS2"].as<int>();
        BS_ANALOG_VALUE_PULS3 = jsonSettings["BS_ANALOG_VALUE_PULS3"].as<int>();
        BS_ANALOG_VALUE_PULS4 = jsonSettings["BS_ANALOG_VALUE_PULS4"].as<int>();
        BS_ANALOG_VALUE_PULS5 = jsonSettings["BS_ANALOG_VALUE_PULS5"].as<int>();
        BS_ANALOG_PULLUP_VALUE = jsonSettings["BS_ANALOG_PULLUP_VALUE"].as<int>();
        BUTTON_MIN_PRESSED = jsonSettings["BUTTON_MIN_PRESSED"].as<int>();
        BUTTON_LONG_PRESSED = jsonSettings["BUTTON_LONG_PRESSED"].as<int>();

        // -------------------------------------------------------------------
        // Ciclo de trabajo
        // -------------------------------------------------------------------
        ciclo = jsonSettings["ciclo"].as<boolean>();
        
        file.close();
        log("[ INFO ] Lectura de las configuraciones correcta");
        Serial.println(ciclo);
        return true;
    }
}
// -------------------------------------------------------------------
// Valores de Fábrica al settings.json
// -------------------------------------------------------------------
void settingsReset()
{
    // -------------------------------------------------------------------
    // Dispositivo settings.json
    // -------------------------------------------------------------------
    device_config_file = true;
    strlcpy(device_config_serial, deviceID().c_str(), sizeof(device_config_serial));
    strlcpy(device_id, "adminvue32", sizeof(device_id));
    strlcpy(device_old_user, "admin", sizeof(device_old_user));
    strlcpy(device_old_password, "admin", sizeof(device_old_password));
    
    // -------------------------------------------------------------------
    // WIFI Cliente settings.json
    // -------------------------------------------------------------------
    //wifi_ip_static = true;
    wifi_ip_static = false;
    strlcpy(wifi_ssid, "X_D48C", sizeof(wifi_ssid));
    strlcpy(wifi_password, "X2020", sizeof(wifi_password));
    strlcpy(wifi_ipv4, "192.168.31.240", sizeof(wifi_ipv4));
    strlcpy(wifi_subnet, "255.255.255.0", sizeof(wifi_subnet));
    strlcpy(wifi_gateway, "192.168.31.1", sizeof(wifi_gateway));
    strlcpy(wifi_dns_primary, "8.8.8.8", sizeof(wifi_dns_primary));
    strlcpy(wifi_dns_secondary, "8.8.4.4", sizeof(wifi_dns_secondary));
    
    // -------------------------------------------------------------------
    // WIFI AP settings.json
    // -------------------------------------------------------------------
    //ap_mode = false; // original
    ap_mode = true;
    strlcpy(ap_ssid, deviceID().c_str(), sizeof(ap_ssid));
    strlcpy(ap_password, "adminVUE32", sizeof(ap_password));
    ap_visibility = false;
    ap_chanel = 9;
    ap_connect = 4;
    
    // -------------------------------------------------------------------
    // Cloud settings.json
    // -------------------------------------------------------------------
    mqtt_cloud_enable = false;
    strlcpy(mqtt_user, "", sizeof(mqtt_user));
    strlcpy(mqtt_password, "", sizeof(mqtt_password));
    strlcpy(mqtt_server, "192.168.31.5", sizeof(mqtt_server));
    strlcpy(mqtt_cloud_id, deviceID().c_str(), sizeof(mqtt_cloud_id));
    mqtt_port = 1883;
    mqtt_retain = false;
    mqtt_qos = 0;
    mqtt_time_send = true;
    mqtt_time_interval = 30000;
    mqtt_status_send = true;

    // -------------------------------------------------------------------
    // Time settings.json
    // -------------------------------------------------------------------
    time_ajuste = true;
    // Original strlcpy(time_date, "2022-09-22T00:00", sizeof(time_date));
    strlcpy(time_date, "2023-07-11T00:00", sizeof(time_date));
    // Original time_z_horaria = -14400;
    time_z_horaria = 7200; // Zona horaria de España
    // Original strlcpy(time_server, "time.nist.gov", sizeof(time_server));
    strlcpy(time_server, "es.pool.ntp.org", sizeof(time_server)); // Servidor NTP España

    // -------------------------------------------------------------------
    // Dht temperatura settings.json
    // -------------------------------------------------------------------
    TEMP_AMB = 0;      // valor de la temperatura ambiente
    HUM_AMB = 0;       // valor de la humedad ambiente
    HUM_SUE = 0;       // valor del sensor 1 de humedad del suelo
    HUM_SUE2 = 0;      // valor del sensor 2 de humedad del suelo
    HUM_SUE3 = 0;      // valor del sensor 3 de humedad del suelo
    HUM_SUE4 = 0;      // valor del sensor 4 de humedad del suelo
    TEMP_AMB_OFFS = 0; // offset para correción temperatura ambiente
    HUM_AMB_OFFS = 0;  // offset para correción humedad ambiente
    HUM_SUE_OFFS = 0;  // offset para la corrección del valor del sensor 1 de la humedad el suelo
    HUM_SUE2_OFFS = 0; // offset para la corrección del valor del sensor 2 de la humedad el suelo
    HUM_SUE3_OFFS = 0; // offset para la corrección del valor del sensor 3 de la humedad el suelo
    HUM_SUE4_OFFS = 0; // offset para la corrección del valor del sensor 4 de la humedad el suelo

    // -----------------------------------------------------------------------
    // Valores del teclado settings.json
    //-------------------------------------------------------------------------
    BS_ANALOG_VALUE_PULS1 = 0;     // valor de lectura analógica del pulsador 1
    BS_ANALOG_VALUE_PULS2 = 1855;  // valor de lectura analógica del pulsador 2
    BS_ANALOG_VALUE_PULS3 = 2525;  // valor de lectura analógica del pulsador 3
    BS_ANALOG_VALUE_PULS4 = 2872;  // valor de lectura analógica del pulsador 4
    BS_ANALOG_VALUE_PULS5 = 3311;  // valor de lectura analógica del pulsador 5 valor 3311
    BS_ANALOG_PULLUP_VALUE = 4095; // valor de lectura analógica sin pulsar ninguna tecla Valor 4095
    BUTTON_MIN_PRESSED = 30;       // Tiempo mínimo de pulsación del botón
    BUTTON_LONG_PRESSED = 3000;    // Tiempo pulsado para que sea larga pulsación

    // ------------------------------------------------------------------------
    // Valores ajustes regulación settings.json
    //-------------------------------------------------------------------------
    TEMP_CONEXION = 28;       // valor de temperatura para apertura trampilla
    TEMP_DESCONEXION = 20;    // valor de temperatura para cierre de la trampilla
    HUMEDAD_CONEXION = 30;    // valor humedad para conexión electroválvula
    HUMEDAD_DESCONEXION = 80; // valor humedad para conexión electroválvula

    // ------------------------------------------------------------------------
    // Ciclo Automatico/Manual True/False
    // ------------------------------------------------------------------------
    ciclo = false;

    log("[ INFO ] Se reiniciaron todos los valores por defecto");
}
// -------------------------------------------------------------------
// Guardar settings.json
// -------------------------------------------------------------------
boolean settingsSave()
{
    // StaticJsonDocument<capacitySettings> jsonSettings;
    DynamicJsonDocument jsonSettings(capacitySettings);

    File file = SPIFFS.open("/settings.json", "w+");

    if (file)
    {
        // -------------------------------------------------------------------
        // Dispositivo settings.json
        // -------------------------------------------------------------------
        jsonSettings["device_config_file"] = device_config_file;
        jsonSettings["device_config_serial"] = device_config_serial;
        jsonSettings["device_id"] = device_id;
        jsonSettings["device_old_user"] = device_old_user;
        jsonSettings["device_old_password"] = device_old_password;
        // -------------------------------------------------------------------
        // WIFI Cliente settings.json
        // -------------------------------------------------------------------
        jsonSettings["wifi_ip_static"] = wifi_ip_static;
        jsonSettings["wifi_ssid"] = wifi_ssid;
        jsonSettings["wifi_password"] = wifi_password;
        jsonSettings["wifi_ipv4"] = wifi_ipv4;
        jsonSettings["wifi_subnet"] = wifi_subnet;
        jsonSettings["wifi_gateway"] = wifi_gateway;
        jsonSettings["wifi_dns_primary"] = wifi_dns_primary;
        jsonSettings["wifi_dns_secondary"] = wifi_dns_secondary;
        // -------------------------------------------------------------------
        // WIFI AP settings.json
        // -------------------------------------------------------------------
        jsonSettings["ap_mode"] = ap_mode;
        jsonSettings["ap_ssid"] = ap_ssid;
        jsonSettings["ap_password"] = ap_password;
        jsonSettings["ap_visibility"] = ap_visibility;
        jsonSettings["ap_chanel"] = ap_chanel;
        jsonSettings["ap_connect"] = ap_connect;
        // -------------------------------------------------------------------
        // Cloud settings.json
        // -------------------------------------------------------------------
        jsonSettings["mqtt_cloud_enable"] = mqtt_cloud_enable;
        jsonSettings["mqtt_user"] = mqtt_user;
        jsonSettings["mqtt_password"] = mqtt_password;
        jsonSettings["mqtt_server"] = mqtt_server;
        jsonSettings["mqtt_cloud_id"] = mqtt_cloud_id;
        jsonSettings["mqtt_port"] = mqtt_port;
        jsonSettings["mqtt_retain"] = mqtt_retain;
        jsonSettings["mqtt_qos"] = mqtt_qos;
        jsonSettings["mqtt_time_send"] = mqtt_time_send;
        jsonSettings["mqtt_time_interval"] = mqtt_time_interval;
        jsonSettings["mqtt_status_send"] = mqtt_status_send;

        // -------------------------------------------------------------------
        // TIME settings.json
        // -------------------------------------------------------------------
        jsonSettings["time_ajuste"] = time_ajuste;
        jsonSettings["time_date"] = time_date;
        jsonSettings["time_z_horaria"] = time_z_horaria;
        jsonSettings["time_server"] = time_server;

        // -------------------------------------------------------------------
        // Dht settings.json
        // -------------------------------------------------------------------
        jsonSettings["TEMP_AMB"] = TEMP_AMB;
        jsonSettings["HUM_AMB"] = HUM_AMB;
        jsonSettings["HUM_SUE"] = HUM_SUE;
        jsonSettings["HUM_SUE2"] = HUM_SUE2;
        jsonSettings["HUM_SUE3"] = HUM_SUE3;
        jsonSettings["HUM_SUE4"] = HUM_SUE4;
        jsonSettings["TEMP_AMB_OFFS"] = TEMP_AMB_OFFS;
        jsonSettings["HUM_AMB_OFFS"] = HUM_AMB_OFFS;
        jsonSettings["HUM_SUE_OFFS"] = HUM_SUE_OFFS;
        jsonSettings["HUM_SUE2_OFFS"] = HUM_SUE2_OFFS;
        jsonSettings["HUM_SUE3_OFFS"] = HUM_SUE3_OFFS;
        jsonSettings["HUM_SUE4_OFFS"] = HUM_SUE4_OFFS;

        // -------------------------------------------------------------------
        // Teclado settings.json
        // -------------------------------------------------------------------
        jsonSettings["BS_ANALOG_VALUE_PULS1"] = BS_ANALOG_VALUE_PULS1;
        jsonSettings["BS_ANALOG_VALUE_PULS2"] = BS_ANALOG_VALUE_PULS2;
        jsonSettings["BS_ANALOG_VALUE_PULS3"] = BS_ANALOG_VALUE_PULS3;
        jsonSettings["BS_ANALOG_VALUE_PULS4"] = BS_ANALOG_VALUE_PULS4;
        jsonSettings["BS_ANALOG_VALUE_PULS5"] = BS_ANALOG_VALUE_PULS5;
        jsonSettings["BS_ANALOG_PULLUP_VALUE"] = BS_ANALOG_PULLUP_VALUE;
        jsonSettings["BUTTON_MIN_PRESSED"] = BUTTON_MIN_PRESSED;
        jsonSettings["BUTTON_LONG_PRESSED"] = BUTTON_LONG_PRESSED;

        // ------------------------------------------------------------------------
        // Valores ajustes regulación settings.json
        //-------------------------------------------------------------------------
        jsonSettings["TEMP_CONEXION"] = TEMP_CONEXION;
        jsonSettings["TEMP_DESCONEXION"] = TEMP_DESCONEXION;
        jsonSettings["HUMEDAD_CONEXION"] = HUMEDAD_CONEXION;
        jsonSettings["HUMEDAD_DESCONEXION"] = HUMEDAD_DESCONEXION;

        // ------------------------------------------------------------------------
        // Valores ciclo de trabajo settings.json
        //-------------------------------------------------------------------------

        jsonSettings["ciclo"] = ciclo;


        serializeJsonPretty(jsonSettings, file);
        file.close();
        log("[ INFO ] Configuración Guardada correctamente");
        serializeJsonPretty(jsonSettings, Serial);
        return true;
    }
    else
    {
        log("[ ERROR ] Falló el guardado de la configuración");
        return false;
    }
}