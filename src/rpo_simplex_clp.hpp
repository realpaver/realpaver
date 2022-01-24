#ifndef RPO_SIMPLEX_CLP_HPP
#define RPO_SIMPLEX_CLP_HPP

#include "ClpSimplex.hpp"
#include "rpo_lin_model.hpp"

class RpoSimplex : public RpoLinModel {
public:
  RpoSimplex();
  ~RpoSimplex();

  bool optimize();

private:
  ClpSimplex* simplex_;
  
  void makeVars();
  void makeCtrs();
  void makeObj();
  void makeClpSimplex();
};

#endif
