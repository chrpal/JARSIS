// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the KR30_16_GC_DRIVER_NATIVE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// KR30_16_GC_DRIVER_NATIVE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef KRC2_GC_H
#define KRC2_GC_H

class TCPUDPServer;
#include "Defines.h"
#include <string>
#include "TypeDefs.h"
#include <vector>

class DLL_SYMBOL KRC2_GC : public TCPUDPDataReceivedDelegator {
protected:
	bool yIsInitialized;
	int iStatus;
	TCPUDPServer* clServer;

	double dX;
	double dY;
	double dZ;
	double dA;
	double dB;
	double dC;

	double dA1;
	double dA2;
	double dA3;
	double dA4;
	double dA5;
	double dA6;

	double dE1;
	double dE2;
	double dE3;
	double dE4;
	double dE5;
	double dE6;

	double dVelocity;
	double dAcceleration;
	double dRounding;

	


	double dXSoll;
	double dYSoll;
	double dZSoll;
	double dASoll;
	double dBSoll;
	double dCSoll;

	double dA1Soll;
	double dA2Soll;
	double dA3Soll;
	double dA4Soll;
	double dA5Soll;
	double dA6Soll;

	double dE1Soll;
	double dE2Soll;
	double dE3Soll;
	double dE4Soll;
	double dE5Soll;
	double dE6Soll;
	int iBreakDirectControl;

	std::string sReceivedXMLData;
	std::string sPreparedXMLData;

	static std::string sXMLResponseTemplate;

	std::vector<KRC2Delegator*> cMessageReceivedDelegators;

	void ParseXML(char* data);
	void PrepareResponse();

public:
	long long llIPOC;
	std::string sStatus;

	static const int ROBOT_STATUS_NONE;
	static const int ROBOT_IDLE;
	static const int ROBOT_IN_START_POSITION;
	static const int ROBOT_ROBOT_MOVING;
	static const int ROBOT_MOVEMENT_READY;
	static const int ROBOT_POSITION_UNREACHABLE;
	static const int ROBOT_ERROR_INITIALIZATION;
	static const double MAX_VELOCITY;
	static const double MOVE_MODE_PTP;
	static const double MOVE_MODE_LIN;
	static const double MOVE_MODE_CIRC;

	KRC2_GC(void);
	virtual ~KRC2_GC(void);

	void OnTCPUDPDataReceived(char* data, unsigned int len);

	void InitializeTCP(std::string ServerIP = "127.0.0.1", unsigned short port = 6008);
	void InitializeUDP(std::string ServerIP = "127.0.0.1", unsigned short port = 6008);
	void Terminate(void);
	bool Initialized(void);
	bool ErrorOccurred(void);

	int GetStatus(void);
	bool IsRobotIdle();
	bool IsRobotInStartPosition();
	bool IsRobotMoving();
	bool IsRobotMovementReady();
	bool IsPositionUnreachable();

	void MovePTP(double a1, double a2, double a3, double a4, double a5, double a6);
	void MoveLIN(double x, double y, double z, double a, double b, double c);
	void MoveCIRC(void);

	void SetPathCorrection(double x, double y, double z, double a, double b, double c);
	void ResetPathCorrection(void);
	void SetPathCorrectionX(double x);
	void SetPathCorrectionY(double y);
	void SetPathCorrectionZ(double z);
	void SetPathCorrectionA(double a);
	void SetPathCorrectionB(double b);
	void SetPathCorrectionC(double c);
	double* GetPathCorrection(bool sollValue = false);
	double* GetJoints(bool sollValue = false);

	void SetVelocity(double velocity);
	void SetVelocityPercentage(double velPercentage);
	void SetAcceleration(double acceleration);
	void SetAccelerationPercentage(double accPercentage);
	void SetRounding(double rounding);
	void SetRoundingPercentage(double rPercentage);

	double GetVelocity(bool sollValue = false);
	double GetVelocityPercentage(bool sollValue = false);
	double GetAcceleration(bool sollValue = false);
	double GetAccelerationPercentage(bool sollValue = false);
	double GetRounding(bool sollValue = false);
	double GetRoundingPercentage(bool sollValue = false);

	void SetCounter(double value);
	void ResetCounter();
	void IncrementCounter(double increment = 1.0);
	void DecrementCounter(double decrement = 1.0);
	double GetCounter();

	void AddKRC2Delegator(KRC2Delegator* cKRC2Delegator);
};

#endif