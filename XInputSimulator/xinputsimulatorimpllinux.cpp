//    Copyright 2013 Dustin Bensing

//    This file is part of XInputSimulator.

//    XInputSimulator is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.

//    XInputSimulator is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser Public License for more details.

//    You should have received a copy of the GNU Lesser Public License
//    along with XInputSimulator.  If not, see <http://www.gnu.org/licenses/>.

#ifdef __linux__

#include <unistd.h> //usleep

#include "xinputsimulatorimpllinux.h"
#include "notimplementedexception.h"
#include <iostream>

//memset
#include <stdio.h>
#include <cstring>



XInputSimulatorImplLinux::XInputSimulatorImplLinux()
{
    if((display = XOpenDisplay(NULL)) == NULL) {
        std::cout << "can not access display server!" << std::endl;
            return;
    }

    root = DefaultRootWindow(display);

    Screen* pscr = DefaultScreenOfDisplay( display );

    this->displayX = pscr->width;
    this->displayY = pscr->height;

   fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
   ioctl(fd, UI_SET_EVBIT, EV_KEY);
   ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
   ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
   ioctl(fd, UI_SET_EVBIT, EV_ABS);
   ioctl(fd, UI_SET_ABSBIT, ABS_X);
   ioctl(fd, UI_SET_ABSBIT, ABS_Y);
   ioctl(fd, UI_SET_EVBIT, EV_REL);

   struct uinput_user_dev uidev;
   memset(&uidev,0,sizeof(uidev));
   snprintf(uidev.name,UINPUT_MAX_NAME_SIZE,"VirtualMouse");
   uidev.id.bustype = BUS_USB;
   uidev.id.version = 1;
   uidev.id.vendor = 0x1;
   uidev.id.product = 0x1;
   uidev.absmin[ABS_X] = 0;
   uidev.absmax[ABS_X] = displayX;
   uidev.absmin[ABS_Y] = 0;
   uidev.absmax[ABS_Y] = displayY;

   write(fd, &uidev, sizeof(uidev));
   ioctl(fd, UI_DEV_CREATE);

   sleep(1);
}

void XInputSimulatorImplLinux::initMouseEvent(int button)
{
    event.xbutton.button = button; //which button
    event.xbutton.same_screen = True;
    event.xbutton.subwindow = DefaultRootWindow(display);
    while (event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer(display, event.xbutton.window,
                      &event.xbutton.root, &event.xbutton.subwindow,
                      &event.xbutton.x_root, &event.xbutton.y_root,
                      &event.xbutton.x, &event.xbutton.y,
                      &event.xbutton.state);
    }
}

void XInputSimulatorImplLinux::mouseGetPosition(int &x, int &y)
{
    Window root, child;
    int rootX, rootY, winX, winY;
    unsigned int mask;

    XQueryPointer(display,DefaultRootWindow(display),&root,&child,
                &rootX,&rootY,&winX,&winY,&mask);

    x = rootX;
    y = rootY;
}

void XInputSimulatorImplLinux::mouseMoveTo(int x, int y)
{
    if(!display){
        return;
    }

    struct input_event ev[2], ev_sync;
    memset(ev, 0, sizeof(ev));
    memset(&ev_sync, 0, sizeof(ev_sync));

    ev[0].type = EV_ABS;
    ev[0].code = ABS_X;
    ev[0].value = x;
    ev[1].type = EV_ABS;
    ev[1].code = ABS_Y;
    ev[1].value = y;


    int res_w = write(fd, ev, sizeof(ev));

    ev_sync.type = EV_SYN;
    ev_sync.value = 0;
    ev_sync.code = 0;
    int res_ev_sync = write(fd, &ev_sync, sizeof(ev_sync));

  /*  XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
    XFlush(display);


    XEvent event;
    memset(&event, 0, sizeof (event));*/
}

void XInputSimulatorImplLinux::mouseMoveRelative(int x, int y)
{
    if(!display){
        return;
    }

    int xx, yy;
    mouseGetPosition(xx, yy);

    // +1 -1 to x y -> strange bug
    if (y == 0)
      mouseMoveTo(xx + x, yy + 1);
    else if (x == 0)
      mouseMoveTo(xx + 1, yy + y);
    else
      mouseMoveTo(xx + x, yy + y);
    /*XWarpPointer(display, None, None, 0, 0, 0, 0, x, y);
    XFlush(display);*/
}

void XInputSimulatorImplLinux::mouseDown(int button)
{
    XTestFakeButtonEvent(display, button, true, CurrentTime);
    XFlush(display);
}

void XInputSimulatorImplLinux::mouseUp(int button)
{
    XTestFakeButtonEvent(display, button, false, CurrentTime);
    XFlush(display);
}

void XInputSimulatorImplLinux::mouseClick(int button)
{
    this->mouseDown(button);
    usleep(100);
    this->mouseUp(button);
}

void XInputSimulatorImplLinux::mouseScrollX(int length)
{
    int button;
    if(length < 0){
        button = 6;  //scroll left button
    }else{
        button = 7;  //scroll right button
    }

    if(length < 0){
        length *= -1;
    }

    for(int cnt = 0; cnt < length; cnt++){
        this->mouseDown(button);
        this->mouseUp(button);
    }
}

void XInputSimulatorImplLinux::mouseScrollY(int length)
{
    int button;
    if(length < 0){
        button = 4;  //scroll up button
    }else{
        button = 5;  //scroll down button
    }

    if(length < 0){
        length *= -1;
    }

    for(int cnt = 0; cnt < length; cnt++){
        this->mouseDown(button);
        this->mouseUp(button);
    }
}

void XInputSimulatorImplLinux::keyDown(int key)
{
    XTestFakeKeyEvent(display, key, True, 0);
    XFlush(display);
}

void XInputSimulatorImplLinux::keyUp(int key)
{
    XTestFakeKeyEvent(display, key, False, 0);
    XFlush(display);
}

void XInputSimulatorImplLinux::keyClick(int key)
{
    std::cout << "key click: " << key << std::endl;

    this->keyDown(key);
    this->keyUp(key);
}

int XInputSimulatorImplLinux::charToKeyCode(char key_char)
{
    std::cout << "cchar: " << (int)key_char << std::endl;

    int keyCode = XKeysymToKeycode(display, key_char);
//    int keyCode = XKeysymToKeycode(display, XStringToKeysym(&key_char));
    std::cout << "ccode: " << keyCode << std::endl;

    return keyCode;
}
void XInputSimulatorImplLinux::keySequence(const std::string &sequence)
{
    std::cout << "key seq: " << sequence << std::endl;

    for(const char c : sequence) {
        std::cout << "cahr: " << c << std::endl;
        int keyCode = this->charToKeyCode(c);
        std::cout << "key code: " << keyCode << std::endl;

        if (isupper(c)) {
            std::cout << "upper " << c << std::endl;

            this->keyDown(XKeysymToKeycode(display, XK_Shift_L));
            this->keyClick(keyCode);
            this->keyUp(XKeysymToKeycode(display, XK_Shift_L));
        }
        else {
            this->keyClick(keyCode);
        }


        std::cout << std::endl;
    }

}


#endif // linux
