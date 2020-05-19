#include "transceiver.h"
#include "message.h"
#include "simulation-monitor.h"
#include "generator.h"
#include <math.h>

Transceiver::Transceiver(unsigned k, SimulationMonitor* m,double seed_cgp, double seed_ack,
		double seed_ctp, double seed_crp) : id_(k),monitor_(m),
gen_cgp_(seed_cgp),gen_ack_(seed_ack),gen_ctp_(seed_ctp),gen_crp_(seed_crp)
{
	
}

Transceiver::Transceiver():id_(-1), monitor_(nullptr),
gen_cgp_(21),gen_ack_(22),gen_ctp_(24),gen_crp_(25)
{
	
}
const int Transceiver::kP=0.8;

void Transceiver::GenMsg()
{
	// Random time for generate
	int Cgp = gen_cgp_.RandExp();
	// Generate new message
	const auto new_msg = new Message(this);
	new_msg->Activate(round(Cgp));
}

void Transceiver::AddToBuffer(Message* msg)
{
	msg_buffer_.push_back(msg);
}

void Transceiver::RecieveResponse()
{
	// Function randomizing that sending message have error or not
	// Ad-hoc just for exercise 3
	bool resp = gen_ack_.RandZeroOne(kP);
	if(!resp) msg_buffer_[0]->SetAck();
	else  msg_buffer_[0]->SetTer();
}

bool Transceiver::TrxCheckChannel()
{
	return monitor_->TrxCheckChannel();
}

void Transceiver::TrxDetectCollsion(Message* msg)
{
	monitor_->TrxDetectCollsion(msg);
}

void Transceiver::TrxUnblockChannel(Message* msg)
{
	monitor_->TrxUnblockChannel(msg);
}

void Transceiver::TrxBlockChannel(Message* msg)
{
	monitor_->TrxBlockChannel(msg);
}

int Transceiver::MsgBufferSize() const
{
	return msg_buffer_.size();
}

void Transceiver::ActivateProcess(uint64_t time, Message* process)
{
	monitor_->AddNewProcessAction(time, process);
}

void Transceiver::ActivateNextProcess()
{
	msg_buffer_[0]->Activate(5);
}

int Transceiver::GenCtpTime()
{
	return round(gen_ctp_.Rand(kMinTime,kMaxTime));
}

int Transceiver::GenCrpTime(int ctp, int r)
{
	int Rv = round(gen_crp_.Rand(0,pow(2,r)-1));
	return ctp*Rv;
}


int Transceiver::GetId() const
{
	return id_;
}

bool Transceiver::IsBufferEmpty() const
{
	return msg_buffer_.empty();
}

uint64_t Transceiver::GetTime() const
{
	return monitor_->GetTime();
}

void Transceiver::SetCollision()
{
	// Set collision in first message in buffer
	msg_buffer_[0]->SetCollision();
}

void Transceiver::DeleteMsg()
{
	// Check that buffer is empty
	if(!msg_buffer_.empty())
	{
		// Delete first message form buffer
		msg_buffer_.erase(msg_buffer_.begin());
	}
	msg_buffer_.shrink_to_fit();
	
}