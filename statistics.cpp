#include "statistics.h"
#include "spdlog/spdlog.h"
#include "simulation-monitor.h"
#include "message.h"
shared_ptr<spdlog::logger> Statistics::logger_ = stdout_color_mt("Statistics");

Statistics::Statistics(SimulationMonitor* m) : avg_packet_error_rate_(0),max_packet_error_rate_(0),
avg_retr_(0),bit_rate_(0),avg_delay_(0),avg_wait_time_(0),
no_of_succes_msg_(0),no_of_lost_msg_(0),monitor_(m)
{
}

void Statistics::CollectData()
{
	// collecting data
	const int all_retr = Message::GetAmountOfRetr();
	no_of_lost_msg_ = Message::GetErrCounter();
	no_of_succes_msg_ = Message::GetAmountOfSucces();
	avg_delay_ = Message::GetAvgDelay();
	avg_wait_time_ = Message::GetAvgWait();
	max_packet_error_rate_ = Transceiver::GetMaxErrorRate();
	// Data processing 
	avg_packet_error_rate_ = static_cast<double>(no_of_lost_msg_)/no_of_succes_msg_;
	avg_retr_ = static_cast<double>(all_retr)/no_of_succes_msg_;
	bit_rate_ = static_cast<double>(no_of_succes_msg_)/monitor_->GetStatTime();
	bit_rate_*=10000;
	avg_delay_/=10;
	avg_wait_time_ /=10;
	// Reset class statistic
	Message::ResetStat();
	Transceiver::ResetStat();
}

void Statistics::ShowData() const
{
	logger_->info("\n\n\tSimulation results:\n"
		"\t Successfully send messages: {}  \n"
		"\t Lost messages: {}  \n\n"

		"\t --Calculated statistics-- \n"
		"\t Average error rate per trx: {:.6f} \n"
		"\t Max error rate : {:.6f}\n"
		"\t Average number of retransmission: {:.5f}\n"
		"\t System bit rate: {:.2f} [msg/s]\n"
		"\t Average message delay time: {:.2f} [ms]\n"
		"\t Average message wait time: {:.2f} [ms]\n"
		,no_of_succes_msg_,no_of_lost_msg_
		,avg_packet_error_rate_,max_packet_error_rate_
		,avg_retr_,bit_rate_,avg_delay_,avg_wait_time_
	);
}

void Statistics::AddStat(Statistics* stat)
{
	no_of_lost_msg_ = no_of_lost_msg_+stat->no_of_lost_msg_;
	no_of_succes_msg_ = no_of_succes_msg_+stat->no_of_succes_msg_;
	avg_packet_error_rate_ = avg_packet_error_rate_+stat->avg_packet_error_rate_;
	max_packet_error_rate_ = max_packet_error_rate_+stat->max_packet_error_rate_;
	avg_retr_ = avg_retr_+stat->avg_retr_;
	bit_rate_ = bit_rate_+stat->bit_rate_;
	avg_delay_ = avg_delay_+stat->avg_delay_;
	avg_wait_time_ = avg_wait_time_+stat->avg_wait_time_;
}

void Statistics::EndStat(int n)
{
	no_of_lost_msg_ = no_of_lost_msg_/n;
	no_of_succes_msg_ = no_of_succes_msg_/n;
	avg_packet_error_rate_ = avg_packet_error_rate_/n;
	max_packet_error_rate_ = max_packet_error_rate_/n;
	avg_retr_ = avg_retr_/n;
	bit_rate_ = bit_rate_/n;
	avg_delay_ = avg_delay_/n;
	avg_wait_time_ = avg_wait_time_/n;
}
