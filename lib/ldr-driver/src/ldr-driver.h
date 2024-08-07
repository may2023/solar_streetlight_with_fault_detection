#include<stdint.h>
#ifndef _LDR_DRIVER_H_
#define _LDR_DRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <common_headers.h>

typedef struct ldr_t ldr_t;

typedef struct 
{
    uint8_t ldr_pin_number;
}ldr_config_t;

error_type_t ldr_init(ldr_t* ldr_obj);
error_type_t ldr_read(ldr_t* ldr_obj, int* data);
ldr_t* ldr_create(const ldr_config_t* config);
error_type_t ldr_deinit(ldr_t* ldr_obj);
error_type_t ldr_destroy(ldr_t** ldr_obj);


#ifdef __cplusplus    
}
#endif

#endif