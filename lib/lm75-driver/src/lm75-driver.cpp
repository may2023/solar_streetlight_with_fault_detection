#include <Arduino.h>
#include <Wire.h>
#include "lm75-driver.h"
#include <stdbool.h>
#include "common_headers.h"

struct lm75_t {
        uint8_t i2c_addr;
        uint8_t os_pin_number;
        lm75_mode_t mode;
        lm75_polarity_t polarity;
        lm75_tolerance_t tolerance;
        lm75_resolution_t resolution;
        lm75_CfgRegister lm_register;
        bool initialized;
        bool has_isr;

    };


lm75_t* lm75_create(const lm75_config_t* config){
    if(config == NULL)return NULL;
    lm75_t*new_lm75_2_obj = (lm75_t*) malloc(sizeof(lm75_t));
    new_lm75_2_obj->i2c_addr = config-> i2c_addr;
    new_lm75_2_obj->mode = config->mode;
    new_lm75_2_obj->os_pin_number = config->os_pin_number;  // Initialize other fields as needed
    new_lm75_2_obj->initialized = false;
    new_lm75_2_obj->has_isr = false;
    return new_lm75_2_obj;
}

 error_type_t lm75_init_comparator_mode(lm75_t* lm75_object){
    if(lm75_object == NULL)return ERROR_INVALID_PARAMETER;
   
    lm75_object->initialized = true;
    Wire.begin();
    return ERROR_OK;
}

 error_type_t lm75_init_interrupt_mode(lm75_t* lm75_object,void(*lm75_interrupt_handler)()){
    if(lm75_object == NULL)return ERROR_INVALID_PARAMETER;
    attachInterrupt(digitalPinToInterrupt(lm75_object->os_pin_number), lm75_interrupt_handler, FALLING);
    lm75_object->initialized = true;
    lm75_object->has_isr = true;
    Wire.begin();
    return ERROR_OK;
}

error_type_t lm75_deinit(lm75_t* lm75_object){
    if(lm75_object){
        lm75_object->initialized = false;
        if(lm75_object->has_isr){
            detachInterrupt(lm75_object->os_pin_number);
        }
        free(lm75_object);
    }
    return ERROR_OK;
}

error_type_t lm75_read(lm75_t *lm75_object,float* data) {
    if (lm75_object == NULL || !lm75_object->initialized) {
        // Handle uninitialized object or other error conditions
        return ERROR_INVALID_TEMPERATURE;
    }
    // Read temperature from the LM75 sensor
    unsigned temp_register = lm75_getReg(lm75_object, LM75_REGISTER_TEMP);

    // Perform sign extension for 16-bit value
    int16_t temperature;
    if (temp_register & 0x8000) {
        temperature = temp_register | 0xFFFF0000;
    } else {
        temperature = temp_register & 0x0000FFFF;
    }

    // Convert to Celsius
    float temperatureCelsius = temperature / 256.0;
  
    *data = temperatureCelsius;
    Serial.printf("temperature celcious is %f\n",*data);
    return ERROR_OK;
}


void lm75_set_thresholds(lm75_t* lm75_object, float threshold_hyst, float threshold_os) {
    if (lm75_object == NULL || !lm75_object->initialized) {
        // Handle uninitialized object or other error conditions
        return;
    }

    // Convert temperature thresholds to the format expected by LM75
    uint8_t thyst_value = (uint8_t)(threshold_hyst * 256.0);
    uint8_t tos_value = (uint8_t)(threshold_os * 256.0);

    // Set the threshold values in the LM75 registers
    lm75_setReg(lm75_object, LM75_REGISTER_THYST, thyst_value);
    lm75_setReg(lm75_object, LM75_REGISTER_TOS, tos_value);
}


unsigned lm75_getReg(lm75_t *lm75_object, lm75_register_t lm75_reg)
{
    uint16_t result = 0xFFFF;
    Wire.beginTransmission(lm75_object->i2c_addr);
    Wire.write((uint8_t)lm75_reg);
    Wire.endTransmission();
    Wire.requestFrom(lm75_object->i2c_addr, (uint8_t)(lm75_reg == LM75_REGISTER_CONFIG ? 1 : 2));

    if (Wire.available())
    {
        result = Wire.read();
        if (lm75_reg != LM75_REGISTER_CONFIG)
        {
            result = (result << 8) | Wire.read();
        }
    }

    return result;
}
void lm75_setReg(lm75_t *lm75_object, lm75_register_t lm75_reg, uint8_t newValue)
{
    Wire.beginTransmission(lm75_object->i2c_addr);
    Wire.write((uint8_t)lm75_reg);
    Wire.write(newValue);
    Wire.endTransmission();
}




