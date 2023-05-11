NCSP solver......................... Realpaver 1.0.0
Input file.......................... ../ncsp/benchmarks/Fourbar.rp
Current date and time............... Wed May 10 14:18:06 2023

--- PREPROCESSING ---

Elapsed time........................ 0.001 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.021 (seconds)
Number of nodes..................... 3
Search status....................... complete
Solution status..................... proved unfeasible
Number of solutions................. 0

--- INPUT PROBLEM ---

REGION
X1 = [-0.0101, 100]
X2 = [-0.0101, 100]
Y1 = [-0.0101, 100]
Y2 = [-0.0101, 100]

CONSTRAINTS
0.0169*X1^2*Y1^2 - 0.889*X1^2*Y2^2 + 0.0412*X2^2*Y1^2 - 0.00437*X2^2*Y2^2 + 0.331*X1*X2*Y1^2 - 1.38*X1*X2*Y2^2 - 0.27*X1^2*Y1*Y2 + 1.44*X2^2*Y1*Y2 + 0.86*X1*X2*Y1*Y2 + 0.0791*X1^2*Y1 - 0.00336*X1^2*Y2 - 0.0621*X1*Y1^2 + 0.502*X1*Y2^2 + 0.647*X2^2*Y1 + 0.0926*X2^2*Y2 - 0.255*X2*Y1^2 - 0.0897*X2*Y2^2 - 0.568*X1*X2*Y2 + 0.096*X1*X2*Y1 + 0.165*X1*Y1*Y2 - 0.564*X2*Y1*Y2 - 0.0785*X1*Y1 - 0.0785*X2*Y2 + 0.0118*X1*Y2 - 0.0118*X2*Y1 + 0.0423*X1^2 + 0.0423*X2^2 + 0.0372*Y1^2 + 0.0372*Y2^2 == 0
0.518*X1^2*Y1^2 - 0.0414*X1^2*Y2^2 + 2.64*X2^2*Y1^2 - 0.799*X2^2*Y2^2 + 0.294*X1^2*Y1*Y2 + 1.47*X2^2*Y1*Y2 - 0.632*X1*X2*Y1^2 - 1.8*X1*X2*Y2^2 - 2.88*X1*X2*Y1*Y2 - 0.988*X1^2*Y1 - 0.531*X1^2*Y2 - 0.0398*X1*Y1^2 + 0.318*X1*Y2^2 - 1.94*X2^2*Y1 + 0.00128*X2^2*Y2 - 0.581*X2*Y1^2 - 0.0672*X2*Y2^2 + 0.532*X1*X2*Y1 + 0.949*X1*X2*Y2 + 0.514*X1*Y1*Y2 - 0.357*X2*Y1*Y2 + 0.141*X1*Y1 + 0.141*X2*Y2 - 0.153*X1*Y2 + 0.153*X2*Y1 + 0.283*X1^2 + 0.283*X2^2 + 0.0383*Y1^2 + 0.0383*Y2^2 == 0
0.0234*X1^2*Y1^2 - 0.00428*X1^2*Y1*Y2 - 0.793*X1^2*Y2^2 + 0.0492*X2^2*Y2^2 + 0.0759*X1*X2*Y1^2 + 1.15*X1*X2*Y1*Y2 - 0.283*X2^2*Y1^2 + 0.46*X2^2*Y1*Y2 - 0.388*X1*X2*Y2^2 - 0.0561*X1*Y1^2 + 0.485*X1*Y2^2 + 0.069*X1^2*Y1 - 0.116*X1^2*Y2 - 0.133*X2*Y1^2 - 0.0844*X2*Y2^2 + 0.64*X2^2*Y1 + 0.101*X2^2*Y2 + 0.217*X1*X2*Y1 - 0.571*X1*X2*Y2 + 0.0485*X1*Y1*Y2 - 0.541*X2*Y1*Y2 - 0.00363*X1*Y2 + 0.00363*X2*Y1 - 0.0781*X1*Y1 - 0.0781*X2*Y2 + 0.0471*X1^2 + 0.0471*X2^2 + 0.0324*Y1^2 + 0.0324*Y2^2 == 0
0.394*X1^2*Y1^2 + 0.598*X1^2*Y2^2 + 0.0736*X2^2*Y1^2 + 0.0549*X2^2*Y2^2 + 0.0116*X1^2*Y1*Y2 + 0.07*X2^2*Y1*Y2 - 0.306*X1*X2*Y1^2 - 0.364*X1*X2*Y2^2 - 0.223*X1*X2*Y1*Y2 + 0.0997*X1^2*Y1 + 0.0114*X1^2*Y2 - 0.381*X1*Y1^2 - 0.473*X1*Y2^2 - 0.0214*X2^2*Y1 - 0.0373*X2^2*Y2 + 0.149*X2*Y1^2 + 0.142*X2*Y2^2 - 0.0487*X1*X2*Y1 + 0.121*X1*X2*Y2 - 0.0065*X1*Y1*Y2 + 0.0922*X2*Y1*Y2 - 0.0483*X1*Y1 - 0.0483*X2*Y2 - 0.00317*X1*Y2 + 0.00317*X2*Y1 + 0.00635*X1^2 + 0.00635*X2^2 + 0.0923*Y1^2 + 0.0923*Y2^2 == 0

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
PROPAGATION_WITH_POLYTOPE = TAYLOR
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