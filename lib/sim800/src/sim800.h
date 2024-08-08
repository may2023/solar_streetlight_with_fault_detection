#include <stdint.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#elif defined(ARDUINO)
#include <"WConstants.h">
#else
#include <stdio.h>
#endif
#ifndef _sim800_H_
#define _sim800_H_
#ifdef __cplusplus
extern "C"
{
#endif
#include "common_headers.h"

    /**
     * @brief
     *
     */
    typedef struct sim800_t sim800_t;
    /**
     * @brief
     *
     */
    typedef struct
    {
        uint8_t serial_number;
        uint32_t baud_rate;
    } sim800_config_t;

    /**
     * @brief
     *
     * @param sim800_object
     * @return error_type_t
     */
    error_type_t sim800_init(sim800_t *sim800_object);
    /**
     * @brief
     *
     * @param sim800_object
     * @return error_type_t
     */
    error_type_t sim800_deinit(sim800_t *sim800_object);
    /**
     * @brief
     *
     * @param config
     * @return sim800_t*
     */
    sim800_t *sim800_create(const sim800_config_t *config);
    /**
     * @brief
     *
     * @param sim800_object
     * @param phone_number
     * @param message
     * @return error_type_t
     */
    error_type_t sim800_send_sms(sim800_t *sim800_object, const char *phone_number, const char *message);
    /**
     * @brief
     *
     * @param sim800_object
     * @return error_type_t
     */
    error_type_t sim800_connect(sim800_t *sim800_object);
    error_type_t sim800_destroy(sim800_t **sim800_object);

#ifdef __cplusplus
}
#endif
#endif