#include <vector>

#include "../../solver.h"

class Three1Solver : public Solver
{
public:
  unsigned long gammaRate(std::vector<unsigned long> const &list, unsigned long length);
  unsigned long epsilonRate(std::vector<unsigned long> const &list, unsigned long length);
  unsigned long solve(std::istream &is);
};
