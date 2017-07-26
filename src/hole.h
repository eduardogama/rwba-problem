#ifndef HOLE_H
#define HOLE_H

#include "connection.h"

class Hole {

public:
	
	Hole(unsigned lambda, unsigned slot)
	{
		this->lambda = lambda;
		this->slot = slot;
	}

	unsigned getLambda()
	{
		return this->lambda;
	}
	
	unsigned getSlot()
	{
		return this->slot;
	}
	
private:
	unsigned slot;
	unsigned lambda;
};

#endif // HOLE_H
