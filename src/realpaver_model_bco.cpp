// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_model_bco.hpp"
#include "realpaver_term_deriver.hpp"

namespace realpaver {

BcoModel::BcoModel(Problem& P) :
   z_(P.addRealVar(Interval::universe(), "_z")),
   dag_(nullptr),
   init_(nullptr)
{
   ASSERT(!P.isConstrained(),
          "BCO model created from a constrained problem.");

   ASSERT(P.hasObjective(),
          "BCO model created from a problem with no objective.");

   // initial box
   init_ = new Box(P.getBox());   
   DEBUG("init : " << (*init_));

   // DAG
   dag_ = new Dag();
   for (size_t i=0; i<P.nbVars()-1; ++i)
   {
      TermDeriver deriver(P.varAt(i));
      P.obj().getTerm().acceptVisitor(deriver);
      
      DEBUG("deriv / " << P.varAt(i).name() << " : " << deriver.get());
      
      dag_->insert(deriver.get() == 0);
   }
   dag_->insert( z_ == P.obj().getTerm() );

   DEBUG( (*dag_) );


   // A FAIRE : tester les inconsistances (domaines vides, contraintes unsat)
   // A FAIRE : créer les contracteurs
   // A FAIRE : preprocessing
}

BcoModel::~BcoModel()
{
   delete init_;
   delete dag_;
}

} // namespace
