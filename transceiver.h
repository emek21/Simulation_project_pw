#ifndef Transceiver_H
#define Transceiver_H

#include <vector>
#include "message.h"
#include "spdlog/spdlog.h"
#include "generator.h"

using std::vector;

class SimulationMonitor;

class Transceiver
{
	// This class describes transmitter-receiver 
	// Generating message, Sending message and receiving message
	// and storing in buffer
public:
	// Constructor and destructor
	Transceiver(unsigned k, SimulationMonitor* m,int seed_cgp, int seed_ack,
		int seed_ctp, int seed_crp);
	~Transceiver() = default;
	// Setters and getters
	unsigned int GetId() const;
	bool IsBufferEmpty() const;
	uint64_t GetTime() const;
	// Set collision flag in message
	void SetCollision();
	// Get buffer size
	int MsgBufferSize() const;
	// -- Generation message action
	void GenMsg();
	// -- Function simulating receive
	// Add created message into buffer
	void AddToBuffer(Message* msg);
	// Delete message from buffer
	void DeleteMsg();
	// Simulate Ack response
	void RecieveResponse();
	// Transmission functions
	bool TrxCheckChannel();
	void TrxDetectCollsion(Message* msg);
	void TrxUnblockChannel(Message* msg);
	void TrxBlockChannel(Message* msg);
	// Place process into queue in simulation monitor
	void ActivateProcess(uint64_t time, Message* process);
	// Start new message action (execute new message)
	void ActivateNextProcess();
	// Generate transmission time
	int GenCtpTime();
	int GenCrpTime(int ctp, int r);
	void SetIncSend() {amout_of_send_++;};
	void SetIncDrop() {amout_of_drop_++;};
	double GetAvgErrorRate() const ;
	static void SetMaxErrorRate(const double err) { max_error_rate_=err; };
	static double GetMaxErrorRate() { return max_error_rate_; };
	static void ResetStat() { max_error_rate_=0; };
	/*static double GetP() { return kP;};
	static void SetP(double p) {kP = p;};
	*/
private:
	// Id is used for inspection Transceiver's object  in SimulationMonitor's object
	// or collision detection
	unsigned id_;
	// Buffer for message to send
	vector<Message*> msg_buffer_;
	// Pointer for access monitor method 
	SimulationMonitor* monitor_;
	
	//Statistic
	// Amount of all successfully send messages 
	int amout_of_send_;
	// Amount of all lost messages 
	int amout_of_drop_;
	// Maximum error rate
	static double max_error_rate_;;

	// Generators
	Generator gen_cgp_;
	Generator gen_ack_;
	Generator gen_ctp_;
	Generator gen_crp_;
	// Static for generator
	// Probability for generator
	const static double kP;
	// Range for generator
    static const int kMaxTime = 100;
    static const int kMinTime = 10;
};


#endif//Transceiver_H