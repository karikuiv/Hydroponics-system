/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#pragma once

#include <stdint.h>

struct readings_t;

#include "sensor.h"

struct data_provider_t {
    uint8_t                 id;
    uint8_t                 type;                   /* TYPE_FLOAT, TYPE_U8, ... */
    char                    *name;

    uint8_t                 output_data_type;       /* f, u8, u16, ... */
    char                    *output_data_format;    /* "%.2f", "04u", ... these could be generated during runtime instead */
    uint8_t                 output_decimals;        /* 0..3? */
    uint8_t                 output_time_per_point;  /* 1..3600 seconds? */
    uint16_t                data_update_rate;       /* update frequency 1 = every second, 3600 = once an hour */
    uint16_t                data_last_updated;      /* is there a better way to track when to update the tracker? */
    uint16_t                data_input_time_range;  /* how many samples (seconds) are averaged */
    
    struct readings_t       *data_points;           /* array/circular buffer from sensor.h*/
    uint8_t                 num_data_sources;       /* for example average two pH sensors in one reservoir */
    struct data_source_t    **data_sources;         /* from sensor */
    void                    *processor;             /* function pointer? (hardcode the processing first)*/
    uint8_t                 num_parents;            /* data provider can be assigned to multiple environments */
    struct environment_t    *parents;               /* tracking data to whom */
};

/* data provider stores the processed (averaged) data points in provider->data_points */
 
 int8_t create_data_provider(struct doser_t *doser, struct data_source_t *source, char *name, 
                             uint8_t type, uint32_t num_data_points, uint8_t data_input_time_range,
                             uint8_t data_update_rate, uint8_t output_time_per_point);
int8_t add_source_to_provider(struct data_provider_t *provider, struct data_source_t *source);
int8_t assign_data_provider(struct environment_t *parent, struct data_provider_t *data_provider);