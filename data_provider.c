/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "data_provider.h"
#include "doser_system.h"
#include "sensor.h"

/**
 * Create data provider from a sensor's data source and settings.
 * Readings from sensors are processed and stored in the data provider's buffer.
 */
 int8_t create_data_provider(struct doser_t *doser, struct data_source_t *source, char *name, 
                             uint8_t type, uint32_t num_data_points, uint8_t data_input_time_range,
                             uint8_t data_update_rate, uint8_t output_time_per_point) {
    struct data_provider_t *data_provider;
    struct data_provider_t **tmp_ptr;
    
    if (doser == NULL) {
        printf("error: doser is empty\n");
        return -1;
    }           
    
    data_provider = malloc(sizeof(struct data_provider_t));
    if (data_provider == NULL) {
        printf("error: malloc data_provider\n");
        return -1;
    }

    data_provider->type = type;
    data_provider->data_sources = malloc(sizeof(struct data_source_t *));
    data_provider->data_sources[0] = source;
    data_provider->num_data_sources = 1;
    data_provider->data_points = malloc(sizeof(struct readings_t));
    if (data_provider->data_points == NULL) {
        printf("error: malloc data_provider->data_points\n");
        return -1;
    }
    if (type == DATA_TYPE_FLOAT) {
        data_provider->data_points->data = malloc(sizeof(float) * num_data_points);
    } else if (type == DATA_TYPE_UINT8) {
        data_provider->data_points->data = malloc(sizeof(uint8_t) * num_data_points);
    }
    if (data_provider->data_points == NULL) {
        printf("error: malloc data_provider->data_points\n");
        return -1;
    }
    data_provider->data_points->buffer_size = num_data_points;
    data_provider->data_points->num_readings = 0;
    data_provider->data_points->latest = -1;

    data_provider->name = malloc(sizeof(char) * (strlen(name) + 1));
    if (data_provider->name == NULL) {
        printf("error: malloc data_provider->name\n");
        return -1;
    }
    strcpy(data_provider->name, name);
    data_provider->name[strlen(name)] = '\0';
    
    data_provider->data_input_time_range = data_input_time_range;
    data_provider->data_update_rate = data_update_rate;
    data_provider->output_time_per_point = output_time_per_point;
    
/*  data_provider->data_last_updated = malloc(sizeof(struct time_hhmmss));
    if (data_provider->data_last_updated == NULL) {
        printf("error: malloc data_last_updated\n");
        return -1;
    }
    data_provider->data_last_updated->hh = 0;
    data_provider->data_last_updated->mm = 0;
    data_provider->data_last_updated->ss = 0;
*/
    data_provider->data_last_updated = 0;
    tmp_ptr = realloc(doser->data_providers,
                      sizeof(struct data_provider_t *) * (doser->num_data_providers + 1));
    if (tmp_ptr == NULL) {
        printf("error: realloc tmp_ptr\n");
        return -1;
    }
    doser->data_providers = tmp_ptr;

    doser->data_providers[doser->num_data_providers] = data_provider;

    doser->num_data_providers++;

    return doser->num_data_providers;
}

/**
 * Add a data source to a data provider.
 * For example to get the average pH from multiple sensors.
 */
int8_t add_source_to_provider(struct data_provider_t *provider, struct data_source_t *source) {
    if (provider == NULL) {
        printf("error: data provider is missing\n");
        return -1;
    } else if (source == NULL) {
        printf("error: data source is missing\n");
        return -1;
    }
    struct data_source_t **tmp_ptr;
    tmp_ptr = realloc(provider, sizeof(struct data_source_t *) * (provider->num_data_sources + 1));
    if (tmp_ptr == NULL) {
        printf("error: malloc tmp_ptr\n");
        return -1;
    }
    provider->data_sources = tmp_ptr;
    provider->data_sources[provider->num_data_sources] = source;
    provider->num_data_sources++;
    
    return provider->num_data_sources;      
}

/**
 * Assign data provider to an environment.
 * 
 */
int8_t assign_data_provider(struct environment_t *parent, struct data_provider_t *data_provider) {
    struct data_provider_t **tmp_ptr;
    
    tmp_ptr = realloc(parent->data_providers,
                      sizeof(struct data_provider_t *) * (parent->num_data_providers + 1));
    if (tmp_ptr == NULL) {
        printf("error: malloc tmp_ptr\n");
        return -1;          
    }
    parent->data_providers = tmp_ptr;
    parent->data_providers[parent->num_data_providers] = data_provider;
    parent->num_data_providers++;

    return parent->num_data_providers;
}