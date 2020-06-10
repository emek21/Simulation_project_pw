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
	// Collecting data form class
	void CollectData();
	// Printing data
	void ShowData() const;
	// Collecting data form other statistic object
	void AddStat(Statistics* stat);
	// Showing average data form all simulation
	void EndStat(int n);
	double GetMaxBer() const { return max_packet_error_rate_; };
	double GetBitRate() const { return bit_rate_; };
	double GetRetr() const {return avg_retr_;};
	
private:
	// Packet error rate
	double avg_packet_error_rate_;
	// Maximum packet error rate for trx
	double max_packet_error_rate_;
	// Average retransmission for simulation
	double avg_retr_;
	// System bit rate per second
	double bit_rate_;
	// Average delay time
	double avg_delay_;
	// Average wait time
	double avg_wait_time_;
	// Number of successfully sent message
	int no_of_succes_msg_;
	// Number of lost message
	int no_of_lost_msg_;
	
	// Pointer to simulation monitor
	SimulationMonitor *monitor_;
	//logger
	static std::shared_ptr<spdlog::logger> logger_;
};

#endif // Statistics_H
