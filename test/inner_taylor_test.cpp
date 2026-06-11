#include "realpaver/Common.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/ContractorFactory.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/DagExtended.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/IntervalBox.hpp"
#include "realpaver/LPModel.hpp"
#include "realpaver/LPSolver.hpp"
#include "realpaver/Linearizer.hpp"
#include "realpaver/LinearizerInnerTaylor.hpp"
#include "realpaver/LinearizerTaylor.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NLPSolver.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Prover.hpp"
#include "realpaver/RealFunction.hpp"
#include "realpaver/RealFunctionVector.hpp"
#include "realpaver/RealPoint.hpp"
#include "test_config.hpp"
#include <cmath>
#include <cstddef>
#include <memory>

using namespace realpaver;

SharedProblem get_problem(std::string &rp_string)
{
   SharedProblem pb = std::make_shared<Problem>();
   Parser parser;
   parser.parseStr(rp_string, *pb);
   return pb;
}

SharedDagExtended get_dag(SharedProblem pb, std::shared_ptr<Env> e = nullptr)
{
   std::shared_ptr<Env> env = e;
   if (env == nullptr)
      env = std::make_shared<Env>();
   ContractorFactory cf(*pb, env);
   return cf.extDag();
}

std::shared_ptr<Linearizer> get_linearizer(SharedProblem pb, bool inner,
                                           bool hansen = true,
                                           CornerStyle corner = CornerStyle::Left,
                                           std::shared_ptr<Env> e = nullptr)
{
   std::shared_ptr<Env> env = e;
   if (env == nullptr)
      env = std::make_shared<Env>();
   ContractorFactory cf(*pb, env);
   std::shared_ptr<Linearizer> linearizer;
   int goal_ctr_id = cf.getObjectiveConstraintDagId();
   LOG_INTER("OBJ CTR ID: " << goal_ctr_id)
   if (inner)
      linearizer = std::make_shared<LinearizerInnerTaylor>(cf.extDag(), goal_ctr_id,
                                                           hansen, CornerStyle::Left);
   else
      linearizer =
          std::make_shared<LinearizerTaylor>(cf.extDag(), hansen, CornerStyle::Left);
   return linearizer;
}

void test1()
{
   std::string rp_str = R"RP(
       Variables
         x in [-3.0,3.0],
         y in [-0,10];
       Constraints
         y >= x^2;
      )RP";
   SharedProblem pb = get_problem(rp_str);
   LOG_INTER("Problem: " << std::endl << *pb);
   SharedDagExtended dag = get_dag(pb);

   LinearizerInnerTaylor lit(dag, -1, false, CornerStyle::Left);
   LinearizerTaylor lt(dag, false, CornerStyle::Left);
   IntervalBox B1(pb->scope());
   IntervalBox B2(pb->scope());
   LPSolver inner, outer;
   lit.makeLP(inner, B1);
   lt.makeLP(outer, B2);
   LinExpr lei;
   lei.addTerm(1, inner.getLinVar(1));
   LinExpr leo;
   leo.addTerm(1, outer.getLinVar(1));
   inner.setCost(lei);
   outer.setCost(leo);
   LPStatus success = inner.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Inner val: " << inner.certifiedCostSolution() << " ~ "
                                   << inner.primalSolution())
   }

   success = outer.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Outer val: " << outer.certifiedCostSolution() << " ~ "
                                   << outer.primalSolution())
   }
}

void test2()
{
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-0,10];
        Constraints
          y >= exp(x);
       )RP";
   SharedProblem pb = get_problem(rp_str);
   LOG_INTER("Problem: " << std::endl << *pb);
   SharedDagExtended dag = get_dag(pb);

   LinearizerInnerTaylor lit(dag, -1, false, CornerStyle::Left);
   LinearizerTaylor lt(dag, false, CornerStyle::Left);
   IntervalBox B1(pb->scope());
   IntervalBox B2(pb->scope());
   LPSolver inner, outer;
   lit.makeLP(inner, B1);
   lt.makeLP(outer, B2);
   LinExpr lei;
   lei.addTerm(1, inner.getLinVar(1));
   LinExpr leo;
   leo.addTerm(1, outer.getLinVar(1));
   inner.setCost(lei);
   outer.setCost(leo);
   LPStatus success = inner.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Inner val: " << inner.certifiedCostSolution() << " ~ "
                                   << inner.primalSolution())
   }

   success = outer.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Outer val: " << outer.certifiedCostSolution() << " ~ "
                                   << outer.primalSolution())
   }
}

void test3()
{
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-0,10];
        Constraints
          y >= sin(x);
       )RP";
   SharedProblem pb = get_problem(rp_str);
   LOG_INTER("Problem: " << std::endl << *pb);
   SharedDagExtended dag = get_dag(pb);

   LinearizerInnerTaylor lit(dag, -1, false, CornerStyle::Left);
   LinearizerTaylor lt(dag, false, CornerStyle::Left);
   IntervalBox B1(pb->scope());
   IntervalBox B2(pb->scope());
   LPSolver inner, outer;
   lit.makeLP(inner, B1);
   lt.makeLP(outer, B2);
   LinExpr lei;
   lei.addTerm(1, inner.getLinVar(1));
   LinExpr leo;
   leo.addTerm(1, outer.getLinVar(1));
   inner.setCost(lei);
   outer.setCost(leo);
   LPStatus success = inner.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Inner val: " << inner.certifiedCostSolution() << " ~ "
                                   << inner.primalSolution())
   }

   success = outer.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Outer val: " << outer.certifiedCostSolution() << " ~ "
                                   << outer.primalSolution())
   }
}

void test4()
{
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-0,10],
          z in [0,5];
        Constraints
          z <= y + x^2,
          y >= sin(x);
       )RP";
   SharedProblem pb = get_problem(rp_str);
   LOG_INTER("Problem: " << std::endl << *pb);
   SharedDagExtended dag = get_dag(pb);

   LinearizerInnerTaylor lit(dag, -1, false, CornerStyle::Left);
   LinearizerTaylor lt(dag, false, CornerStyle::Left);
   IntervalBox B1(pb->scope());
   IntervalBox B2(pb->scope());
   LPSolver inner, outer;
   lit.makeLP(inner, B1);
   lt.makeLP(outer, B2);
   LinExpr lei;
   lei.addTerm(1, inner.getLinVar(1));
   LinExpr leo;
   leo.addTerm(1, outer.getLinVar(1));
   inner.setCost(lei);
   outer.setCost(leo);
   LPStatus success = inner.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Inner val: " << inner.certifiedCostSolution() << " ~ "
                                   << inner.primalSolution())
   }

   success = outer.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Outer val: " << outer.certifiedCostSolution() << " ~ "
                                   << outer.primalSolution())
   }
}

void test5()
{
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-0,10],
          z in [0,5];
        Constraints
          z == y + x^2,
          y >= sin(x);
       )RP";
   SharedProblem pb = get_problem(rp_str);
   LOG_INTER("Problem: " << std::endl << *pb);
   SharedDagExtended dag = get_dag(pb);

   LinearizerInnerTaylor lit(dag, -1, false, CornerStyle::Left);
   LinearizerTaylor lt(dag, false, CornerStyle::Left);
   IntervalBox B1(pb->scope());
   IntervalBox B2(pb->scope());
   LPSolver inner, outer;
   lit.makeLP(inner, B1);
   lt.makeLP(outer, B2);
   LinExpr lei;
   lei.addTerm(1, inner.getLinVar(1));
   LinExpr leo;
   leo.addTerm(1, outer.getLinVar(1));
   inner.setCost(lei);
   outer.setCost(leo);
   LPStatus success = inner.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Inner val: " << inner.certifiedCostSolution() << " ~ "
                                   << inner.primalSolution())
   }

   success = outer.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Outer val: " << outer.certifiedCostSolution() << " ~ "
                                   << outer.primalSolution())
   }
}

void test_process()
{
   std::string rp_str = R"RP(
    Variables
    x1 in [10, 2000],
    x2 in [0, 16000],
    x3 in [0, 120],
    x4 in [0, 5000],
    x5 in [0, 2000],
    x6 in [85, 93],
    x7 in [90, 95],
    x8 in [3, 12],
    x9 in [1.2, 4],
    x10 in [145, 162];

    Min - 0.063*x4*x7 + 5.04*x1 + 0.035*x2 + 10*x3 + 3.36*x5;
    Constraints
    - x1*(1.12 + 0.13167*x8 - 0.00667*x8^2) + x4 == 0,
    - x1 + 1.22*x4 - x5 == 0,
    - 0.001*x4*x9*x6/(98 - x6) + x3 == 0,
    - (1.098*x8 - 0.038*x8^2) - 0.325*x6 + x7 == 57.425,
    - (x2 + x5)/x1 + x8 == 0,
      x9 + 0.222*x10 == 35.82,
    - 3*x7 + x10 == -133;
// )RP";
   SharedProblem pb = get_problem(rp_str);
   pb->generateObjectiveConstraint();
   LOG_INTER("Problem: " << std::endl << *pb);

   std::shared_ptr<Linearizer> lit = get_linearizer(pb, true); // LinearizerInnerTaylor
   std::shared_ptr<Linearizer> lt = get_linearizer(pb, false); // LinearizerTaylor

   IntervalBox B1(pb->scope());
   IntervalBox B2(pb->scope());
   B1.set(pb->getObjective().getVar(), pb->getObjective().getTerm().eval(B1));
   B2.set(pb->getObjective().getVar(), pb->getObjective().getTerm().eval(B2));
   LOG_INTER("Obj eval: " << pb->getObjective().getTerm().eval(B1))
   LPSolver inner, outer;
   lit->makeLP(inner, B1);
   lt->makeLP(outer, B2);
   LinExpr lei;
   lei.addTerm(1, inner.getLinVar(lit->linVarIndex(pb->getObjective().getVar())));
   LinExpr leo;
   leo.addTerm(1, outer.getLinVar(lt->linVarIndex(pb->getObjective().getVar())));
   inner.setCost(lei);
   outer.setCost(leo);
   LOG_INTER("Linear problem: ")
   LOG_INTER(inner)
   LPStatus success = inner.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Inner val: " << inner.certifiedCostSolution() << " ~ "
                                   << inner.primalSolution())
   }

   success = outer.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Outer val: " << outer.certifiedCostSolution() << " ~ "
                                   << outer.primalSolution())
   }
}

void test_harker()
{
   std::string rp_str = R"RP(
       Variables
          x1 in [0, 1.0e2],
          x2 in [0, 1.0e2],
          x3 in [0, 1.0e2],
          x4 in [0, 1.0e2],
          x5 in [0, 1.0e2],
          x6 in [0, 1.0e2],
          x7 in [0, 1.0e2],
          x8 in [0, 1.0e2],
          x9 in [0, 1.0e2],
          x10 in [0, 1.0e2],
          x11 in [0, 1.0e2],
          x12 in [0, 1.0e2],
          x13 in [0, 1.0e2],
          x14 in [0, 1.0e2],
          x15 in [0, 1.0e2],
          x16 in [0, 1.0e2],
          x17 in [0, 1.0e2],
          x18 in [0, 1.0e2],
          x19 in [0, 1.0e2],
          x20 in [0, 1.0e2];

       Min  - (19*x15 - 0.1*x15^2 - 0.5*x18^2 - x18 - 0.005*x16^2 + 27*x16
                      - 0.4*x19^2 - 2*x19 - 0.15*x17^2 + 30*x17 - 0.3*x20^2 - 1.5*x20
                      - (0.166666666666667*x1^3 + x1 + 0.0666666666666667*x2^3 + 2*x2
                      + 0.1*x3^3 + 3*x3 + 0.133333333333333*x4^3 + x4 + 0.1*x5^3 + 2*
                     x5 + 0.0333333333333333*x6^3 + x6 + 0.0333333333333333*x7^3 + x7
                      + 0.166666666666667*x8^3 + 3*x8 + 0.0666666666666667*x9^3 + 2*x9
                      + 0.333333333333333*x10^3 + x10 + 0.0833333333333333*x11^3 + 2*
                     x11 + 0.0666666666666667*x12^3 + 2*x12 + 0.3*x13^3 + x13 +
                     0.266666666666667*x14^3 + 3*x14));

       Constraints
       x15 + x16 + x17 - x18 - x19 - x20 == 0,
       - x1 - x2 + x5 + x8 - x15 + x18 == 0,
       - x3 + x11 - x16 + x19 == 0,
       - x4 + x12 - x17 + x20 == 0,
       x1 - x5 - x6 - x7 + x9 + x13 == 0,
       x2 + x6 - x8 - x9 - x10 + x14 == 0,
       x3 + x4 + x7 + x10 - x11 - x12 - x13 - x14 == 0;
)RP";
   SharedProblem pb = get_problem(rp_str);
   pb->generateObjectiveConstraint();
   LOG_INTER(*pb)

   std::shared_ptr<Linearizer> lit = get_linearizer(pb, true); // LinearizerInnerTaylor
   std::shared_ptr<Linearizer> lt = get_linearizer(pb, false); // LinearizerTaylor

   IntervalBox B1(pb->scope());
   IntervalBox B2(pb->scope());
   B1.set(pb->getObjective().getVar(), pb->getObjective().getTerm().eval(B1));
   B2.set(pb->getObjective().getVar(), pb->getObjective().getTerm().eval(B2));
   LOG_INTER("Obj eval: " << B1)
   LPSolver inner, outer;
   lit->makeLP(inner, B1);
   lt->makeLP(outer, B2);
   LinExpr lei;
   lei.addTerm(1, inner.getLinVar(lit->linVarIndex(pb->getObjective().getVar())));
   LinExpr leo;
   leo.addTerm(1, outer.getLinVar(lt->linVarIndex(pb->getObjective().getVar())));
   inner.setCost(lei);
   outer.setCost(leo);
   LPStatus success = inner.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Inner val: " << inner.certifiedCostSolution() << " ~ "
                                   << inner.primalSolution())
   }

   success = outer.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("**** Outer val: " << outer.certifiedCostSolution() << " ~ "
                                   << outer.primalSolution())
   }
}

void test_process_box()
{
   std::string rp_str = R"RP(
    Variables
    x1 in [10, 2000],
    x2 in [0, 16000],
    x3 in [0, 120],
    x4 in [0, 5000],
    x5 in [0, 2000],
    x6 in [85, 93],
    x7 in [90, 95],
    x8 in [3, 12],
    x9 in [1.2, 4],
    x10 in [145, 162];

    Min - 0.063*x4*x7 + 5.04*x1 + 0.035*x2 + 10*x3 + 3.36*x5;
    Constraints
    - x1*(1.12 + 0.13167*x8 - 0.00667*x8^2) + x4 == 0,
    - x1 + 1.22*x4 - x5 == 0,
    - 0.001*x4*x9*x6/(98 - x6) + x3 == 0,
    - (1.098*x8 - 0.038*x8^2) - 0.325*x6 + x7 == 57.425,
    - (x2 + x5)/x1 + x8 == 0,
      x9 + 0.222*x10 == 35.82,
    - 3*x7 + x10 == -133;
   )RP";

   SharedProblem pb = get_problem(rp_str);
   pb->generateObjectiveConstraint();
   LOG_INTER("Problem: " << std::endl << *pb);

   std::shared_ptr<Linearizer> lit = get_linearizer(pb, true); // LinearizerInnerTaylor
   Prover prover(get_dag(pb));                                 //, 1e-6);
   // Box:
   // [1502.489171701478, 1502.5] ; [15271.50499026968, 15300.34957871695] ;
   // [83.8085066693219, 83.94618541417282] ; [2637.929797585163, 2638.471166916035] ;
   // [1715.774353043899, 1716.434823647562] ; [91.48006340565329, 91.48193865333164] ;
   // [94.71275913582526, 94.71781550485418] ; [11.30652238949597, 11.32520726571438] ;
   // [2.263934861547113, 2.267302427760385] ; [151.1382773974757, 151.1534465245626]
   IntervalBox B0(pb->scope());
   B0.set(pb->varAt(0), Interval(1502.489171701478, 1502.5));
   B0.set(pb->varAt(1), Interval(15271.50499026968, 15300.34957871695));
   B0.set(pb->varAt(2), Interval(83.8085066693219, 83.94618541417282));
   B0.set(pb->varAt(3), Interval(2637.929797585163, 2638.471166916035));
   B0.set(pb->varAt(4), Interval(1715.774353043899, 1716.434823647562));
   B0.set(pb->varAt(5), Interval(91.48006340565329, 91.48193865333164));
   B0.set(pb->varAt(6), Interval(94.71275913582526, 94.71781550485418));
   B0.set(pb->varAt(7), Interval(11.30652238949597, 11.32520726571438));
   B0.set(pb->varAt(8), Interval(2.263934861547113, 2.267302427760385));
   B0.set(pb->varAt(9), Interval(151.1382773974757, 151.1534465245626));
   B0.set(pb->getObjective().getVar(), pb->getObjective().getTerm().eval(B0));
   LOG_INTER("Obj eval: " << pb->getObjective().getTerm().eval(B0))
   LPSolver inner0;
   lit->makeLP(inner0, B0);
   LinExpr lei;
   lei.addTerm(1, inner0.getLinVar(lit->linVarIndex(pb->getObjective().getVar())));
   inner0.setCost(lei);
   LPStatus success = inner0.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("Inner val: " << inner0.certifiedCostSolution() << " ~ "
                              << inner0.primalSolution())
      IntervalBox sol0(pb->scope(), inner0.primalSolution());
      Proof proof = prover.certify(sol0);
      if (proof == Proof::Feasible)
      {
         LOG_INTER("****** Proved solution: " << sol0)
      }
      else
      {
         LOG_INTER("****** Solution not being proved!")
      }
   }
   else
   {
      LOG_INTER("****** No feasible point found")
   }
   LOG_INTER(" ======= " << std::endl)

   // Box:
   // ([1728.904377323658, 1728.920902396929] ; [15999.75301955081, 16000] ;
   // [89.66812800242099, 89.67177489859224] ; [3056.464008493834, 3056.492537188241] ;
   // [1999.98171304882, 2000] ; [88.99964060348244, 89] ;
   // [93.66237792488112, 93.66255936227484] ; [10.41098593954356, 10.41119673650761] ;
   // [2.966735452803028, 2.966856289809164] ; [147.9871337846433, 147.9876780968246])
   IntervalBox B1(pb->scope());
   B1.set(pb->varAt(0), Interval(1728.904377323658, 1728.920902396929));
   B1.set(pb->varAt(1), Interval(15999.75301955081, 16000));
   B1.set(pb->varAt(2), Interval(89.66812800242099, 89.67177489859224));
   B1.set(pb->varAt(3), Interval(3056.464008493834, 3056.492537188241));
   B1.set(pb->varAt(4), Interval(1999.98171304882, 2000));
   B1.set(pb->varAt(5), Interval(88.99964060348244, 89));
   B1.set(pb->varAt(6), Interval(93.66237792488112, 93.66255936227484));
   B1.set(pb->varAt(7), Interval(10.41098593954356, 10.41119673650761));
   B1.set(pb->varAt(8), Interval(2.966735452803028, 2.966856289809164));
   B1.set(pb->varAt(9), Interval(147.9871337846433, 147.9876780968246));
   B1.set(pb->getObjective().getVar(), pb->getObjective().getTerm().eval(B1));
   LOG_INTER("Obj eval: " << pb->getObjective().getTerm().eval(B1))
   LPSolver inner1;
   lit->makeLP(inner1, B1);
   inner1.setCost(lei);
   success = inner1.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("****** Inner val: " << inner1.certifiedCostSolution() << " ~ "
                                     << inner1.primalSolution())
      IntervalBox sol1(pb->scope(), inner1.primalSolution());
      Proof proof = prover.certify(sol1);
      if (proof == Proof::Feasible)
      {
         LOG_INTER("****** Proved solution: " << sol1)
      }
      else
      {
         LOG_INTER("****** Solution not being proved!")
      }
   }
   else
   {
      LOG_INTER("****** No feasible point found")
   }
   LOG_INTER(" ======= " << std::endl)

   // Box:
   // [1728.915405624773, 1728.920909117964] ; [15999.94032134023, 16000] ;
   // [98.1183301474912, 98.16771842458343] ; [3056.482457368713, 3056.492536829011] ;
   // [1999.993192375056, 2000] ; [90.609375, 90.6171875] ;
   // [94.18557599914915, 94.18812833436596] ; [10.41108638330841, 10.41114751730836] ;
   // [2.616706522440267, 2.618406372346661] ; [149.5567280074474, 149.5643850130978]
   IntervalBox B2(pb->scope());
   B2.set(pb->varAt(0), Interval(1728.915405624773, 1728.920909117964));
   B2.set(pb->varAt(1), Interval(15999.94032134023, 16000));
   B2.set(pb->varAt(2), Interval(98.1183301474912, 98.16771842458343));
   B2.set(pb->varAt(3), Interval(3056.482457368713, 3056.492536829011));
   B2.set(pb->varAt(4), Interval(1999.993192375056, 2000));
   B2.set(pb->varAt(5), Interval(90.609375, 90.6171875));
   B2.set(pb->varAt(6), Interval(94.18557599914915, 94.18812833436596));
   B2.set(pb->varAt(7), Interval(10.41108638330841, 10.41114751730836));
   B2.set(pb->varAt(8), Interval(2.616706522440267, 2.618406372346661));
   B2.set(pb->varAt(9), Interval(149.5567280074474, 149.5643850130978));
   B2.set(pb->getObjective().getVar(), pb->getObjective().getTerm().eval(B2));

   LPSolver inner2;
   lit->makeLP(inner2, B2);
   inner2.setCost(lei);
   success = inner2.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_INTER("****** Inner val: " << inner2.certifiedCostSolution() << " ~ "
                                     << inner2.primalSolution())
      IntervalBox sol2(pb->scope(), inner2.primalSolution());
      Proof proof = prover.certify(sol2);
      if (proof == Proof::Feasible)
      {
         LOG_INTER("****** Proved solution: " << sol2)
      }
      else
      {
         LOG_INTER("****** Solution not being proved!")
      }
   }
   else
   {
      LOG_INTER("****** No feasible point found")
   }
}

int main()
{
   Logger::init(LogLevel::inter, "inner_taylor_relax_test.log");
   // test1();
   // test2();
   // test3();
   // test4();
   // test5();
   // test_process();
   // test_harker();
   test_process_box();

   END_TEST
}
