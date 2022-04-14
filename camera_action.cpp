/* implementaion of ptz controller, the driver code.*/

#include "camera_action.h"
using namespace std;

CameraAction::CameraAction()
{
	//BYTE byDbytes[] = {0xFF, 0, 0, 0, 0, 0, 0,	0};	//plus one more byte for convernice
	//BYTE byPbytes[] = {0xA0, 0, 0, 0, 0, 0, 0xAF, 0};

	// here, we use Pelco-D protocol bytes.
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		pDbytes[i] = 0;
		pPbytes[i] = 0;
	}
	pDbytes[0] = 0xFF;
	pPbytes[0] = 0xA0;
	pPbytes[6] = 0xAF;

	// select Pelco-D by default.
	this->setPelcoD();

	// set 0 addr by default
	this->setAddress(1);

}

CameraAction::~CameraAction()
{

}

BOOL CameraAction::openCamera(int com_port)
{
	
	string portnum;
	portnum = "COM" + to_string(com_port);
	hComm = CreateFileA(portnum.c_str(), GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);


	if (hComm == (HANDLE)-1)
	{
		cout << "Connect PTZ motor failed!" << endl;
		return FALSE;
	}

	if (hComm == INVALID_HANDLE_VALUE)
	{
		cout << "Open PTZ motor failed!" << endl;
		return FALSE;
	}

	SetupComm(hComm, 100, 100);//设置输入输出缓冲区为100

	COMMTIMEOUTS TimeOuts;
	//设定读超时,在读一次输入缓冲区的内容后读操作就立即返回，不管是否读入了要求的字符。
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;

	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.ReadTotalTimeoutConstant = 500;

	SetCommTimeouts(hComm, &TimeOuts);//设置超时



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
		return FALSE;
	}
	return TRUE;
}

BOOL CameraAction::closeCamera()
{
	return(CloseHandle(hComm));  //shut it down
}

BOOL CameraAction::sendCommand(BYTE* pByte)
{
	if (!WriteFile(hComm, pByte, PELCO_P_SIZE, &dwBytesRead, NULL)) {
		cout << "WRITE ERROR" << endl;
		return FALSE;
		//system("pause");
	}
	return TRUE;
}


void CameraAction::setPelcoD()
{
	this->byPelco = 0;
}

void CameraAction::setPelcoP()
{
	this->byPelco = 1;
}

BOOL CameraAction::isPelcoD()
{
	return this->byPelco == 0;
}

BOOL CameraAction::isPelcoP()
{
	return this->byPelco == 1;
}

void CameraAction::setAddress(BYTE addr)
{
	this->byCurAddr = addr;
}

BYTE CameraAction::getBytesSize()
{
	return this->isPelcoD() ? PELCO_D_SIZE : PELCO_P_SIZE;
}

void CameraAction::prepare(BYTE* pByte)
{
	memcpy(pByte, this->isPelcoD() ? pDbytes : pPbytes, PELCO_P_SIZE);
	*(pByte + 1) = this->byCurAddr;
}

void CameraAction::calCheckByte(BYTE* pByte)
{
	if (this->isPelcoD())
	{
		int tmp = 0;
		for (int i = 1; i <= 5; i++)
		{
			tmp += *(pByte + i);
		}
		*(pByte + PELCO_D_SIZE - 1) = BYTE(tmp % 256);
	}
	else
	{
		BYTE tmp = *(pByte + 1);
		for (int i = 2; i <= 5; i++)
		{
			tmp ^= *(pByte + i);
		}
		*(pByte + PELCO_P_SIZE - 1) = tmp;
	}

}

// reset the camera, then stop it.
void CameraAction::stop(BYTE* pByte)
{
	this->prepare(pByte);
	this->calCheckByte(pByte);
}

void CameraAction::UpDowm(BYTE* pByte, BYTE speed, BOOL b_Up)
{
	if (speed > 0x3F)
	{
		speed = 0x3F;
	}
	this->prepare(pByte);
	BYTE byAction = b_Up ? 0x08 : 0x10;
	// set up or down action;
	*(pByte + 3) = byAction;
	// set speed;
	*(pByte + 5) = speed;
	this->calCheckByte(pByte);
}

void CameraAction::toUp(BYTE* pByte, BYTE speed, BOOL b)
{
	if (!b) return;
	this->UpDowm(pByte, speed, TRUE);
}

void CameraAction::toDown(BYTE* pByte, BYTE speed, BOOL b)
{
	if (!b) return;
	this->UpDowm(pByte, speed, FALSE);
}

void CameraAction::LeftRight(BYTE* pByte, BYTE speed, BOOL b_left)
{
	if (speed > 0x3F)
	{
		speed = this->isPelcoD() ? 0xFF : 0x40;
	}
	this->prepare(pByte);

	BYTE byAction = b_left ? 0x04 : 0x02;
	// set left or right;
	*(pByte + 3) = byAction;

	// set up or down;
	*(pByte + 4) = speed;

	this->calCheckByte(pByte);
}

void CameraAction::toLeft(BYTE* pByte, BYTE speed, BOOL b)
{
	if (!b)return;
	this->LeftRight(pByte, speed, TRUE);
}

void CameraAction::toRight(BYTE* pByte, BYTE speed, BOOL b)
{
	if (!b) return;
	this->LeftRight(pByte, speed, FALSE);
}

void CameraAction::toLeftUp(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b)
{
	if (!b) return;
	speedlr = (speedlr > 0x3F) ? 0x3F : speedlr;
	speedud = (speedud > 0x3F) ? 0x3F : speedud;

	this->toLeft(pByte, speedlr);

	BYTE tmpByte[PELCO_P_SIZE];
	this->toUp(tmpByte, speedud);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		*(pByte + i) |= tmpByte[i];
	}
	this->calCheckByte(pByte);
}


void CameraAction::toLeftDown(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b)
{
	if (!b) return;
	speedlr = (speedlr > 0x3F) ? 0x3F : speedlr;
	speedud = (speedud > 0x3F) ? 0x3F : speedud;

	this->toLeft(pByte, speedlr);

	BYTE tmpByte[PELCO_P_SIZE];
	this->toDown(tmpByte, speedud);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		*(pByte + i) |= tmpByte[i];
	}
	this->calCheckByte(pByte);
}

void CameraAction::toRightUp(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b)
{
	if (!b) return;
	speedlr = (speedlr > 0x3F) ? 0x3F : speedlr;
	speedud = (speedud > 0x3F) ? 0x3F : speedud;

	this->toRight(pByte, speedlr);

	BYTE tmpByte[PELCO_P_SIZE];
	this->toUp(tmpByte, speedud);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		*(pByte + i) |= tmpByte[i];
	}
	this->calCheckByte(pByte);
}

void CameraAction::toRightDown(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b)
{
	if (!b) return;
	speedlr = (speedlr > 0x3F) ? 0x3F : speedlr;
	speedud = (speedud > 0x3F) ? 0x3F : speedud;

	this->toRight(pByte, speedlr);

	BYTE tmpByte[PELCO_P_SIZE];
	this->toDown(tmpByte, speedud);
	for (int i = 0; i < PELCO_P_SIZE; i++)
	{
		*(pByte + i) |= tmpByte[i];
	}
	this->calCheckByte(pByte);
}

void CameraAction::autoStart(BYTE* pByte, BYTE speedlr, BYTE speedud, BOOL b)
{
	if (!b) return;
	speedlr = (speedlr > 0x3F) ? 0x3F : speedlr;
	speedud = (speedud > 0x3F) ? 0x3F : speedud;

	this->prepare(pByte);

	// set up action;
	*(pByte + 2) = this->isPelcoD() ? 0x98 : 0x20;
	// set speed;
	*(pByte + 4) = speedlr;
	*(pByte + 5) = speedud;
	this->calCheckByte(pByte);
}

void CameraAction::setPreset(BYTE* pByte, BYTE pos)
{
	this->preparePreset(pByte,pos);
	*(pByte + 3) = 0x03;
	this->calCheckByte(pByte);
}

void CameraAction::delPreset(BYTE* pByte, BYTE pos)
{
	this->preparePreset(pByte,pos);
	*(pByte + 3) = 0x05;
	this->calCheckByte(pByte);
}

void CameraAction::callPreset(BYTE* pByte, BYTE pos)
{
	this->preparePreset(pByte,pos);
	*(pByte + 3) = 0x07;
	this->calCheckByte(pByte);
}

void CameraAction::preparePreset(BYTE* pByte, BYTE pos)
{
	if (pos < 1) pos = 1;
	if (pos > 0xFF)pos = 0xFF;
	if (this->isPelcoD() && pos > 0x20)pos = 0x20;
	this->prepare(pByte);
	*(pByte + 5) = pos;
}

int CameraAction::getCorrectPos(int pos)
{
	if (pos < 1) pos = 1;
	if (pos > 0xFF) pos = 0xFF;
	if (this->isPelcoD() && pos > 0x20) pos = 0x20;
	return pos;
}