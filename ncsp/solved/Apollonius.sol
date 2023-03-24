NCSP solver......................... Realpaver 1.0
Input file.......................... unsolved/Apollonius.rp
Current date and time............... Fri Mar 24 15:12:02 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 6
x1 = 1
x2 = <0.5000000000, 0.5000000000>
x3 = 1
x4 = <0.5000000000, 0.5000000000>
x5 = [0.3999999999, 0.4000000001]
x6 = [0.7999999999, 0.8000000001]
Number of inactive constraints...... 6
2*x1 == 2.0000000000
2*x2 == 1.0000000000
2*x3 == 2.0000000000
2*x4 == 1.0000000000
2*x5 == x6
x5+2*x6 == 2.0000000000

--- SOLVING ---

Elapsed time........................ 0.833 (seconds)
Number of nodes..................... 3447
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 1

SOLUTION 1 [1.665e-16] [safe]
x7 = [0.4999999999, 0.5000000001]
x8 = [0.2499999999, 0.2500000001]

--- INPUT PROBLEM ---

REGION
x1 = [-100, 100]
x2 = [-100, 100]
x3 = [-100, 100]
x4 = [-100, 100]
x5 = [-100, 100]
x6 = [-100, 100]
x7 = [-100, 100]
x8 = [-100, 100]

CONSTRAINTS
2*x1 == 2
2*x2 == 1
2*x3 == 2
2*x4 == 1
2*x5 == x6
x5+2*x6 == 2
(x1-x7)^2 + x8^2 - x7^2 == (-x2+x8)^2
(x1-x7)^2 + x8^2 == (x3-x7)^2 + (x4-x8)^2

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