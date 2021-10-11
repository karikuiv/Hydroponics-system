#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "environment.h"
#include "doser_system.h"
#include "device.h"
#include "sensor.h"
#include "controller.h"

int8_t create_environment(struct doser_t *doser, char *name, uint8_t type) {
    struct environment_t *environment;
    struct environment_t **tmp_ptr;
    int8_t ret = 0;
    
    if (doser == NULL) {
        printf("error: doser is missing\n");
        return -1;
    } else if (name == NULL) {
        printf("error: name is missing\n");
        return -1;
    } else if (strlen(name) == 0) {
        printf("error: name is empty\n");
        return -1;
    }
    
    environment = malloc(sizeof(struct environment_t));
    if (environment == NULL) {
        printf("error: malloc environment\n");
        return -1;
    }
    
    environment->name = malloc(sizeof(char) * (strlen(name) + 1));
    if (environment->name == NULL) {
        printf("error: malloc environment->name\n");
        return -1;
    }
    strcpy(environment->name, name);
    environment->name[strlen(name)] = '\0';
    
    environment->num_tasks = 0;
    environment->tasks = NULL;
    environment->num_devices = 0;
    environment->devices = NULL;
    environment->num_sensors = 0;   
    environment->sensors = NULL;
    environment->num_controllers = 0;
    environment->controllers = NULL;
    environment->num_data_providers = 0;
    environment->data_providers = NULL;
    environment->num_environments = 0;
    environment->environments = NULL;
    environment->num_reservoirs = 0;
    environment->reservoirs = NULL;
    environment->parent = NULL; 
    
    environment->type = type;
    
    if (type == ENVIRONMENT_TYPE_RESERVOIR) {
        tmp_ptr = realloc(doser->reservoirs, sizeof(struct environment_t *) * (doser->num_reservoirs + 1));
        if (tmp_ptr == NULL) {
            printf("error: realloc tmp_ptr\n");
            return -1;      
        }
        doser->reservoirs = tmp_ptr;
        environment->id = doser->num_reservoirs;
        doser->reservoirs[doser->num_reservoirs] = environment;
        doser->num_reservoirs++;
        ret = doser->num_reservoirs;
    } else if (type == ENVIRONMENT_TYPE_ENVIRONMENT) {
        tmp_ptr = realloc(doser->environments, sizeof(struct environment_t *) * (doser->num_environments + 1));
        if (tmp_ptr == NULL) {
            printf("error: realloc tmp_ptr\n");
            return -1;      
        }
        doser->environments = tmp_ptr;
        environment->id = doser->num_environments;
        doser->environments[doser->num_environments] = environment;
        doser->num_environments++;
        ret = doser->num_environments;
    } else {
        printf("invalid environment type!\n");
        return -1;
    }
    
    return ret;
}

int8_t assign_environment(struct environment_t *parent, struct environment_t *environment) {
    struct environment_t **tmp_ptr;
    int8_t ret = 0;
    
    if (parent == NULL) {
        printf("error: parent is missing\n");
        return -1;
    } else if (environment == NULL) {
        printf("error: environment is missing\n");
        return -1;
    }
    
    environment->parent = parent;
    if (environment->type == ENVIRONMENT_TYPE_RESERVOIR) {
        tmp_ptr = realloc(parent->reservoirs, sizeof(struct environment_t *) * (parent->num_reservoirs + 1));
        if (tmp_ptr == NULL) {
            printf("error: realloc parent->reservoirs\n");
            return -1;          
        }       
        parent->reservoirs = tmp_ptr;
        parent->reservoirs[parent->num_reservoirs] = environment;
        parent->num_reservoirs++;               
        ret = parent->num_reservoirs;
    } else if (environment->type == ENVIRONMENT_TYPE_ENVIRONMENT) {
        tmp_ptr = realloc(parent->environments, sizeof(struct environment_t *) * (parent->num_environments + 1));
        if (tmp_ptr == NULL) {
            printf("error: realloc parent->environments\n");
            return -1;          
        }
        parent->environments = tmp_ptr;
        parent->environments[parent->num_environments] = environment;
        parent->num_environments++;
        ret = parent->num_environments;
    }

    return ret;
}