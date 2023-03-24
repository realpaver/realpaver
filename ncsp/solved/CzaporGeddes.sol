NCSP solver......................... Realpaver 1.0
Input file.......................... unsolved/CzaporGeddes.rp
Current date and time............... Fri Mar 24 15:29:25 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.147 (seconds)
Number of nodes..................... 37
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 2

SOLUTION 1 [8.882e-16] [safe]
x = [-0.07109159441, -0.0710915944]
y = [-0.5787235213, -0.5787235212]
z = [1.142958465, 1.142958466]

SOLUTION 2 [1.249e-15] [safe]
x = [-0.6988995834, -0.6988995833]
y = [0.2326846488, 0.2326846489]
z = [0.1654295542, 0.1654295543]

--- INPUT PROBLEM ---

REGION
x = [-1000, 1000]
y = [-1000, 1000]
z = [-1000, 1000]

CONSTRAINTS
8*x^2 - (2*x)*y - (6*x)*z + 3*x + 3*y^2 - (7*y)*z + 10*y + 10*z^2 - (8*z) == 4
10*x^2 - (2*x)*y + (6*x)*z - (6*x) + 9*y^2 - y*z - (4*y) - 2*z^2 + 5*z == 9
5*x^2 + (8*x)*z + (4*x)*z + 8*x + 9*y^2 - (6*y)*z + 2*y - z^2 - (7*z) + 5 == 0

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
PROPAGATION_BASE          = BC4
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