#include "cac_layer.h"

CACLayer::CACLayer(unsigned *matrixLength, unsigned tNos, unsigned lambda, unsigned slots):lc(tNos,lambda,slots),routing(matrixLength,tNos)
{	
    wave.setStateNetwork(&lc);
    routing.setStateNetwork(&lc);
}

//método usado para tentar conectar de "from" pata "to"
bool CACLayer::tryRequest(unsigned from, unsigned to, unsigned nSlots, double QoS, double QoSThereshold)
{
    routing.searchRoute(from,to);

    if (routing.hasFindedRoute()) //sub problema do roteamento
    {
        Path p = routing.getPath();
      vector<Path> p_int = routing.getRoutingInt(from,to);
        
      wave.searchLambda(p,p_int,nSlots);
//		std::cout << "entrou\n";
//        wave.searchLambda(p,nSlots);
        if (wave.hasFindedLambda()) //subproblema do comprimento de onda
        {
            Connection con;
            con.setPath(p);
            con.setLambda(wave.getLastLambdaFinded());

            con.setSlots(wave.getLastSlotFinded(),nSlots);

            conList.push_back(con);

            return true;
        }
    }

    return false;
}

void CACLayer::allocateResources(double actualTime, double duraction)
{
    Connection &con = getLastConnection();

    //setUp new Connection
    con.setDuraction(duraction);
    con.setStartTime(actualTime);

    //allocating
    for ( int slot_i = con.getSlot() ; slot_i < con.getSlot() + con.getTSlot() ; slot_i++ )
        lc.allocConnection(con,con.getLambda(),slot_i);
}

Connection &CACLayer::getLastConnection()
{
    return conList.back();
}

std::list<Connection> &CACLayer::getListOfConnections()
{
    return conList;
}

void CACLayer::clearMatrixAllocation()
{
    lc.clear();
}

void CACLayer::clearListOfConnections()
{
    conList.clear();
}

void CACLayer::updateConnectionsByTime(double actualTime)
{
    std::list<Connection>::iterator it = conList.begin();

    for (; it != conList.end();it++)
        if (it->hasEnded(actualTime))
        {
            for ( int slot_i = (*it).getSlot() ; slot_i < (*it).getSlot() + (*it).getTSlot() ; slot_i++ )
            {
                lc.disallocConnection(*it,slot_i);
            }
            it = conList.erase(it);
        }
}

