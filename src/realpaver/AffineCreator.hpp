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
 * @file   AffineCreator.hpp
 * @brief  Creators of affine forms
 * @author Laurent Granvilliers
 * @date   16/03/2025
 */

#ifndef REALPAVER_AFFINE_CREATOR_HPP
#define REALPAVER_AFFINE_CREATOR_HPP

#include "realpaver/AffineForm.hpp"
#include "realpaver/Dag.hpp"
#include <memory>

namespace realpaver {

/**
 * @brief Creator of affine forms.
 *
 * This is applied to a DAG (first constructor) or a subset of a DAG
 * (second constructor). The method create(B) creates an affine form on a box B
 * for each node of the functions considered. It is then possible to get their
 * affine forms.
 *
 * It is possible to select the approximation method of the elementary
 * functions, namely Minrange or Chebyshev.
 *
 * The variables in affine forms are represented by integers. Variable i corresponds
 * to the i-th variable of the scope of the DAG. Given this scope S, the original
 * variable is S.var(i).
 */
class AffineCreator {
public:
   /// Type of lists of indexes
   using IndexList = std::vector<size_t>;

   /**
    * @brief Creates a creator on a DAG.
    *
    * The boolean minrage is true if the linearization of the elementary
    * functions must use the Minrange approximation, false for the Chebyshev
    * approximation.
    */
   AffineCreator(SharedDag dag, bool minrange = true);

   /**
    * @brief Creates a creator on a subset of a DAG.
    *
    * The boolean minrage is true if the linearization of the elementary
    * functions must use the Minrange approximation, false for the Chebyshev
    * approximation.
    */
   AffineCreator(SharedDag dag, const IndexList &lfun, bool minrange = true);

   /**
    * @brief Creates a creator on one function of a DAG.
    *
    * The function considered has index i.
    *
    * The boolean minrage is true if the linearization of the elementary
    * functions must use the Minrange approximation, false for the Chebyshev
    * approximation.
    */
   AffineCreator(SharedDag dag, size_t i, bool minrange = true);

   /// No copy
   AffineCreator(const AffineCreator &) = delete;

   /// No assignment
   AffineCreator &operator=(const AffineCreator &) = delete;

   /// Default destructor
   ~AffineCreator() = default;

   /// Creates the affine forms on B
   void create(const IntervalBox &B);

   /// Selects the Minrange or the Chebyshev approximation
   void useMinrange(bool minrange = true);

   /// Returns the number of affine forms created
   size_t nbFuns() const;

   /**
    * @brief Gets the i-th affine form in this.
    *
    * It corresponds to the affine form of the i-th function of the DAG.
    */
   const AffineForm &fun(size_t i) const;

   /**
    * @brief Gets the ownership of the i-th affine form in this.
    *
    * It corresponds to the affine form of the i-th function of the DAG.
    */
   std::unique_ptr<AffineForm> moveFun(size_t i);

   /// Gets the affine form of a variable.
   const AffineForm &fun(const Variable &v) const;

private:
   SharedDag dag_;                              // a DAG
   bool minrange_;                              // true: Minrange, false:Chebyshev
   std::vector<std::unique_ptr<AffineForm>> v_; // affine forms
   IndexList lfun_;                             // list of indexes of functions

   // Creates the affine form of the i-th node of the DAG
   void makeNode(int i, const IntervalBox &B);
};

} // namespace realpaver

#endif
