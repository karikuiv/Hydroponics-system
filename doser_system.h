/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#pragma once

#include <stdint.h>

struct doser_t;

#include "environment.h"
#include "device.h"
#include "sensor.h"
#include "data_provider.h"
#include "controller.h"
#include "schedule.h"

#define DATA_TYPE_FLOAT 0x00
#define DATA_TYPE_UINT8 0x01

#define COMMAND_DEVICE_NOP  0x00
#define COMMAND_DEVICE_OFF  0x01
#define COMMAND_DEVICE_ON   0x02
#define SYSTEM_NUM_COMMANDS 0x03

struct doser_t {
    uint8_t                 num_environments;
    struct environment_t    **environments;
    
    uint8_t                 num_reservoirs;
    struct environment_t    **reservoirs;
    
    uint8_t                 num_devices;
    struct device_t         **devices;
    
    uint8_t                 num_sensors;
    struct sensor_t         **sensors;
    
    uint8_t                 num_tasks;
    struct schedule_item_t  **tasks;
    
    uint8_t                 num_controllers;
    struct controller_t     **controllers;
    
    uint8_t                 num_data_providers;
    struct data_provider_t  **data_providers;

    uint8_t                 num_data_sources;
    struct data_source_t    **data_sources;
    
    uint8_t                 num_nodes;
    struct node_t           **nodes;
    
    struct timers_t         *timers;
    
    /* every important data structure that is stored */
    /* sort by how? */
};

struct timers_t {
    struct tm               *system_datetime;
    struct time_hhmmss      *system_time;
};

struct ip_addr_t {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
};

struct node_t {
    uint8_t                 id;
    char                    *name;
    
    struct ip_addr_t        ip_addr;
    struct ip_addr_t        subnet_mask;
    
    uint8_t                 num_devices;
    struct device_t         *devices;                   /* array of pointers to items on the master device list */  
    
    uint8_t                 num_sensors;
    struct sensor_t         *sensors;                   /* array of pointers to items on the master sensor list */
};

int8_t load_node(struct doser_t *doser, char *name, uint8_t a, uint8_t b, uint8_t c, uint8_t d);
int8_t add_result_to_readings(struct readings_t *readings, uint8_t type, void *result);
int8_t get_avg_of_range_from_readings(struct readings_t *readings, uint8_t data_type,
                                      uint32_t begin, uint32_t end, float *result);
int8_t load_readings(struct doser_t *doser);
int8_t data_acquisition(struct doser_t *doser);
int8_t update_data_providers(struct doser_t *doser);
int8_t process_scheduled_tasks(struct doser_t *doser);
int8_t device_control(struct device_t *device, int8_t command);
/*
uint8_t msg_node (struct node_t *provider, uint8_t cmd, uint8_t data);
*/
int8_t update_system_time(struct doser_t *doser);