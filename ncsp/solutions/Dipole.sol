NCSP solver......................... Realpaver 1.0.0
Input file.......................... ../ncsp/benchmarks/Dipole.rp
Current date and time............... Wed May 10 14:17:56 2023

--- PREPROCESSING ---

Elapsed time........................ 0.001 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 0.024 (seconds)
Number of nodes..................... 3
Search status....................... complete
Solution status..................... proved unfeasible
Number of solutions................. 0

--- INPUT PROBLEM ---

REGION
a = [-10, 10]
b = [-10, 10]
c = [-10, 10]
d = [-10, 10]
t = [-10, 10]
u = [-10, 10]
v = [-10, 10]
w = [-10, 10]

CONSTRAINTS
a+b == 0.633
c+d == -1.35
t*a + u*b - v*c - w*d == -0.837
v*a + w*b + t*c + u*d == 1.73
a*t^2 - a*v^2 - (2*c)*t*v + b*u^2 - b*w^2 - (2*d)*u*w == 1.35
c*t^2 - c*v^2 + (2*a)*t*v + d*u^2 - d*w^2 + (2*b)*u*w == -0.843
a*pow(t,3) - (3*a)*t*v^2 + c*pow(v,3) - (3*c)*v*t^2 + b*pow(u,3) - (3*b)*u*w^2 + d*pow(w,3) - (3*d)*w*u^2 == -0.956
c*pow(t,3) - (3*c)*t*v^2 - a*pow(v,3) + (3*a)*v*t^2 + d*pow(u,3) - (3*d)*u*w^2 - b*pow(w,3) + (3*b)*w*u^2 == 1.23

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