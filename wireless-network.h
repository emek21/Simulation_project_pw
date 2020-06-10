#ifndef WirelessNetwork_H
#define WirelessNetwork_H

#include "transceiver.h"
#include "channel.h"
#include <memory>

class SimulationMonitor;

class WirelessNetwork
{
public:
	WirelessNetwork(SimulationMonitor* monitor,vector<int> &seed);
	~WirelessNetwork() = default;
	// Check channel status
	bool IsChannelFree() const;
	// Operate on channel function
	void BlockChannel(Message* msg);
	void DetectCollsion(Message* msg);
	void UnblockChannel(Message* msg);
	// Find max error rate
	void CalcMaxErrorRate();
	
private:
	// Channel
	Channel ch_;
	// List of all transceivers
	vector<Transceiver*> list_of_transceivers_;
	// Static const amount of trx
	static int const kK=12; // kk = 4 
};
#endif//EVENT_H
