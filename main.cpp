/*  Author: Przemysław Wasilewicz 
 *	Method: M4
 *	Algorihm: A6b
 *	Group: SK
 */

#include <cstdint>
#include <memory>
#include "simulation-monitor.h"
#include "generator.h"
#include <fstream>
#include <iostream>


using std::uint64_t;

int main()
{
	spdlog::set_pattern("*** [%n]->[%^%l%$] ***%v");
	int type=0,simulation_time=0,start_stat_time=0,logger_type;
	printf("\t--Digital simulation--\n\n"
		"\tAuthor: Przemyslaw Wasilewicz \t"
		"Method: M4\n"
		"\tAlgorihm: A6b\t"
		"Group: SK\n"
	);
	printf("\n\nSelect mode: continuous[0] | step[1]\n"
	">>");
	std::cin>>type;
	printf("Enter the simulation time: \n"
	">>");
	std::cin>>simulation_time;
	printf("Enter the length of the initial phase: \n"
	">>");
	std::cin>>start_stat_time;
	printf("Select mode: debug[0] | info[1] | warn[2]: \n"
	">>");
	std::cin>>logger_type;
	switch (logger_type)
	{
	case 0:
		spdlog::set_level(spdlog::level::debug);
		break;
	case 1:
		spdlog::set_level(spdlog::level::info);
		break;
	case 2:
		spdlog::set_level(spdlog::level::warn);
		break;
	default:
		spdlog::set_level(spdlog::level::info);
		break;
	}
	
	// --Generate for histograms--
	//std::vector<double> hist_rand;
	//std::vector<double> hist_rand_exp;
	//std::vector<double> hist_rand_minmax;
	//std::vector<bool> hist_rand_zeo_one;
	//auto numbers_for_hist = 1000000;
	//
	//
	//for (int i = 0; i < numbers_for_hist; ++i)
	//{
	//	hist_rand.push_back(seed_generator.Rand());
	//	hist_rand_exp.push_back(seed_generator.RandExp());
	//	hist_rand_minmax.push_back(seed_generator.Rand(0,5));
	//	hist_rand_zeo_one.push_back(seed_generator.RandZeroOne(0.80));
	//	
	//}
	//// Save data to file
	//std::ofstream output_file1("./rand.txt");
	//std::ostream_iterator<double> output_iterator1(output_file1, "\n");
	//std::copy(hist_rand.begin(), hist_rand.end(), output_iterator1);
	//// Save data to file
	//std::ofstream output_file2("./RandExp.txt");
	//std::ostream_iterator<double> output_iterator2(output_file2, "\n");
	//std::copy(hist_rand_exp.begin(), hist_rand_exp.end(), output_iterator2);
	//// Save data to file
	//std::ofstream output_file3("./Randminmax.txt");
	//std::ostream_iterator<double> output_iterator3(output_file3, "\n");
	//std::copy(hist_rand_minmax.begin(), hist_rand_minmax.end(), output_iterator3);
	//// Save data to file
	//std::ofstream output_file4("./RandZeroOne.txt");
	//std::ostream_iterator<double> output_iterator4(output_file4, "\n");
	//std::copy(hist_rand_zeo_one.begin(), hist_rand_zeo_one.end(), output_iterator4);
	
	auto monitor = new SimulationMonitor(simulation_time,start_stat_time,type);
	
	for(int i=0 ; i < 1 ; i++)
	{
		printf("The beginning of the simulation #%d...\n",i+1);
		monitor->Start();
	}
	
	monitor->ShowResult();
	delete monitor;
	
    return 0;
}

