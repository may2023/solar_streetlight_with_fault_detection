#include <Arduino.h>
#include  "relay-driver.h"
#include<stdbool.h>

#define RELAY_PIN_NUMBER 32

struct relay_t
{
    uint8_t relay_pin_number;
    bool initialized;
};

relay_t* relay_create(const relay_config_t* config){
    if (config == NULL)
    {
        return NULL;
    }

    relay_t* relay_obj = (relay_t*)malloc(sizeof(relay_t));
    relay_obj->relay_pin_number = config->relay_pin_number;
    return relay_obj;
    
}

error_type_t relay_init(relay_t* relay_obj){
    if (relay_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }
    pinMode(relay_obj->relay_pin_number, OUTPUT);
    relay_obj->initialized = true;
    return ERROR_OK;
    
}

error_type_t set_relay(relay_t* relay_obj, relay_state_t rstate){
    if (relay_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }

    if (!relay_obj->initialized)
    {
        return ERROR_INVALID_STATE;
    }
    digitalWrite(relay_obj->relay_pin_number, (int)rstate);
    return ERROR_OK;
}

error_type_t relay_deinit(relay_t* relay_obj){
    if (relay_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }

     if (!relay_obj->initialized)
    {
        return ERROR_INVALID_STATE;
    }
    return ERROR_OK;
}

error_type_t relay_destroy(relay_t** relay_obj){
    if (*relay_obj)
    {
        free(*relay_obj);
    }
    return ERROR_OK;
}