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
	Apollonius.rp \
	Bellido.rp \
	Bifurcation.rp \
	Bronstein.rp \
	Brown5.rp \
	Broyden10.rp \
	BurdenFaires.rp \
	Butcher.rp \
	Caprasse.rp \
	Cassou.rp \
	Chemistry.rp \
	Chemk.rp \
	Combustion.rp \
	Cpdm5.rp \
	Cyclo.rp \
	CzaporGeddes.rp \
	DCcircuit.rp \
	DescartesFolium.rp \
	Dipole.rp \
	Eco9.rp \
	Emiris.rp \
	Enfant1.rp \
	Enfant2.rp \
	Flower.rp \
	Fourbar.rp \
	Geneig.rp \
	H106.rp \
	Hexane.rp \
	Hong1.rp \
	Hong2.rp \
	I1.rp \
	I2.rp \
	I4.rp \
	I5.rp \
	Ineq1.rp \
	Ipp.rp \
	Kapur.rpv \
	Katsura2.rp \
	Katsura3.rp \
	Katsura5.rp \
	Katsura6.rp \
	Kear11.rp \
	Kin1.rp \
	Kin2.rp \
	Kruppa.rp \
	Ku.rp \
	Ladders.rp \
	Lhomme1.rp \
	Lorentz.rp \
	More10.rp \
	Morgan.rp \
	Nauheim.rp \
	Nbody5.1.rp \
	Neuro.rp \
	Noon5.rp \
	O32.rp \
	OceanCurrent.rp \
	Parabola.rp \
	Pentagon.rp \
	Piano.rp \
	Puma.rp \
	Quadfor.rp \
	Quadgrid.rp \
	Rabinowitz.rp \
	Rbpl.rp \
	Realkin.rp \
	Reimer5.rp \
	Rose.rp \
	Rueher.rp \
	SchoolBuilding.rp \
	SeyfertFilter.rp \
	Solotarev.rp \
	StackedGymnasticHalls.rp \
	SteelFramedComputerBuilding.rp \
	Transistor.rp \
	Trinks.rp \
	Virasoro.rp \
	Vrahatis.rp \
	Wright.rp
