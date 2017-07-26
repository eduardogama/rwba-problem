#ifndef CLIENTLAYER_H
#define CLIENTLAYER_H

#include <cstdlib>
#include <cmath>
#include <iostream>

class ClientLayer {

public:
	ClientLayer(unsigned tNoss, unsigned slotss, double gamaa = 7.0, double gamaToll = 4.0, double miii = 1.0, double laa = 20.0);
	~ClientLayer(){}
	void getConnection(unsigned &s, unsigned &t);
	unsigned requestFrom();
	unsigned requestTo();
	unsigned requestSlots();
	double requestQoS() const;
	double requestQoSThereshold() const;
	double requestDuration();
	double nextConnectionInterval();
	void reconfigureLoad(double la);

	friend std::ostream &operator<<(std::ostream &os, const ClientLayer &cL);

protected:
	double expone(double Tmed);
	//variaveis de controle
	int last;
	bool requestedOne;

	//variaveis de estado
	double la, mii;
	double gama, gamaTol;
	unsigned tNos;
	unsigned slots;
};

#endif // CLIENTLAYER_H
