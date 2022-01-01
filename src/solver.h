#pragma once

#include <iostream>
#include <fstream>

class Solver
{
public:
  virtual ~Solver() = default;
  virtual unsigned long solve(std::istream &is) = 0;
};
