#pragma once

#include <stdint.h>

#include "doser_system.h"
#include "environment.h"

extern struct doser_t doser;

struct device_t {
    uint8_t                 id;                 /* internal id used by daemon. for now the index in an array */
    char                    *name;
    uint8_t                 provider_id;        /* id of node that provides the device. 0 = internal */

    uint8_t                 status;             /* schedule.h: TASK_STATUS_X & TASK_STATUS_STRINGS */
    
    uint8_t                 num_parents;
    struct environment_t    **parents;          /* a device can be assigned to multiple environments */

    uint8_t                 num_attached_tasks;
    struct schedule_item_t  **attached_tasks;   /* array or list of tasks that handle the device */
};

int8_t assign_device(struct environment_t *environment, struct device_t *device);
int8_t load_device(struct doser_t *doser, uint8_t provider_id, char *name);