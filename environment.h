/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#pragma once

#include <stdint.h>

#include "doser_system.h"
#include "device.h"
#include "sensor.h"
#include "data_provider.h"
#include "controller.h"
#include "schedule.h"

#define ENVIRONMENT_TYPE_ENVIRONMENT    0x00
#define ENVIRONMENT_TYPE_RESERVOIR      0x01

struct environment_t {                              /* each environment tracks its own sensors, devices, etc */
    uint8_t                     id;                 /* is it necessary if using an array of at most 3? */
    char                        *name;
    uint8_t                     type;
    
    uint8_t                     num_tasks;
    struct schedule_item_t      **tasks;            /* scheduled tasks like device operation or setpoint */
    
    uint8_t                     num_devices;
    struct device_t             **devices;
    
    uint8_t                     num_sensors;
    struct sensor_t             **sensors;
    
    uint8_t                     num_controllers;
    struct controller_t         **controllers;
    
    uint8_t                     num_data_providers;
    struct data_provider_t      **data_providers;   /* should these be only listed globally by the system? */
    
    uint8_t                     num_environments;
    struct environment_t        **environments;
    
    uint8_t                     num_reservoirs;
    struct environment_t        **reservoirs;   
    
    struct environment_t        *parent;            /* only one parent, i.e. building's parent = location */
};

int8_t create_environment(struct doser_t *doser, char *name, uint8_t type);
int8_t assign_environment(struct environment_t *parent, struct environment_t *environment);