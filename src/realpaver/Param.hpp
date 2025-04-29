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
 * @file   Param.hpp
 * @brief  Parameters of solvers
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_PARAM_HPP
#define REALPAVER_PARAM_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace realpaver {

/// Categories of parameters
enum class ParamCat {
   General,      ///< general parameters
   Branch,       ///< branch-and-contract algorithms
   Split,        ///< splitting strategies
   Contract,     ///< contractors and propagation
   Polytope,     ///< polytope hull contractor
   Acid,         ///< ACID contractor
   LocalOptim,   ///< local optimization
   LinearSolver, ///< linear solvers
   Interval,     ///< interval methods,
   NoCat         ///< special value (not a category)
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const ParamCat &cat);

/*----------------------------------------------------------------------------*/

/**
 * @brief Base class of parameters.
 *
 * A parameter has a name, a category, a description (what), and an additional text.
 */
class Param {
public:
   /// Default constructor
   Param();

   /// Virtual destructor
   virtual ~Param();

   /// Default copy constructor
   Param(const Param &) = default;

   /// No assignment
   Param &operator=(const Param &) = delete;

   /// Returns the name
   const std::string &name() const;

   /// Assigns the name and returns a reference to this
   Param &setName(const std::string &name);

   /// Returns the category
   const ParamCat &cat() const;

   /// Assigns the category and returns a reference to this
   Param &setCat(const ParamCat &cat);

   /// Returns the description
   const std::string &what() const;

   /// Assigns the description and returns a reference to this
   Param &setWhat(const std::string &what);

   /// Returns the text
   const std::string &text() const;

   /// Assigns the text and returns a reference to this
   Param &setText(const std::string &str);

   /// Clones this
   virtual Param *clone() const = 0;

   /// Prints this on a stream using the format 'name = value'
   virtual void printValue(std::ostream &os, int spaces) const = 0;

   /// Prints this on a stream using the format of a parameter file
   virtual void print(std::ostream &os, size_t ncols) const = 0;

protected:
   std::string name_; // name
   ParamCat cat_;     // category
   std::string what_; // description
   std::string text_; // additional text

   // Prints a string on a stream given a column limit
   static void printStr(std::ostream &os, const std::string &str, size_t ncols);

   // Splits a string along with the '\n' character
   static std::vector<std::string> splitStr(const std::string &str);

   /// Returns the length of the longest word in a string
   static size_t maxWordLength(const std::string &str);
};

/**
 * @brief Compares two parameters.
 *
 * Lexicographic ordering that considers first the category and second the name.
 */
struct ParamComparator {
   bool operator()(const Param *p, const Param *q) const
   {
      return (p->cat() < q->cat()) || (p->cat() == q->cat() && p->name() < q->name());
   }
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Parameter having a double value.
 *
 * Such a parameter is defined by a tuple (name, cat, what, min, val, max) where
 * name is a name, cat is a category, what is a description, min is a minimum value,
 * max is a maximum value, and val is the current value such that min <= val <= max.
 *
 * An exception is thrown in a call that leads to invalidate the condition min <= val <=
 * max.
 */
class ParamDbl : public Param {
public:
   /// Default constructor
   ParamDbl();

   /// Default destructor
   ~ParamDbl() = default;

   /// Default constructor
   ParamDbl(const ParamDbl &) = default;

   /// No assignment
   ParamDbl &operator=(const ParamDbl &) = delete;

   /// Assigns the value and returns a reference to this
   ParamDbl &setValue(const double &a);

   /// Assigns the minimum value and returns a reference to this
   ParamDbl &setMinValue(const double &a);

   /// Assigns the maximum value and returns a reference to this
   ParamDbl &setMaxValue(const double &a);

   /// Returns the value
   const double &value() const;

   /// Returns the minimum value
   const double &minValue() const;

   /// Returns the maximum value
   const double &maxValue() const;

   /// Assigns the name and returns a reference to this
   ParamDbl &setName(const std::string &name);

   /// Assigns the category and returns a reference to this
   ParamDbl &setCat(const ParamCat &cat);

   /// Assigns the description and returns a reference to this
   ParamDbl &setWhat(const std::string &what);

   /// Assigns the text and returns a reference to this
   ParamDbl &setText(const std::string &str);

   ParamDbl *clone() const override;
   void printValue(std::ostream &os, int spaces) const override;
   void print(std::ostream &os, size_t ncols) const override;

private:
   double val_; // current value
   double min_; // minimum value
   double max_; // maximum value

   // Checks the condition min <= val <= max
   void check() const;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Parameter having an int value.
 *
 * Such a parameter is defined by a tuple (name, cat, what, min, val, max) where
 * name is a name, cat is a category, what is a description, min is a minimum value,
 * max is a maximum value, and val is the current value such that min <= val <= max.
 *
 * An exception is thrown in a call that leads to invalidate the condition min <= val <=
 * max.
 */
class ParamInt : public Param {
public:
   /// Default constructor
   ParamInt();

   /// Default destructor
   ~ParamInt() = default;

   /// Default constructor
   ParamInt(const ParamInt &) = default;

   /// No assignment
   ParamInt &operator=(const ParamInt &) = delete;

   /// Assigns the value and returns a reference to this
   ParamInt &setValue(const int &a);

   /// Assigns the minimum value and returns a reference to this
   ParamInt &setMinValue(const int &a);

   /// Assigns the maximum value and returns a reference to this
   ParamInt &setMaxValue(const int &a);

   /// Returns the value
   const int &value() const;

   /// Returns the minimum value
   const int &minValue() const;

   /// Returns the maximum value
   const int &maxValue() const;

   /// Assigns the name and returns a reference to this
   ParamInt &setName(const std::string &name);

   /// Assigns the category and returns a reference to this
   ParamInt &setCat(const ParamCat &cat);

   /// Assigns the description and returns a reference to this
   ParamInt &setWhat(const std::string &what);

   /// Assigns the text and returns a reference to this
   ParamInt &setText(const std::string &str);

   ParamInt *clone() const override;
   void printValue(std::ostream &os, int spaces) const override;
   void print(std::ostream &os, size_t ncols) const override;

private:
   int val_; // current value
   int min_; // minimum value
   int max_; // maximum value

   // Checks the condition min <= val <= max
   void check() const;
};

/*----------------------------------------------------------------------------*/

class ParamStr : public Param {
public:
   /// Default constructor
   ParamStr();

   /// Default destructor
   ~ParamStr() = default;

   /// Default constructor
   ParamStr(const ParamStr &) = default;

   /// No assignment
   ParamStr &operator=(const ParamStr &) = delete;

   /// Assigns the value and returns a reference to this
   ParamStr &setValue(const std::string &str);

   /// Returns the value
   const std::string &value() const;

   /// Assigns the name and returns a reference to this
   ParamStr &setName(const std::string &name);

   /// Assigns the category and returns a reference to this
   ParamStr &setCat(const ParamCat &cat);

   /// Assigns the description and returns a reference to this
   ParamStr &setWhat(const std::string &what);

   /// Assigns the text and returns a reference to this
   ParamStr &setText(const std::string &str);

   /// Inserts a possible value and its description
   ParamStr &addChoice(const std::string &val, const std::string &what);

   ParamStr *clone() const override;
   void printValue(std::ostream &os, int spaces) const override;
   void print(std::ostream &os, size_t ncols) const override;

private:
   struct Choice {
      std::string val;  // a paremeter value
      std::string what; // its description
   };
   std::string val_;         // current value
   std::vector<Choice> cho_; // vector  of possible choices

   // Checks if the value belongs to the vector of choices
   void check() const;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Registers the set of parameters of an application.
 *
 * This class has a static instance storing the default values of the
 * parameters used to initialize the components of the library.
 */
class Params {
public:
   /// Constructor
   Params();

   /// Destructor
   ~Params();

   /// Copy constructor
   Params(const Params &other);

   /// Assignment operator
   Params &operator=(const Params &other);

   /// Loads settings from a file
   void loadParam(const std::string &filename);

   /// Returns the number of columns in a file of parameters
   size_t nbColumns() const;

   /// Assigns the number of columns in a file of parameters
   void setNbColumns(size_t col);

   /// Gets the value of a real parameter called 'name'
   double getDblParam(const std::string &name) const;

   /// Sets the value of a real parameter called 'name' to val
   void setDblParam(const std::string &name, const double &val);

   /// Gets the value of a string parameter called 'name'
   const std::string &getStrParam(const std::string &name) const;

   /// Sets the value of a string parameter called 'name' to val
   void setStrParam(const std::string &name, const std::string &val);

   /// Gets the value of an int parameter called 'name'
   int getIntParam(const std::string &name) const;

   /// Sets the value of an int parameter called 'name' to val
   void setIntParam(const std::string &name, const int &val);

   /// Adds a parameter in this
   void add(const Param &prm);

   /// Prints this on a stream using the format 'name = value'
   void printValues(std::ostream &os) const;

   /// Prints this on a stream using the format of a parameter file
   void print(std::ostream &os) const;

   /// Gets the value of a real parameter called 'name' in the static instance
   static double GetDblParam(const std::string &name);

   /// Gets the value of a string parameter called 'name' in the static instance
   static const std::string &GetStrParam(const std::string &name);

   /// Gets the value of an int parameter called 'name' in the static instance
   static int GetIntParam(const std::string &name);

   /// Prints the static instance on a stream using the format of a parameter file
   static void Print(std::ostream &os);

private:
   static Params instance_; // default instance

   std::unordered_map<std::string, Param *> map_; // map name -> parameter
   size_t ncols_;     // number of columns in a file of parameters
   std::string path_; // file path
   int lineno_;       // line number in a file

   /// Makes the parameters of the different categories
   void makeBranch();
   void makeGeneral();
   void makeSplit();
   void makeContract();
   void makePolytope();
   void makeAcid();
   void makeLocalOptim();
   void makeLinearSolver();
   void makeInterval();

   // Returns the size of the longest parameter name
   size_t maxLength() const;

   // Returns a sorted vector of parameters
   std::vector<Param *> sort() const;

   // Processing of lines in a file
   void processParam(const std::string &name, const std::string &val);
   void readParam(const std::string &line, size_t first);
   void processLine(const std::string &line);
   void throwParsing();

   // Utility functions
   static size_t eatSpaces(const std::string &line, size_t first);
   static bool isDigit(char c);
   static bool isSpace(char c);
   static bool isLetter(char c);
   static bool isCharName(char c);
   static bool isInt(const std::string &s);
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const Params &P);

} // namespace realpaver

#endif
