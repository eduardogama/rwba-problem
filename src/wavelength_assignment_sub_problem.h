#ifndef WAVELENGTHASSIGNMENTSUBPROBLEM_H
#define WAVELENGTHASSIGNMENTSUBPROBLEM_H

#include "path.h"
#include <iostream>
#include <climits>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "lambda_control.h"
#include "hole.h"


class WavelengthAssignmentSubProblem {

public:
	WavelengthAssignmentSubProblem();
	virtual ~WavelengthAssignmentSubProblem(){}

	virtual bool searchLambda(Path &path, unsigned nSlots);
	virtual bool searchLambda(Path &path, vector<Path> &path_int, unsigned nSlots);
	
	void setStateNetwork(LambdaControl *lambdaControl);
	bool hasFindedLambda() const;
	unsigned getLastLambdaFinded() const;
	unsigned getLastSlotFinded() const;
	
	void getActualSateNetwork(vector<bool> &actualStateNetwork, Path &path);
	void getNextStateNetwork(Path &path, Hole &hole, vector<bool> &nextStateNetwork, unsigned nSlots);

	double calcPossibilitiesLambdas(Path &path);

protected:
	bool findedLambda;
	unsigned lastLambdaFinded;

	bool findedSlots;
	unsigned lastSlotFinded;

	LambdaControl *lambdaControl;
	
	bool firstFitHeuristic(Path &path, unsigned nSlots);
	bool firstFitExtremesHeuristic(Path &path, unsigned nSlots);
	bool randomHeuristic(Path &path, unsigned nSlots);
    bool leastCapacityLossHeuristc(Path &path, unsigned nSlots);
    bool lossOfCapacityHeuristc(Path &path, vector<Path> &path_int, unsigned nSlots);
    int calculateScore(bool*allocationSetup, int lambda, int nSlots);
    void copyArray(int pathSize, bool *from, bool *to);

};

#endif // WAVELENGTHASSIGNMENTSUBPROBLEM_H
