// This is the main DLL file.

#include "stdafx.h"

#include <msclr\marshal_cppstd.h>
#include "KRC2_GC_Managed.h"
#include <KRC2_GC.h>

using namespace System;
using namespace System::Configuration;

namespace RobotDrivers 
{

	KRC2_GC_M::KRC2_GC_M(void)
		:
			bKRC2DriverRunning(false),
			cMainThread(nullptr),
			aPathCorrection(nullptr),
			aJoints(nullptr)
	{
		this->krc2_gc_driver = new KRC2_GC();
		this->aPathCorrection = gcnew array<double>(6);
		this->aJoints = gcnew array<double>(6);
	}

	KRC2_GC_M::~KRC2_GC_M(void)
	{
		delete this->krc2_gc_driver;
	}

	void KRC2_GC_M::ThreadMain()
	{
		long long llIPOC = this->krc2_gc_driver->llIPOC;

		while (this->bKRC2DriverRunning == true)
		{
			this->bKRC2DriverRunning = this->bKRC2DriverRunning && !this->krc2_gc_driver->ErrorOccurred();

			if (this->krc2_gc_driver->llIPOC != llIPOC)
			{
				this->cMessageReceivedDelegator();
			}

			if (this->krc2_gc_driver->ErrorOccurred())
			{
				this->cErrorOccurred();
			}
		}

		this->bKRC2DriverRunning = false;
	}

	void KRC2_GC_M::InitializeTCP(String^ ServerIP, unsigned short port)
	{
		this->krc2_gc_driver->InitializeTCP(msclr::interop::marshal_as<std::string>(ServerIP), port);
		if (this->krc2_gc_driver->Initialized() == true)
		{
			this->cMainThread = gcnew Thread(gcnew ThreadStart(this, &KRC2_GC_M::ThreadMain));
			this->cMainThread->Name = "KRC2_GC_M MainThread";
			this->bKRC2DriverRunning = true;
			this->cMainThread->Start();
		}
	}

	void KRC2_GC_M::InitializeUDP(String^ ServerIP, unsigned short port)
	{
		this->krc2_gc_driver->InitializeUDP(msclr::interop::marshal_as<std::string>(ServerIP), port);
		if (this->krc2_gc_driver->Initialized() == true)
		{
			this->cMainThread = gcnew Thread(gcnew ThreadStart(this, &KRC2_GC_M::ThreadMain));
			this->cMainThread->Name = "KRC2_GC_M MainThread";
			this->bKRC2DriverRunning = true;
			this->cMainThread->Start();
		}
	}

	void KRC2_GC_M::Terminate(void)
	{
		this->krc2_gc_driver->Terminate();
	}

	bool KRC2_GC_M::Initialized(void)
	{
		return this->krc2_gc_driver->Initialized();
	}

	bool KRC2_GC_M::ErrorOccurred(void)
	{
		return this->krc2_gc_driver->ErrorOccurred();
	}


	int KRC2_GC_M::GetStatus(void)
	{
		return this->krc2_gc_driver->GetStatus();
	}


	void KRC2_GC_M::MovePTP(double a1, double a2, double a3, double a4, double a5, double a6)
	{
		this->krc2_gc_driver->MovePTP(a1, a2, a3, a4, a5, a6);
	}

	void KRC2_GC_M::MoveLIN(double x, double y, double z, double a, double b, double c)
	{
		this->krc2_gc_driver->MoveLIN(x, y, z, a, b, c);
	}

	void KRC2_GC_M::MoveCIRC(void)
	{
		this->krc2_gc_driver->MoveCIRC();
	}


	void KRC2_GC_M::SetPathCorrection(double x, double y, double z, double a, double b, double c)
	{
		this->krc2_gc_driver->SetPathCorrection(x, y, z, a, b, c);
	}

	void KRC2_GC_M::ResetPathCorrection(void)
	{
		this->krc2_gc_driver->ResetPathCorrection();
	}

	void KRC2_GC_M::SetPathCorrectionX(double x)
	{
		this->krc2_gc_driver->SetPathCorrectionX(x);
	}

	void KRC2_GC_M::SetPathCorrectionY(double y)
	{
		this->krc2_gc_driver->SetPathCorrectionY(y);
	}

	void KRC2_GC_M::SetPathCorrectionZ(double z)
	{
		this->krc2_gc_driver->SetPathCorrectionZ(z);
	}

	void KRC2_GC_M::SetPathCorrectionA(double a)
	{
		this->krc2_gc_driver->SetPathCorrectionA(a);
	}

	void KRC2_GC_M::SetPathCorrectionB(double b)
	{
		this->krc2_gc_driver->SetPathCorrectionB(b);
	}

	void KRC2_GC_M::SetPathCorrectionC(double c)
	{
		this->krc2_gc_driver->SetPathCorrectionC(c);
	}

	array<double>^ KRC2_GC_M::GetPathCorrection()
	{
		double* dTmpArr = this->krc2_gc_driver->GetPathCorrection(false);

		this->aPathCorrection[0] = dTmpArr[0];
		this->aPathCorrection[1] = dTmpArr[1];
		this->aPathCorrection[2] = dTmpArr[2];
		this->aPathCorrection[3] = dTmpArr[3];
		this->aPathCorrection[4] = dTmpArr[4];
		this->aPathCorrection[5] = dTmpArr[5];

		delete dTmpArr;

		return this->aPathCorrection;
	}

	array<double>^ KRC2_GC_M::GetPathCorrection(bool sollValue)
	{
		double* dTmpArr = this->krc2_gc_driver->GetPathCorrection(sollValue);

		this->aPathCorrection[0] = dTmpArr[0];
		this->aPathCorrection[1] = dTmpArr[1];
		this->aPathCorrection[2] = dTmpArr[2];
		this->aPathCorrection[3] = dTmpArr[3];
		this->aPathCorrection[4] = dTmpArr[4];
		this->aPathCorrection[5] = dTmpArr[5];

		delete dTmpArr;

		return this->aPathCorrection;
	}

	array<double>^ KRC2_GC_M::GetJoints()
	{
		double* dTmpArr = this->krc2_gc_driver->GetJoints();

		this->aJoints[0] = dTmpArr[0];
		this->aJoints[1] = dTmpArr[1];
		this->aJoints[2] = dTmpArr[2];
		this->aJoints[3] = dTmpArr[3];
		this->aJoints[4] = dTmpArr[4];
		this->aJoints[5] = dTmpArr[5];

		delete dTmpArr;

		return this->aJoints;
	}

	array<double>^ KRC2_GC_M::GetJoints(bool sollValue)
	{
		double* dTmpArr = this->krc2_gc_driver->GetJoints(sollValue);

		this->aJoints[0] = dTmpArr[0];
		this->aJoints[1] = dTmpArr[1];
		this->aJoints[2] = dTmpArr[2];
		this->aJoints[3] = dTmpArr[3];
		this->aJoints[4] = dTmpArr[4];
		this->aJoints[5] = dTmpArr[5];

		delete dTmpArr;

		return this->aJoints;
	}

	void KRC2_GC_M::SetVelocity(double velocity)
	{
		this->krc2_gc_driver->SetVelocity(velocity);
	}

	void KRC2_GC_M::SetVelocityPercentage(double velPercentage)
	{
		this->krc2_gc_driver->SetVelocityPercentage(velPercentage);
	}

	void KRC2_GC_M::SetAcceleration(double acceleration)
	{
		this->krc2_gc_driver->SetAcceleration(acceleration);
	}

	void KRC2_GC_M::SetAccelerationPercentage(double accPercentage)
	{
		this->krc2_gc_driver->SetAccelerationPercentage(accPercentage);
	}

	void KRC2_GC_M::SetRounding(double rounding)
	{
		this->krc2_gc_driver->SetRounding(rounding);
	}

	void KRC2_GC_M::SetRoundingPercentage(double rPercentage)
	{
		this->krc2_gc_driver->SetRounding(rPercentage);
	}


	double KRC2_GC_M::GetVelocity()
	{
		return this->GetVelocity(false);
	}

	double KRC2_GC_M::GetVelocityPercentage()
	{
		return this->GetVelocityPercentage(false);
	}

	double KRC2_GC_M::GetAcceleration()
	{
		return this->GetAcceleration(false);
	}

	double KRC2_GC_M::GetAccelerationPercentage()
	{
		return this->GetAccelerationPercentage(false);
	}

	double KRC2_GC_M::GetRounding()
	{
		return this->GetRounding(false);
	}

	double KRC2_GC_M::GetRoundingPercentage()
	{
		return this->GetRoundingPercentage(false);
	}


	double KRC2_GC_M::GetVelocity(bool sollValue)
	{
		return this->krc2_gc_driver->GetVelocity(sollValue);
	}

	double KRC2_GC_M::GetVelocityPercentage(bool sollValue)
	{
		return this->krc2_gc_driver->GetVelocityPercentage(sollValue);
	}

	double KRC2_GC_M::GetAcceleration(bool sollValue)
	{
		return this->krc2_gc_driver->GetAcceleration(sollValue);
	}

	double KRC2_GC_M::GetAccelerationPercentage(bool sollValue)
	{
		return this->krc2_gc_driver->GetAccelerationPercentage(sollValue);
	}

	double KRC2_GC_M::GetRounding(bool sollValue)
	{
		return this->krc2_gc_driver->GetRounding(sollValue);
	}

	double KRC2_GC_M::GetRoundingPercentage(bool sollValue)
	{
		return this->krc2_gc_driver->GetRoundingPercentage(sollValue);
	}


	void KRC2_GC_M::SetCounter(double value)
	{
		this->krc2_gc_driver->SetCounter(value);
	}

	void KRC2_GC_M::ResetCounter()
	{
		return this->krc2_gc_driver->ResetCounter();
	}

	void KRC2_GC_M::IncrementCounter(double increment)
	{
		this->krc2_gc_driver->IncrementCounter(increment);
	}

	void KRC2_GC_M::DecrementCounter(double decrement)
	{
		this->krc2_gc_driver->DecrementCounter(decrement);
	}

	double KRC2_GC_M::GetCounter()
	{
		return this->krc2_gc_driver->GetCounter();
	}

}