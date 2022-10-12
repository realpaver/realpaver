///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REFORMULATION_HPP
#define REALPAVER_REFORMULATION_HPP

#include <functional>
#include "realpaver/Interval.hpp"
#include "realpaver/LPModel.hpp"

namespace realpaver {

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
