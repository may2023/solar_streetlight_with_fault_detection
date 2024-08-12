#include<Arduino.h>
#include "voltage-driver.h"
#include <stdbool.h>

#define VOLTAGE_PIN_NUMBER 34
static double f13_semPow(double x);
static double poly_convert(int reading);
struct volt_t
{
    uint8_t voltage_pin_number;
    // bool read;
};

volt_t* voltage_create(const voltage_config_t* config){
    if (config == NULL)
    {
        // Serial.print("voltage driver is null");
        return NULL;
    }
    volt_t* volt_obj  = (volt_t*)malloc(sizeof(volt_t));
    volt_obj->voltage_pin_number = config->voltage_pin_number;
    // volt_obj->read = false;
    return volt_obj;
}

error_type_t voltage_init(volt_t* volt_obj){
    if (volt_obj == NULL)
    {
        return ERROR_NULL_PARAMETER;
    }
    
    if(volt_obj->voltage_pin_number != VOLTAGE_PIN_NUMBER){
        return ERROR_INVALID_PIN_NUMBER;
    }
    pinMode(volt_obj->voltage_pin_number,INPUT);
    // volt_obj->read = true;
    // Serial.print("voltage driver is null");
    return ERROR_OK;
}
error_type_t voltage_read(volt_t* volt_obj, float* volt_data){
    long sum =0;
    static int number_of_sample = 500;
    //analogSetAttenuation(ADC_0db);
    for(int x=0; x< number_of_sample; x++){
        sum+=analogRead(volt_obj->voltage_pin_number);
    }
    int voltage_value = sum/number_of_sample;
    // Serial.printf("the voltagevalue: %d\n",voltage_value);
    double converted_value = poly_convert(voltage_value);
    // Serial.printf("the converted voltagevalue: %f\n",converted_value);
    float R1 = 4700.0;
    float R2 = 100.0;

    // float normal_value = (voltage_value*3.3)/4096;
    // Serial.printf("the normal_value voltagevalue: %f\n",normal_value);
    //calculating battery voltage 
    float battery_value =  converted_value * (R1 +R2)/R2;
    *volt_data = battery_value;

    Serial.printf("Battery value in volt %f/n ", *volt_data);
    // Serial.print("voltage driver is null");
    return ERROR_OK;
}
error_type_t voltage_deinit(volt_t* volt_obj){
    if (volt_obj)
    {
        // volt_obj->read = false;
        free(volt_obj);
    }
    return ERROR_OK;   
}

static double poly_convert(int reading){
    return -0.000000000000016 * pow(reading,4) + 0.000000000118171 * pow(reading,3)- 0.000000301211691 * pow(reading,2)+ 0.001109019271794 * reading + 0.034143524634089;
} 
