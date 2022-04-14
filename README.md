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

The driver class is in:
```
camera_action.h 
camera_action.cpp
```
The demo for turn the PTZ camera left, right, up, down is in:
```
main.cpp
```

1) The format of Pelco-D (pelcod)
Pelco-D consists of 7 hexadecimal bytes (all byte data used in this page are in Hexadecimal format unless otherwise specified).

Byte 1	Byte 2	Byte 3	Byte 4	Byte 5	Byte 6	Byte 7
Sync	Camera Address	Command 1	Command 2	Data 1	Data 2	Checksum
 

Byte 1 (Sync) - the synchronization byte, fixed to FF
Byte 2 (Camera Address) - logical address of the camera being controlled (Address 1 is 01)
Byte 3 & 4 (Command 1 and 2) are shown below
Byte 5 (Data 1) - pan speed, range from 00 (stop) to 3F (high speed) and FF for "turbo" speed (the maximum pan speed that the device can go)
Byte 6 (Data 2) - tilt speed, range from 00 (stop) to 3F (maximum speed)
Byte 7 (Checksum) - sum of bytes (excluding the synchronization byte), then modulo 100 (Decimal code: 256)
Command 1 and 2 details

 	Bit 7	Bit 6	Bit 5	Bit 4	Bit 3	Bit 2	Bit 1	Bit 0
Command 1	Sense	Reserved	Reserved	Auto / Manual Scan	Camera On/Off	Iris Close	Iris Open	Focus Near
Command 2	Focus Far	Zoom Wide	Zoom Tele	Tilt Down	Tilt Up	Pan Left	Pan Right	Fixed to 0
