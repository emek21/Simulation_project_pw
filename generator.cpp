#include "generator.h"
#include <cmath>

Generator::Generator(int const seed):seed_(seed)
{
	
}

const double Generator::lambda = 0.001;

double Generator::Rand()
{
	double h = seed_/kQ;
   seed_ = kA*(seed_-kQ*h)-kR*h;
   if (seed_ < 0)
   {
   	seed_ = seed_ + static_cast<int>(kM);
   }
   return seed_/kM;
}

double Generator::Rand(int const min, int const max)
{
	return Rand()*(max - min) + min;
}

double Generator::RandExp()
{
	auto k = Rand();
	return -1 * (1.0 / lambda) * log(k);
}

bool Generator::RandZeroOne(double const p)
{
	auto k = Rand();
	if(k < p)
	{
		return true;
	}
	else
	{
		return false;
	}
}
