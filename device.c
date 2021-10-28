/* 
 * Copyright 2021 Kari Kuivalainen ( https://github.com/karikuiv )
 * Read only license: These files are uploaded for the sole purpose of showing code samples to potential employers.
 * See readme_license.txt for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "device.h"
#include "doser_system.h"
#include "environment.h"
#include "schedule.h"

/**
 * Assign device to an environment.
 * 
 */
int8_t assign_device(struct environment_t *environment, struct device_t *device) {
    struct device_t **tmp_ptr;
    struct environment_t **tmp_ptr_e;
    
    if (environment == NULL) {
        printf("error: environment is empty\n");
        return -1;
    } else if (device == NULL) {
        printf("error: device is empty\n");
        return -1;
    } 
    
    tmp_ptr = realloc(environment->devices, sizeof(struct device_t *) * (environment->num_devices + 1));
    if (tmp_ptr == NULL) {
        printf("error: realloc tmp_ptr\n");
        return -1;
    }
    environment->devices = tmp_ptr;
    environment->devices[environment->num_devices] = device;
    environment->num_devices++;
    
    tmp_ptr_e = realloc(device->parents, sizeof(struct environment_t *) * (device->num_parents + 1));
    if (tmp_ptr_e == NULL) {
        printf("error: realloc tmp_ptr_e\n");
        return -1;
    }
    device->parents = tmp_ptr_e;
    device->parents[device->num_parents] = environment;
    device->num_parents++;
    
    return environment->num_devices;
}

/**
 * Set up a device from settings.
 *
 * For now just resize the arrays every time a new device is added to list
 * later might want to use linked lists, although resizing an array ~20 times
 * from size 1 to ~20 maybe once during the program's life is probably not so bad
 *
 * returns: for now, the number of devices in the array. (should be the id of the device?)
*/
int8_t load_device(struct doser_t *doser, uint8_t provider_id, char *name) {
    struct device_t **tmp_ptr;
    struct device_t *device;
    
    if (doser == NULL) {
        printf("error: doser is missing\n");
        return -1;
    } else if (name == NULL) {
        printf("error: name is missing\n");
        return -1;
    } else if (strlen(name) < 1) {
        printf("error: name is empty\n");
        return -1;
    } else if ((provider_id < 0) || (provider_id > (doser->num_nodes - 1))) {
        printf("error: provider is invalid\n");
        return -1;
    }

    tmp_ptr = realloc(doser->devices, sizeof(struct device_t *) * (doser->num_devices + 1));
    if (tmp_ptr == NULL) {
        printf("error: realloc tmp_ptr\n");
        return -1;
    }
    doser->devices = tmp_ptr;   

    device = malloc(sizeof(struct device_t));
    if (device == NULL) {
        printf("error: malloc device\n");
        return -1;
    }

    device->name = malloc(sizeof(char) * (strlen(name) + 1));
    if (device->name == NULL) {
        printf("error: malloc device->name\n");
        return -1;
    }
    strcpy(device->name, name);
    device->name[strlen(device->name)] = '\0';
    
    device->provider_id = provider_id;
    device->id = doser->num_devices; /* replace with a unique identifier like sha? */
    device->status = TASK_STATUS_OFF;
    device->num_attached_tasks = 0;
    device->attached_tasks = NULL;
    device->num_parents = 0;
    device->parents = NULL;
    
    doser->devices[doser->num_devices] = device;
    doser->num_devices++;
    
    return doser->num_devices;  
}