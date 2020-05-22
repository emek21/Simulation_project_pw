#include "message.h"
#include <cstdlib>
#include "spdlog/spdlog.h"
#include "transceiver.h"

using std::shared_ptr;
using spdlog::stdout_color_mt;

// Define static values
int Message::amount_of_retr_ = 0;
int Message::amount_of_msg_ = 0;
int Message::amount_of_success_ = 0;
int Message::drop_counter_ = 0;
double Message::avg_delay_ = 0;
double Message::avg_wait_ = 0;
int Message::no_of_wait_msg_ = 0;
int Message::no_of_delay_msg_ = 0;

Message::Message(Transceiver* trx): trx_(trx),retr_counter_(0),
ack_(false),ter_(false),terminated_(false),collision_(0),wait_for_ack_(false),
wait_time_(false),active_(false),schedule_time_(0),create_time_(0),send_time_(0),
exit_buffer_time_(0)
{
	// Random ctp value
	ctp_ = trx_->GenCtpTime();
	// inc amount of all message
	amount_of_msg_++;
	// set starting phase
	current_phase_ = phase_::gen_msg;
}

shared_ptr<spdlog::logger> Message::logger_ = stdout_color_mt("Process");

bool Message::Retransmission()
{
	// Set flags to default
	ack_ = false;
	ter_ = false;
	// Increment counters
	retr_counter_++;
	amount_of_retr_++;
	// Return that is possible 
	return retr_counter_<=kLR? true:false;
}

bool Message::GetAck() const
{
	return ack_;
}

void Message::SetAck()
{
	ack_ = true;
}

bool Message::GetTer() const
{
	return ter_;
}

void Message::SetTer()
{
	ter_ = true;
}

void Message::SetSendTime(uint64_t time)
{
	send_time_ = time;
}

void Message::SetExitBufferTime(uint64_t time)
{
	exit_buffer_time_ = time;
}

int Message::GetTrxId() const
{
	return trx_->GetId();
}

void Message::SetCollision()
{
	collision_=true;
}

uint64_t Message::GetTime() const
{
	return schedule_time_;
}

void Message::SetScheduleTime(uint64_t time)
{
	schedule_time_ = time;
}

bool Message::IsTerminated() const
{
	return terminated_;
}

void Message::Activate(uint64_t time)
{
	// Put process into queue
	// time = duration of the action
	trx_->ActivateProcess(time,this);
}

void Message::DeleteMsg()
{
	// Delete message form buffer and
	// set terminated flag
	terminated_ = true;
	trx_->DeleteMsg();
}

// Handle first phase
void Message::GenMsgExec()
{
	logger_->debug("\n\tMessage in {} transceiver created\n"
									,trx_->GetId());
	// Add created message to buffer
	trx_->AddToBuffer(this);
	create_time_ = trx_->GetTime();
	// check that this is the first message in buffer
	// if not wait for activation
	if(trx_->MsgBufferSize() > 1) active_ = false;
	// Generate new message
	trx_->GenMsg();
	// Go to next phase
	current_phase_ = phase_::check_ch;
}

// Handle second phase
void Message::CheckChExec()
{
	// Check channel status
	// and inc or reset counter
	if (trx_->TrxCheckChannel() == true) wait_time_++;
	else wait_time_ = 0;
	
	logger_->debug("\n\tMessage in {} transceiver checking channel \n"
		"\t** phase CheckCh(1) **\n "
		"\twaiting time for channel = {}\n"
		,trx_->GetId(),wait_time_);
	// Decide to go to next phase
	if (wait_time_ == kMaxWaitTime)
	{
		// Next phase
		current_phase_ = phase_::send_msg;
		wait_time_ = 0;
		// wait one quantum of time
		Activate(1);
	}
	else
	{
		// Wait to next check
		Activate(5);
	}
	active_ = false;
}

// Handle third phase
void Message::SendMsgExec()
{
	logger_->debug("\n\tMessage in {} transceiver sending\n"
		"\tphase SendMsg(2)\n"
		,trx_->GetId());
	// Set statistic wait time
	exit_buffer_time_ = trx_->GetTime()-create_time_;
	SetAvgWait(exit_buffer_time_);
	// Block channel by itself (Push to channel's vector)
	trx_->TrxBlockChannel(this);
	// Detect first collision possible
	trx_->TrxDetectCollsion(this);
	logger_->debug("\n\tMessage in {} transceiver send\n"
			,trx_->GetId());
	current_phase_ = phase_::wait_for_resp;
	// wait sending message time + ack time
	Activate(ctp_);
	active_ = false;
	
}

// Handle fourth phase
void Message::WaitForRespExec()
{
	// Detect second possible collision
	if(collision_ == false)
	{
		trx_->TrxDetectCollsion(this);
	}
	
	// Decide about transmission
	if(collision_ == false)
	{
		if(wait_for_ack_ == false)
		{
			// Receive response form trx
			trx_->RecieveResponse();
		}
		// Receive Ack or Ter
		if (ack_ == true) // Receive Ack
		{
			if(wait_for_ack_ == false)
			{
				logger_->debug("\n\tMessage in {} transceiver waiting for ack\n"
								"\tphase wait_for_resp(3)\n"
								,trx_->GetId());
				Activate(kCTIZ);
			}
			else
			{
				logger_->debug("\n\tMessage in {} transceiver send success! \n"
								"\tphase wait_for_resp(3)\n"
								,trx_->GetId());
				// Inc counters, Calculate times
				CalcSuccesStat();
				// Unlock channel (pop form channel's vector) when success
				trx_->TrxUnblockChannel(this);
				// Delete message form buffer 
				DeleteMsg();
				// Check size of trx buffer
				if(!trx_->IsBufferEmpty())
				{
					// Start new process action
					trx_->ActivateNextProcess();
				}
			}
			wait_for_ack_ = true;
			active_ = false;
		}
		else // Error in message sending 
		{
			logger_->debug("\n\tMessage in {} transceiver TER error occured\n"
								"\tphase wait_for_resp(3)\n"
								,trx_->GetId());
			// Unlock channel (pop form channel's vector) when error
			trx_->TrxUnblockChannel(this);
			// Active process when error ter 
			current_phase_ = phase_::retrans;
			active_ = true;
		}
	}
	else
	{
		// Unlock channel (pop form channel's vector) when collision
		trx_->TrxUnblockChannel(this);
		// collision handle
		logger_->debug("\n\tMessage in {} transceiver detect collision\n"
			,trx_->GetId());
		
		collision_ = false;
		current_phase_ = phase_::retrans;
		active_ = true;
	}
}

// Handle fifth phase
void Message::RetransExec()
{
	// Start Retransmission function in message
	// increment retransmission counter
	bool retr_able = Retransmission();
	logger_->debug("\nMessage in {} transceiver retransmission"
		"\tphase retrans(4)\n"
		"\tRetransmission nr = {}\n"
		,trx_->GetId(),retr_counter_);
	
	// check if it is possible to retransmission 
	if(retr_able) // possible
	{
		// Retransmit message
		// Go to seccond phase
		current_phase_ = phase_::check_ch;
		Activate(trx_->GenCrpTime(ctp_,static_cast<int>(retr_counter_)));
		amount_of_retr_++;
		active_ = false;
	}
	else 
	{
		// Delete message if
		// max retransmission attempt
		active_ = false;
		DeleteMsg();
		trx_->ActivateNextProcess();
		// Inc lost message counter
		drop_counter_++;
		// Inc lost message counter for one trx
		trx_->SetIncDrop();
	}
}


void Message::SetAvgDelay(uint64_t time)
{
	// Used to calculate the delay time
	avg_delay_ = avg_delay_+time;
	no_of_delay_msg_++;
}

void Message::SetAvgWait(uint64_t time)
{
	// Used to calculate the wait time
	avg_wait_ = avg_wait_+time;
	no_of_wait_msg_++;
}

void Message::ResetStat()
{
	// Reset statistic values
	amount_of_msg_ = 0;
	amount_of_retr_ = 0;
	amount_of_success_ = 0;
	drop_counter_ = 0;
	avg_delay_ = 0;
	avg_wait_ = 0;
	no_of_wait_msg_=0;
	no_of_delay_msg_=0;
}

void Message::CalcSuccesStat()
{
	// Calculating statistic for sent message
	amount_of_retr_ += static_cast<int>(retr_counter_);
	amount_of_success_++;
	trx_->SetIncSend();
	send_time_ = trx_->GetTime()-create_time_;
	SetAvgDelay(send_time_);
}


void Message::Execute() 
{
	// Transmission function
	active_ = true;

	while (active_)
	{
		switch (current_phase_)
		{
			// Waiting for message
		case phase_::gen_msg:
			{
				// This phase generate message and
				// push it to message buffer
				GenMsgExec();
				break;
			}
			// Checking channel
		case phase_::check_ch:
			{
				// Check that channel is free
				// if is check for 2ms go to phase 2
				CheckChExec();
				break;
			}
			// Send message
		case phase_::send_msg:
			{
				// Checking first time for collision
				// Blocks channel
				SendMsgExec();
				break;
			}
			// Wait for respond
		case phase_::wait_for_resp:
			{
				// Get response and check transmission status
				WaitForRespExec();
				break;
			}
			// Retransmission
		case phase_::retrans:
			{
				// Decide to retransmit or not
				RetransExec();
				break;
			}
		default:
			{
				// Error log 
				logger_->warn("\nProcess nr {} is in the default \n"
								,trx_->GetId());
				break;
			}
		}
	}
}
