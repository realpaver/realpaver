///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/ExclusionRegionContractor.hpp"

#ifdef LOCAL_OPTIMIZER_IPOPT
#include "realpaver/LocalOptimizerIpopt.hpp"  
#endif
#ifdef LOCAL_OPTIMIZER_NLOPT
#include "realpaver/LocalOptimizerNlopt.hpp"  
#endif


namespace realpaver {


Constraint2BarrierTerm::Constraint2BarrierTerm(Term* barrier)
    : barrier_(barrier)
{

}

Constraint2BarrierTerm::~Constraint2BarrierTerm()
{
    if (barrier_!=nullptr)
    {
        delete barrier_;
    }
}

void Constraint2BarrierTerm::apply(const ArithCtrEq* c)
{
    if (barrier_==nullptr)
    {
        barrier_ = new Term(0.0);
    }
    barrier_ = new Term((*barrier_) + pow(c->left() - c->right(), 2));
}

void Constraint2BarrierTerm::apply(const ArithCtrLe* c)
{
    if (barrier_==nullptr)
    {
        barrier_ = new Term(0.0);
    }
    barrier_ = new Term((*barrier_) - log(c->right() - c->left()));
}

void Constraint2BarrierTerm::apply(const ArithCtrLt* c)
{
    if (barrier_==nullptr)
    {
        barrier_ = new Term(0.0);
    }
    barrier_ = new Term((*barrier_) - log(c->right() - c->left()));
}

void Constraint2BarrierTerm::apply(const ArithCtrGe* c)
{
    if (barrier_==nullptr)
    {
        barrier_ = new Term(0.0);
    }
    barrier_ = new Term((*barrier_) + log(c->right() - c->left()));
}

void Constraint2BarrierTerm::apply(const ArithCtrGt* c)
{
    if (barrier_==nullptr)
    {
        barrier_ = new Term(0.0);
    }
    barrier_ = new Term((*barrier_) + log(c->right() - c->left()));
}

Term Constraint2BarrierTerm::getBarrier() const
{
    return *barrier_;
}



ExclusionRegionContractor::ExclusionRegionContractor(const Problem& p)
{
    assert(p.isCSP() && p.isContinuous());
    csp_ = std::make_shared<Problem>(p);

    Constraint2BarrierTerm c2b;

    for(size_t i = 0; i<p.nbCtrs(); i++)
    {
        Constraint c = p.ctrAt(i);
        c.acceptVisitor(c2b);
    }
    bop_ = std::make_shared<Problem>();
    for(size_t i = 0; i<p.nbVars(); i++)
    {
        Variable v=p.varAt(i);
        std::cout<<v<<std::endl;
        bop_->addRealVar(p.getDomain(v),v.getName());
    }
    bop_->addObjective(MIN(c2b.getBarrier()));
    localsearch_ = std::make_shared<DefaultLocalOptimizer>(*bop_);
    prover_ = std::make_shared<Prover>(*csp_);
    assert(localsearch_->nbVars()==p.nbVars());
}

ExclusionRegionContractor::~ExclusionRegionContractor()
{

}

///@{
Scope ExclusionRegionContractor::scope() const
{
    return csp_->scope();
}

Proof ExclusionRegionContractor::contract(IntervalRegion& reg)
{
    Proof proof=Proof::Maybe;

    OptimizationStatus status = localsearch_->minimize(reg,reg.midpoint());

    if(status == OptimizationStatus::Optimal)
    {
        /// TODO
        bool already_found = false;
        for(auto re : exclusion_)
        {
            if (re.contains(*localsearch_->bestPoint()))
            {
                already_found = true;
                break;
            }
        }
        if (!already_found)
        {
            // std::cerr<<"Search for a new exclusion region..."<<std::endl;
            compute_new_exclusion_region(*localsearch_->bestPoint());
        }
    }

    // Naive loop to remove intersected parts
    for(auto re : exclusion_)
    {
        /// TODO
    }
    

    return proof;
}

void ExclusionRegionContractor::compute_new_exclusion_region(const RealPoint& rp)
{
    /// TODO
    IntervalRegion r(rp);
    IntervalRegion re(rp);
    double eps=initial_inflate_coefficient_;
    r.inflate(1.0+eps,eps);
    Proof p = prover_->certify(*r.clone());
    // std::cerr<<"Current proof: "<<p<<" for "<<r<<std::endl;
    while(p==Proof::Feasible)
    {
        re.setOnScope(r,r.scope());
        eps *= inflate_coefficient_;

        r.inflate(1.0+eps,eps);
        
        p = prover_->certify(*r.clone());
        // std::cerr<<"Current proof: "<<p<<" for "<<r<<std::endl;
    }
    if(re.strictlyContains(rp))
    {
        exclusion_.push_back(re);
    }
}

void ExclusionRegionContractor::print(std::ostream& os) const
{
    os<<"TODO ExclusionRegionContractor print"<<std::endl;
    os<<*bop_<<std::endl;
    os<<"Computed exclusion regions:";
    for(auto re : exclusion_)
    {
        os<<re<<std::endl;
    }
}


} // namespace
