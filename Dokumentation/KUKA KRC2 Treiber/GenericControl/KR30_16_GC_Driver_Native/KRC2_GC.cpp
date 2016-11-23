// KR30_16_GC_Driver_Native.cpp : Defines the exported functions for the DLL application.
//

#include "common.h"
#include "KRC2_GC.h"
#include "TCPUDPServer.h"
#include "rapidxml.hpp"
#include "LogManager.h"

using namespace std;
using namespace rapidxml;

const int KRC2_GC::ROBOT_STATUS_NONE = 0x0000;
const int KRC2_GC::ROBOT_IDLE = 0x0001;
const int KRC2_GC::ROBOT_IN_START_POSITION = 0x0002;
const int KRC2_GC::ROBOT_ROBOT_MOVING = 0x0004;
const int KRC2_GC::ROBOT_MOVEMENT_READY = 0x0008;
const int KRC2_GC::ROBOT_POSITION_UNREACHABLE = 0x0010;

const int KRC2_GC::ROBOT_ERROR_INITIALIZATION = 0x0100;

std::string KRC2_GC::sXMLResponseTemplate = "<Sen Type=\"CoRob\"><Dat TaskType=\"b\"><EStr>Info: ERX Message!</EStr><RKorr X=\"%5.4f\" Y=\"%5.4f\" Z=\"%5.4f\" A=\"%5.4f\" B=\"%5.4f\" C=\"%5.4f\" /><AKorr A1=\"%5.4f\" A2=\"%5.4f\" A3=\"%5.4f\" A4=\"%5.4f\" A5=\"%5.4f\" A6=\"%5.4f\" /><EKorr E1=\"%5.4f\" E2=\"%5.4f\" E3=\"%5.4f\" E4=\"%5.4f\" E5=\"%5.4f\" E6=\"%5.4f\" /><Tech x=\"1\" p3=\"0.0000\" p4=\"0.0000\" p5=\"0.0000\" p3x1=\"0.0000\" p4x1=\"0.0000\" p5x1=\"0.0000\" p3x2=\"0.0000\" p4x2=\"0.0000\" p5x2=\"0.0000\" p3x3=\"0.0000\" p4x3=\"0.0000\" p5x3=\"0.0000\" /><DiO>%d</DiO><IPOC>%ld</IPOC></Dat></Sen>";

const double KRC2_GC::MAX_VELOCITY = 2.0;

const double KRC2_GC::MOVE_MODE_PTP = 1.0;
const double KRC2_GC::MOVE_MODE_LIN = 2.0;
const double KRC2_GC::MOVE_MODE_CIRC = 3.0;

KRC2_GC::KRC2_GC(void)
	:
	yIsInitialized(false),
	iStatus(KRC2_GC::ROBOT_STATUS_NONE),
	clServer(0),
	dX(0.0),
	dY(0.0),
	dZ(0.0),
	dA(0.0),
	dB(0.0),
	dC(0.0),
	dA1(0.0),
	dA2(0.0),
	dA3(0.0),
	dA4(0.0),
	dA5(0.0),
	dA6(0.0),
	dE1(0.0),
	dE2(0.0),
	dE3(0.0),
	dE4(0.0),
	dE5(0.0),
	dE6(0.0),
	dVelocity(0.0),
	dAcceleration(0.0),
	dRounding(0.0),
	llIPOC(0),
	dXSoll(0),
	dYSoll(0),
	dZSoll(0),
	dASoll(0),
	dBSoll(0),
	dCSoll(0),
	dA1Soll(0),
	dA2Soll(0),
	dA3Soll(0),
	dA4Soll(0),
	dA5Soll(0),
	dA6Soll(0),
	dE1Soll(0),
	dE2Soll(0),
	dE3Soll(0),
	dE4Soll(0),
	dE5Soll(0),
	dE6Soll(0),
	iBreakDirectControl(0)
{

}

KRC2_GC::~KRC2_GC(void)
{

}

void KRC2_GC::ParseXML(char* data)
{
	this->sReceivedXMLData = std::string(data);

	LogManager::GetInstance().LogLine("Received data from Robot");
	LogManager::GetInstance().LogLine(this->sReceivedXMLData);

	//Build up XML Document structure from data
	//NOTE: it is not checked if this is a valid xml document!
	xml_document<> doc;
	try
	{
		doc.parse<parse_full>(data);

		//Now go through all elements and extract necessary information
		xml_node<> *robNode = doc.first_node("Rob");
		if (robNode == 0)
		{
			//No node exists
			return;
		}
		xml_node<> *datNode = robNode->first_node("Dat");
		xml_node<> *rIstNode = datNode->first_node("RIst");
		xml_node<> *aIstNode = datNode->first_node("AIPos");
		xml_node<> *eIstNode = datNode->first_node("EIPos");
		xml_node<> *ipocNode = datNode->first_node("IPOC");
		xml_node<> *rghNode = datNode->first_node("RGH");

		//Now get axis parameters and data
		this->dX = atof(rIstNode->first_attribute("X")->value());
		this->dY = atof(rIstNode->first_attribute("Y")->value());
		this->dZ = atof(rIstNode->first_attribute("Z")->value());
		this->dA = atof(rIstNode->first_attribute("A")->value());
		this->dB = atof(rIstNode->first_attribute("B")->value());
		this->dC = atof(rIstNode->first_attribute("C")->value());

		this->dA1 = atof(aIstNode->first_attribute("A1")->value());
		this->dA2 = atof(aIstNode->first_attribute("A2")->value());
		this->dA3 = atof(aIstNode->first_attribute("A3")->value());
		this->dA4 = atof(aIstNode->first_attribute("A4")->value());
		this->dA5 = atof(aIstNode->first_attribute("A5")->value());
		this->dA6 = atof(aIstNode->first_attribute("A6")->value());

		this->dE1 = atof(eIstNode->first_attribute("E1")->value());
		this->dE2 = atof(eIstNode->first_attribute("E2")->value());
		this->dE3 = atof(eIstNode->first_attribute("E3")->value());
		this->dE4 = atof(eIstNode->first_attribute("E4")->value());
		this->dE5 = atof(eIstNode->first_attribute("E5")->value());
		this->dE6 = atof(eIstNode->first_attribute("E6")->value());

		//Now get IPOC
		this->llIPOC = atoll(ipocNode->value());

		//Get velocity, acceleration, rounding and status
		this->dVelocity = atof(rghNode->first_attribute("X")->value());
		this->dAcceleration = atof(rghNode->first_attribute("Y")->value());
		this->dRounding = atof(rghNode->first_attribute("Z")->value());
		this->sStatus = string(rghNode->first_attribute("T")->value());

		unsigned short index = 0;
		if (this->sStatus[index++] == '1')
		{
			this->iStatus |= ROBOT_IDLE;
		}
		else
		{
			this->iStatus &= (~ROBOT_IDLE);
		}

		if (this->sStatus[index++] == '1')
		{
			this->iStatus |= ROBOT_IN_START_POSITION;
		}
		else
		{
			this->iStatus &= (~ROBOT_IN_START_POSITION);
		}

		if (this->sStatus[index++] == '1')
		{
			this->iStatus |= ROBOT_ROBOT_MOVING;
		}
		else
		{
			this->iStatus &= (~ROBOT_ROBOT_MOVING);
		}

		if (this->sStatus[index++] == '1')
		{
			this->iStatus |= ROBOT_MOVEMENT_READY;
		}
		else
		{
			this->iStatus &= (~ROBOT_MOVEMENT_READY);
		}

		if (this->sStatus[index++] == '1')
		{
			this->iStatus |= ROBOT_POSITION_UNREACHABLE;
		}
		else
		{
			this->iStatus &= (~ROBOT_POSITION_UNREACHABLE);
		}
	}
	catch (const std::runtime_error& e)
	{
		LogManager::GetInstance().LogError(std::string("Runtime error occurred => ") + std::string(e.what()));
	}
	catch (const rapidxml::parse_error& e)
	{
		LogManager::GetInstance().LogError(std::string("Rapidxml parse error occurred => ") + std::string(e.what()));
	}
	catch (const std::exception& e)
	{
		LogManager::GetInstance().LogError(std::string("Exception occurred => ") + std::string(e.what()));
	}
	catch (...)
	{
		LogManager::GetInstance().LogError(std::string("Unkown error occurred => "));
	}
}

void KRC2_GC::PrepareResponse()
{
	//Now prepare the response
	std::string responseXML = KRC2_GC::sXMLResponseTemplate;

	char tmpBuffer[4096] = { 0 };
	sprintf_s(tmpBuffer, responseXML.c_str(), 
		this->dXSoll, this->dYSoll, this->dZSoll, this->dASoll, this->dBSoll, this->dCSoll, 
		this->dA1Soll, this->dA2Soll, this->dA3Soll, this->dA4Soll, this->dA5Soll, this->dA6Soll, 
		this->dE1Soll, this->dE2Soll, this->dE3Soll, this->dE4Soll, this->dE5Soll, this->dE6Soll, 
		this->iBreakDirectControl, this->llIPOC);

	responseXML = std::string(tmpBuffer);

	this->sPreparedXMLData = responseXML;

	//Reset relevant movement commands
	this->dE2Soll = 0.0;

	LogManager::GetInstance().LogLine("Prepared response XML to robot");
	LogManager::GetInstance().LogLine(this->sPreparedXMLData);
}

void KRC2_GC::OnTCPUDPDataReceived(char* data, unsigned int len)
{
	this->ParseXML(data);
	this->PrepareResponse();

	//Send response back
	this->clServer->SendToAll(this->sPreparedXMLData.c_str(), this->sPreparedXMLData.length());

	for (unsigned int i = 0; i < this->cMessageReceivedDelegators.size(); i++)
	{

		if (!this->ErrorOccurred())
		{
			this->cMessageReceivedDelegators[i]->OnMessageReceived();
		}
		else if (this->ErrorOccurred())
		{
			this->cMessageReceivedDelegators[i]->OnErrorOccurred();
		}
	}
}

void KRC2_GC::InitializeTCP(std::string ServerIP, unsigned short port)
{
	if (this->yIsInitialized == false)
	{
		this->clServer = new TCPUDPServer();
		this->clServer->RegisterDataReceivedDelegate(this);
		this->clServer->StartTCPServer(ServerIP, port);

		this->yIsInitialized = true;
	}
}

void KRC2_GC::InitializeUDP(std::string ServerIP, unsigned short port)
{
	if (this->yIsInitialized == false)
	{
		this->clServer = new TCPUDPServer();
		this->clServer->RegisterDataReceivedDelegate(this);
		this->clServer->StartUDPServer(ServerIP, port);

		this->yIsInitialized = true;
	}
}

void KRC2_GC::Terminate(void)
{
	//SHUTDOWN SERVER and reset all flags etc.
	if (this->yIsInitialized)
	{
		this->clServer->StopServer();
		this->clServer = 0;
		delete this->clServer;
		this->yIsInitialized = false;
		this->iStatus = KRC2_GC::ROBOT_STATUS_NONE;
	}
}

bool KRC2_GC::Initialized()
{
	return this->yIsInitialized;
}

bool KRC2_GC::ErrorOccurred()
{
	return false;
}

int KRC2_GC::GetStatus(void)
{
	return this->iStatus;
}

bool KRC2_GC::IsRobotIdle()
{
	return (this->iStatus&KRC2_GC::ROBOT_IDLE) != 0;
}

bool KRC2_GC::IsRobotInStartPosition()
{
	return (this->iStatus&KRC2_GC::ROBOT_IN_START_POSITION) != 0;
}

bool KRC2_GC::IsRobotMoving()
{
	return (this->iStatus&KRC2_GC::ROBOT_ROBOT_MOVING) != 0;
}

bool KRC2_GC::IsRobotMovementReady()
{
	return (this->iStatus&KRC2_GC::ROBOT_MOVEMENT_READY) != 0;
}

bool KRC2_GC::IsPositionUnreachable()
{
	return (this->iStatus&KRC2_GC::ROBOT_POSITION_UNREACHABLE) != 0;
}


void KRC2_GC::MovePTP(double a1, double a2, double a3, double a4, double a5, double a6)
{
	this->dE2Soll = KRC2_GC::MOVE_MODE_PTP;
	this->dA1Soll = a1;
	this->dA2Soll = a2;
	this->dA3Soll = a3;
	this->dA4Soll = a4;
	this->dA5Soll = a5;
	this->dA6Soll = a6;
}

void KRC2_GC::MoveLIN(double x, double y, double z, double a, double b, double c)
{
	this->dE2Soll = KRC2_GC::MOVE_MODE_LIN;
	this->dA1Soll = x;
	this->dA2Soll = y;
	this->dA3Soll = z;
	this->dA4Soll = a;
	this->dA5Soll = b;
	this->dA6Soll = c;
}

void KRC2_GC::MoveCIRC(void)
{

}


void KRC2_GC::SetPathCorrection(double x, double y, double z, double a, double b, double c)
{
	this->dXSoll = x;
	this->dYSoll = y;
	this->dZSoll = z;
	this->dASoll = a;
	this->dBSoll = b;
	this->dCSoll = c;
}

void KRC2_GC::ResetPathCorrection(void)
{
	this->dXSoll = 0.0;
	this->dYSoll = 0.0;
	this->dZSoll = 0.0;
	this->dASoll = 0.0;
	this->dBSoll = 0.0;
	this->dCSoll = 0.0;
}

void KRC2_GC::SetPathCorrectionX(double x)
{
	this->dXSoll = x;
}

void KRC2_GC::SetPathCorrectionY(double y)
{
	this->dYSoll = y;
}

void KRC2_GC::SetPathCorrectionZ(double z)
{
	this->dZSoll = z;
}

void KRC2_GC::SetPathCorrectionA(double a)
{
	this->dASoll = a;
}

void KRC2_GC::SetPathCorrectionB(double b)
{
	this->dBSoll = b;
}

void KRC2_GC::SetPathCorrectionC(double c)
{
	this->dCSoll = c;
}

double* KRC2_GC::GetPathCorrection(bool sollValue)
{
	double* result = new double[6];
	if (sollValue == false)
	{
		result[0] = this->dX;
		result[1] = this->dY;
		result[2] = this->dZ;
		result[3] = this->dA;
		result[4] = this->dB;
		result[5] = this->dC;
	}
	else
	{
		result[0] = this->dXSoll;
		result[1] = this->dYSoll;
		result[2] = this->dZSoll;
		result[3] = this->dASoll;
		result[4] = this->dBSoll;
		result[5] = this->dCSoll;
	}
	return result;
}

double* KRC2_GC::GetJoints(bool sollValue)
{
	double* result = new double[6];
	if (sollValue == false)
	{
		result[0] = this->dA1;
		result[1] = this->dA2;
		result[2] = this->dA3;
		result[3] = this->dA4;
		result[4] = this->dA5;
		result[5] = this->dA6;
	}
	else
	{
		result[0] = this->dA1Soll;
		result[1] = this->dA2Soll;
		result[2] = this->dA3Soll;
		result[3] = this->dA4Soll;
		result[4] = this->dA5Soll;
		result[5] = this->dA6Soll;
	}
	return result;
}

void KRC2_GC::SetVelocity(double velocity)
{
	this->dE3Soll = velocity / KRC2_GC::MAX_VELOCITY * 100;
}

void KRC2_GC::SetVelocityPercentage(double velPercentage)
{
	this->dE3Soll = velPercentage;
}

void KRC2_GC::SetAcceleration(double acceleration)
{
	this->dE4Soll = acceleration;
}

void KRC2_GC::SetAccelerationPercentage(double accPercentage)
{
	//TODO
}

void KRC2_GC::SetRounding(double rounding)
{
	this->dE5Soll = rounding;
}

void KRC2_GC::SetRoundingPercentage(double rPercentage)
{
	//TODO
}


double KRC2_GC::GetVelocity(bool sollValue)
{
	if (sollValue == false)
	{
		return this->dVelocity;
	}
	return this->dE3Soll;
}

double KRC2_GC::GetVelocityPercentage(bool sollValue)
{
	if (sollValue == false)
	{
		return this->dVelocity / KRC2_GC::MAX_VELOCITY;
	}

	return this->dE3Soll / KRC2_GC::MAX_VELOCITY;
}

double KRC2_GC::GetAcceleration(bool sollValue)
{
	if (sollValue == false)
	{
		return this->dAcceleration;
	}
	return this->dE4Soll;
}

double KRC2_GC::GetAccelerationPercentage(bool sollValue)
{
	return 0;
}

double KRC2_GC::GetRounding(bool sollValue)
{
	if (sollValue == false)
	{
		return this->dRounding;
	}
	return this->dE5Soll;
}

double KRC2_GC::GetRoundingPercentage(bool sollValue)
{
	return 0;
}


void KRC2_GC::SetCounter(double value)
{
	//TODO
}

void KRC2_GC::ResetCounter()
{
	//TODO
}

void KRC2_GC::IncrementCounter(double increment)
{
	//TODO
}

void KRC2_GC::DecrementCounter(double decrement)
{
	//TODO
}

double KRC2_GC::GetCounter()
{
	return 0;
}

void KRC2_GC::AddKRC2Delegator(KRC2Delegator* cKRC2Delegator)
{
	this->cMessageReceivedDelegators.push_back(cKRC2Delegator);
}