// KR30_16_GC_Driver_Native_Test.cpp : Defines the entry point for the console application.
//

#define WINDOWS
#include "common.h"
#include <KRC2_GC.h>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;

struct Position
{
	double x;
	double y;
	double z;
	double a;
	double b;
	double c;
};

Position positions[] = { { 120.79, -148.76, 413.92 , -145.08, 0, 0},
{ 98.75, 901.16, 773.66, -145.08, 0, 0 },
{ 617.29, 736.81, 337.45, -145.08, 0, 0 },
{ 617.29, 736.81, 773.59, -145.08, 0, 0 }};

int _tmain(int argc, _TCHAR* argv[])
{
	char xmlData[] = "<?xml version=\"1.0\" encoding=\"utf - 8\"?><Rob Type=\"KUKA\" xmlns:xsi=\"http://www.w3org/2001/XMLSchema-in- stance\" xsi:noNamespaceSchemaLocation=\"KrcData.xsd\"> <Dat TaskType=\"b\"> <ComStatus>continuous</ComStatus> <RIst X=\"1620.0000\" Y=\"1620.0000\" Z=\"1620.0000\" A=\"100.0000\" B=\"100.0000\" C=\"100.0000\" /> <RIst X=\"1620.0000\" Y=\"1620.0000\" Z=\"1620.0000\" A=\"100.0000\" B=\"100.0000\" C=\"100.0000\" /> <AIPos A1=\"-180.0000\" A2=\"-10.0000\" A3=\"20.0000\" A4=\"20.0000\" A5=\"20.0000\" A6=\"20.0000\" /> <ASPos A1=\"-180.0000\" A2=\"-10.0000\" A3=\"20.0000\" A4=\"20.0000\" A5=\"20.0000\" A6=\"20.0000\" /> <EIPos E1=\"0.00000\" E2=\"0.00000\" E3=\"0.00000\" E4=\"0.00000\" E5=\"0.00000\" E6=\"0.00000\" /> <ESPos E1=\"0.00000\" E2=\"0.00000\" E3=\"0.00000\" E4=\"0.00000\" E5=\"0.00000\" E6=\"0.00000\" /> <MACur A1=\"1620.0000\" A2=\"1620.0000\" A3=\"1620.0000\" A4=\"1620.0000\" A5=\"1620.0000\" A6=\"1620.0000\" /> <MECur E1=\"1620.0000\" E2=\"1620.0000\" E3=\"1620.0000\" E4=\"1620.0000\" E5=\"1620.0000\" E6=\"1620.0000\" /> <IPOC>64</IPOC> <BMode>5</BMode> <IPOStat>255</IPOStat> <Tech x=\"1\" p6=\"0\" p7=\"0\" p8=\"0\" p6x1=\"0\" p7x1=\"0\" p8x1=\"0\" p6x2=\"0\" p7x2=\"0\" p8x2=\"0\" p6x3=\"0\" p7x3=\"0\" p8x3=\"0\" /> <RGH X=\"234\" Y=\"12332\" Z=\"223\" A=\"132\" B=\"123\" C=\"-123\" T=\"10101\" /> <Dil>255</Dil> <Tick>0</Tick> <RWMode>C</RWMode> </Dat> </Rob>";
	KRC2_GC* robControl = new KRC2_GC();
	
	robControl->InitializeTCP();
	//robControl->Terminate();

	bool running = true;
	int i = 5;

	cout << "Server started...." << endl;

	char c = 0;
	robControl->SetVelocityPercentage(100);
	robControl->MoveLIN(0, 0, 0, 0, 0, 0);
	
	::srand(time(NULL));
	double x = 0;
	double y = 0;
	double z = 0;

	Position p;
	while (running == true && robControl->ErrorOccurred() == false)
	{
		cout << "Please press s for next movement... ";
		cin >> c;
		if (c == 's' || c == 'S')
		{
			cout << "Performing movement..." << endl;
			if (robControl->IsRobotIdle() == true)
			{
				for (int i = 0; i < sizeof(positions); i++)
				{
					p = positions[i];
					robControl->MoveLIN(p.x, p.y, p.z, p.a, p.b, p.c);
					::_sleep(5000);
				}
			}
			cout << "Movement completed!" << endl;
		}
		/*if (robControl->IsRobotIdle() == true)
		{
			x = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / 300));
			y = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / 1300));
			z = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / 250));

			robControl->MoveLIN(x, y, z, 0, 0, 0);
			::_sleep(30);

			cout << "SStatus=" << robControl->sStatus << "      Sending next position,  x = " << x << "  y = " << y << "  z = " << z << endl;
		}*/
		/*while (robControl->GetStatus() != KRC2_GC::ROBOT_IDLE)
		{
			y = y;
		}*/
		//if (robControl->llIPOC != 0) cout << robControl->llIPOC << endl;
		/*cin >> c;
		if (c == 's')
		{
			running = false;
			robControl->Terminate();
		}*/
	}

	cout << "Server terminated";
	
	cin >> c;

	return 0;
}

