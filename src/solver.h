#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>

class Solver
{
public:
  virtual ~Solver() = default;
  virtual unsigned long solve(std::istream &is) = 0;
};

class solver_runtime_error : public std::runtime_error
{
public:
  solver_runtime_error(const std::string &what = "") : std::runtime_error(what) {}
};
