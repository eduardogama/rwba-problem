#ifndef CONNECTION_H
#define CONNECTION_H

#include "path.h"

class Connection {

public:
	Connection();
	~Connection(){}

	void setPath(const Path &path);
	const Path &getPath() const;
	bool hasEnded(double actualTime) const;
	void setStartTime(double timeStarted);
	void setDuraction(double duraction);
	void setLambda(unsigned lambda);
	unsigned getLambda() const;
	unsigned getSlot() const;
	unsigned getTSlot() const;

	void setSlots(unsigned slot_i, unsigned tSlots);

	enum TRANSMISSION_TYPE {UNIDIRECTIONAL, BIDIRECTIONAL};

	void setTransmissionType(TRANSMISSION_TYPE tT);
	TRANSMISSION_TYPE getTransmissionType() const;

protected:
	double timeStarted;
	double duraction;
	unsigned lambdaAllocated;

	unsigned slotAllocated;
	unsigned tSlotsAllocated;

	bool update;
	Path path;

	TRANSMISSION_TYPE tT;
};

#endif // CONNECTION_H
