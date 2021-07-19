#ifndef CUM_DISTRIB_FUNCTION_HH_
#define CUM_DISTRIB_FUNCTION_HH_

# include <random>
# include <iostream>
# include <vector>

class CumDistribFunction
{
public:
  CumDistribFunction(std::mt19937_64& ng, const std::string& fname);

  double draw(double p);
protected:
  std::mt19937_64& ng_;
  std::vector<double> x_;
  std::vector<double> y_;
  double bin_w_;
  std::uniform_real_distribution<double> randu_;
};

#endif /// !CUM_DISTRIB_FUNCTION_HH
