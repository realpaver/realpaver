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
 * @file   AffineForm.hpp
 * @brief  Affine arithmetic
 * @author Laurent Granvilliers
 * @date   10/03/2025
 */

#ifndef REALPAVER_AFFINE_FORM_HPP
#define REALPAVER_AFFINE_FORM_HPP

#include "realpaver/Interval.hpp"
#include <initializer_list>
#include <list>

namespace realpaver {

/**
 * @brief Affine form of a nonlinear expression.
 *
 * This represents a modified affine form AF1 described in:
 *   F. Messine. Extensions of Affine Arithmetic: Application to
 *   Unconstrained Global Optimization, JUCS 2002.
 *
 * The modifications are twofold: a reliable form is implemented using
 * interval coefficients and the linearization of square terms is improved
 * (as the AF2 form does).
 *
 * The reliable AF1 form of f(x1, ..., xn) is defined as
 * a_0 + sum_(i=0 to n+1) a_i*e_i where:
 *   - a_i is an interval for each i,
 *   - e_i is a variable lying in [-1, 1] for each i,
 *   - x_i is associated with e_i for each i=1,...,n,
 *   - a_{i+1}>=0 is the magnitude of the error term stemming from
 *     the linearization of the nonlinear part of f.
 *
 * The reliable AF1 form is represented by the triple (a_0, L, a_(n+1)) where
 * L is the list of linear terms (a_i, i), i.e. a_i*e_i where e_i is
 * represented by its index i, for i=1,...,n.
 *
 * The elemenrary functions can be linearized with either the Minrange
 * approximation or the Chebyshev approximation. It suffices to call
 * useMinrange or useChebyshev before the creation of an affine form.
 */
class AffineForm {
public:
  /// Creates an affine form equal to zero
  AffineForm();

  /// Creates an affine form of a variable given its domain
  AffineForm(int var, const Interval &domain);

  /// Creates an affine form of a constant
  AffineForm(const Interval &x);

  /// Creates an affine form of a constant
  AffineForm(const double &a);

  /**
   * @brief Creates an affine form.
   *
   * The affine form is defined as a0 + sum_i A_i*V_i + e*V_err given
   * a constant a0, a list of coefficients A, a list of variable indexes V
   * and the magnitude e of the error term.
   *
   * Assumes that V is sorted by increasing ordering.
   */
  AffineForm(const double &a0, const std::initializer_list<double> &A,
             const std::initializer_list<int> &V,
             const Interval &e = Interval::zero());

  /**
   * @brief Creates an affine form.
   *
   * The affine form is defined as a0 + sum_i A_i*V_i + e*V_err given
   * a constant a0, a list of coefficients A, a list of variable indexes V
   * and the magnitude e of the error term.
   *
   * Assumes that V is sorted by increasing ordering.
   */
  AffineForm(const Interval &a0, const std::initializer_list<Interval> &A,
             const std::initializer_list<int> &V,
             const Interval &e = Interval::zero());

  /// Default copy constructor
  AffineForm(const AffineForm &) = default;

  /// Default assignment operator
  AffineForm &operator=(const AffineForm &) = default;

  /// Default destructor
  ~AffineForm() = default;

  /// Equality test
  bool equals(const AffineForm &f) const;

  /// Returns the constant term a_0
  const Interval &constantTerm() const;

  /// Returns the magnitude of the error term a_(n+1)
  const Interval &errorTerm() const;

  // Inserts a linear term in the list
  void push(const Interval &x, int var);

  // Inserts an affine form into this (this := this + f)
  void push(const AffineForm &f);

  /// Addition
  friend AffineForm operator+(const AffineForm &f, const AffineForm &g);

  /// Subtraction
  friend AffineForm operator-(const AffineForm &f, const AffineForm &g);

  /// Unary subtraction
  friend AffineForm operator-(const AffineForm &f);

  /// Multiplication
  friend AffineForm operator*(const AffineForm &f, const AffineForm &g);

  /// Unary division
  friend AffineForm udiv(const AffineForm &f);

  /// Division
  friend AffineForm operator/(const AffineForm &f, const AffineForm &g);

  /// Square
  friend AffineForm sqr(const AffineForm &f);

  /// Square root
  friend AffineForm sqrt(const AffineForm &f);

  /// Power
  friend AffineForm pow(const AffineForm &f, int e);

  /// Minimum
  friend AffineForm min(const AffineForm &f, const AffineForm &g);

  /// Maximum
  friend AffineForm max(const AffineForm &f, const AffineForm &g);

  /// Absolute value
  friend AffineForm abs(const AffineForm &f);

  /// Sign function
  friend AffineForm sgn(const AffineForm &f);

  /// Exponential
  friend AffineForm exp(const AffineForm &f);

  /// Logarithm
  friend AffineForm log(const AffineForm &f);

  /// Cosine
  friend AffineForm cos(const AffineForm &f);

  /// Sine
  friend AffineForm sin(const AffineForm &f);

  /// Tangent
  friend AffineForm tan(const AffineForm &f);

  /// Hyperbolic cosine
  friend AffineForm cosh(const AffineForm &f);

  /// Hyperbolic sine
  friend AffineForm sinh(const AffineForm &f);

  /// Hyperbolic tangent
  friend AffineForm tanh(const AffineForm &f);

  /// Interval evaluation
  Interval eval() const;

  /// assigns this to a form representing the empty set
  void setEmpty();

  /// Returns true if this represents an empty set
  bool isEmpty() const;

  /// Returns the number of linear terms
  size_t nbLinearTerms() const;

  /// Returns an affine form that represents an empty set
  static AffineForm emptyForm();

  /// Returns an affine form that represents the universe
  static AffineForm infForm();

  /// Returns true if this represents the universe
  bool isInf() const;

  /// Is the Minrange approximation used?
  static bool usesMinrange();

  /// Forces the use of the Minrage approximation
  static void useMinrange(bool b = true);

  /// Forces the use of the Chebyshev approximation
  static void useChebyshev(bool b = true);

  /// Output on a stream
  friend std::ostream &operator<<(std::ostream &os, const AffineForm &f);

  /// Calculates dzeta and delta
  static void approxDzetaDelta(const Interval &alpha, const Interval &a,
                               const Interval &fa, const Interval &b,
                               const Interval &fb, Interval &dzeta,
                               Interval &delta);

  // Minrange approximation of function 1/z with z in x
  // Assumes that 0 does not belong to x
  static void minrangeUdiv(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  /// Chebyshev approximation of function 1/z with z in x.
  static void chebyshevUdiv(const Interval &x, Interval &alpha, Interval &dzeta,
                            Interval &delta);

  // Minrange approximation of function sqrt(z) with z in x
  static void minrangeSqrt(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  /// Chebyshev approximation of function sqrt(z) with z in x
  static void chebyshevSqrt(const Interval &x, Interval &alpha, Interval &dzeta,
                            Interval &delta);

  // Minrange approximation of function exp(z) with z in x
  static void minrangeExp(const Interval &x, Interval &alpha, Interval &dzeta,
                          Interval &delta);

  /// Chebyshev approximation of function exp(z) with z in x
  static void chebyshevExp(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  // Minrange approximation of function log(z) with z in x
  static void minrangeLog(const Interval &x, Interval &alpha, Interval &dzeta,
                          Interval &delta);

  /// Chebyshev approximation of function log(z) with z in x
  static void chebyshevLog(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  // Minrange approximation of function cos(z) with z in x
  static void minrangeCos(const Interval &x, Interval &alpha, Interval &dzeta,
                          Interval &delta);

  // Minrange approximation of function sin(z) with z in x
  static void minrangeSin(const Interval &x, Interval &alpha, Interval &dzeta,
                          Interval &delta);

  // Minrange approximation of function tan(z) with z in x
  static void minrangeTan(const Interval &x, Interval &alpha, Interval &dzeta,
                          Interval &delta);

  // Minrange approximation of function cosh(z) with z in x
  static void minrangeCosh(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  // Minrange approximation of function sinh(z) with z in x
  static void minrangeSinh(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  // Minrange approximation of function tanh(z) with z in x
  static void minrangeTanh(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  /// Chebyshev approximation of function cos(z) with z in x
  static void chebyshevCos(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  /// Chebyshev approximation of function sin(z) with z in x
  static void chebyshevSin(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  /// Chebyshev approximation of function tan(z) with z in x
  static void chebyshevTan(const Interval &x, Interval &alpha, Interval &dzeta,
                           Interval &delta);

  /// Chebyshev approximation of function cosh(z) with z in x
  static void chebyshevCosh(const Interval &x, Interval &alpha, Interval &dzeta,
                            Interval &delta);

  /// Chebyshev approximation of function sinh(z) with z in x
  static void chebyshevSinh(const Interval &x, Interval &alpha, Interval &dzeta,
                            Interval &delta);

  /// Chebyshev approximation of function tanh(z) with z in x
  static void chebyshevTanh(const Interval &x, Interval &alpha, Interval &dzeta,
                            Interval &delta);

private:
  // linear term
  struct Item {
    Interval itv; // coefficient
    int var;      // variable
  };
  Interval c_;        // constant term
  std::list<Item> l_; // list of linear terms
  Interval e_;        // error interval

  static bool MINRANGE; // true if the Minrange approximation is used
                        // false for Chebyshev

  // Inserts a linear term at the end of the list
  void pushBack(const Interval &x, int var);

  // Linearization of elementary function
  // return alpha*f + dzeta + alpha*[-1,1]
  static AffineForm linearize(const AffineForm &f, const Interval &alpha,
                              const Interval &dzeta, const Interval &delta);

  /// Power with e>2
  static AffineForm powrec(const AffineForm &f, int e);

public:
  typedef std::list<Item>::iterator iterator;

  /// Returns an iterator on the beginning of the list of linear terms
  iterator begin();

  /// Returns an iterator on the end of the list of linear terms
  iterator end();

  /// Returns the variable of the linear term pointed by it
  static int var(const iterator &it);

  /// Returns the coefficient of the linear term pointed by it
  static const Interval &itv(const iterator &it);

  typedef std::list<Item>::const_iterator const_iterator;

  /// Returns an iterator on the beginning of the list of linear terms
  const_iterator cbegin() const;

  /// Returns an iterator on the end of the list of linear terms
  const_iterator cend() const;

  /// Returns the variable of the linear term pointed by it
  static int var(const const_iterator &it);

  /// Returns the coefficient of the linear term pointed by it
  static const Interval &itv(const const_iterator &it);
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const AffineForm &f);

} // namespace realpaver

#endif
