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
	//int all_msg = Message::GetAmountOfMsg();
	const int all_retr = Message::GetAmountOfRetr();
	const int no_err = Message::GetErrCounter();
	const int no_succes = Message::GetAmountOfSucces();
	avg_delay_ = Message::GetAvgDelay();
	avg_wait_time_ = Message::GetAvgWait();
	max_packet_error_rate_ = Transceiver::GetMaxErrorRate();
	
	avg_packet_error_rate_ = static_cast<double>(no_err)/no_succes;
	avg_retr_ = static_cast<double>(all_retr)/no_succes;
	bit_rate_ = static_cast<double>(no_succes)/monitor_->GetStatTime();
	bit_rate_*=10000;
	avg_delay_/=10;
	avg_wait_time_ /=10;
	Message::ResetStat();
	Transceiver::ResetStat();
}

void Statistics::ShowData()
{
	logger_->info("\n\n\tSimulation results:\n"
		"\t Average error rate per trx: {:.6f} \n"
		"\t Max error rate : {:.6f}\n"
		"\t Average number of retransmission: {:.5f}\n"
		"\t System bit rate: {:.2f} [msg/s]\n"
		"\t Average message delay time: {:.2f} [ms]\n"
		"\t Average message wait time: {:.2f} [ms]\n"
		,avg_packet_error_rate_,max_packet_error_rate_
		,avg_retr_,bit_rate_,avg_delay_,avg_wait_time_
	);
}

void Statistics::EndStat(Statistics* stat)
{
	avg_packet_error_rate_ = (avg_packet_error_rate_+stat->avg_packet_error_rate_)/2;
	max_packet_error_rate_ = (max_packet_error_rate_+stat->max_packet_error_rate_)/2;
	avg_retr_ = (avg_retr_+stat->avg_retr_)/2;
	bit_rate_ = (bit_rate_+stat->bit_rate_)/2;
	avg_delay_ = (avg_delay_+stat->avg_delay_)/2;
	avg_wait_time_ = (avg_wait_time_+stat->avg_wait_time_)/2;
}
