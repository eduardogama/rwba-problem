#ifndef LAMBDACONTROL_H
#define LAMBDACONTROL_H

#include "connection.h"
#include "hole.h"


class LambdaControl {

public:
	LambdaControl(unsigned tNos, unsigned lambda, unsigned slots);
	~LambdaControl(){}
	bool canAlloc(const Connection &connection, unsigned lambda, unsigned slot, Connection::TRANSMISSION_TYPE tT = Connection::UNIDIRECTIONAL);
	bool canAlloc(const Path &path, unsigned lambda, unsigned slot, Connection::TRANSMISSION_TYPE tT = Connection::UNIDIRECTIONAL);
	void allocConnection(Connection &connection, unsigned lambda, unsigned slot, Connection::TRANSMISSION_TYPE tT = Connection::UNIDIRECTIONAL);
	void disallocConnection(Connection &connection, unsigned slot);
	Connection *getConnection(unsigned from, unsigned to, unsigned lambda, unsigned slot); // recebe uma conexao e marka todas as adjacentes
	unsigned getTotalLambdas() const;

	unsigned getTotalSlots() const;

	unsigned getTotalNodes() const;

	vector<Connection*> *getMatrixAllocation();

	unsigned getTotalLambdasAllocatedIn(unsigned from, unsigned to);
	unsigned getTotalLambdasFreeIn(unsigned from, unsigned to);

	void clear();
	unsigned getMap(unsigned from, unsigned to, unsigned lambda, unsigned slots);


	void getActualSateNetwork();
	vector<Hole> Possibilities(Path &path, unsigned nSlots);
	bool canAllocNextState(const Path &path, unsigned lambda, unsigned slot);

	double calcRelativesPossibilities(Path &path, unsigned lambda);
	unsigned calcPossibilities(Path &path, unsigned lambda);
	unsigned calcPossibilities(Path &path, unsigned lambda, unsigned nSlots);

	void initNextStateNetwork();
	void allocNextStateNetwork(const Path &path, unsigned lambda, unsigned slot, unsigned nSlots);
	void disallocNextStateNetwork(const Path &path, unsigned lambda, unsigned slot, unsigned nSlots);

	double getAvailability();

private:
	vector<Connection*> matrixAllocation;
	vector<unsigned> lambdaUsed;
	unsigned tNos;
	unsigned tLambdas;
	unsigned tSlots;

	double availability;
	vector<bool> nextStateNetwork;
};

#endif // LAMBDACONTROL_H
