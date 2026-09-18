/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2026 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   LinearizerInnerTaylor.hpp
 * @brief  Linearization of nonlinear problems based on Taylor forms
 * @author Raphaël Chenouard
 * @date   11 Feb 2026
 */

#ifndef REALPAVER_LINEARIZER_INNER_TAYLOR_HPP
#define REALPAVER_LINEARIZER_INNER_TAYLOR_HPP

#include "Bitset.hpp"
#include "IntervalMatrix.hpp"
#include "LPModel.hpp"
#include "realpaver/IntRandom.hpp"
#include "realpaver/LinearizerInner.hpp"
#include "realpaver/LinearizerTaylor.hpp"

namespace realpaver {

/**
 * @brief Linearizer that generates Taylor forms.
 *
 * It generates a relaxation using Taylor forms in two opposite corners.
 * The first corner can be fixed manually or selected randomly.
 */
class LinearizerInnerTaylor : public LinearizerInner {
public:
   /**
    * @brief Creates a linearizer of a DAG.
    *
    * The boolean hansen is true if Hansen's differentiation algorithm is
    * used, false otherwise. The style specifies how the first corner is
    * selected. A seed is used to initialize the generator of pseudo-random
    * numbers is necessary (0: system time, a positive value otherwise).
    */
   LinearizerInnerTaylor(SharedDagExtended dag, int cobj = -1, bool hansen = false,
                         CornerStyle style = CornerStyle::RandomOpposite,
                         unsigned seed = 0);

   /// No copy
   LinearizerInnerTaylor(const LinearizerInnerTaylor &) = delete;

   /// No assignment
   LinearizerInnerTaylor &operator=(const LinearizerInnerTaylor &) = delete;

   /// Default destructor
   ~LinearizerInnerTaylor() = default;

   /// Makes the variables in the linear model
   void makeVars(LPModel &lpm, const IntervalBox &B);

   /// Makes the constraints in the linear model
   bool makeCtrs(LPModel &lpm, const IntervalBox &B, const Bitset &active);

   bool makeLPImpl(LPModel &lpm, const IntervalBox &B, const Bitset &active) override;

   /// Uses Hansen's derivatives if b=true, classical ones if b=false.
   void useHansenDerivatives(bool b);

   /**
    * @brief Assigns the selection method of corners.
    *
    * When this method is called, the first corner is given as input.
    * Hence, it is not selected randomly. This is useful for testing.
    *
    * If opposute is true, the relaxation is generated for the given
    * corner and its opposite.
    */
   void fixCorner(const Bitset &corner, bool opposite = false);

private:
   bool hansen_;       // true if Hansen's differentiation algorithm is used
   CornerStyle style_; // selection method of corners
   Bitset corner_;     // one bit per variable: 0 left bound, 1 right bound
   IntRandom gen_;     // generator of pseudo-random numbers

   // Selects the first corner of B before solving and assigns corner_
   void selectCorner(const IntervalBox &B);

   // Makes the constraints on a given corner
   bool makeOne(LPModel &lpm, const IntervalBox &B, const Bitset &corner,
                const Bitset &active);
};

} // namespace realpaver

#endif
