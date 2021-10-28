Test environment for the doser:
A test run making use of all important data structures.
Creates mock hub nodes, devices, sensors and their data sources, data providers from data sources, environments, tasks and their schedules, and controllers.
Loads data and prints system lists and the system hierarchy, and then shows a simple tasking system.

Building:
Compiles with compile.sh on raspberry pi: gcc -g -Wall -lwiringPi test_environment.c doser_system.c environment.c device.c sensor.c data_provider.c controller.c ph_controller.c ec_controller.c schedule.c print_hierarchy.c -o test
Dependencies: wiringPi library (for now used only for delay() and millis(), but the full environment will use GPIO etc and requires raspberry pi)
If using non-raspberry pi linux or windows, remove all references to "delay" from print_hierarchy.c and all references to "delay" and "millis, as well as "wiringPiSetup()" and "#include <wiringPi.h>" from test_environment.c, but this will screw up timing of the main task loop.
Entry point: main() at test_environment.c
Requires minimum terminal size 90x39

Next steps:
Integrating the system with a console user interface. See https://www.youtube.com/watch?v=x8l4Z8xdHIY for testing.
Building the web interface and backend.
Putting finishing touches on things such as ph/ec controller.

License:
These files are uploaded to github for the sole purpose of showing source code to potential employers.
Any other use is undefined behaviour! :)
