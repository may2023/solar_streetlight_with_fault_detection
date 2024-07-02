#include <Arduino.h>
#include "dust_sensor.h"
#include <stdbool.h>

#define VOUT_PIN_NUMBER 15
#define LED_PIN_NUMBER  13


struct dust_t
{
    uint8_t vout_pin_number;
    uint8_t led_pin_number;
    bool read;
};

dust_t* dust_create(const dust_config_t* config){
    if (config == NULL)
    {
        // Serial.print("dust driver is null");
        return NULL;
    }

    dust_t* dust_obj = (dust_t*)malloc(sizeof(dust_t));
    dust_obj->vout_pin_number = config->vout_pin_number;
    dust_obj->led_pin_number = config->led_pin_number;
    dust_obj->read = false;
    return dust_obj;  
}

error_type_t dust_init(dust_t* dust_obj){
    if (dust_obj == NULL)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if (dust_obj->vout_pin_number != VOUT_PIN_NUMBER)
    {
        return ERROR_INVALID_PIN_NUMBER;
    }

    pinMode(dust_obj->vout_pin_number, INPUT);
    pinMode(dust_obj->led_pin_number, OUTPUT);
    dust_obj->read = true;
    // Serial.printf("dust driver is inisiated");
    return ERROR_OK;   
}

error_type_t dust_read(dust_t* dust_obj, float* dust_data){
    int value = analogRead(dust_obj->vout_pin_number);
    //formular for ADC converter
     float dust_reading = (value *3.3)/4096;
     Serial.printf("ADC reading: %f", dust_reading);
     

     //formular for dust concentration
     float dust_density = 0.20 * dust_reading - 0.1;
     *dust_data = dust_density;
     Serial.printf("dust density: %lf", *dust_data);
    // Serial.printf("dust driver can read");

     return ERROR_OK;
}

error_type_t dust_deinit(dust_t* dust_obj){
    if (dust_obj)
    {
        dust_obj->read = false;
        free(dust_obj);
    }
    // Serial.printf("dust driver is deinitsated");
    return ERROR_OK;
    
}


