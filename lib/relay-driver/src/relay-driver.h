#include <stdint.h>
#ifndef _RELAY_DRIVER_H_
#define _RELAY_DRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <common_headers.h>

typedef struct relay_t relay_t;
typedef enum{
    relay_OFF=0,
    relay_ON
}relay_state_t;

typedef struct 
{
    uint8_t relay_pin_number;
}relay_config_t;

error_type_t relay_init(relay_t* relay_obj);
error_type_t set_relay(relay_t* relay_obj, relay_state_t rstate);
relay_t* relay_create(const relay_config_t* config);
error_type_t relay_deinit(relay_t* relay_obj);
error_type_t relay_destroy(relay_t** relay_obj);


#ifdef __cplusplus    
}
#endif

#endif