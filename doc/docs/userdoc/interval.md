## Introduction to interval computations
What is an interval computation? Let $C=d\times\pi$ be the circumference of a circle of diameter $d$. Given $d=2$ it comes $$C = 2\times\pi\approx 6.2831853$$
using floating-point arithmetic. Now let $[\pi]=[3.1415926,3.1415927]$ be an <span style="color:cyan">_interval enclosure_</span> of $\pi$, _i.e._ a tight interval that contains $\pi$. An interval enclosure of $C$ is derived by an interval multiplication $$[C] = 2\times [\pi] = [6.2831852,6.2831854].$$ As shown above, intervals can be propagated through numerical algorithms to find interval enclosures of solutions to continuous problems.

### Interval arithmetic
What is an interval operation? The resistance of a series circuit with two resistors $R_1$ and $R_2$ is equal to $R = R_1+R_2$. Suppose that the resistors are not exactly known, _e.g._ $R_1=3\Omega$ and $R_2=5\Omega$ plus or minus $1\%$. An interval enclosure
$$R\in [2.97,3.03]+[4.95,5.05] = [7.92,8.08]\Omega$$
follows from an interval addition of the domains of $R_1$ and $R_2$. In this way, interval arithmetic extends real arithmetic to intervals so as to calculate the tightest possible interval enclosures.

An interval enclosure of the range of a real function $f(x)$ over a domain can be obtained by mapping real numbers and operations to their interval counterparts. For example, given $f(x) = 0.5+x+x^2$ we have $$\{f(x):x\in[1,2]\}\subseteq 0.5+[1,2]+[1,2]^2 = [2.5, 6.5].$$
In this case, the computed range is exact but, in general, it may be pessimistic. Various techniques, not detailed here, have been developed to counteract this so-called _fundamental problem of interval arithmetic_.

### Constraint satisfaction
