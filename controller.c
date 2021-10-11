#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "controller.h"
#include "doser_system.h"
#include "environment.h"

int8_t create_controller(struct doser_t *doser, char type, char *name) {
    struct controller_t **tmp_ptr;
    struct controller_t *controller;
    
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
    
    controller = malloc(sizeof(struct controller_t));
    if (controller == NULL) {
        printf("error: malloc controller\n");
        return -1;  
    }
    controller->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(controller->name, name);
    controller->name[strlen(name)] = '\0';
    
    controller->type = type;
    if (controller->type == CONTROLLER_TYPE_PH) {
        controller->settings = malloc(sizeof(struct ph_controller_t));
        if (controller->settings == NULL) {
            printf("error: malloc controller->settings\n");
            return -1;
        }
    } else if (controller->type == CONTROLLER_TYPE_EC) {
        controller->settings = malloc(sizeof(struct ec_controller_t));
        if (controller->settings == NULL) {
            printf("error: malloc controller->settings\n");
            return -1;
        }           
    } else {
        printf("error: invalid controller type!\n");
        return -1;
    }
    
    controller->num_attached_tasks = 0;
    controller->attached_tasks = NULL;
    
    tmp_ptr = realloc(doser->controllers, sizeof(struct controller_t *) * (doser->num_controllers + 1));
    if (tmp_ptr == NULL) {
        printf("error: realloc tmp_ptr\n");
        return -1;          
    }
    doser->controllers = tmp_ptr;
    doser->controllers[doser->num_controllers] = controller;
    doser->num_controllers++;
    
    return doser->num_controllers;
}

int8_t assign_controller (struct environment_t *parent, struct controller_t *controller) {
    struct controller_t **tmp_ptr;
    
    if (parent == NULL) {
        printf("error: parent is empty\n");
        return -1;
    } else if (controller == NULL) {
        printf("error: controller is empty\n");
        return -1;
    }
    
    tmp_ptr = realloc(parent->controllers, sizeof(struct controller_t *) * (parent->num_controllers + 1));
    if (tmp_ptr == NULL) {
        printf("error: realloc tmp_ptr\n");
        return -1;          
    }
    parent->controllers = tmp_ptr;
    
    parent->controllers[parent->num_controllers] = controller;
    controller->parent = parent;
    parent->num_controllers++;
    
    return parent->num_controllers;
}