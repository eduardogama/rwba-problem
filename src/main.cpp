#include <iostream>

#include "simulation.h"
#include "client_layer.h"
#include "cac_layer.h"
#include "version.h"
#include <ctime>
#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

    //int tNos = 6;
    int tNos = 5;
    char nome[] = "Rede Anel MORPHEUS.";

    unsigned dist[]=

//        {
//          //A  B  C  D  E  F  G  H  I
//            0, 1, 0, 0, 0, 0, 0, 0, 1,
//            1, 0, 1, 0, 0, 0, 0, 0, 0,
//            0, 1, 0, 1, 0, 0, 0, 0, 0,
//            0, 0, 1, 0, 1, 0, 0, 0, 0,
//            0, 0, 0, 1, 0, 1, 0, 0, 0,
//            0, 0, 0, 0, 1, 0, 1, 0, 0,
//            0, 0, 0, 0, 0, 1, 0, 1, 0,
//            0, 0, 0, 0, 0, 0, 1, 0, 1,
//            1, 0, 0, 0, 0, 0, 0, 1, 0
//        };
        
        {
            //A  B  C  D  E
              0, 1, 0, 0, 1,
              1, 0, 1, 0, 0,
              0, 1, 0, 1, 0,
              0, 0, 1, 0, 1,
              1, 0, 0, 1, 0,
        };

//O programa começa aqui
//Apenas faço algumas inicializações e chamo sim.rum() para rodar a simuação propriamente dita
int main(int argc, char *argv[])
{
    srand(time(0));
    unsigned lambda = atoi(argv[1]);
    unsigned slots = atoi(argv[2]);
    double la_min = atoi(argv[3]), la_max = atoi(argv[4]), la_step = atoi(argv[5]);
    unsigned totalConnections = atoi(argv[6]);

    double gama = 7.0, gamaTol = 4.0, mii = 1.0;

    ClientLayer cL(tNos,slots,gama,gamaTol,mii);
    CACLayer cac(dist,tNos,lambda,slots);

    cout << "IA-RWA Simulator: " << AutoVersion::UBUNTU_VERSION_STYLE << " - " << AutoVersion::FULLVERSION_STRING << endl;

    Simulation sim(lambda,slots,tNos);

    sim.setUpClientLayer(cL);
    sim.setUpCACLayer(cac);

    printf("Trafego: %g %g %g\n",la_min,la_max,la_step);
    sim.run(la_min,la_max,la_step,totalConnections);
    
	return 0;
}
