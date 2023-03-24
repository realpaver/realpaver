NCSP solver......................... Realpaver 1.0
Input file.......................... unsolved/Broyden10.rp
Current date and time............... Fri Mar 24 15:20:22 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.016 (seconds)
Number of nodes..................... 1
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 1

SOLUTION 1 [3.331e-16] [safe]
x1  = [-0.4774293257, -0.4774293256]
x2  = [-0.4774463174, -0.4774463173]
x3  = [-0.5204104721, -0.520410472]
x4  = [-0.5587772325, -0.5587772324]
x5  = [-0.5931138675, -0.5931138674]
x6  = [-0.6250711391, -0.625071139]
x7  = [-0.6231993195, -0.6231993194]
x8  = [-0.6213496144, -0.6213496143]
x9  = [-0.620414028, -0.6204140279]
x10 = [-0.5864379442, -0.5864379441]

--- INPUT PROBLEM ---

REGION
x1  = [-1000, 1000]
x2  = [-1000, 1000]
x3  = [-1000, 1000]
x4  = [-1000, 1000]
x5  = [-1000, 1000]
x6  = [-1000, 1000]
x7  = [-1000, 1000]
x8  = [-1000, 1000]
x9  = [-1000, 1000]
x10 = [-1000, 1000]

CONSTRAINTS
x1*(2 + 5*x1^2) + 1 - (x1*(x1+1) + x2*(x2+1)) == 0
x2*(2 + 5*x2^2) + 1 - (x1*(x1+1) + x3*(x3+1)) == 0
x3*(2 + 5*x3^2) + 1 - (x1*(x1+1) + x2*(x2+1) + x4*(x4+1)) == 0
x4*(2 + 5*x4^2) + 1 - (x1*(x1+1) + x2*(x2+1) + x3*(x3+1) + x5*(x5+1)) == 0
x5*(2 + 5*x5^2) + 1 - (x1*(x1+1) + x2*(x2+1) + x3*(x3+1) + x4*(x4+1) + x6*(x6+1)) == 0
x6*(2 + 5*x6^2) + 1 - (x1*(x1+1) + x2*(x2+1) + x3*(x3+1) + x4*(x4+1) + x5*(x5+1) + x7*(x7+1)) == 0
x7*(2 + 5*x7^2) + 1 - (x2*(x2+1) + x3*(x3+1) + x4*(x4+1) + x5*(x5+1) + x6*(x6+1) + x8*(x8+1)) == 0
x8*(2 + 5*x8^2) + 1 - (x3*(x3+1) + x4*(x4+1) + x5*(x5+1) + x6*(x6+1) + x7*(x7+1) + x9*(x9+1)) == 0
x9*(2 + 5*x9^2) + 1 - (x4*(x4+1) + x5*(x5+1) + x6*(x6+1) + x7*(x7+1) + x8*(x8+1) + x10*(x10+1)) == 0
x10*(2 + 5*x10^2) + 1 - (x5*(x5+1) + x6*(x6+1) + x7*(x7+1) + x8*(x8+1) + x9*(x9+1)) == 0

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