#ifndef _LM75_DRIVER_H_
#define _LM75_DRIVER_H_
#ifdef __cplusplus
extern "C"{
    #endif
    #include<Arduino.h>
    #include <stdint.h>
    #include "common_headers.h"
     typedef enum
    {
        LM75_RESOLUTION_9_BITS = 0,
        LM75_RESOLUTION_10_BITS,
        LM75_RESOLUTION_11_BITS,
        LM75_RESOLUTION_12_BITS,

    } lm75_resolution_t;
    typedef enum
    {
        LM75_COMPARATOR_MODE = 0,
        LM75_INTERRUPT_MODE,
    } lm75_mode_t;
    typedef enum
    {
        LM75_TOLERANCE_ONE_SAMPLE = 0,
        LM75_TOLERANCE_TWO_SAMPLES,
        LM75_TOLERANCE_FOUR_SAMPLES,
        LM75_TOLERANCE_SIX_SAMPLES

    } lm75_tolerance_t;

    typedef enum
    {
        LM75_POLARITY_ACTIVE_LOW = 0,
        LM75_POLARITY_ACTIVE_HIGH

    } lm75_polarity_t;

    typedef enum
    {
        LM75_REGISTER_TEMP = 0,
        LM75_REGISTER_CONFIG,
        LM75_REGISTER_THYST,
        LM75_REGISTER_TOS

    } lm75_register_t;


    typedef struct {
        struct {
            uint8_t shutdown : 1;
            uint8_t thermostat_mode : 1;
            uint8_t thermostat_output_polarity : 1;
            uint8_t thermostat_fault_tolerance : 2;
            uint8_t resolution : 2;
            uint8_t reserved : 1;
        } mbits;

        uint8_t mbyte;
        
    } lm75_CfgRegister;

    typedef struct
    {
        uint8_t i2c_addr;
        lm75_mode_t mode;
        uint8_t os_pin_number;
        lm75_polarity_t polarity;
        lm75_tolerance_t tolerance;
        lm75_resolution_t resolution;
    } lm75_config_t;

    typedef struct lm75_t lm75_t;
    error_type_t lm75_init(lm75_t *lm75_object);
    error_type_t lm75_deinit(lm75_t *lm75_object);
    lm75_t *lm75_create(const lm75_config_t *config);
    error_type_t lm75_read(lm75_t *lm75_object,float* data);
    unsigned lm75_getReg(lm75_t *lm75_object, lm75_register_t lm75_reg);
    void lm75_setReg(lm75_t *lm75_object, lm75_register_t lm75_reg, uint8_t newValue);

    void lm75_set_thresholds(lm75_t* lm75_object, float threshold_hyst, float threshold_os);
    void lm75_interrupt_handler();
    error_type_t lm75_init_comparator_mode(lm75_t* lm75_object);

 error_type_t lm75_init_interrupt_mode(lm75_t* lm75_object,void(*lm75_interrupt_handler)());


#ifdef __cplusplus    
}
#endif
#endif