///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_RLT_RELAXATION_HPP
#define REALPAVER_RLT_RELAXATION_HPP

#include <functional>
#include <unordered_map>
#include "realpaver/Dag.hpp"
#include "realpaver/LPModel.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This visits any dag node and generates linear under-estimation and
/// over-estimation constraints.
///////////////////////////////////////////////////////////////////////////////
class RltVisitor : public DagVisitor {
public:
   /// Creates a visitor
   /// @param lpm linear program
   /// @param mpi map node index -> index of linear variable in lpm
   RltVisitor(LPModel* lpm, std::unordered_map<size_t, size_t>* mpi);

   ///@{
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
   void apply(const DagLin* node) override;
   ///@}

private:
   LPModel* lpm_;
   std::unordered_map<size_t, size_t>* mpi_;

   /// @param node a node
   /// @return the index of the linear variable associated with this node
   size_t indexLinVar(const DagNode* node) const;
};

///////////////////////////////////////////////////////////////////////////////

/// Generates a linear under-estimator constraint of a convex function y = f(x)
/// @param lm linear program in which the new consraint is inserted
/// @param iy index of y in lm
/// @param ix index of x in lm
/// @param a left bound of x
/// @param b right bound of x
/// @param c point where the new constraint and the curve of f are tangent
/// @param f the considered function
/// @param df the derivative of f
void underConvex(LPModel& lm, size_t iy, size_t ix,
                 double a, double b, double c,
                 std::function<Interval(Interval)> f, 
                 std::function<Interval(Interval)> df);

/// Generates a linear over-estimator constraint of a convex function y = f(x)
/// @param lm linear program in which the new consraint is inserted
/// @param iy index of y in lm
/// @param ix index of x in lm
/// @param a left bound of x
/// @param b right bound of x
/// @param f the considered function
void overConvex(LPModel& lm, size_t iy, size_t ix,
                double a, double b,
                std::function<Interval(Interval)> f);

/// Generates a linear over-estimator constraint of a concave function y = f(x)
/// @param lm linear program in which the new consraint is inserted
/// @param iy index of y in lm
/// @param ix index of x in lm
/// @param a left bound of x
/// @param b right bound of x
/// @param c point where the new constraint and the curve of f are tangent
/// @param f the considered function
/// @param df the derivative of f
void overConcave(LPModel& lm, size_t iy, size_t ix,
                 double a, double b, double c,
                 std::function<Interval(Interval)> f, 
                 std::function<Interval(Interval)> df);

/// Generates a linear under-estimator constraint of a convex function y = f(x)
/// @param lm linear program in which the new consraint is inserted
/// @param iy index of y in lm
/// @param ix index of x in lm
/// @param a left bound of x
/// @param b right bound of x
/// @param f the considered function
void underConcave(LPModel& lm, size_t iy, size_t ix,
                  double a, double b,
                  std::function<Interval(Interval)> f);

/// Generates a linear relaxation of a trigonometric function y = f(x)
/// @param lm linear program in which the new consraint is inserted
/// @param iy index of y in lm
/// @param ix index of x in lm
/// @param a left bound of x
/// @param b right bound of x
/// @param f the considered function
/// @param df the derivative of f
void relaxConcavoConvexCosSin(LPModel& lm, size_t iy, size_t ix,
                              double a, double b,
                              std::function<Interval(Interval)> f, 
                              std::function<Interval(Interval)> df);

/// Generates a linear over-estimator constraint passing through two points
/// @param lm linear program in which the new consraint is inserted
/// @param iy index of y in lm
/// @param ix index of x in lm
/// @param x1 abcissa of the first point
/// @param y1 ordinate of the first point
/// @param x2 abcissa of the second point
/// @param y2 ordinate of the second point
void overLine(LPModel& lm, size_t iy, size_t ix,
              double x1, double y1, double x2, double y2);

/// Generates a linear under-estimator constraint passing through two points
/// @param lm linear program in which the new consraint is inserted
/// @param iy index of y in lm
/// @param ix index of x in lm
/// @param x1 abcissa of the first point
/// @param y1 ordinate of the first point
/// @param x2 abcissa of the second point
/// @param y2 ordinate of the second point
void underLine(LPModel& lm, size_t iy, size_t ix,
              double x1, double y1, double x2, double y2);

} // namespace

#endif
