/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   IntervalFunction.cpp
 * @brief  Interval functions
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalFunction.hpp"

namespace realpaver {

IntervalFunctionRep::~IntervalFunctionRep()
{
}

void IntervalFunctionRep::setImage(const Interval &img)
{
   img_ = img;
}

Interval IntervalFunctionRep::getImage() const
{
   return img_;
}

/*----------------------------------------------------------------------------*/

IntervalFunction::IntervalFunction(SharedDagFun f)
    : rep_(std::make_shared<IntervalFunctionDag>(f))
{
}

IntervalFunction::IntervalFunction(Term t, const Interval &img)
    : rep_(std::make_shared<IntervalFunctionDag>(t, img))
{
}

IntervalFunction::IntervalFunction(SharedRep rep)
    : rep_(rep)
{
   REALPAVER_ASSERT(rep != nullptr,
                    "Creation of an interval function from a null pointer");
}

IntervalFunction::SharedRep IntervalFunction::rep() const
{
   return rep_;
}

void IntervalFunction::setImage(const Interval &img)
{
   rep_->setImage(img);
}

Interval IntervalFunction::getImage() const
{
   return rep_->getImage();
}

Scope IntervalFunction::scope() const
{
   return rep_->scope();
}

size_t IntervalFunction::nbVars() const
{
   return rep_->nbVars();
}

Interval IntervalFunction::eval(const IntervalBox &B)
{
   return rep_->eval(B);
}

void IntervalFunction::diff(const IntervalBox &B, IntervalVector &G)
{
   return rep_->diff(B, G);
}

void IntervalFunction::diffHansen(const IntervalBox &B, const RealPoint &c,
                                  IntervalVector &G)
{
   rep_->diffHansen(B, c, G);
}

/*----------------------------------------------------------------------------*/

IntervalFunctionDag::IntervalFunctionDag(SharedDagFun f)
    : IntervalFunctionRep()
    , f_(f)
{
}

IntervalFunctionDag::IntervalFunctionDag(Term t, const Interval &img)
    : f_(nullptr)
{
   SharedDag dag = std::make_shared<Dag>();
   size_t i = dag->insert(t, img);
   f_ = dag->fun(i);
}

Scope IntervalFunctionDag::scope() const
{
   return f_->scope();
}

size_t IntervalFunctionDag::nbVars() const
{
   return f_->nbVars();
}

Interval IntervalFunctionDag::eval(const IntervalBox &B)
{
   return f_->iEval(B);
}

void IntervalFunctionDag::diff(const IntervalBox &B, IntervalVector &G)
{
   return f_->iDiff(B, G);
}

void IntervalFunctionDag::diffHansen(const IntervalBox &B, const RealPoint &c,
                                     IntervalVector &G)
{
   return f_->iDiffHansen(B, c, G);
}

} // namespace realpaver
