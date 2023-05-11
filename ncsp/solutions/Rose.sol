NCSP solver......................... Realpaver 1.0.0
Input file.......................... ../ncsp/benchmarks/Rose.rp
Current date and time............... Wed May 10 16:15:32 2023

--- PREPROCESSING ---

Elapsed time........................ 0.001 (seconds)
Status.............................. checked
Number of variables fixed........... 0
Number of inactive constraints...... 0

--- SOLVING ---

Elapsed time........................ 20.154 (seconds)
Number of nodes..................... 4111
Search status....................... complete
Solution status..................... proved feasible
Number of solutions................. 18

SOLUTION 1 [4.619e-14] [safe]
x = [-2.251071872, -2.251071871]
y = [-1.950642443, -1.950642442]
z = [-1.164669858, -1.164669857]

SOLUTION 2 [5.551e-15] [safe]
x = [-0.8922598497, -0.8922598496]
y = [1.228085671, 1.228085672]
z = [-1.144486598, -1.144486597]

SOLUTION 3 [6.939e-16] [safe]
x = [-0.03009698734, -0.03009698733]
y = [0.2255508674, 0.2255508675]
z = [-1.108158096, -1.108158095]

SOLUTION 4 [2.609e-15] [safe]
x = [-0.1186441676, -0.1186441675]
y = [0.447822784, 0.4478227841]
z = [-1.065659129, -1.065659128]

SOLUTION 5 [7.717e-08] [unsafe]
x = [-0.3500000213, -0.349999955]
y = [-0.7691605931, -0.7691605158]
z = [-7.088695831e-09, 1.420110556e-08]

SOLUTION 6 [7.717e-08] [unsafe]
x = [-0.3500000213, -0.349999955]
y = [0.7691605158, 0.7691605931]
z = [-1.420110556e-08, 7.088695831e-09]

SOLUTION 7 [4.619e-14] [safe]
x = [-2.251071872, -2.251071871]
y = [1.950642442, 1.950642443]
z = [1.164669857, 1.164669858]

SOLUTION 8 [5.551e-15] [safe]
x = [-0.8922598497, -0.8922598496]
y = [-1.228085672, -1.228085671]
z = [1.144486597, 1.144486598]

SOLUTION 9 [2.609e-15] [safe]
x = [-0.1186441676, -0.1186441675]
y = [-0.4478227841, -0.447822784]
z = [1.065659128, 1.065659129]

SOLUTION 10 [6.939e-16] [safe]
x = [-0.03009698734, -0.03009698733]
y = [-0.2255508675, -0.2255508674]
z = [1.108158095, 1.108158096]

SOLUTION 11 [7.216e-16] [safe]
x = [0.03966867746, 0.03966867747]
y = [0.2589445946, 0.2589445947]
z = [-1.125592706, -1.125592705]

SOLUTION 12 [1.332e-15] [safe]
x = [0.7093400892, 0.7093400893]
y = [-1.094990224, -1.094990223]
z = [-1.160558927, -1.160558926]

SOLUTION 13 [2.22e-15] [safe]
x = [0.6161682103, 0.6161682104]
y = [1.020546113, 1.020546114]
z = [-1.159367209, -1.159367208]

SOLUTION 14 [7.216e-16] [safe]
x = [0.03966867746, 0.03966867747]
y = [-0.2589445947, -0.2589445946]
z = [1.125592705, 1.125592706]

SOLUTION 15 [2.22e-15] [safe]
x = [0.6161682103, 0.6161682104]
y = [-1.020546114, -1.020546113]
z = [1.159367208, 1.159367209]

SOLUTION 16 [1.443e-15] [safe]
x = [0.7093400892, 0.7093400893]
y = [1.094990223, 1.094990224]
z = [1.160558926, 1.160558927]

SOLUTION 17 [2.576e-14] [safe]
x = [4.476091674, 4.476091675]
y = [-2.7506319, -2.750631899]
z = [1.166253188, 1.166253189]

SOLUTION 18 [2.576e-14] [safe]
x = [4.476091674, 4.476091675]
y = [2.750631899, 2.7506319]
z = [-1.166253189, -1.166253188]

--- INPUT PROBLEM ---

REGION
x = [-1000, 1000]
y = [-1000, 1000]
z = [-1000, 1000]

CONSTRAINTS
pow(y,4) - 2.857142857*x^2 == 0
x^2*pow(z,4) + ([0.6999999999, 0.7000000001]*x)*pow(z,4) + 0.1458333333*pow(z,4) - 1.851851852*x^2 - ([1.296296296, 1.296296297]*x) - 0.2268518519 == 0
0.6*pow(x,6)*y^2*z + pow(x,5)*pow(y,3) + 0.4285714286*pow(x,5)*y^2*z + 1.4*pow(x,4)*pow(y,3) - 0.35*pow(x,4)*y*z^2 - 0.15*pow(x,4)*pow(z,3) + 0.609*pow(x,3)*pow(y,3) + 0.315*pow(x,3)*y^2*z - 0.616*pow(x,3)*y*z^2 - 0.42*pow(x,3)*pow(z,3) + 0.0392*x^2*pow(y,3) + 0.0735*x^2*y^2*z - 0.3977166667*x^2*y*z^2 - 0.2275*x^2*pow(z,3) - ([0.03423874999, 0.03423875001]*x)*pow(y,3) + ([0.005171249999, 0.005171250001]*x)*y^2*z - ([0.1149866666, 0.1149866667]*x)*y*z^2 - ([0.02943499999, 0.02943500001]*x)*pow(z,3) - 0.00643125*pow(y,3) - ([0.01268385416, 0.01268385417]*y)*z^2 - 0.0026796875*pow(z,3) == 0

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
SPLIT_SELECTOR            = MAX_SMEAR
SPLIT_SLICER              = BISECTION
TIME_LIMIT                = 3600
TRACE                     = YES
UNI_NEWTON_ITER_LIMIT     = 20
XTOL                      = 1e-08A