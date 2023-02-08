###############################################################################
## This file is part of Realpaver, an interval constraint and NLP solver.    ##
##                                                                           ##
## Copyright (c) 2017-2022 LS2N, Nantes                                      ##
##                                                                           ##
## Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   ##
## COPYING for information.                                                  ##
###############################################################################

AUTOMAKE_OPTIONS = foreign

LDADD = ../../src/realpaver/.libs/librealpaver.la @INTERVAL_LIB@ @LP_LIB@

AM_CPPFLAGS = -I.. -I../../src

CLEANFILES = *~ *.log

noinst_PROGRAMS = ncsp_solver

ncsp_solver_SOURCES = ncsp_solver.cpp

EXTRA_DIST = \
	ParamFile \
	Apollonius.rpv \
	Bellido.rpv \
	Bifurcation.rpv \
	Bronstein.rpv \
	Brown5.rpv \
	Broyden10.rpv \
	BurdenFaires.rpv \
	Butcher.rpv \
	Caprasse.rpv \
	Cassou.rpv \
	Chemistry.rpv \
	Chemk.rpv \
	Combustion.rpv \
	Cpdm5.rpv \
	Cyclo.rpv \
	CzaporGeddes.rpv \
	DCcircuit.rpv \
	DescartesFolium.rpv \
	Dipole.rpv \
	Eco9.rpv \
	Emiris.rpv \
	Enfant1.rpv \
	Enfant2.rpv \
	Flower.rpv \
	Fourbar.rpv \
	Geneig.rpv \
	H106.rpv \
	Hexane.rpv \
	Hong1.rpv \
	Hong2.rpv \
	I1.rpv \
	I2.rpv \
	I4.rpv \
	I5.rpv \
	Ineq1.rpv \
	Ipp.rpv \
	Kapur.rpv \
	Katsura5.rpv \
	Kear11.rpv \
	Kin1.rpv \
	Kin2.rpv \
	Kruppa.rpv \
	Ku.rpv \
	Ladders.rpv \
	Lhomme1.rpv \
	Lorentz.rpv \
	More10.rpv \
	Morgan.rpv \
	Nauheim.rpv \
	Nbody5.1.rpv \
	Neuro.rpv \
	Noon5.rpv \
	O32.rpv \
	OceanCurrent.rpv \
	Parabola.rpv \
	Pentagon.rpv \
	Piano.rpv \
	Puma.rpv \
	Quadfor.rpv \
	Quadgrid.rpv \
	Rabinowitz.rpv \
	Rbpl.rpv \
	Realkin.rpv \
	Reimer5.rpv \
	Rose.rpv \
	Rueher.rpv \
	SchoolBuilding.rpv \
	SeyfertFilter.rpv \
	Solotarev.rpv \
	StackedGymnasticHalls.rpv \
	SteelFramedComputerBuilding.rpv \
	Transistor.rpv \
	Trinks.rpv \
	Virasoro.rpv \
	Vrahatis.rpv \
	Wright.rpv
