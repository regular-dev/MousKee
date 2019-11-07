# MousKee

MousKee - is small Linux programm/daemon that allows you to move cursor with keyboard arrows.
Uses Uinput - supports both X and Wayland.

### How-to build on Linux

`cmake <path_to_src> && make`

### How-to use

Firstly, you need to know what input device is your keyboard  :

`cat /proc/bus/input/devices`

You can check event by :

`sudo cat /dev/input/event6`

Then launch with something like this :

`./mouskee -s -d /dev/input/event6`

Controll mouse with keyboard arrows :  **0 - Right Click** , **5 - Left Click**

### Code used from
Unix keylogger : https://github.com/kernc/logkeys
XInputSimulator : https://github.com/pythoneer/XInputSimulator

### License
2019 @REGULAR-DEV WTFPL
