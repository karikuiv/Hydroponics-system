Test environment for the doser:
A test run making use of all important data structures.
Creates mock hub nodes, devices, sensors and their data sources, data providers from data sources, environments, tasks and their schedules, and controllers.
Loads data and prints system lists and the system hierarchy, and then shows a simple tasking system.

Building:
Compiles with compile.sh on raspberry pi
Dependencies: wiringPi library (for now used only for delay() and millis(), but the full environment will use GPIO etc and requires raspberry pi)
If using non-raspberry pi linux or windows, remove all references to "delay" from print_hierarchy.c and all references to "delay" and "millis, as well as "wiringPiSetup()" and "#include <wiringPi.h>" from test_environment.c, but this will screw up timing of the main task loop.
Entry point: main() at test_environment.c
Requires minimum terminal size 90x39

Next steps:
Integrating the system with a console user interface.
Setting up network communications with hubs.
Building the web interface and backend.

License:
These files are uploaded to github for the sole purpose of showing source code to potential employers.
Any other use is undefined behaviour! :)
