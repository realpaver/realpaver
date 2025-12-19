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
 * @file   realpaver_api.hpp
 * @brief  Imports the main classes for solving nonlinear problems.
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Timer.hpp"

#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintCond.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/ConstraintTable.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/LPSolver.hpp"
#include "realpaver/Optimizer.hpp"
#include "realpaver/Solver.hpp"
