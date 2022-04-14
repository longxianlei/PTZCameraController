#pragma once

#ifndef  __CAMERA_ACTION_H__
#define __CAMERA_ACTION_H__




#include <string.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <malloc.h>
#include <mmsystem.h>
#include <string>

const int PELCO_D_SIZE = 7;
const int PELCO_P_SIZE = 8;


class CameraAction
{
public:
	CameraAction();
	BYTE pDbytes[8];
	BYTE pPbytes[8];
	HANDLE hComm;
	DWORD dwBytesRead = PELCO_P_SIZE;

	BOOL openCamera(int com_port);
	BOOL closeCamera();
	BOOL sendCommand(BYTE* pByte);
	int getCorrectPos(int pos);
	void delPreset(BYTE* pByte, BYTE pos);
	void callPreset(BYTE* pByte, BYTE pos);
	void setPreset(BYTE* pByte, BYTE pos);

	// set protocol
	void setPelcoP();
	void setPelcoD();

	// set the address.
	void setAddress(BYTE addr);

	// query the protocol
	BOOL isPelcoP();
	BOOL isPelcoD();

	BYTE getBytesSize();

	// finish code
	void stop(BYTE* pByte);

	void toUp(BYTE* pByte, BYTE speed, BOOL b = TRUE);
	void toDown(BYTE* pByte, BYTE speed, BOOL b = TRUE);
	void toLeft(BYTE* pByte, BYTE speed, BOOL b = TRUE);
	void toRight(BYTE* pByte, BYTE speed, BOOL b = TRUE);

	void toLeftUp(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b = TRUE);
	void toLeftDown(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b = TRUE);
	void toRightUp(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b = TRUE);
	void toRightDown(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b = TRUE);

	void autoStart(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b = TRUE);

	virtual ~CameraAction();

private:
	BYTE byPelco;
	BYTE byCurAddr;
	BYTE bySpeed;



	void UpDowm(BYTE* pByte, BYTE speed, BOOL b_Up);
	void LeftRight(BYTE* pByte, BYTE speed, BOOL b_left);

	void calCheckByte(BYTE* pByte);
	void preparePreset(BYTE* pByte, BYTE pos);
	void prepare(BYTE* pByte);

};

#endif // ! __CAMERA_ACTION_H__