#ifndef _WEBSERVER_
#define _WEBSERVER_
#ifdef __cplusplus
extern "C"{
    #endif
    #include <common_headers.h>
    // #include "ldr-driver.h"
    //#include "lm75-driver.h"


    // void begin_webserver(lm75_t* lm75);
    // void webserver_send_data(lm75_t* lm75);

    typedef struct webserver_t webserver_t;

    typedef String(*webserservercallback)(void);
    typedef struct {
        uint16_t port_number;
        webserservercallback callback;
    } webserver_config_t;

    webserver_t* webserver_create(webserver_config_t* config);
    error_type_t webserver_init(webserver_t* webserver_obj);
    error_type_t webserver_begin(webserver_t* webserver_obj);
    error_type_t webserver_end(webserver_t* webserver_obj);
    error_type_t webserver_deinit(webserver_t* webserver_obj);
    error_type_t webserver_destroy(webserver_t** webserver_obj);



#ifdef __cplusplus    
}
#endif
#endif