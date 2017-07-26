#ifndef CACLAYER_H
#define CACLAYER_H

#include "lambda_control.h"
#include "connection.h"
#include "routing_sub_problem.h"
#include "wavelength_assignment_sub_problem.h"
#include <list>
#include <iostream>

class CACLayer {

public:
	CACLayer(unsigned *matrixLength, unsigned tNos, unsigned lambda, unsigned slots);
	~CACLayer(){}

    enum QUALITY {QoT = 0, QoTThereshold};

	bool tryRequest(unsigned from, unsigned to, unsigned nSlots, double QoS, double QoSThereshold);//continuidade teste

	void allocateResources(double actualTime, double duraction);

    //remover conexoes que passaram do tempo
	void updateConnectionsByTime(double actualTime);

	void clearMatrixAllocation();
	void clearListOfConnections();

	Connection &getLastConnection();

	std::list<Connection> &getListOfConnections();

private:
	
    std::list<Connection> conList;

    LambdaControl lc;
    RoutingSubProblem routing;
    WavelengthAssignmentSubProblem wave;
    
    
};

#endif // CACLAYER_H
