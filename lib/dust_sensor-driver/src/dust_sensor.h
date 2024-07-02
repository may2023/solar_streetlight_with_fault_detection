#include <stdint.h>
#ifndef _DUST_SENSOR_H_
#define _DUST_SENSOR_H_
#ifdef ___cpluscplus
extern "C"{
    #endif
    #include <common_headers.h>

    typedef struct dust_t dust_t;
    typedef struct 
    {
        uint8_t vout_pin_number;
        uint8_t led_pin_number;
    }dust_config_t;

    error_type_t dust_init(dust_t* dust_obj);
    error_type_t dust_read(dust_t* dust_obj, float* dust_data);
    error_type_t dust_deinit(dust_t* dust_obj);
    dust_t* dust_create(const dust_config_t* config);
    
#ifdef ___cplusplus    
}
#endif
#endif
