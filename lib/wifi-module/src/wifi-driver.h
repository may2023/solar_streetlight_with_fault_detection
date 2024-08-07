#ifndef _WIFI_DRIVER_H_
#define _WIFI_DRIVER_H_
#include<WiFi.h>
#include <common_headers.h>
#define WIFI_CONNECT_DELAY 30
    typedef struct wifi_t wifi_t;

    typedef struct 
    {
         const char *ssid ;
         const char *password ;        
    }wifi_configuration_t;

    wifi_t* wifi_create( wifi_configuration_t* config);
    error_type_t wifi_init(wifi_t* wifi_obj);
    error_type_t wifi_connect(wifi_t* wifi_obj);
    error_type_t wifi_disconnect(wifi_t* wifi_obj);
    error_type_t wifi_deinit(wifi_t* wifi_obj);
    error_type_t wifi_destroy(wifi_t** wifi_obj);


    
#endif


