#ifndef SIMULATIONMONITOR_H_
#define SIMULATIONMONITOR_H_

#include <algorithm>
#include <vector>
#include "wireless-network.h"
#include "statistics.h"

using std::vector;

class SimulationMonitor
{
	// This class represents simulating environment.
	// Controls the simulation and manages the clock.
	// Helps process in communicating with others
public:
	// Constructors and destructor
	~SimulationMonitor();
	SimulationMonitor(int sim_time,int fist_state_time,bool type,int add_condition);
	// Setters and getters
	uint64_t GetTime() const;
	uint64_t GetStatTime() const { return full_time_ - stat_time_ ;};
	// Add new process to queue
	void AddNewProcessAction(uint64_t time, Message* process);
	// Start simulation
	void Start(int sim_nr);
	// Reset func for next simulation
	void Reset(int sim_nr);
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
	// Simulation variable
	// Full simulation time
	const uint64_t full_time_;
	// Time to measure statistics
	const uint64_t stat_time_;
	// Additional end condition ( message sent)
	int add_condition_;
	// Stepping flag
	bool stepping_;
	// Vector of process to execute in schedule time
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
	vector<Statistics*> statistics_;
	// Seeds
	vector<int> seeds_;
};

#endif//SIMULATIONMONITOR_H_

