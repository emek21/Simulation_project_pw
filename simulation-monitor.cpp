#include "simulation-monitor.h"
#include <iostream>
#include <algorithm>

SimulationMonitor::~SimulationMonitor()
{
	delete wireless_network_;
}

SimulationMonitor::SimulationMonitor(int sim_time,int fist_state_time,bool type):
total_num_(0),num_of_fail_(0),full_time_(sim_time),stat_time_(fist_state_time),
stepping_(type),clock_(0),current_(nullptr),statistics_(this)
{
	GenerateSeeds();
	// Initialize simulation
	wireless_network_ = new WirelessNetwork(this,seeds_);
	
}

uint64_t SimulationMonitor::GetTime() const
{
	return clock_;
}

void SimulationMonitor::Start()
{
	bool stat_flag = false;
	// Main loop

	while(clock_ < full_time_)
	{
		if(clock_>= stat_time_ && !stat_flag) 
		{
			Message::ResetStat();
			stat_flag = true;
		}
		// Find next planed action and activate process
		// Check for process in queue
		if(process_queue_.empty() != true)
		{
			// fetch first element
			current_ = process_queue_.front();
			// check it is right time for execute 
			if(clock_ == current_->GetTime())
			{
				// Pop current from queue
				DeleteProcess();
				// execute current
				current_->Execute();
				if(current_->IsTerminated())
				{
					delete current_;
				}
			}

		}
		// change clock time to next 
		change_time();
		// Stepping mode
		if(stepping_)
		{
			system("pause");
			
		}
	}
	Reset();
	statistics_.ShowData();
}

void SimulationMonitor::Reset()
{
	// TODO Divide this function
	statistics_.CollectData();
	clock_ = 0;
	current_ = nullptr;
	process_queue_.clear();
	delete wireless_network_;
	wireless_network_ = new WirelessNetwork(this,seeds_);
}

bool SimulationMonitor::TrxCheckChannel() const
{
	return wireless_network_->IsChannelFree();
}

void SimulationMonitor::TrxBlockChannel(Message* msg) const
{
	wireless_network_->BlockChannel(msg);
}

void SimulationMonitor::TrxDetectCollsion(Message* msg) const
{
	wireless_network_->DetectCollsion(msg);
}

void SimulationMonitor::TrxUnblockChannel(Message* msg) const
{
	wireless_network_->UnblockChannel(msg);
}

void SimulationMonitor::change_time()
{
	uint64_t next_time = -1;
	//Fetch time form first element in list
	Message* tmp_msg_event = process_queue_.empty() ? nullptr:process_queue_.front();
	if(tmp_msg_event != nullptr)
	{
		next_time = tmp_msg_event->GetTime();
	}
	// Change time
	clock_ = next_time;

	
}


bool SimulationMonitor::Comp(Message* lhs, Message* rhs)
{
	// Function for sorting action
	return lhs->GetTime() < rhs->GetTime();
}

void SimulationMonitor::AddNewProcessAction(uint64_t time, Message* process)
{
	// Add new event to list and sort by schedule time
	process->SetScheduleTime(clock_+time);
	process_queue_.push_back(process);
	std::sort(process_queue_.begin(),process_queue_.end(),Comp);
}

void SimulationMonitor::DeleteProcess()
{
	process_queue_.erase(process_queue_.begin());
}

void SimulationMonitor::GenerateSeeds()
{
	// Generate seeds
	int number_of_seeds = 100000*200;
	int seed_for_seeds = 123;
	auto seed_generator = Generator(seed_for_seeds);

	printf("Seed drawing...\n");
	
	for (int i = 0; i < number_of_seeds; ++i)
	{
		seed_generator.Rand();
		if(i % 100000 == 0) seeds_.push_back(seed_generator.GetSeed());
	}
}

void SimulationMonitor::ShowResult()
{
	printf("End Result:");
}
