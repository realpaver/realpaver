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
 * @file   Linearizer.hpp
 * @brief  Linearization of nonlinear problems
 * @author Laurent Granvilliers
 * @date   2025-fev-11
 */

#ifndef REALPAVER_LINEARIZER_HPP
#define REALPAVER_LINEARIZER_HPP

#include "realpaver/Dag.hpp"
#include "IntRandom.hpp"
#include "realpaver/LPModel.hpp"

namespace realpaver {

   /// Base class of linearizers of nonlinear problems
   class Linearizer {
   public:
      /// Type of lists of indexes
      using IndexList = std::vector<size_t>;

      /// Creates a linearizer for a DAG
      Linearizer(SharedDag dag);

      /// Creates a linearizer for a subset of a DAG given by a list of indexes
      Linearizer(SharedDag dag, const IndexList& lfun);

      /// Destructor
      virtual ~Linearizer();

      /// No copy
      Linearizer(const Linearizer&) = delete;

      /// No assignment
      Linearizer& operator=(const Linearizer&) = delete;

      /// Returns the dag of this
      SharedDag dag() const;

      /// Returns the scope of this
      Scope scope() const;

      /**
       * @brief Makes the linear relaxation.
       *
       * It makes a linear relaxation of the DAG (or a subset) on
       * the given box B. The new linear program is assigned to lpm
       * and it returns true in case of success, false otherwise.
       */
      virtual bool make(LPModel& lpm, const IntervalBox& B) = 0;

      /// Returns the index of the linear variable associated with v
      size_t linVarIndex(Variable v) const;

      /// Returns the relaxation tolerance for the equations
      double getRelaxTol() const;

      /// Assigns the relaxation tolerance for the equations
      void setRelaxTol(double tol);

   protected:
      SharedDag dag_;                     // DAG
      Scope scop_;                        // scope
      std::unordered_map<size_t, size_t>  // map var index -> lin var index
         mvv_;
      IndexList lfun_;                    // list of indexes of functions
      double tol_;                        // relaxation tol for the equations
   };

   /*----------------------------------------------------------------------------*/

   /// Choice of corner in a LinearizerTaylor object
   enum class CornerStyle {
      Random,        ///< random with the current system time as seed
      RandomSeed,    ///< random with a fixed seed
      User           ///< fixed by the user (useful for testing)
   };

   /**
    * @brief Linearizer that generates Taylor forms.
    *
    * It generates a relaxation using Taylor forms in two opposite corners.
    * The first corner can be fixed manually or selected randomly.
    */
   class LinearizerTaylor : public Linearizer {
   public:
      /**
       * @brief Creates a linearizer of a DAG.
       *
       * The boolean hansen is true if Hansen's differentiation algorithm is
       * used, false otherwise. The style specifies how the first corner is
       * selected.
       */
      LinearizerTaylor(SharedDag dag, bool hansen = false,
                       CornerStyle style = CornerStyle::RandomSeed);

      /**
       * @brief Creates a creator for a subset of a DAG.
       *
       * The boolean hansen is true if Hansen's differentiation algorithm is
       * used, false otherwise. The style specifies how the first corner is
       * selected.
       */
      LinearizerTaylor(SharedDag dag, const IndexList& lfun,
                       bool hansen = false,
                       CornerStyle style = CornerStyle::RandomSeed);

      bool make(LPModel& lpm, const IntervalBox& B) override;

      /// Uses Hansen's derivatives if b=true, classical ones if b=false.
      void useHansenDerivatives(bool b);

      /**
       * @brief Assigns the selection method of corners.
       *
       * When this method is called, the corners are selected randomly using
       * a generator of pseudo-random numbers initialized either with a fixed
       * seed given as input if it is different from 0, or with the current
       * system time as seed if the input is equal to 0.
       */
      void useRandomCorners(unsigned seed = SEED);

      /**
       * @brief Assigns the selection method of corners.
       *
       * When this method is called, the first corner is given as input.
       * Hence, it is not selected randomly. This is useful for testing.
       */
      void fixFirstCorner(const Bitset& corner);

   private:
      bool hansen_;        // true if Hansen's differentiation algorithm is used
      CornerStyle style_;  // selection method of corners
      Bitset corner_;      // one bit per variable: 0 left bound, 1 right bound
                           // this corner and its opposite will be considered
                           // to generate the Taylor relaxation
      IntRandom gen_;      // generator of pseudo-random numbers

      // Selects the first corner of B before solving and assigns corner_
      void selectCorner(const IntervalBox& B);

      static bool areEquals(const LinExpr& e1, const LinExpr& e2);

      static const unsigned SEED = 6907;
};

} // namespace

#endif
