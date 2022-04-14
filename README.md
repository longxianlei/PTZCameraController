# PTZCameraController
This is the **driver code (c++ code)** for ptz motor and camera, which uses **rs485 and Pelco-D** control protocol. **Aiming for motor controlling.**

Pelco-D is a popular PTZ (Pan / Tilt / Zoom) camera control protocol used in the CCTV industry. 

You can directly control the camera or motor through the USB COM port, and send command using the predefined protocol.

**Notes**
1) Before you can control any Pelco-D cameras, you need to prepare the following items:
   - A PC running 232Analyzer program
   - An RS-232 port (or USB / RS-232 converter)
   - An RS232 / RS485 converter
2) Make sure your COM port settings in the 232Analyzer program matches with camera's settings (Pelco-D cameras use No parity, 8 Data bits and 1 Stop bit, baud rate depends on your camera setting)

The driver class is in
```
camera_action.h and camera_action.cpp
```
