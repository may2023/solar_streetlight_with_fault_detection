#include<Arduino.h>
#include "led-driver.h"
#include<stdbool.h>

struct led_t
{
    uint8_t led_pin_number;
    bool is_initialized;
};


led_t* led_create(const led_config_t* config){
    if(config == NULL){
        // Serial.print("led is null");
        return NULL;
    }

    led_t* led_obj = (led_t*)malloc(sizeof(led_t));
    led_obj->led_pin_number = config->led_pin_number;
    return led_obj;
}

error_type_t led_init(led_t* led_obj){
    if (led_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }    
    pinMode(led_obj->led_pin_number,OUTPUT);
    led_obj->is_initialized = true;
    //   Serial.println("led is null");
    return ERROR_OK;        
}

error_type_t led_set(led_t* led_obj, led_state_t state){
    if (led_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }
    if (!led_obj->is_initialized)
    {
        return ERROR_INVALID_STATE;
    }
    Serial.printf("value to be et is %d\n",(int)state);
    digitalWrite(led_obj->led_pin_number,(int)state);
    return ERROR_OK;
} 

error_type_t led_deinit(led_t* led_obj){
    if (led_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }
    if (!led_obj->is_initialized)
    {
        return ERROR_INVALID_STATE;
    }
    led_obj->is_initialized = false;
    return ERROR_OK;
}

error_type_t led_destroy(led_t** led_obj){
    if (*led_obj)
    {
        free(*led_obj);
    }
    return ERROR_OK;
}