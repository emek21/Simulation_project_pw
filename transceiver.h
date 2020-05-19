#ifndef Transceiver_H
#define Transceiver_H

#include<vector>
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
private:
	// Id is used for inspection Transceiver's object  in SimulationMonitor's object
	// or collision detection
	unsigned id_;
	// Buffer for message to send
	vector<Message*> msg_buffer_;
	// Pointer for access monitor method 
	SimulationMonitor* monitor_;
	
	//Statistic
	static int amout_of_send_;
	static int amout_of_drop_;

	// Generators
	Generator gen_cgp_;
	Generator gen_ack_;
	Generator gen_ctp_;
	Generator gen_crp_;
	// Static for generator
	static const int kP;
    static const int kMaxTime = 100;
    static const int kMinTime = 10;
public:
	// Constructor and destructor
	Transceiver();
	Transceiver(unsigned k, SimulationMonitor* m,double seed_cgp, double seed_ack,
		double seed_ctp, double seed_crp);
	~Transceiver() = default;
	// Setters and getters
	int GetId() const;
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
};


#endif//Transceiver_H