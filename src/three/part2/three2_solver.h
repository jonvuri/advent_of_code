#include <vector>

#include "../../solver.h"

class Three2Solver : public Solver
{
public:
  unsigned long findByBitCriteria(std::vector<unsigned long> list, unsigned long length, bool keepHigh);
  unsigned long oxygenRating(std::vector<unsigned long> list, unsigned long length);
  unsigned long CO2scrubberRating(std::vector<unsigned long> const &list, unsigned long length);
  unsigned long solve(std::istream &is);
};
