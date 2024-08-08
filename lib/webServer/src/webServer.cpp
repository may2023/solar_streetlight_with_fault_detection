//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "webServer.h"
#include "ldr-driver.h"
#include "voltage-driver.h"
#include "dust_sensor.h"
#include "lm75-driver.h"
AsyncWebServer* server;
static webserver_t* instance;
struct webserver_t
{
    bool is_initialized;
    uint16_t port_number;
    webserservercallback callback;

};

webserver_t* webserver_create(webserver_config_t* config)
{
    webserver_t* webserver_obj = (webserver_t*)malloc(sizeof(webserver_t));
    instance = webserver_obj;
    webserver_obj->port_number = config->port_number;
    webserver_obj->callback = config->callback;
    webserver_obj->is_initialized = false;
    return webserver_obj;   
}
error_type_t webserver_init(webserver_t* webserver_obj){
    if (webserver_obj == NULL)return ERROR_NULL_PARAMETER;

    webserver_obj->is_initialized = true; 
    return ERROR_OK; 
}
error_type_t webserver_begin(webserver_t* webserver_obj)
{
    if (webserver_obj == NULL)return ERROR_NULL_PARAMETER;
    if(!webserver_obj->is_initialized) return ERROR_INVALID_STATE;
    server = new AsyncWebServer(webserver_obj->port_number);
    Serial.printf("webserver object is at %ld\n",instance);
    server->on("/getData", HTTP_GET,[&](AsyncWebServerRequest *request) 
    {
        //getData();
        Serial.printf("end point hit\n");
        Serial.printf("webserver object is at %ld\n",instance);
        String data = instance->callback();
        // float lm75_value;
        // error_type_t err = get_lm75_data(lm75_value);
        // if(err == ERROR_OK){
        //     Serial.printf("value red is: %f\n",lm75_value);
        // }else{
        //     Serial.println("Error occurred");
        // }
        
        request->send(200, "text/plain", data);
        });
    server->begin();
    return ERROR_OK;

}
error_type_t webserver_end(webserver_t* webserver_obj){
    if (webserver_obj == NULL)return ERROR_NULL_PARAMETER;
    if(!webserver_obj->is_initialized) return ERROR_INVALID_STATE;
    server->end();
    return ERROR_OK;

}
error_type_t webserver_deinit(webserver_t* webserver_obj){
    if (webserver_obj == NULL)return ERROR_NULL_PARAMETER;
    if(!webserver_obj->is_initialized) return ERROR_INVALID_STATE;
     webserver_obj->is_initialized = false;
    server->end();
     return ERROR_OK;
}
error_type_t webserver_destroy(webserver_t** webserver_obj){
    if(*webserver_obj){
        free(*webserver_obj);
        return ERROR_OK;
    }
    return ERROR_NULL_PARAMETER;
}



// dust_t* dust_readings;
// // ldr_t* my_ldr_driver,
// volt_t* volt_readings;


// void webserver_Send_data(lm75_t* lm75){
//      StaticJsonDocument<200> json;
//   json["sensor_data"]["temperature"] =lm75_read(lm75) ;
// //   json["sensor_data"]["ldr"] = ;
// //   json["sensor_data"]["dust sensor"] = ;
// //   json["sensor_data"]["voltage sensor"] = ;
//   json["device_info"]["device_id"] = "";
//   json["device_info"]["firmware_version"] = "";
//   String jsonString;
//   json.printTo(jsonString);
//   server.on("/senddata", HTTP_GET, [jsonString](AsyncWebServerRequest *request) {
//         request->send(200, "application/json", jsonString);
//     });
// }









