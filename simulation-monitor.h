#ifndef SIMULATIONMONITOR_H_
#define SIMULATIONMONITOR_H_

#include <algorithm>
#include <vector>
#include "wireless-network.h"
#include "statistics.h"

using std::vector;

class SimulationMonitor
{
	// This class represents simulating environment
	// Store vector of available Transceiver
	// Vector of all events
	// and helps the process in communicating with others
public:
	// Constructors and destructor
	~SimulationMonitor();
	SimulationMonitor(int sim_time,int fist_state_time,bool type);
	// Setters and getters
	uint64_t GetTime() const;
	uint64_t GetStatTime() const { return full_time_ - stat_time_ ;};
	// Add new process to queue
	void AddNewProcessAction(uint64_t time, Message* process);
	// Start simulation
	void Start();
	// Reset func for next simulation
	void Reset();
	// Operate on channel
	bool TrxCheckChannel() const;
	void TrxBlockChannel(Message* msg) const;
	void TrxDetectCollsion(Message* msg) const;
	void TrxUnblockChannel(Message* msg) const;
	// Clock operation
	void change_time();
	// Delete process from queue
	void DeleteProcess();
	// Generate seed
	void GenerateSeeds();

	// Statistic function
	void ShowResult();
private:
	// Statistic variable
	unsigned total_num_;
	unsigned num_of_fail_;
	// Simulation variable
	const uint64_t full_time_;
	uint64_t stat_time_;
	// Stepping
	bool stepping_;
	//-- Vector of process to execute in schedule time
	vector<Message*> process_queue_;
	// Main clock
	uint64_t clock_;
	// Tmp variable for current operate process
	Message* current_;
	// Wireless network object
	WirelessNetwork* wireless_network_;
	// Function for sorting event by time
	static bool Comp(Message* lhs, Message* rhs);
	//Statistic object
	Statistics statistics_;
	// Seeds
	vector<double> seeds_;
};

#endif//SIMULATIONMONITOR_H_

