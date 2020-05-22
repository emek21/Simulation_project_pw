 #include "wireless-network.h"
#include "simulation-monitor.h"

WirelessNetwork::WirelessNetwork(SimulationMonitor* monitor,vector<int> &seed)
{
	for(int i=0; i<kK; i++)
	{
		// Giving out seeds
		int seed_cgp = seed.back();
		seed.pop_back();
		int seed_ack = seed.back();
		seed.pop_back();
		int seed_ctp = seed.back();
		seed.pop_back();
		int seed_crp = seed.back();
		seed.pop_back();
		list_of_transceivers_.push_back(new Transceiver(i,monitor,seed_cgp,seed_ack,seed_ctp,seed_crp));
		list_of_transceivers_[i]->GenMsg(); // Start process
	}
}

bool WirelessNetwork::IsChannelFree() const
{
	return ch_.IsEmpty();
}

void WirelessNetwork::BlockChannel(Message* msg)
{
	ch_.BlockChannel(msg->GetTrxId());
}

void WirelessNetwork::DetectCollsion(Message* msg)
{
	vector<int> colission_trx_id;
	bool collision = ch_.DetectCollision();
	if(collision)
	{
		colission_trx_id = ch_.CollsionTrx();
		for(auto x: colission_trx_id)
		{
			list_of_transceivers_[x]->SetCollision();
		}
	}
}

void WirelessNetwork::UnblockChannel(Message* msg)
{
	ch_.UnblockChannel(msg->GetTrxId());
}

 void WirelessNetwork::CalcMaxErrorRate()
 {
	// Finding max error rate
	double max = -1;
	for(auto x: list_of_transceivers_)
	{
		if(x->GetAvgErrorRate() > max) max=x->GetAvgErrorRate();
	}
	// Set max error rate
	Transceiver::SetMaxErrorRate(max);
 }
