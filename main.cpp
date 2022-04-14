//#include "stdafx.h"
#include <Windows.h>
#include <cstring>
#include <WinBase.h>
#include <iostream>
#include <time.h>
#include <thread>
#include "camera_action.h"

using namespace std;


int main()
{
	
	CameraAction camera1;
	
	// Open camera and connect the usb port.
	// The com port is COM4. You can change it decided.
	if (!camera1.openCamera(4))
	{
		cout << "Connect com port error!" << endl;
		system("pause");
	}
	BYTE pByte[8] = { 0 };

	
	// The speed range for pan-tilt is:
	// 0-63 for pan speed. i.e., 0x00-0x3F, when using turbo (0xFF), the pan motor will working in maximum speed.
	// 0-63 for tilt speed.
	cout << "to left: " << endl;
	camera1.toLeft(pByte, 0xFF);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & pByte[i]) << " ";
	}
	cout << endl;
	camera1.sendCommand(pByte);
	this_thread::sleep_for(chrono::milliseconds(2000));


	cout << "to down:" << endl;
	camera1.toDown(pByte, 0xFF);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & pByte[i]) << " ";
	}
	cout << endl;
	camera1.sendCommand(pByte);
	this_thread::sleep_for(chrono::milliseconds(2000));


	cout << "stop!!!" << endl;
	camera1.stop(pByte);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & pByte[i]) << " ";
	}
	cout << endl;
	camera1.sendCommand(pByte);
	this_thread::sleep_for(chrono::milliseconds(2000));

	
	cout << "up!!!" << endl;
	camera1.toUp(pByte,0xFF);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & pByte[i]) << " ";
	}
	cout << endl;
	camera1.sendCommand(pByte);
	this_thread::sleep_for(chrono::milliseconds(2000));


	cout << "to right!!!" << endl;
	camera1.toRight(pByte, int(80));
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & pByte[i]) << " ";
	}
	cout << endl;
	camera1.sendCommand(pByte);
	this_thread::sleep_for(chrono::milliseconds(2000));


	cout << "to left up!!!" << endl;
	camera1.toLeftUp(pByte, int(80), 25);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & pByte[i]) << " ";
	}
	cout << endl;
	camera1.sendCommand(pByte);
	this_thread::sleep_for(chrono::milliseconds(5000));


	cout << "to right down!!!" << endl;
	camera1.toRightDown(pByte, 44, 25);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & pByte[i]) << " ";
	}
	cout << endl;
	camera1.sendCommand(pByte);
	this_thread::sleep_for(chrono::milliseconds(5000));

	// close camera and usb port.
	camera1.stop(pByte);
	camera1.sendCommand(pByte);
	camera1.closeCamera();

	system("pause");
	return 0;
}
