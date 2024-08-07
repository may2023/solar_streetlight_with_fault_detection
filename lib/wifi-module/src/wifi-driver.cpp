// #include <Arduino.h>
#include "wifi-driver.h"
#include <WiFiClient.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <HTTPClient.h>
#include <FS.h>
#include<WiFi.h>

#include <stdbool.h>

struct wifi_t
{
    const char* ssid;
    const char* password;
    bool is_initialized;
    bool connected;
    
};


wifi_t* wifi_create( wifi_configuration_t* config){
    if (config == NULL)
    {
        return NULL;
    }
    wifi_t* wifi_obj = (wifi_t*)malloc(sizeof(wifi_t));
    wifi_obj->ssid = config->ssid;
    wifi_obj->password = config->password;
    wifi_obj->connected = false;
    wifi_obj->is_initialized = false;
    return wifi_obj;   
}

error_type_t wifi_connect(wifi_t* wifi_obj){
    if (wifi_obj == NULL)return ERROR_NULL_PARAMETER;
    if(!wifi_obj->is_initialized) return ERROR_INVALID_STATE;
    WiFi.begin(wifi_obj->ssid,wifi_obj->password);
    int counter = 0;
    while(WiFi.status() != WL_CONNECTED && counter++ < WIFI_CONNECT_DELAY){
        Serial.println("attempting connection");
        delay(1);
    }
    return counter >= WIFI_CONNECT_DELAY ? ERROR_FAILED : ERROR_OK;
}
error_type_t wifi_disconnect(wifi_t* wifi_obj){
    if (wifi_obj == NULL)return ERROR_NULL_PARAMETER;
    if(!wifi_obj->is_initialized) return ERROR_INVALID_STATE;
    
    return WiFi.disconnect()? ERROR_OK : ERROR_FAILED;
}


error_type_t wifi_init(wifi_t* wifi_obj){
    if (wifi_obj == NULL)return ERROR_NULL_PARAMETER;
    if(!wifi_obj->ssid || !wifi_obj->password){
        return ERROR_NULL_PARAMETER;
    }
    wifi_obj->is_initialized = true; 
    return ERROR_OK;
}
error_type_t wifi_deinit(wifi_t* wifi_obj){
    if (wifi_obj == NULL)return ERROR_NULL_PARAMETER;
    wifi_obj->is_initialized = false;
    return ERROR_OK;
}

error_type_t wifi_destroy(wifi_t** wifi_obj){
    if (*wifi_obj)
    {
        free(*wifi_obj);
        return ERROR_OK;
    }
    return ERROR_FAILED;
}


