NCSP solver......................... Realpaver 1.0
Input file.......................... unsolved/Bronstein.rp
Current date and time............... Fri Mar 24 15:16:33 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.001 (seconds)
Number of nodes..................... 7
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 4

SOLUTION 1 [1.776e-15] [safe]
x = [-4.838829991, -4.83882999]
y = [1.756622988, 1.756622989]
z = [-3.082207002, -3.082207001]

SOLUTION 2 [2.665e-15] [safe]
x = [-1.756622989, -1.756622988]
y = [4.83882999, 4.838829991]
z = [3.082207001, 3.082207002]

SOLUTION 3 [2.665e-15] [safe]
x = [1.756622988, 1.756622989]
y = [-4.838829991, -4.83882999]
z = [-3.082207002, -3.082207001]

SOLUTION 4 [1.776e-15] [safe]
x = [4.83882999, 4.838829991]
y = [-1.756622989, -1.756622988]
z = [3.082207001, 3.082207002]

--- INPUT PROBLEM ---

REGION
x = [-100000000, 100000000]
y = [-100000000, 100000000]
z = [-100000000, 100000000]

CONSTRAINTS
x^2 + y^2 + z^2 == 36
x+y == z
x*y + z^2 == 1

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