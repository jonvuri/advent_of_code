#include <vector>

#include "../../solver.h"

class Solver_03_part2 : public Solver
{
public:
  static unsigned long findByBitCriteria(const std::vector<unsigned long> &list, unsigned long length, bool keepHigh);
  static unsigned long oxygenRating(const std::vector<unsigned long> &list, unsigned long length);
  static unsigned long CO2scrubberRating(const std::vector<unsigned long> &list, unsigned long length);
  std::string solve(std::istream &is) override;
};
