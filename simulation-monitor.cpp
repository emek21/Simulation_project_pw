#include "simulation-monitor.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iostream>

SimulationMonitor::~SimulationMonitor()
{
	delete wireless_network_;
}

SimulationMonitor::SimulationMonitor(int sim_time,int fist_state_time,bool type,int add_con):
full_time_(sim_time),stat_condition_(fist_state_time),stat_time_(0),end_time(0),
add_condition_(add_con),stepping_(type),clock_(0),current_(nullptr)
{
	// Generate seeds
	GenerateSeeds();
	// Initialize simulation
	wireless_network_ = new WirelessNetwork(this,seeds_);
	
}

uint64_t SimulationMonitor::GetTime() const
{
	return clock_;
}

void SimulationMonitor::Start(const int sim_nr)
{
	statistics_.push_back(new Statistics(this));
	bool stat_flag = false;
	// Report - initial phase data
	/*bool report_mode = true;
	std::vector<std::pair<double,double>> init_phase_vector;
	int messages_send = 0 ;*/
	// Main loop

	while(clock_ < full_time_)
	{
		// Handle additional end condition only when is stat time 
		if(stat_flag) 
		{
			if(add_condition_ > 0 && add_condition_ <= Message::GetAmountOfSucces()) 
			{
				// End
				end_time = clock_;
				break;
			}
		}
		// Handle stat time
		if(Message::GetAmountOfSucces() >= stat_condition_ && !stat_flag) 
		{
			// Reset statistic
			Message::ResetStat();
			stat_time_ = clock_;
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
					// Delete if process end life 
					delete current_;
				}
			}

		}
		// Report - initial phase data
		//if(messages_send < Message::GetAmountOfSucces())
		//{
		//	init_phase_vector.push_back(std::make_pair(Message::GetAmountOfSucces(),
		//		static_cast<double>(Message::GetAmountOfRetr())/Message::GetAmountOfSucces()));
		//	messages_send = Message::GetAmountOfSucces();
		//}
		// change clock time to next 
		change_time();
		// Stepping mode
		if(stepping_)
		{
			system("pause");
			
		}
	}
	// Report - initial phase data
	/*std::ofstream output_file1;
	output_file1.open("init_phase.txt",std::ofstream::out | std::ofstream::app);
	output_file1 << "Lambda=" << Generator::GetLambda()<<'\n';
	for (auto const& x : init_phase_vector) {
		output_file1 << x.first << ":"<<x.second;
		output_file1 << '\n';
	}
	output_file1.close();*/
	// Statistic and reset function
	if(end_time == 0) end_time = full_time_;
	Reset(sim_nr);
	// Showing statistic of this simulation
	statistics_[sim_nr]->ShowData();
	/*	int stat_t = clock_ - stat_time_;
	int bit_rate_ = static_cast<double>(Message::GetAmountOfSucces())/stat_t;
	bit_rate_*=10000;*/
}

void SimulationMonitor::Reset(int sim_nr)
{
	// Calculate statistic and reset simulation
	// Collect data 
	wireless_network_->CalcMaxErrorRate();
	statistics_[sim_nr]->CollectData();
	// clear clock and current pointer
	clock_ = 0;
	current_ = nullptr;
	// Clear process queue
	process_queue_.clear();
	delete wireless_network_;
	// Create new network
	wireless_network_ = new WirelessNetwork(this,seeds_);
	end_time = 0;
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
	int number_of_seeds = 100000*5000;
	int seed_for_seeds = 984653;//Reprot --> 984653
	auto seed_generator = Generator(seed_for_seeds);

	printf("Seed drawing...\n");
	
	for (int i = 1; i < number_of_seeds; ++i)
	{
		seed_generator.Rand();
		if(i % 100000 == 0) seeds_.push_back(seed_generator.GetSeed());
	}
}

void SimulationMonitor::ShowResult()
{
	auto end_stat = new Statistics(this);
	int n=0;
	for(auto stat: statistics_)
	{
		end_stat->AddStat(stat);
		delete stat;
		n++;
	}
	end_stat->EndStat(n);
	printf("All simulation average result:\n");
	end_stat->ShowData();
}
