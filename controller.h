/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#pragma once

#include <stdint.h>

#include "doser_system.h"
#include "environment.h"
#include "controller.h"

#define CONTROLLER_TYPE_PH  0x00
#define CONTROLLER_TYPE_EC  0x01

struct controller_t {
    uint8_t                 id;
    char                    *name;
    char                    type;           /* CONTROLLER_TYPE_X */
    void                    *settings;      /* ph_controller_t, ec_controller_t */
    struct data_provider_t  *data_provider; /* sensor data for the controller */
    struct environment_t    *parent;
    uint8_t                 num_attached_tasks;
    struct schedule_item_t  **attached_tasks;
};

/* are there any pH controller specific stuff that must be tracked? */
/* dosing settings, for example different amount based on reservoir size... */
/* not to mention if the system is adaptive and learns from feedback... */

/* TODO: move these to ph_controller.h and ec_controller.h and retain these names */

struct ph_controller_t {
    float setpoint;             /* absolute value, probably around 5.70 - 6.00 depending on stuff */
    float dosing_threshold;     /* relative value, recommended range 0.05 - 0.10 */
    uint8_t stabilization_time; /* minutes, recommended range 3 - 5 minutes */
};

/* are there any EC controller stuff that does not exist in ph controller and must be tracked? */
/* dosing settings, for example different amount based on reservoir size... */
struct ec_controller_t {
    float setpoint;             /* absolute value probably around 0.20 - 1.0 depending on stuff */
    float dosing_threshold;     /* relative value, recommended range 0.02 - 0.05 */
    uint8_t stabilization_time; /* minutes, recommended range 3 - 5 minutes */
};

int8_t create_controller(struct doser_t *doser, char type, char* name);
int8_t assign_controller (struct environment_t *parent, struct controller_t *controller);
