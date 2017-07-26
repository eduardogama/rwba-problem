#ifndef SIMULATION_H
#define SIMULATION_H

#include "client_layer.h"
#include "cac_layer.h"
#include "connection.h"
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Simulation {

public:
	Simulation(unsigned lambda, unsigned slots, unsigned tNos);
	~Simulation(){}

    void setUpClientLayer(ClientLayer &clientLayer);
    void setUpCACLayer(CACLayer &cacLayer);

    void setConsiderBlockByQoT(bool considerBlockByQoT);

    void reset();
    void simulate(unsigned totalCalls);
    void store();
    void storeXML(Connection con, unsigned i, unsigned s, unsigned t);
    
    void run(double la_min, double la_max, double la_step, unsigned totalCalls);

private:

    ClientLayer *cL;
    CACLayer *cac;
    unsigned actualizations;
    //variaveis de bloqueio (3 tipos)
    unsigned blockByCont;

    unsigned tCalls;//total de chamadas

    unsigned sizeInHops;//somente das con admitidas pela continuidade

    //
    unsigned slotsBloqueados, tSlotsBloqueados, tSlotsSocilitados;
    unsigned tPerSlotsBloqueados[100],tPerSlotsSocilitados[100];

    unsigned lambda;
    unsigned slots;
    unsigned tNos;

	void WriteLog(int req, int from, int to, int bloq);
};

#endif // SIMULATION_H
