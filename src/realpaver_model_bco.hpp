// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_MODEL_BCO_HPP
#define REALPAVER_MODEL_BCO_HPP

#include "realpaver_dag.hpp"
#include "realpaver_problem.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of bound constrained optimization models.
 *****************************************************************************/
class BcoModel {
public:
   // constructor
   BcoModel(Problem& P);

   // destructor
   ~BcoModel();

   // copy protection
   BcoModel(const BcoModel&) = delete;
   BcoModel& operator=(const BcoModel&) = delete;

private:
   Variable z_;   // new variable representing the objective function
   Dag* dag_;     // expression graph
   Box* init_;    // initial box
};

} // namespace

#endif
