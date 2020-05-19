#ifndef GENERATOR_H_
#define GENERATOR_H_

class Generator
{
private:
	int seed_;
    const double kM = 2147483647.0;
    static const int kA = 16807;
    static const int kQ = 127773;
    static const int kR = 2836;
	static const double lambda;
	
	
public:
	Generator(int seed);
	~Generator() = default;

	// Draws between <0,1>
	double Rand();
	// Draws between <min,max>
	double Rand(int min, int max);
	double RandExp();
	bool RandZeroOne(double const p);
	int GetSeed() const { return static_cast<int>(seed_);};
};

#endif /* GENERATOR_H_ */