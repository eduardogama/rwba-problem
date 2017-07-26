#include "routing_sub_problem.h"
#include <unistd.h>


RoutingSubProblem::RoutingSubProblem(unsigned *matrixLength, unsigned tNos):route(tNos), routing_int(tNos*tNos)
{
	findedRoute = false;
	this->tNos = tNos;
	this->matrixLength = matrixLength;
	
	initRouting_Int();	
}

bool RoutingSubProblem::searchRoute(unsigned from, unsigned to)
{
	return findedRoute = dijkstra(from, to);
}

//Essa é a implementação do dijkstra (com complexidade O(n²)
bool RoutingSubProblem::dijkstra(unsigned s, unsigned t)
{
    const unsigned inf = 0xFFFFFFFF;

	vector<unsigned> dist(this->tNos,inf);
	vector<unsigned> prev(this->tNos,inf);
	vector<bool> pego(this->tNos,false);

    dist[t] = 0;
    prev[t] = t;
    pego[t] = true;

    unsigned k = t, min = inf;
    unsigned assegure = 0;
    do
    {
        for (int i = 0; i < tNos; i++){
            if (matrixLength[k*tNos + i] != 0 && !pego[i]){
                if (dist[k] + matrixLength[k*tNos + i] < dist[i]){
                    prev[i] = k;
                    dist[i] = dist[k] + matrixLength[k*tNos + i];
                }
            }
        }
        
        k = 0;
        min = inf;
        
        for (int i = 0; i < tNos; i++){
            if (!pego[i] && dist[i] < min){
                min = dist[i];
				k = i;
            }
        }
        
        pego[k] = true;
        
        if (assegure++ >= tNos) //grafo disconexo
            return false;
            
    } while (k!=s);

    k = s;

    route.clear();
    do
    {
        route.addLinkToPath(k);
        k = prev[k];
    } while (prev[k] != k);
    route.addLinkToPath(k);
    
    return true;
}

const vector<Path> &RoutingSubProblem::getRoutingInt(unsigned from, unsigned to) const
{
	return routing_int[from*tNos + to];
}

unsigned RoutingSubProblem::getEdge(unsigned s, unsigned t)
{
    return matrixLength[s*tNos + t];
}

bool RoutingSubProblem::hasFindedRoute() const
{
	return findedRoute;
}

const Path &RoutingSubProblem::getPath() const
{
	return route;
}

void RoutingSubProblem::setStateNetwork(LambdaControl *lambdaControl)
{
    this->lambdaControl = lambdaControl;
}

void RoutingSubProblem::setMatrixLength(unsigned *matrixLength, unsigned tNos)
{
	this->matrixLength = matrixLength;
	this->tNos = tNos;
}

void RoutingSubProblem::initRouting_Int()
{
	int orN, deN, orN_int, deN_int, sNd, dNd, sNd_int, dNd_int;
	Path path, path_int;

	for(orN = 0; orN < tNos; orN++)
	{
		for(deN = 0; deN < tNos; deN++)
		{
			if(orN != deN)	// Um conjunto de rotas válidas
			{
				dijkstra(orN, deN);
				path = getPath();
			
				for(orN_int = 0; orN_int < tNos; orN_int++)
				{
					for(deN_int = 0; deN_int < tNos; deN_int++)
					{
						if(orN_int != deN_int)
						{
							unsigned aux=0;
							//Checar se a path_int interfere com path
							dijkstra(orN_int, deN_int);
							path_int = getPath();
							
							path.goStart();
							while (!path.isEndPath() && aux==0)
							{
								sNd = path.getActualStep();
								dNd = path.getNextStep();
								
								path_int.goStart();
								while (!path_int.isEndPath() && aux==0)
								{
									sNd_int = path_int.getActualStep();
									dNd_int = path_int.getNextStep();
									
									if(sNd == sNd_int && dNd == dNd_int) //As rotas interferem => Incluir a de índice path_int no vetor
									{
										routing_int[orN*tNos + deN].push_back(path_int);
										aux=1;
									}
									path_int.goAhead();
								}
								
								path.goAhead();
								
							}				
						}
					}
				}
			}
		}
	}
//	printGRAPH();
}

void RoutingSubProblem::printGRAPH()
{
	int i,j,k;
	
	for (i = 0; i < tNos; i += 1)
	{
		for (j = 0; j < tNos; j += 1)
		{
			std::cout << "[" << i << "][" << j << "] ";
			for(k = 0; k < routing_int[i*tNos + j].size(); k += 1)
			{
				Path p = routing_int[i*tNos + j][k];
				std::cout << p << " - ";	
			}
			std::cout << std::endl;
		}
	}
}
