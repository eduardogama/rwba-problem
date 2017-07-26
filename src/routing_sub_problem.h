#ifndef ROUTINGSUBPROBLEM_H
#define ROUTINGSUBPROBLEM_H

#include "path.h"
#include "lambda_control.h"
#include <vector>
#include <queue>
#include <iostream>
#include <cmath>


using std::vector;
using std::queue;
using std::cout;


class RoutingSubProblem {

public:
	RoutingSubProblem(unsigned *matrixLength = NULL, unsigned tNos = 0);
	virtual ~RoutingSubProblem(){}

	virtual bool searchRoute(unsigned from, unsigned to);
	void setMatrixLength(unsigned *matrixLength, unsigned tNos);
	void setStateNetwork(LambdaControl *lambdaControl);
	bool hasFindedRoute() const;
	const Path &getPath() const;
	const vector<Path> &getRoutingInt(unsigned from, unsigned to) const;
	
	void printGRAPH();
	
protected:
	bool findedRoute;
	unsigned tNos;
	unsigned *matrixLength;
	Path route;
	LambdaControl *lambdaControl;

	vector< vector<Path> > routing_int;
	
	bool dijkstra(unsigned s, unsigned t);
	unsigned getEdge(unsigned s, unsigned t);
	void initRouting_Int();
	
};

#endif // ROUTINGSUBPROBLEM_H
