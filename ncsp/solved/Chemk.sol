NCSP solver......................... Realpaver 1.0
Input file.......................... unsolved/Chemk.rp
Current date and time............... Fri Mar 24 15:22:54 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.002 (seconds)
Number of nodes..................... 1
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 1

SOLUTION 1 [7.216e-16] [safe]
x1 = [0.0001587987897, 0.0001587987898]
x2 = [2.521705562e-08, 2.521705563e-08]
x3 = [0.1478617598, 0.1478617599]
x4 = [0.3845279701, 0.3845279702]

--- INPUT PROBLEM ---

REGION
x1 = [0, 1]
x2 = [0, 1]
x3 = [0, 1]
x4 = [0, 1]

CONSTRAINTS
x1^2 - x2 == 0
x4^2 - x3 == 0
21770000*x2 - (16970000*x2)*x4 + ([0.5499999999, 0.5500000001]*x1)*x4 + [0.4499999999, 0.4500000001]*x1 == x4
(<1.585e+14, 1.585e+14>*x2)*x4 + (41260000*x1)*x3 - (8282000*x1)*x4 + (22840000*x3)*x4 - (19180000*x3) + [48.39999999, 48.40000001]*x4 == 27.73

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