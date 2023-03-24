NCSP solver......................... Realpaver 1.0
Input file.......................... unsolved/BroydenTridiagonal10.rp
Current date and time............... Fri Mar 24 15:17:44 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.004 (seconds)
Number of nodes..................... 7
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 2

SOLUTION 1 [9.992e-16] [safe]
x1  = [-0.5707221321, -0.570722132]
x2  = [-0.68180695, -0.6818069499]
x3  = [-0.7022100761, -0.702210076]
x4  = [-0.7055106299, -0.7055106298]
x5  = [-0.7049061558, -0.7049061557]
x6  = [-0.7014966071, -0.701496607]
x7  = [-0.6918893224, -0.6918893223]
x8  = [-0.6657965145, -0.6657965144]
x9  = [-0.5960351091, -0.596035109]
x10 = [-0.4164122576, -0.4164122575]

SOLUTION 2 [7.772e-16] [safe]
x1  = [1.832600401, 1.832600402]
x2  = [-0.1095236289, -0.1095236288]
x3  = [-0.5925810692, -0.5925810691]
x4  = [-0.6852621128, -0.6852621127]
x5  = [-0.7011867978, -0.7011867977]
x6  = [-0.7008120655, -0.7008120654]
x7  = [-0.6917622505, -0.6917622504]
x8  = [-0.6657723542, -0.6657723541]
x9  = [-0.5960302336, -0.5960302335]
x10 = [-0.4164112126, -0.4164112125]

--- INPUT PROBLEM ---

REGION
x1  = [-10, 10]
x2  = [-10, 10]
x3  = [-10, 10]
x4  = [-10, 10]
x5  = [-10, 10]
x6  = [-10, 10]
x7  = [-10, 10]
x8  = [-10, 10]
x9  = [-10, 10]
x10 = [-10, 10]

CONSTRAINTS
-2*((x1-<0.75, 0.75>)^2 - 0.5625) - (2*x2) + 1 == 0
-2*((x2-<0.75, 0.75>)^2 - 0.5625) - x1 - (2*x3) + 1 == 0
-2*((x3-<0.75, 0.75>)^2 - 0.5625) - x2 - (2*x4) + 1 == 0
-2*((x4-<0.75, 0.75>)^2 - 0.5625) - x3 - (2*x5) + 1 == 0
-2*((x5-<0.75, 0.75>)^2 - 0.5625) - x4 - (2*x6) + 1 == 0
-2*((x6-<0.75, 0.75>)^2 - 0.5625) - x5 - (2*x7) + 1 == 0
-2*((x7-<0.75, 0.75>)^2 - 0.5625) - x6 - (2*x8) + 1 == 0
-2*((x8-<0.75, 0.75>)^2 - 0.5625) - x7 - (2*x9) + 1 == 0
-2*((x9-<0.75, 0.75>)^2 - 0.5625) - x8 - (2*x10) + 1 == 0
-2*((x10-<0.75, 0.75>)^2 - 0.5625) - x9 + 1 == 0

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