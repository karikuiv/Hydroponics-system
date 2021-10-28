/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "sensor.h"
#include "doser_system.h"
#include "environment.h"

/**
 * Assign a sensor to an environment or reservoir.
 */
int8_t assign_sensor(struct environment_t *environment, struct sensor_t *sensor) {
    struct sensor_t **tmp_ptr;
    
    if (environment == NULL) {
        printf("error: environment is empty\n");
        return -1;
    } else if (sensor == NULL) {
        printf("error: sensor is empty\n");
        return -1;
    }
    
    tmp_ptr = realloc(environment->sensors, sizeof(struct sensor_t *) * (environment->num_sensors + 1));
    if (tmp_ptr == NULL) {
        printf("error: realloc tmp_ptr\n");
        return -1;
    }
    environment->sensors = tmp_ptr;
    environment->sensors[environment->num_sensors] = sensor;
    environment->num_sensors++;
    sensor->parent = environment;
    
    return environment->num_sensors;
}

/**
 * Creates two types of default data providers for the sensor:
 *  5 s running average, input  1 reading  / s, output 1 data point / s
 *      store 24 hours of data = 86400 readings
 *  1 m running average, input 60 readings / m, output 1 data point / m
 *      store a week of data = 10080 readings
 */
int8_t sensor_create_default_data_providers(struct doser_t *doser, struct sensor_t *sensor) {
    uint8_t data_provider_id;   

    sensor->num_data_providers = 0;
    sensor->data_providers = malloc(sizeof(struct data_provider_t *) * (2 * sensor->num_data_sources));

    for (uint8_t i = 0; i < sensor->num_data_sources; i++) {
        data_provider_id = create_data_provider(doser, sensor->data_sources[i], "5 second average", 
                                                sensor->data_sources[i]->data_type, 86400,
                                                5, 1, 1);

        if (data_provider_id == -1) {
            printf("error: create_data_provider()\n");
            return -1;
        }
        sensor->data_providers[(i * 2)] = doser->data_providers[data_provider_id - 1];
        
        data_provider_id = create_data_provider(doser, sensor->data_sources[i], "1 minute average",
                                                sensor->data_sources[i]->data_type, 10080,
                                                60, 60, 60);

        if (data_provider_id == -1) {
            printf("error: create_data_provider()\n");
            return -1;
        }
        sensor->data_providers[((i * 2) + 1)] = doser->data_providers[data_provider_id - 1];
        
        sensor->num_data_providers += 2;
    }
    
    return sensor->num_data_providers;
}

/**
 * Create data sources from settings and add to sensor.
 */
int8_t sensor_add_data_sources(struct doser_t *doser, struct sensor_t *sensor,
                        uint8_t sensor_type_1, uint8_t data_type_1,
                        uint8_t sensor_type_2, uint8_t data_type_2) {
    struct data_source_t **tmp_ptr_d;
    uint8_t sensor_types[2];
    uint8_t data_types[2];
    sensor_types[0] = sensor_type_1;
    sensor_types[1] = sensor_type_2;
    data_types[1] = data_type_2;
    data_types[0] = data_type_1;
    
    if (sensor_type_1 > 0) {
        sensor->num_data_sources++;
    }
    if (sensor_type_2 > 0) {
        sensor->num_data_sources++;
    }
    if (sensor->num_data_sources == 0) {
        printf("error: no data sources!\n");
        return -1;
    }   
    
    sensor->data_sources = malloc(sizeof(struct data_source_t *) * sensor->num_data_sources);
    if (sensor->data_sources == NULL) {
        printf("error: malloc sensor->data_sources\n");
        return -1;
    }
    
    tmp_ptr_d = realloc(doser->data_sources,
        sizeof(struct data_source_t *) * (doser->num_data_sources + sensor->num_data_sources));
    if (tmp_ptr_d == NULL) {
        printf("error: realloc tmp_ptr_d\n");
        return -1;
    }
    doser->data_sources = tmp_ptr_d;
    
    for (uint8_t i = 0; i < sensor->num_data_sources; i++) {    
        sensor->data_sources[i] = malloc(sizeof(struct data_source_t));
        if (sensor->data_sources[i] == NULL) {
            printf("error: malloc sensor->data_sources[%02u]\n", i);
            return -1;
        }
        sensor->data_sources[i]->sensor_type = sensor_types[i];
        sensor->data_sources[i]->data_type = data_types[i];
        sensor->data_sources[i]->parent = sensor;
        sensor->data_sources[i]->id = doser->num_data_sources + i;
        sensor->data_sources[i]->readings = malloc(sizeof(struct readings_t));
        if (sensor->data_sources[i]->readings == NULL) {
            printf("error: malloc sensor->data_sources[%02u]->readings\n", i);
            return -1;
        }
        sensor->data_sources[i]->readings->buffer_size = SENSOR_READINGS_BUFFER_SIZE;
        if (sensor->data_sources[i]->data_type == DATA_TYPE_FLOAT) {
            sensor->data_sources[i]->readings->data = malloc(sizeof(float) * sensor->data_sources[i]->readings->buffer_size);
        } else if (sensor->data_sources[i]->data_type == DATA_TYPE_UINT8) {
            sensor->data_sources[i]->readings->data = malloc(sizeof(uint8_t) * sensor->data_sources[i]->readings->buffer_size);
        }
        if (sensor->data_sources[i]->readings->data == NULL) {
            printf("error: malloc sensor->data_sources[%02u]->readings->data\n", i);
            return -1;
        }
        sensor->data_sources[i]->readings->num_readings = 0;
        sensor->data_sources[i]->readings->latest = -1;

        doser->data_sources[doser->num_data_sources + i] = sensor->data_sources[i];
    }
    doser->num_data_sources += sensor->num_data_sources;    
    
    return sensor->num_data_sources;
}

/**
 * Create sensor from settings and add to system.
 */
int8_t load_sensor(struct doser_t *doser, uint8_t sensor_provider_id, char *name,
                 uint8_t sensor_type_1, uint8_t data_type_1, uint8_t sensor_type_2, uint8_t data_type_2) {
                     
    struct sensor_t **tmp_ptr;
    struct sensor_t *sensor;
    
    if (doser == NULL) {
        printf("error: doser is missing\n");
        return -1;
    } else if (name == NULL) {
        printf("error: name is missing\n");
        return -1;
    } else if (strlen(name) < 1) {
        printf("error: name is empty\n");
        return -1;
    }
    
    tmp_ptr = realloc(doser->sensors, sizeof(struct sensor_t *) * (doser->num_sensors + 1));
    if (tmp_ptr == NULL) {
        printf("error: malloc tmp_ptr\n");
        return -1;
    }
    doser->sensors = tmp_ptr;
    
    sensor = malloc(sizeof(struct sensor_t));
    if (sensor == NULL) {
        printf("error: malloc sensor\n");
        return -1;
    }
    sensor->provider_id = sensor_provider_id;
    sensor->id = doser->num_sensors;
    
    sensor->name = malloc(sizeof(char) * (strlen(name) + 1));
    if (sensor->name == NULL) {
        printf("error: malloc\n");
        return -1;
    }
    strcpy(sensor->name, name);
    sensor->name[strlen(name)] = '\0';
    sensor->num_data_sources = 0;
    
    int8_t ret = 0;
    ret = sensor_add_data_sources(doser, sensor, sensor_type_1, data_type_1, sensor_type_2, data_type_2);
    if(ret != sensor->num_data_sources) {
        printf("error: sensor data source mismatch: ret: %02u num: %02u\n", ret, sensor->num_data_sources);
        return -1;
    }
    ret = sensor_create_default_data_providers(doser, sensor);
    if (ret != (2 * sensor->num_data_sources)) {
        printf("error: default data provider mismatch\n");
        return -1;
    } else {
        printf("Created %02u default data providers for %02u data sources for %s\n",
                    sensor->num_data_providers, sensor->num_data_sources, sensor->name);
    }
    
    doser->sensors[doser->num_sensors] = sensor;
    doser->num_sensors++;

    return doser->num_sensors;
}

/**
 * Print all readings from a data source's buffer. Probably unneeded.
 */
void print_data_source(struct data_source_t *data_source) {
    /* TODO: remove? */
    for(int i = 0; i < data_source->readings->num_readings; i++) {
        printf("%04u:", i);     
        if (data_source->data_type == DATA_TYPE_FLOAT) {
            printf(data_source->data_format, ((float *)data_source->readings->data)[i]);
            printf("\n");
        } else if (data_source->data_type == DATA_TYPE_UINT8) {
            printf(data_source->data_format, ((uint8_t *)data_source->readings->data)[i]);
            printf("\n");
        }
    }
}