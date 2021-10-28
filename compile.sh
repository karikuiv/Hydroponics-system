rm ./test
clear
gcc -g -Wall -lwiringPi test_environment.c doser_system.c environment.c device.c sensor.c data_provider.c controller.c ph_controller.c ec_controller.c schedule.c print_hierarchy.c -o test
