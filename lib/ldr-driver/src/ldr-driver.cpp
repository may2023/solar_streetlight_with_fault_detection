#include<Arduino.h>
#include "ldr-driver.h"
#include<stdbool.h>

#define LDR_PIN_NUMBER 33

struct ldr_t
{
    uint8_t ldr_pin_number;
    bool is_initialized;
};


ldr_t* ldr_create(const ldr_config_t* config){
    if(config == NULL){
        // Serial.print("ldr is null");
        return NULL;
    }

    ldr_t* ldr_obj = (ldr_t*)malloc(sizeof(ldr_t));
    ldr_obj->ldr_pin_number = config->ldr_pin_number;
    return ldr_obj;
}

error_type_t ldr_init(ldr_t* ldr_obj){
    if (ldr_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }    
    pinMode(ldr_obj->ldr_pin_number,INPUT);
    ldr_obj->is_initialized = true;
    //   Serial.println("ldr is null");
    return ERROR_OK;        
}

error_type_t ldr_read(ldr_t* ldr_obj,int* data){
    if (ldr_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }
    if (!ldr_obj->is_initialized)
    {
        return ERROR_INVALID_STATE;
    }
    int value = analogRead(ldr_obj->ldr_pin_number);
    *data = value;
    Serial.print("value: ");
    Serial.println(value);
    return ERROR_OK;
} 

error_type_t ldr_deinit(ldr_t* ldr_obj){
    if (ldr_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }
    if (!ldr_obj->is_initialized)
    {
        return ERROR_INVALID_STATE;
    }
    ldr_obj->is_initialized = false;
    return ERROR_OK;
}

error_type_t ldr_destroy(ldr_t** ldr_obj){
    if (*ldr_obj)
    {
        free(*ldr_obj);
    }
    return ERROR_OK;
}