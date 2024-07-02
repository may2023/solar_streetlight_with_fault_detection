#ifndef _COMMON_HEADERS_H_
#define _COMMON_HEADERS_H_
#ifdef __cplusplus
extern "C"
{
#endif
#define SENSOR_INVALID_VALUE (-1)
    typedef enum
    {
        ERROR_OK,
        ERROR_NULL_PARAMETER,
        ERROR_INVALID_PARAMETER,
        ERROR_INVALID_PIN_NUMBER,
        ERROR_INVALID_STATE,
        ERROR_INVALID_MODE,
        ERROR_INVALID_TEMPERATURE,
        ERROR_FAILED,
        ERROR_BUFFER_OVERFLOW,
        ERROR_TIMED_OUT
        

    } error_type_t;
  


#ifdef __cplusplus
}
#endif
#endif