#include <vector>

#include "../../solver.h"

class Solver_03_part1 : public Solver
{
public:
  unsigned long gammaRate(std::vector<unsigned long> const &list, unsigned long length);
  unsigned long epsilonRate(std::vector<unsigned long> const &list, unsigned long length);
  unsigned long solve(std::istream &is);
};
