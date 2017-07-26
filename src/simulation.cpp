#include "simulation.h"

Simulation::Simulation(unsigned lambda, unsigned slots, unsigned tNos)
{
    this->lambda = lambda;
    this->slots = slots;
    this->tNos = tNos;
    this->cL = 0;
    this->cac = 0;
    this->actualizations = 0;
    reset();
}

void Simulation::reset()
{
    sizeInHops = blockByCont = 0;

    slotsBloqueados = 0;
    tSlotsBloqueados = 0;
    tSlotsSocilitados = 0;

    for (int i = 0 ; i < 100 ; i++)
        tPerSlotsBloqueados[i] = tPerSlotsSocilitados[i] = 0;
}


//Aqui eu armazeno os resultados em arquivos de saída
void Simulation::store()
{
    static bool flag = true;
    fstream file, file2, file3;

    ostringstream arq1,arq2,arq3;   // stream used for the conversion

    arq1 << "PBTotal_" << lambda << "_" << slots << "_" << tNos << ".txt";
    arq2 << "PBdoSlot_" << lambda << "_" << slots << "_" << tNos << ".txt";
    arq3 << "PBdoSlotInd_" << lambda << "_" << slots << "_" << tNos << ".txt";

    if (flag)//para quando iniciar uma simulação apagar o arquivo.
    {
        flag = false;

        file.open(arq1.str().c_str(),fstream::out);
        file << "PB total" << endl;

        file2.open(arq2.str().c_str(),fstream::out);
        file2 << "PB do Slot total" << endl;

        file3.open(arq3.str().c_str(),fstream::out);
        file3 << "PB do Slot Ind" << endl;
    }
    else
    {
        file.open(arq1.str().c_str(),fstream::out | fstream::app);
        file2.open(arq2.str().c_str(),fstream::out | fstream::app);
        file3.open(arq3.str().c_str(),fstream::out | fstream::app);
    }

    locale mylocale("");
    file.imbue( mylocale );
    file2.imbue( mylocale );
    file3.imbue( mylocale );

    double t = 0.0;

    //PBTOTAL
    t = ((double)blockByCont)/tCalls*100;
    cout << "Bloqueio por continuidade: " << t << "%" << endl;
    //cout << "Bloqueio total: " << t << "%" <<  endl;
    file << t << endl;
    file.close();
    //

    //PBdoSlot
    t = ((double)tSlotsBloqueados)/tSlotsSocilitados*100;
    cout << "Bloqueio dos slots: " << t << "%" << endl;
    file2 << t << endl;
    file2.close();

    //PBdoSlotInd
    cout << "Bloqueio slots Ind: " << endl;
    for (int i = 1 ; i <= 7 ; i++)
    {
        t = ((double)tPerSlotsBloqueados[i])/tCalls*100;
        cout << t << "% ";
        file3 << t << " ";
    }
    cout << endl;


    file3 << endl;
    file3.close();
}

void Simulation::setUpClientLayer(ClientLayer &clientLayer)
{
    cL = &clientLayer;
}

void Simulation::setUpCACLayer(CACLayer &cacLayer)
{
    cac = &cacLayer;
}

void Simulation::run(double la_min, double la_max, double la_step, unsigned totalCalls, unsigned count)
{
    tCalls = totalCalls;
    cout << "Calls: " << tCalls << endl;
//#	pragma omp parallel num_threads(4)
	for (int i = 0; i < count; i++) {
	    for (double la = la_min ; la <= la_max ; la+= la_step)
	    {
			reset();
		    cL->reconfigureLoad(la);
		    cout << (*cL) << endl;
		    simulate(totalCalls);
		    cac->clearMatrixAllocation();//limpa alocações
		    cac->clearListOfConnections();//limpa lista de conexoes
		    store();
	    }
	}
}

void Simulation::simulate(unsigned totalCalls)
{
    double actualTime = 0.0;
    unsigned s,t;

    unsigned slotsSolicitados = 0;

    for (unsigned i = 0 ; i < totalCalls ; i++)
    {
        cL->getConnection(s,t); //pega as conexões da camada cliente (ClientLayer)

        slotsSolicitados = cL->requestSlots();

        tSlotsSocilitados += slotsSolicitados;

        tPerSlotsSocilitados[slotsSolicitados]++;

		int aux;
        if (aux=cac->tryRequest(s,t,slotsSolicitados ,cL->requestQoS(),cL->requestQoSThereshold())) //tenta estabelecer a conexão
        {
            sizeInHops += cac->getLastConnection().getPath().getLength(); //pega o tamanho da rota

            cac->allocateResources(actualTime,cL->requestDuration());
        }
        else
        {
            //bloqueio dos slots
            tSlotsBloqueados += slotsSolicitados;
            tPerSlotsBloqueados[slotsSolicitados]++;

            //bloqueio por continuidade
            blockByCont++;
        }
        actualTime += cL->nextConnectionInterval();

        cac->updateConnectionsByTime(actualTime);

//		std::cout << cL->nextConnectionInterval() << std::endl;
    }
}
