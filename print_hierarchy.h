/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#pragma once

#include <stdint.h>

#include "doser_system.h"
#include "environment.h"

void delayms(uint16_t delay_count);
void print_tabs(uint8_t tabs);
int8_t print_status(void *task, uint8_t tabs);
int8_t print_task(struct schedule_item_t *task, uint8_t tabs);
int8_t print_devices(struct environment_t *environment, uint8_t tabs);
int8_t print_controllers(struct environment_t *reservoir, uint8_t tabs);
int8_t print_sensors(struct environment_t *environment, uint8_t tabs);
int8_t print_hierarchy(struct doser_t *doser);