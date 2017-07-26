#include "client_layer.h"


ClientLayer::ClientLayer(unsigned tNoss, unsigned slotss, double gamaa, double gamaToll, double miii, double laa):
tNos(tNoss),slots(slotss),gama(gamaa),gamaTol(gamaToll),mii(miii),la(laa),last(-1),requestedOne(false)
{

}

void ClientLayer::getConnection(unsigned &s, unsigned &t)
{
	//por semente no main
	s = rand() % tNos;
    do
    {
        t = rand() % tNos;
    }
    while (t == s);
}

unsigned ClientLayer::requestFrom()
{
	unsigned t;
	if (!requestedOne)
		t = rand() % tNos;
	else
	{
		do
		{
			t = rand() % tNos;
		}
		while (t == last);
	}
	requestedOne = !requestedOne;
	return last = t;
}

unsigned ClientLayer::requestTo()
{
	return requestFrom();
}

unsigned ClientLayer::requestSlots()
{
//    return ( ( rand() % slots ) + 1 );
    return ( ( rand() % 3 ) + 1 );        
}


double ClientLayer::expone(double Tmed)
{
    double va;
    do
    {
        va = (double) rand()/RAND_MAX;
    }
    while ((va==0)||(va==1));

    return -(log(1-va))*Tmed;
}

double ClientLayer::requestDuration()
{
	return expone(1/mii);
}

double ClientLayer::nextConnectionInterval()
{
	return expone(1/la);
}

double ClientLayer::requestQoS() const
{
    return gama;
}

double ClientLayer::requestQoSThereshold() const
{
    return gamaTol;
}

void ClientLayer::reconfigureLoad(double la)
{
    this->la = la;
}

std::ostream &operator<<(std::ostream &os, const ClientLayer &cL)
{
    os << "Load: " << cL.la;
    return os;
}
