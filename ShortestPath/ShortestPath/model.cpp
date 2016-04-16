
// $Id: sample2.cpp 1898 2013-04-09 18:06:04Z stefan $
// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
// This code is licensed under the terms of the Eclipse Public License (EPL).

#include <cassert>
#include <iomanip>

#include "CoinPragma.hpp"

// For Branch and bound
#include "OsiSolverInterface.hpp"
#include "CbcModel.hpp"
#include "CbcBranchUser.h"
#include "CbcCompareUser.h"
#include "CbcCutGenerator.hpp"
#include "CbcStrategy.hpp"
#include "CbcHeuristicLocal.hpp"
#include "OsiClpSolverInterface.hpp"

// Cuts
#include "CglGomory.hpp"
#include "CglProbing.hpp"
#include "CglKnapsackCover.hpp"
#include "CglRedSplit.hpp"
#include "CglClique.hpp"
#include "CglFlowCover.hpp"
#include "CglMixedIntegerRounding2.hpp"
// Preprocessing
#include "CglPreProcess.hpp"

// Heuristics

#include "CbcHeuristic.hpp"

#include "CoinTime.hpp"

#include "graph.h"

//#############################################################################


/************************************************************************

This main program reads in an integer model from an mps file.

It then sets up some Cgl cut generators and calls branch and cut.

Branching is simple binary branching on integer variables.

Node selection is depth first until first solution is found and then
based on objective and number of unsatisfied integer variables.
In this example the functionality is the same as default but it is
a user comparison function.

Variable branching selection is on maximum minimum-of-up-down change
after strong branching on 5 variables closest to 0.5.

A simple rounding heuristic is used.

Preprocessing may be selected in two ways - the second is preferred now

************************************************************************/
#define PREPROCESS 2

bool solve(Graph *graph,OsiClpSolverInterface *solver1,double *result,int &length,double &value)
{
	long time1 = CoinCpuTime();
//	solver1->setRowLower(2, 300);
//	solver1->initialSolve();
	// Reduce printout
	solver1->setHintParam(OsiDoReducePrint, true, OsiHintTry);

	CbcModel model(*solver1);
	model.solver()->setHintParam(OsiDoReducePrint, true, OsiHintTry);
	/*
	// Set up some cut generators and defaults
	// Probing first as gets tight bounds on continuous
	CglProbing generator1;
	generator1.setUsingObjective(true);
	generator1.setMaxPass(1);
	generator1.setMaxPassRoot(5);
	// Number of unsatisfied variables to look at
	generator1.setMaxProbe(10);
	generator1.setMaxProbeRoot(1000);
	// How far to follow the consequences
	generator1.setMaxLook(50);
	generator1.setMaxLookRoot(500);
	// Only look at rows with fewer than this number of elements
	generator1.setMaxElements(200);
	generator1.setRowCuts(3);

	CglGomory generator2;
	// try larger limit
	generator2.setLimit(300);

	CglKnapsackCover generator3;

	CglRedSplit generator4;
	// try larger limit
	generator4.setLimit(200);

	CglClique generator5;
	generator5.setStarCliqueReport(false);
	generator5.setRowCliqueReport(false);

	CglMixedIntegerRounding2 mixedGen;
	CglFlowCover flowGen;

	// Add in generators
	// Experiment with -1 and -99 etc
	model.addCutGenerator(&generator1, 1, "Probing");
	model.addCutGenerator(&generator2, 1, "Gomory");
	model.addCutGenerator(&generator3, 1, "Knapsack");
	model.addCutGenerator(&generator4,1,"RedSplit");
	model.addCutGenerator(&generator5, 1, "Clique");
	model.addCutGenerator(&flowGen, 1, "FlowCover");
	model.addCutGenerator(&mixedGen, 1, "MixedIntegerRounding");
	
	// Say we want timings
	int numberGenerators = model.numberCutGenerators();
	int iGenerator;
	for (iGenerator = 0; iGenerator<numberGenerators; iGenerator++) {
		CbcCutGenerator * generator = model.cutGenerator(iGenerator);
		generator->setTiming(true);
	}
	
	OsiClpSolverInterface * osiclp = dynamic_cast< OsiClpSolverInterface*> (model.solver());
	// go faster stripes
	if (osiclp) {
		// Turn this off if you get problems
		// Used to be automatically set
		osiclp->setSpecialOptions(128);
		if (osiclp->getNumRows()<300 && osiclp->getNumCols()<500) {
			//osiclp->setupForRepeatedUse(2,0);
			osiclp->setupForRepeatedUse(0, 0);
		}
	}
	
	// Uncommenting this should switch off all CBC messages
	// model.messagesPointer()->setDetailMessages(10,10000,NULL);
	// Allow rounding heuristic

	CbcRounding heuristic1(model);
	model.addHeuristic(&heuristic1);

	// And local search when new solution found

	CbcHeuristicLocal heuristic2(model);
	model.addHeuristic(&heuristic2);
	
	// Redundant definition of default branching (as Default == User)
	CbcBranchUserDecision branch;
	model.setBranchingMethod(&branch);

	// Definition of node choice
	CbcCompareUser compare;
	model.setNodeComparison(compare);
	model.setMaximumSavedSolutions(10);

	*/
	// Do initial solve to continuous
//	model.initialSolve();
	model.setNumberStrong(10);
	model.setMaximumSavedSolutions(100);
	model.setSolutionCount(100);
	model.solver()->setIntParam(OsiMaxNumIterationHotStart, 100);

	model.setLogLevel(0);


	// Do complete search

	model.branchAndBound();
	long t0 = getTime();
	for (int i = 0; i < 1; i++){
		if (checkCircle(graph, model.savedSolution(i), model.getNumCols())){
			if (i==0)
			return true;
		}
			
	}
	printf("Circle time %d\n", getTime() - t0);
	std::cout <<" took " << CoinCpuTime() - time1 << " seconds, "
		<< model.getNodeCount() << " nodes with objective "
		<< model.getObjValue()
		<< (!model.status() ? " Finished" : " Not finished")
		<< std::endl;
	value = model.getObjValue();
	const double *r = model.getColSolution();
	length = model.getNumCols();
	for (int i = 0; i < length; i++){
		result[i] = r[i];
	}

	return false;
}
