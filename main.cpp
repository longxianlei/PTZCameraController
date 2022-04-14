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

	if (!camera1.openCamera(4))
	{
		cout << "Connect com port error!" << endl;
		system("pause");
	}

	BYTE pByte[8] = { 0 };

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



	camera1.stop(pByte);
	camera1.sendCommand(pByte);
	camera1.closeCamera();

	
	// This is the original code for implement RS485 code for control ptz camera.
	/*
	HANDLE hComm;
	string portnum;
	portnum = "COM" + to_string(4);
	hComm = CreateFileA(portnum.c_str(), GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);


	if (hComm == (HANDLE)-1)
	{
		cout << "´ò¿ªsigma COMÊ§°Ü£¡" << endl;
		return FALSE;
	}
	if (hComm == INVALID_HANDLE_VALUE)
	{
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(hComm, &dcbSerialParams))
	{
		cout << "ERROR GETTING STATE" << endl;
		system("pause");
	}

	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
	dcbSerialParams.fOutX = FALSE;
	dcbSerialParams.EofChar = '\n';

	if (!SetCommState(hComm, &dcbSerialParams))
	{
		cout << "ERROR SETTING STATE" << endl;
		system("pause");
	}




	DWORD dwBytesRead = PELCO_P_SIZE;

	//cout << "stop!!!" << endl;
	//camera1.stop(pByte);
	//if (!WriteFile(hComm, pByte, PELCO_P_SIZE, &dwBytesRead, NULL)) {
	//	cout << "WRITE ERROR" << endl;
	//	system("pause");
	//}
	//GetCommState(hComm, &dcbSerialParams);
	//this_thread::sleep_for(chrono::milliseconds(2000));


	cout << "turn right!!!!!!" << endl;
	BYTE temp = (0x01 + 0x02 + 0xFF) % 256; //right
	BYTE byDbytes[] = { 0xFF, 0x01, 0x00, 0x02, 0xFF, 0x00, temp,	0x00 }; //right
	if (!WriteFile(hComm, byDbytes, PELCO_P_SIZE, &dwBytesRead, NULL)) {
		cout << "WRITE ERROR" << endl;
		system("pause");
	}
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & byDbytes[i]) << " ";
	}
	cout << endl;
	GetCommState(hComm, &dcbSerialParams);
	this_thread::sleep_for(chrono::milliseconds(4000));


	cout << "turn left!!!!!!" << endl;
	BYTE temp1 = (0x01 + 0x04 + 0x3F) % 256;  // left
	BYTE byDbytes1[] = { 0xFF, 0x01, 0x00, 0x04, 0x3F, 0x00, temp1,	0x00 }; //left
	if (!WriteFile(hComm, byDbytes1, PELCO_P_SIZE, &dwBytesRead, NULL)) {
		cout << "WRITE ERROR" << endl;
		system("pause");
	}
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & byDbytes1[i]) << " ";
	}
	cout << endl;
	this_thread::sleep_for(chrono::milliseconds(4000));


	cout << "turn up!!!!!!" << endl;
	BYTE temp_up = (0x01 + 0x08 + 0x3F) % 256;
	BYTE byDbytes2[] = { 0xFF, 0x01, 0x00, 0x08, 0x00, 0x3F, temp_up,0x00 }; //left
	if (!WriteFile(hComm, byDbytes2, PELCO_P_SIZE, &dwBytesRead, NULL)) {
		cout << "WRITE ERROR" << endl;
		system("pause");
	}
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & byDbytes2[i]) << " ";
	}
	cout << endl;
	this_thread::sleep_for(chrono::milliseconds(4000));

	cout << "turn dowm!!!!!!" << endl;
	BYTE temp3 = (0x01 + 0x10 + 0xFF) % 256;
	BYTE byDbytes3[] = { 0xFF, 0x01, 0x00, 0x10, 0x00, 0xFF, temp3, 0x00 }; //left
	if (!WriteFile(hComm, byDbytes3, PELCO_P_SIZE, &dwBytesRead, NULL)) {
		cout << "WRITE ERROR" << endl;
		system("pause");
	}
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & byDbytes3[i]) << " ";
	}
	cout << endl;
	this_thread::sleep_for(chrono::milliseconds(4000));


	cout << "stop!!!!!!" << endl;
	BYTE byDbytes4[] = { 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01,	0x00 };
	if (!WriteFile(hComm, byDbytes4, PELCO_P_SIZE, &dwBytesRead, NULL)) {
		cout << "WRITE ERROR" << endl;
		system("pause");
	}
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		cout << hex << (0xFF & byDbytes4[i]) << " ";
	}
	cout << endl;

	CloseHandle(hComm);  //shut it down
	system("pause");
	*/
	system("pause");
	int temp_test;
	cin >> temp_test;
	return 0;
}