NCSP solver......................... Realpaver 1.0.0
Input file.......................... ../ncsp/benchmarks/Kapur.rp
Current date and time............... Fri Apr 21 09:12:19 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.920 (seconds)
Number of nodes..................... 145
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 1

SOLUTION 1 [5.773e-15] [safe]
x = [1.355201792, 1.355201793]
y = [3.215637659, 3.21563766]
z = [2.547051192, 2.547051193]
w = [0.1404910597, 0.1404910598]
t = [2.966109606, 2.966109607]

--- INPUT PROBLEM ---

REGION
x = [0, 1000]
y = [0, 1000]
z = [0, 1000]
w = [0, 1000]
t = [0, 1000]

CONSTRAINTS
y*z + z*w + w*y == 9
z*x + x*w + w*z == 4
w*x + x*y + y*w == 5
x*y + y*z + z*x == 16
(2*x)*y*z + (2*y)*z*w + (2*z)*w*x + (2*w)*x*y == 9*t

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
PROPAGATION_WITH_POLYTOPE = RLT
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