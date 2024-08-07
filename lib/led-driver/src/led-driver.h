#include<stdint.h>
#ifndef _led_DRIVER_H_
#define _led_DRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <common_headers.h>

typedef struct led_t led_t;
typedef enum{
    LED_OFF=0,
    LED_ON
}led_state_t;
typedef struct 
{
    uint8_t led_pin_number;
}led_config_t;

error_type_t led_init(led_t* led_obj);
error_type_t led_set(led_t* led_obj, led_state_t state);
led_t* led_create(const led_config_t* config);
error_type_t led_deinit(led_t* led_obj);
error_type_t led_destroy(led_t** led_obj);


#ifdef __cplusplus    
}
#endif

#endif