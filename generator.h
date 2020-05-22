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
	static double lambda_;
	
	
public:
	Generator(int seed);
	~Generator() = default;

	// Draws between <0,1>
	double Rand();
	// Draws between <min,max>
	double Rand(int min, int max);
	// Draws exponential values
	double RandExp();
	// Draws 0 or 1
	bool RandZeroOne(double const p);
	// Return seedsd
	int GetSeed() const { return static_cast<int>(seed_);};
	// Setting lambda_ value
	static void SetLambda(double lmb) { lambda_ = lmb; };
};

#endif /* GENERATOR_H_ */