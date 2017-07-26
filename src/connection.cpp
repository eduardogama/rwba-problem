#include "connection.h"

Connection::Connection():path(Path())
{
	timeStarted = duraction = 0.0;
	lambdaAllocated = -1;
	update = false;
	tT = UNIDIRECTIONAL;
}

void Connection::setPath(const Path &path)
{
	this->path = path;
}

const Path &Connection::getPath() const
{
	return path;
}

bool Connection::hasEnded(double actualTime) const
{
	return ( actualTime >= (timeStarted + duraction) );
}


void Connection::setStartTime(double timeStarted)
{
	this->timeStarted = timeStarted;
}

void Connection::setDuraction(double duraction)
{
	this->duraction = duraction;
}

void Connection::setLambda(unsigned lambda)
{
	this->lambdaAllocated = lambda;
}

unsigned Connection::getLambda() const
{
	return lambdaAllocated;
}

unsigned Connection::getSlot() const
{
    return slotAllocated;
}

unsigned Connection::getTSlot() const
{
    return tSlotsAllocated;
}

void Connection::setSlots(unsigned slot_i, unsigned tSlots)
{
    this->slotAllocated = slot_i;
    this->tSlotsAllocated = tSlots;
}

void Connection::setTransmissionType(Connection::TRANSMISSION_TYPE tT)
{
    this->tT = tT;
}

Connection::TRANSMISSION_TYPE Connection::getTransmissionType() const
{
    return tT;
}
