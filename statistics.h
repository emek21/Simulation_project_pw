#ifndef Statistics_H
#define Statistics_H

#include "spdlog/spdlog.h"
using std::shared_ptr;
using spdlog::stdout_color_mt;

class SimulationMonitor;
class Message;
class Statistics
{
public:
	Statistics(SimulationMonitor* m);
	~Statistics() = default;
	void CollectData();
	void ShowData();
	void EndStat(Statistics* stat);
	
private:
	double avg_packet_error_rate_;
	double max_packet_error_rate_;
	double avg_retr_;
	double bit_rate_;
	double avg_delay_;
	double avg_wait_time_;
	// Pointer to simulation monitor
	SimulationMonitor *monitor_;
	//logger
	static std::shared_ptr<spdlog::logger> logger_;
	// Static const amount of trx
	static int const kK=4;
};

#endif // Statistics_H
