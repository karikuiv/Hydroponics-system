/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#pragma once

#include <stdint.h>

#include "doser_system.h"
#include "device.h"

const char *sensor_type_strings[] = {"\0", "pH", "EC", "Humi", "Temp"};

#define NODE_HUB_1  0x01
#define NODE_HUB_2  0x02

int8_t load_nodes(struct doser_t *doser);
int8_t load_devices(struct doser_t *doser);
int8_t load_sensors(struct doser_t *doser);
int8_t create_environments(struct doser_t *doser);
int8_t setup_test_environment(struct doser_t *doser);
int8_t create_tasks(struct doser_t *doser);
uint8_t print_systen_lists(struct doser_t *doser);
int8_t update_ui(struct doser_t *doser);