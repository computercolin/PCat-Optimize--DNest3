#ifndef _RandomNumberGenerator_
#define _RandomNumberGenerator_

#include <gsl/gsl_rng.h>
#include <vector>

namespace NewSampler
{

/*
* RandomNumberGenerator is a wrapper around GSL's random number generator.
*/

class RandomNumberGenerator
{
	private:
		gsl_rng* rng;

	public:
		RandomNumberGenerator(int seed);
		~RandomNumberGenerator();

		double randomU() const;
		double randn() const;
		int randInt(int numPossibilities) const;

		// Getter - allows access to gsl object
		// for calls to other distributions
		gsl_rng* get_rng() { return rng; }

};

} // namespace NewSampler

#endif
