#include<stdint.h>
#ifndef _VOLTAGE_DRIVER_H_
#define _VOLTAGE_DRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif 
#include <common_headers.h>

typedef struct volt_t volt_t;

typedef struct 
{
    uint8_t voltage_pin_number;

}voltage_config_t;

error_type_t voltage_init(volt_t* volt_obj);
error_type_t voltage_read(volt_t* volt_obj, float* volt_data);
error_type_t voltage_deinit(volt_t* volt_obj);
volt_t* voltage_create(const voltage_config_t* config);

#ifdef __cplusplus
}
#endif

#endif