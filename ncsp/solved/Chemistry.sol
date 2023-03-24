NCSP solver......................... Realpaver 1.0
Input file.......................... unsolved/Chemistry.rp
Current date and time............... Fri Mar 24 15:22:20 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.355 (seconds)
Number of nodes..................... 91
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 1

SOLUTION 1 [1.975e-12] [safe]
x1 = [0.003114102265, 0.003114102266]
x2 = [34.59792453, 34.59792454]
x3 = [0.06504177869, 0.0650417787]
x4 = [0.8593780505, 0.8593780506]
x5 = [0.03695185914, 0.03695185915]

--- INPUT PROBLEM ---

REGION
x1 = [0, 1000]
x2 = [0, 1000]
x3 = [0, 1000]
x4 = [0, 1000]
x5 = [0, 1000]

CONSTRAINTS
3*x5 == x1*(x2+1)
x3*(x2*(2*x3+[0.0005451766686, 0.0005451766687]) + [0.3859999999, 0.3860000001]*x3 + 0.0004106217542) == 8*x5
x4*([3.407354178e-05, 3.407354179e-05]*x2+2*x4) == 40*x5
x2*(2*x1 + x3*(x3+[0.0005451766686, 0.0005451766687]) + 4.4975e-07 + [1.922999999e-06, 1.923000001e-06]*x2 + [3.407354178e-05, 3.407354179e-05]*x4) + x1 == 10*x5
x2*(x1+[9.614999999e-07, 9.615000001e-07]*x2 + x3*(x3+[0.0005451766686, 0.0005451766687]) + 4.4975e-07 + [3.407354178e-05, 3.407354179e-05]*x4) + x1 + x3*([0.1929999999, 0.1930000001]*x3+[0.0004106217541, 0.0004106217542]) + x4^2 == 1

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