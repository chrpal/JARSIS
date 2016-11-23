// KRC2_GC_Managed.h

#pragma once

class TCPUDPServer;
class KRC2_GC;

using namespace System;
using namespace System::Threading;

namespace RobotDrivers {

	public delegate void KRC2_MessageReceivedDelegator();
	public delegate void KRC2_ErrorOccurred();

	public ref class KRC2_GC_M
	{
	protected:
		bool bKRC2DriverRunning;
		Thread^ cMainThread;
		KRC2_GC* krc2_gc_driver;
		array<double>^ aPathCorrection;
		array<double>^ aJoints;

		void ThreadMain();

	public:

		KRC2_MessageReceivedDelegator^ cMessageReceivedDelegator;
		KRC2_ErrorOccurred^ cErrorOccurred;

		KRC2_GC_M(void);
		virtual ~KRC2_GC_M(void);

		void InitializeTCP(String^ ServerIP, unsigned short port);
		void InitializeUDP(String^ ServerIP, unsigned short port);
		void Terminate(void);
		bool Initialized(void);
		bool ErrorOccurred(void);

		int GetStatus(void);

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
		array<double>^ GetPathCorrection();
		array<double>^ GetPathCorrection(bool sollValue);
		array<double>^ GetJoints();
		array<double>^ GetJoints(bool sollValue);

		void SetVelocity(double velocity);
		void SetVelocityPercentage(double velPercentage);
		void SetAcceleration(double acceleration);
		void SetAccelerationPercentage(double accPercentage);
		void SetRounding(double rounding);
		void SetRoundingPercentage(double rPercentage);

		double GetVelocity();
		double GetVelocityPercentage();
		double GetAcceleration();
		double GetAccelerationPercentage();
		double GetRounding();
		double GetRoundingPercentage();

		double GetVelocity(bool sollValue);
		double GetVelocityPercentage(bool sollValue);
		double GetAcceleration(bool sollValue);
		double GetAccelerationPercentage(bool sollValue);
		double GetRounding(bool sollValue);
		double GetRoundingPercentage(bool sollValue);

		void SetCounter(double value);
		void ResetCounter();
		void IncrementCounter(double increment);
		void DecrementCounter(double decrement);
		double GetCounter();
	};
}
