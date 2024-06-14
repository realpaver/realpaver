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
 * @file   RLTRelaxation.hpp
 * @brief  Reformulation-Linearization Techniques
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_RLT_RELAXATION_HPP
#define REALPAVER_RLT_RELAXATION_HPP

#include <functional>
#include <unordered_map>
#include "realpaver/Dag.hpp"
#include "realpaver/LPModel.hpp"

namespace realpaver {

/**
 * @brief Visitor used to linearize a DAG.
 * 
 * This visits any dag node and generates linear under-estimation and
 * over-estimation constraints.
 */
class RltVisitor : public DagVisitor {
public:
   /**
    * @brief Constructor.
    * 
    * lpm is the resulting linear program and mpi is a map that associates
    * the index of a linear variable in lpm to a node index in the DAG.
    */
   RltVisitor(LPModel* lpm, std::unordered_map<size_t, size_t>* mpi);

   void apply(const DagConst* node) override;
   void apply(const DagVar* node) override;
   void apply(const DagAdd* node) override;
   void apply(const DagSub* node) override;
   void apply(const DagMul* node) override;
   void apply(const DagDiv* node) override;
   void apply(const DagMin* node) override;
   void apply(const DagMax* node) override;
   void apply(const DagUsb* node) override;
   void apply(const DagAbs* node) override;
   void apply(const DagSgn* node) override;
   void apply(const DagSqr* node) override;
   void apply(const DagSqrt* node) override;
   void apply(const DagPow* node) override;
   void apply(const DagExp* node) override;
   void apply(const DagLog* node) override;
   void apply(const DagCos* node) override;
   void apply(const DagSin* node) override;
   void apply(const DagTan* node) override;
   void apply(const DagCosh* node) override;
   void apply(const DagSinh* node) override;
   void apply(const DagTanh* node) override;

private:
   LPModel* lpm_;
   std::unordered_map<size_t, size_t>* mpi_;

   /// Returns the index of the linear variable associated with this node
   size_t indexLinVar(const DagNode* node) const;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Under-estimator constraint of a convex function.
 * 
 * Generates a linear under-estimator constraint of a convex function y = f(x).
 * 
 * @param lm linear program in which the new consraint is inserted
 * @param iy index of y in lm
 * @param ix index of x in lm
 * @param a left bound of x
 * @param b right bound of x
 * @param c point where the new constraint and the curve of f are tangent
 * @param f the considered function
 * @param df the derivative of f
 */
void underConvex(LPModel& lm, size_t iy, size_t ix,
                 double a, double b, double c,
                 std::function<Interval(Interval)> f, 
                 std::function<Interval(Interval)> df);

/**
 * @brief Over-estimator constraint of a convex function.
 * 
 * Generates a linear over-estimator constraint of a convex function y = f(x).
 * 
 * @param lm linear program in which the new consraint is inserted
 * @param iy index of y in lm
 * @param ix index of x in lm
 * @param a left bound of x
 * @param b right bound of x
 * @param f the considered function
 */
void overConvex(LPModel& lm, size_t iy, size_t ix, double a, double b,
                std::function<Interval(Interval)> f);

/**
 * @brief Over-estimator constraint of a concave function.
 * 
 * Generates a linear over-estimator constraint of a concave function y = f(x).
 * 
 * @param lm linear program in which the new consraint is inserted
 * @param iy index of y in lm
 * @param ix index of x in lm
 * @param a left bound of x
 * @param b right bound of x
 * @param c point where the new constraint and the curve of f are tangent
 * @param f the considered function
 * @param df the derivative of f
 */
void overConcave(LPModel& lm, size_t iy, size_t ix,
                 double a, double b, double c,
                 std::function<Interval(Interval)> f, 
                 std::function<Interval(Interval)> df);

/**
 * @brief Under-estimator constraint of a concave function.
 * 
 * Generates a linear under-estimator constraint of a concave function y = f(x).
 * 
 * @param lm linear program in which the new consraint is inserted
 * @param iy index of y in lm
 * @param ix index of x in lm
 * @param a left bound of x
 * @param b right bound of x
 * @param f the considered function
 */
void underConcave(LPModel& lm, size_t iy, size_t ix,
                  double a, double b,
                  std::function<Interval(Interval)> f);

/**
 * @brief Relaxation of a concavo-convex trigonometric function.
 * 
 * Generates a linear relaxation of a concavo-convex trigonometric function
 * y = f(x).
 * 
 * such that there is no stationary point in the considered domain of x
 * @param lm linear program in which the new consraint is inserted
 * @param iy index of y in lm
 * @param ix index of x in lm
 * @param a left bound of x
 * @param b right bound of x
 * @param f the considered function
 * @param df the derivative of f
 */
void relaxConcavoConvexCosSin(LPModel& lm, size_t iy, size_t ix,
                              double a, double b,
                              std::function<Interval(Interval)> f, 
                              std::function<Interval(Interval)> df);

/**
 * @brief Over-estimator constraint passing through two points.
 * 
 * Generates a linear over-estimator constraint passing through two points.
 * 
 * @param lm linear program in which the new consraint is inserted
 * @param iy index of y in lm
 * @param ix index of x in lm
 * @param x1 abcissa of the first point
 * @param y1 ordinate of the first point
 * @param x2 abcissa of the second point
 * @param y2 ordinate of the second point
 */
void overLine(LPModel& lm, size_t iy, size_t ix,
              double x1, double y1, double x2, double y2);

/**
 * @brief Under-estimator constraint passing through two points.
 * 
 * Generates a linear under-estimator constraint passing through two points.
 * 
 * @param lm linear program in which the new consraint is inserted
 * @param iy index of y in lm
 * @param ix index of x in lm
 * @param x1 abcissa of the first point
 * @param y1 ordinate of the first point
 * @param x2 abcissa of the second point
 * @param y2 ordinate of the second point
 */
void underLine(LPModel& lm, size_t iy, size_t ix,
               double x1, double y1, double x2, double y2);

/**
 * @brief Over-estimator constraint passing through one point given its slope.
 * 
 * Generates a linear over-estimator constraint passing through one point
 * given its slope.
 * 
 * @param lm linear program in which the new consraint is inserted
 * @param iy index of y in lm
 * @param ix index of x in lm
 * @param x1 abcissa of the point
 * @param y1 ordinate of the point
 * @param m slope
 */
void overLine(LPModel& lm, size_t iy, size_t ix, double x1, double y1, double m);

/**
 * @brief Under-estimator constraint passing through one point given its slope.
 * 
 * Generates a linear under-estimator constraint passing through one point
 * given its slope.
 * 
 * @param lm linear program in which the new consraint is inserted
 * @param iy index of y in lm
 * @param ix index of x in lm
 * @param x1 abcissa of the point
 * @param y1 ordinate of the point
 * @param m slope
 */
void underLine(LPModel& lm, size_t iy, size_t ix, double x1, double y1, double m);

} // namespace

#endif
