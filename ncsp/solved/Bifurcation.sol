NCSP solver......................... Realpaver 1.0
Input file.......................... unsolved/Bifurcation.rp
Current date and time............... Fri Mar 24 15:15:47 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.008 (seconds)
Number of nodes..................... 55
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 12

SOLUTION 1 [2.776e-16] [safe]
X = [-0.2798546923, -0.2798546922]
Y = [-0.4327890378, -0.4327890377]
Z = [-0.01418918857, -0.01418918856]

SOLUTION 2 [2.22e-16] [safe]
X = [-0.5153882033, -0.5153882032]
Y = [-3.206571051e-23, 3.206571051e-23]
Z = [-0.01244559885, -0.01244559884]

SOLUTION 3 [2.776e-16] [safe]
X = [-0.4669800112, -0.4669800111]
Y = [-0.2180703309, -0.2180703308]
Z = [-8.342877621e-18, 8.572761758e-18]

SOLUTION 4 [2.22e-16] [safe]
X = [-3.61087124e-23, 3.61087124e-23]
Y = [-0.5153882033, -0.5153882032]
Z = [-1.04491598e-28, 1.04491598e-28]

SOLUTION 5 [2.776e-16] [safe]
X = [-0.2798546923, -0.2798546922]
Y = [0.4327890377, 0.4327890378]
Z = [-0.01418918857, -0.01418918856]

SOLUTION 6 [2.776e-16] [safe]
X = [-0.4669800112, -0.4669800111]
Y = [0.2180703308, 0.2180703309]
Z = [-8.342877621e-18, 8.572761758e-18]

SOLUTION 7 [2.22e-16] [safe]
X = [-3.61087124e-23, 3.61087124e-23]
Y = [0.5153882032, 0.5153882033]
Z = [-1.04491598e-28, 1.04491598e-28]

SOLUTION 8 [2.776e-16] [safe]
X = [0.2798546922, 0.2798546923]
Y = [-0.4327890378, -0.4327890377]
Z = [-0.01418918857, -0.01418918856]

SOLUTION 9 [2.22e-16] [safe]
X = [0.5153882032, 0.5153882033]
Y = [-3.206571051e-23, 3.206571051e-23]
Z = [-0.01244559885, -0.01244559884]

SOLUTION 10 [2.776e-16] [safe]
X = [0.4669800111, 0.4669800112]
Y = [-0.2180703309, -0.2180703308]
Z = [-8.342877621e-18, 8.572761758e-18]

SOLUTION 11 [2.776e-16] [safe]
X = [0.2798546922, 0.2798546923]
Y = [0.4327890377, 0.4327890378]
Z = [-0.01418918857, -0.01418918856]

SOLUTION 12 [2.776e-16] [safe]
X = [0.4669800111, 0.4669800112]
Y = [0.2180703308, 0.2180703309]
Z = [-8.342877621e-18, 8.572761758e-18]

--- INPUT PROBLEM ---

REGION
X = [-1000, 1000]
Y = [-1000, 1000]
Z = [-1000, 1000]

CONSTRAINTS
5*pow(X,9) - 6*pow(X,5)*Y^2 + X*pow(Y,4) + (2*X)*Z == 0
-2*pow(X,6)*Y + 2*X^2*pow(Y,3) + (2*Y)*Z == 0
X^2 + Y^2 == 0.265625

--- PARAMETERS ---

BB_SPACE_FREQUENCY        = 1
BC3_ITER_LIMIT            = 30
BC3_PEEL_FACTOR           = 2
BP_NODE_SELECTION         = DFS
DEPTH_LIMIT               = 10000
DISPLAY_REGION            = STD
DTOL                      = 1e-06R
FLOAT_PRECISION           = 10
GAUSSIAN_MIN_PIVOT        = 1e-08
GAUSS_SEIDEL_DTOL         = 1e-06R
GAUSS_SEIDEL_ITER_LIMIT   = 50
GAUSS_SEIDEL_XTOL         = 1e-08A
GRADIENT_DESCENT_TOL      = 0.001
INFLATION_CHI             = 1e-12
INFLATION_DELTA           = 1.125
LINE_SEARCH_ARMIJO        = 0.09375
LINE_SEARCH_ITER_LIMIT    = 20
LINE_SEARCH_STEP_TOL      = 1e-08
LOCAL_SOLVER_ALGORITHM    = GRADIENT
LOCAL_SOLVER_TIME_LIMIT   = 10
LOG_LEVEL                 = INTER
LP_ITER_LIMIT             = 50
LP_TIME_LIMIT             = 10
NEWTON_CERTIFY_DTOL       = 0.001R
NEWTON_CERTIFY_ITER_LIMIT = 20
NEWTON_DTOL               = 1e-06R
NEWTON_ITER_LIMIT         = 30
NEWTON_XTOL               = 1e-08A
NODE_LIMIT                = 500000
OBJ_TOL                   = 1e-06R
PREPROCESSING             = YES
PROPAGATION_BASE          = HC4
PROPAGATION_DTOL          = 1e-10R
PROPAGATION_ITER_LIMIT    = 200
PROPAGATION_WITH_MAX_CID  = YES
PROPAGATION_WITH_NEWTON   = YES
PROPAGATION_WITH_POLYTOPE = NO
RELAXATION_EQ_TOL         = 1e-08
SOLUTION_CLUSTER_GAP      = 1e-06
SOLUTION_LIMIT            = 5000
SPLIT_DOM_ROBIN           = 2
SPLIT_INNER               = NO
SPLIT_NB_SLICES           = 7
SPLIT_OBJECTIVE           = NO
SPLIT_PEEL_FACTOR         = 10
SPLIT_SELECTOR            = HYBRID_DOM_ROBIN
SPLIT_SLICER              = BISECTION
TIME_LIMIT                = 3600
TRACE                     = YES
UNI_NEWTON_ITER_LIMIT     = 20
XTOL                      = 1e-08A