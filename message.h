#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include "spdlog/spdlog.h"

class Transceiver;

class Message
{
	// This class describes sending message
	// Stores actual data about message
	// In simulation method this is a process
public:
	// Constructors
	Message(Transceiver* trx);
	~Message() = default;
	// Setters and getters
	bool GetAck() const;
	void SetAck();
	bool GetTer() const;
	void SetTer();
	void SetSendTime(uint64_t time);
	void SetExitBufferTime(uint64_t time);
	int GetTrxId() const;
	void SetCollision();
	uint64_t GetTime() const;
	void SetScheduleTime(uint64_t time);
	bool IsTerminated() const;
	// Functions
	// used in transmission
	// Reset flags, inc counter
	bool Retransmission();
	// Main process function
	void Execute();
	// Plan new process action
	void Activate(uint64_t time);
	// Delete message form trx buffer
	void DeleteMsg();
	// Execute functions
	void GenMsgExec();
	void CheckChExec();
	void SendMsgExec();
	void WaitForRespExec();
	void RetransExec();
	//Static getters and setters
	static int GetAmountOfMsg() { return amount_of_msg_; };
	static int GetAmountOfRetr() { return amount_of_retr_; };
	static int GetAmountOfSucces() { return amount_of_success_; };
	static int GetErrCounter() { return drop_counter_; };
	static double GetAvgDelay() { return avg_delay_/no_of_delay_msg_; };
	static double GetAvgWait() { return avg_wait_/no_of_wait_msg_; };
	// Stats function
	static void SetAvgDelay(uint64_t time);
	static void SetAvgWait(uint64_t time);
	static void ResetStat();
	void CalcSuccesStat();
	
private:
	// Pointer to parent transceiver 
	Transceiver* trx_;
	// Variable describing message settings
	unsigned retr_counter_;
	// Transmission time
	int ctp_;
	// Flags
	// Get ter or ack
	bool ack_;
	bool ter_;
	// End of process
	bool terminated_;
	// Detected collision 
	bool collision_;
	bool wait_for_ack_;
	// Counter for counting channel free time (max 40 = 2ms)
	int wait_time_;
	// Phase determine phase of action
	enum class phase_
	{
		gen_msg,
		check_ch,
		send_msg,
		wait_for_resp,
		retrans,
	};
	phase_ current_phase_;
	// Variable flag used in Execute function
	bool active_;
	// Process next execute time
	uint64_t schedule_time_;
	// Const retransmission limit counter
	// Project kLR = 10
	static const int kLR=5;
	// Const limit counter 2ms=20=4*5
	static int const kMaxWaitTime=4;
	// Ack time 1ms
	static int const kCTIZ = 10;
	
	// Statistic variable
	uint64_t create_time_;
	uint64_t send_time_;
	uint64_t exit_buffer_time_;
	// Amount of all created messages (debug)
	static int amount_of_msg_;
	// Amount of all retransmissions 
	static int amount_of_retr_;
	// Amount of all successfully send messages 
	static int amount_of_success_;
	// Amount of all lost messages 
	static int drop_counter_;
	// Average delay time
	static double avg_delay_;
	// Average wait time
	static double avg_wait_;
	// Numbers to calculate average of times
	static int no_of_wait_msg_;
	static int no_of_delay_msg_;
	//logger
	static std::shared_ptr<spdlog::logger> logger_;
};

#endif//MESSAGE_H