/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#pragma once

#include <stdint.h>

#include "doser_system.h"
#include "environment.h"

#define SENSOR_TYPE_PH      0x01
#define SENSOR_TYPE_EC      0x02
#define SENSOR_TYPE_HUMI    0x03
#define SENSOR_TYPE_TEMP    0x04

#define SENSOR_READINGS_BUFFER_SIZE 86400               /* stores 24 hours of data at one reading per second */

struct readings_t {
    uint8_t                 id;
    uint32_t                buffer_size;                /* buffer size */
    uint32_t                num_readings;               /* maybe a counter that counts up to buf size? */
    uint32_t                latest;                     /* buffer pos, 1..buffer_size */
    void                    *data;                      /* should probably be a circular buffer for now */
};

struct data_source_t {
    uint8_t                 id;
    uint8_t                 data_type;
    uint8_t                 sensor_type;                /* obsolete when parent.type exists? */
    struct sensor_t         *parent;
    
    char                    *data_format;               /* printf formatting, ".2f, %02u, %s, etc..."? */
    uint8_t                 data_resolution_decimals;   /* do away with data_format and build it runtime? */

    struct readings_t       *readings;
};

struct sensor_t {
    uint8_t                 id;
    char                    *name;
    uint8_t                 type;                       /* temp, ph, ec, humi, combo (>1 sources) */
    uint8_t                 provider_id;
    
    uint8_t                 num_data_sources;
    struct data_source_t    **data_sources;             /* most sensors have 1 source, some 2 or more */
    struct environment_t    *parent;
    uint8_t                 num_data_providers;
    struct data_provider_t  **data_providers;           /* providers using the sensor's sources linked here? */
};

int8_t assign_sensor(struct environment_t *environment, struct sensor_t *sensor);
int8_t sensor_create_default_data_providers(struct doser_t *doser, struct sensor_t *sensor);
int8_t sensor_add_data_sources(struct doser_t *doser, struct sensor_t *sensor,
                        uint8_t sensor_type_1, uint8_t data_type_1,
                        uint8_t sensor_type_2, uint8_t data_type_2);
int8_t load_sensor(struct doser_t *doser, uint8_t sensor_provider_id, char *name,
                   uint8_t sensor_type_1, uint8_t data_type_1, uint8_t sensor_type_2, uint8_t data_type_2);
void print_data_source(struct data_source_t *data_source);
