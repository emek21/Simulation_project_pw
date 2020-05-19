#include "statistics.h"
#include "spdlog/spdlog.h"
#include "simulation-monitor.h"
#include "message.h"
shared_ptr<spdlog::logger> Statistics::logger_ = stdout_color_mt("Statistics");

Statistics::Statistics(SimulationMonitor* m):avg_packet_error_rate_(0),max_packet_error_rate_(0),
avg_retr_(0),bit_rate_(0),avg_delay_(0),avg_wait_time_(0),
monitor_(m)
{
}

void Statistics::CollectData()
{
	int all_msg = Message::GetAmountOfMsg();
	int all_retr = Message::GetAmountOfRetr();
	int no_err = Message::GetErrCounter();
	int no_succes = Message::GetAmountOfSucces();
	avg_delay_ = Message::GetAvgDelay();
	avg_wait_time_ = Message::GetAvgWait();
	Message::ResetStat();
	
	avg_packet_error_rate_ = static_cast<double>(no_err)/no_succes;
	avg_retr_ = static_cast<double>(all_retr)/all_msg;
	bit_rate_ = static_cast<double>(no_succes)/(monitor_->GetStatTime()/10000);
	avg_delay_/=10;
	avg_wait_time_ /=10;
}

void Statistics::ShowData()
{
	logger_->info("\n\n\tSimulation results:\n"
		"\t Average error rate per trx: {:.6f} \n"
		"\t Max error rate : !!!!!!!!!!!!!!\n"
		"\t Average number of retransmission: {:.5f}\n"
		"\t System bit rate: {:.2f} [msg/s]\n"
		"\t Average message delay time: {:.2f} [ms]\n"
		"\t Average message wait time: {:.2f} [ms]\n"
		,avg_packet_error_rate_,avg_retr_,bit_rate_,avg_delay_,avg_wait_time_
	);
}
