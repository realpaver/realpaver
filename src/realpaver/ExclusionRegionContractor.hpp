///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_EXCLUSION_REGION_CONTRACTOR_HPP
#define REALPAVER_EXCLUSION_REGION_CONTRACTOR_HPP

#include <vector>
#include "realpaver/Contractor.hpp"
#include "realpaver/Prover.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/LocalOptimizer.hpp"


namespace realpaver {

class Constraint2BarrierTerm : public ConstraintVisitor
{
public:
   /// Creates a visitor
   /// @param barrier term modified by a visit
   Constraint2BarrierTerm(Term* barrier=nullptr);

   ~Constraint2BarrierTerm();

   ///@{
   void apply(const ArithCtrEq* c) override;
   void apply(const ArithCtrLe* c) override;
   void apply(const ArithCtrLt* c) override;
   void apply(const ArithCtrGe* c) override;
   void apply(const ArithCtrGt* c) override;
   // void apply(const ArithCtrIn* c) override;
   ///@}

   Term getBarrier() const;

private:
   Term* barrier_;
};


///////////////////////////////////////////////////////////////////////////////
/// This is the Exclusion region contractor.
///
/// Given a system of equations f(x1, ..., xn) = 0, it applies ...
///////////////////////////////////////////////////////////////////////////////
class ExclusionRegionContractor : public Contractor {
public:
   /// Creates a contractor
   /// @param p a Problem representing a system of equations
   ExclusionRegionContractor(const Problem& p);

   /// Destructor
   ~ExclusionRegionContractor();

   /// No copy
   ExclusionRegionContractor(const ExclusionRegionContractor&) = delete;

   /// No assignment
   ExclusionRegionContractor& operator=(const ExclusionRegionContractor&) = delete;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalRegion& reg) override;
   void print(std::ostream& os) const override;
   ///@}

protected:
   void compute_new_exclusion_region(const RealPoint& rp);

private:
   std::shared_ptr<Problem> csp_;                     // a csp problem
   std::shared_ptr<Problem> bop_;                     // a csp problem

   std::shared_ptr<LocalOptimizer> localsearch_;
   std::shared_ptr<Prover> prover_;
   std::vector<IntervalRegion> exclusion_;

   double initial_inflate_coefficient_=1e-8;
   double inflate_coefficient_=10;
};

} // namespace

#endif
