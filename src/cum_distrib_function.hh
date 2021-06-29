#ifndef CUM_DISTRIB_FUNCTION_HH_
#define CUM_DISTRIB_FUNCTION_HH_

# include <iostream>
# include <vector>

class CumDistribFunction
{
public:
  CumDistribFunction(const std::string& fname);

  double draw(double p) const;
protected:
  std::vector<double> x_;
  std::vector<double> y_;
};

#endif /// !CUM_DISTRIB_FUNCTION_HH
