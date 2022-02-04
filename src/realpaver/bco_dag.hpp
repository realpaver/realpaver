

#ifndef REALPAVER_BCO_DAG_HPP
#define REALPAVER_BCO_DAG_HPP

#include "realpaver/contractor_bco.hpp"
#include "realpaver/dag.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/propagator.hpp"
#include "realpaver/real_fun.hpp"

namespace realpaver {

/** This represents a bound constrained optimization model.
 * 
 *  Given an objective function f(x0, ..., xk) a dag is created and we have:
 *    - the i-th function in the DAG (i in 0..k) corresponds to df / dxi ;
 *    - the (k+1)-th function in the DAG represents the equation z - f = 0 if
 *      f has to be minimized or z + f = 0 otherwise.
 */
class BcoDag : public RealFun {
public:
   // constructor
   BcoDag(Problem& P);

   // destructor
   ~BcoDag();

   // returns the dag
   Dag* getDag() const;

   // returns the variable reprsenting the objective function
   Variable objVar() const;

   // returns the scope of the objective function
   Scope fScope() const;

   // returns the number of decision variables in the objective function
   // i.e. the size of fscope()
   size_t dim() const;

   // returns the scope of the objective function plus the objective variable
   Scope dScope() const;

   // returns the initial box
   SharedIntervalVector initialBox() const;

   // returns the propagator for contracting domains
   Propagator* propagator();

   // calls makeHc4Propagator if the propagator has not been created
   void makeDefaultPropagator();

   // makes contractors based on HC4
   void makeHc4Propagator();

   // makes contractors based on HC4
   void makeBc4Propagator();

   // returns false if some inconsistency is detected
   bool checkProblem();

   // @Override
   // evaluation and differentiation of the objective function over the reals
   double reval(const RealVector& x);
   void rdiff(const RealVector& x, RealVector& g);
   void revalDiff(const RealVector& x, double& val, RealVector& g);

private:
   Dag* dag_;        // expression graph
   SharedIntervalVector init_;  // initial box
   Variable z_;      // variable representing the objective function
   Scope fscope_;    // scope of the objective function
   Scope dscope_;    // scope of the dag (fscope_ + z_)

   ContractorVector pool_;   // pool of contractors
   Propagator propag_;       // propagator based on the pool of contractors

   // methods applied by checkProblem
   bool checkInitialBox() const;
   bool checkObjRange() const;
   bool checkDerivRange() const;
};

} // namespace

#endif
