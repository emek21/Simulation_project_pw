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
	spdlog::set_pattern("\t*** [%n]->[%^%l%$] ***%v");
	// Tmp config values 
	int type=0,simulation_time=0,start_stat_time=0,logger_type=0,add_condition=0;
	double lambda=0;
	
	// User interface. Getting config values
	printf("\t--Digital simulation--\n\n"
		"\tAuthor: Przemyslaw Wasilewicz \t"
		"Method: M4\n"
		"\tAlgorihm: A6b\t"
		"Group: SK\n"
	);
	printf("\n\nSelect mode: continuous[0] | step[1]\n"
	">>");
	std::cin>>type;
	printf("\nEnter the simulation time: \n"
	">>");
	std::cin>>simulation_time;
	printf("\nEnter the quantity of send message for the initial phase: \n"
	">>");
	std::cin>>start_stat_time;
	printf("\nEnter lambda value: [0 = default] \n"
	">>");
	std::cin>>lambda;
	printf("\nSelect mode: debug[0] | info[1] | warn[2]: \n"
	">>");
	std::cin>>logger_type;
	printf("\nProvide an additional end condition. The number of packets: [0 = off] \n"
	">>");
	std::cin>>add_condition;

	
	
	if(lambda == 0)
	{
		lambda = 0.000423;//0.000423
	}

	//// --------Report values -------
	/*type = 0;
	simulation_time = 100000000;
	start_stat_time= 15000 ;
	logger_type = 1;
	add_condition = 50000;*/

	// Set simulation values
	// Set lambda
	Generator::SetLambda(lambda);
	// Set log level
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
	
	// --Generate histograms--
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
	/*double pp = 0.00;*/
	
	// Main simulation object
	auto monitor = new SimulationMonitor(simulation_time,start_stat_time,type,add_condition);
	
	// Run 10 simulations
	for(int i=0 ; i < 10 ; i++)
	{
		// Report 
		//lambda = lambda+0.00005;//0.000001
		//Generator::SetLambda(lambda);
		/*pp = pp +0.05;
		Transceiver::SetP(pp);*/
		printf("The beginning of the simulation #%d...\n",i+1);
		monitor->Start(i);
	}
	
	// Showing average result of all simulations 
	monitor->ShowResult();
	delete monitor;
	
    return 0;
}

