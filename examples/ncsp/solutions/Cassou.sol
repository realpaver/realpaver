NCSP solver......................... Realpaver 1.0.0
Input file.......................... benchmarks/Cassou.rp
Current date and time............... Wed May 10 15:43:30 2023

--- PREPROCESSING ---

Elapsed time........................ 0.000 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 111.328 (seconds)
Number of nodes..................... 8275
Search status....................... complete
Solution status..................... proved unfeasible
Number of solutions................. 0

--- INPUT PROBLEM ---

REGION
b = [-10, 10]
c = [-10, 10]
d = [-10, 10]
e = [-10, 10]

CONSTRAINTS
pow(b,4)*((15*c)*d^2 + 6*pow(c,3) + 21*c^2*d + 9*pow(d,3)) + b^2*(-144*c - 8*c^2*e - (28*c)*d*e - (648*d) + 36*d^2*e) - 120 == 0
pow(b,4)*(30*pow(c,3)*d + 9*pow(c,4) + 39*d^2*c^2 + 18*pow(d,3)*c) + b^2*((-720*d)*c - 24*pow(c,3)*e - 432*c^2 + (16*c)*d^2*e - 432*d^2 + 24*pow(d,3)*e - 16*c^2*d*e) - (32*d)*e^2*c + (576*e)*c - (576*d)*e + 16*d^2*e^2 + 16*e^2*c^2 + 5.18e+03 - (240*c) == 0
b^2*((216*d)*c - 162*d^2 - 81*c^2 + 15*c^2*d*e - 15*pow(c,3)*e) + 5.18e+03 + (1008*e)*c - (1008*d)*e - e^2*((80*d)*c + 40*d^2 + 40*c^2) == 0
261 + b^2*((4*d)*c - 3*d^2 - 4*c^2) + e*(22*c-22*d) == 0

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