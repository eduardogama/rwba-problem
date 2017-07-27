#include "wavelength_assignment_sub_problem.h"
#include <float.h>

WavelengthAssignmentSubProblem::WavelengthAssignmentSubProblem()
{
	findedLambda = false;
	lastLambdaFinded = 0;
	findedSlots = false;
	lastSlotFinded = 0;

	lambdaControl = NULL;
}

bool WavelengthAssignmentSubProblem::searchLambda(Path &path, unsigned nSlots)
{
	return findedSlots = findedLambda = firstFitHeuristic(path, nSlots);
}

bool WavelengthAssignmentSubProblem::searchLambda(Path &path, vector<Path> &path_int, unsigned nSlots)
{
	return findedSlots = findedLambda = lossOfCapacityHeuristc(path, path_int, nSlots);
}

void WavelengthAssignmentSubProblem::setStateNetwork(LambdaControl *lambdaControl)
{
	this->lambdaControl = lambdaControl;
}

bool WavelengthAssignmentSubProblem::hasFindedLambda() const
{
	return findedLambda;
}

unsigned WavelengthAssignmentSubProblem::getLastLambdaFinded() const
{
	return this->lastLambdaFinded;
}

unsigned WavelengthAssignmentSubProblem::getLastSlotFinded() const
{
	return this->lastSlotFinded;
}

bool WavelengthAssignmentSubProblem::firstFitHeuristic(Path &path, unsigned nSlots)
{
//	std::cout << nSlots <<"entrou1\n";
	for ( int slot = 0 ; slot <= lambdaControl->getTotalSlots() - nSlots ; slot++)
	{
		for ( int lambda = 0 ; lambda < lambdaControl->getTotalLambdas() ; lambda++)
		{
			bool findedSlots = true;
			for ( int slot_i = slot ; slot_i < slot + nSlots ; slot_i++ )
			{
				if (!lambdaControl->canAlloc(path,lambda,slot_i))
				{
					findedSlots = false;
//					slot = slot_i; // otimização bacana ???
					break;
				}
			}

			if (findedSlots == true)
			{
//				std::cout << lambda << " " << slot << "entrou\n";
				lastLambdaFinded = lambda;
				lastSlotFinded = slot;
				return true;
			}

		}
	}

//	std::cout << "entrou\n";
	return false;
}

bool WavelengthAssignmentSubProblem::firstFitExtremesHeuristic(Path &path, unsigned nSlots)
{
	for ( unsigned lambda = 0 ; lambda < lambdaControl->getTotalLambdas() ; lambda++)
	{
		for ( unsigned slot = 0 ; slot <= lambdaControl->getTotalSlots() - nSlots ; slot++)
		{
			bool findedSlots = true, invertedSearch = false;
			for ( unsigned slot_i = slot ; slot_i < slot + nSlots ; slot_i++ )
			{
				//cout << "Searching: Lambda(" << lambda << ")-Slot(" << slot_i << ") -> ";
				if (!lambdaControl->canAlloc(path,lambda,slot_i))
				{
					//cout << "Used" << endl;
					findedSlots = false;
					break;
				}//else
				//cout << "Free" << endl;
			}

			//Se não foi encontrado slots contiguos vagos no sentido começo->fim
			if (!findedSlots){
				findedSlots = true;

				unsigned endPointer = lambdaControl->getTotalSlots() - slot - nSlots;
				invertedSearch = true;

				//Procure no sentido fim->começo slots contiguos
				for ( unsigned slot_i = endPointer ; slot_i < endPointer + nSlots ; slot_i++ )
				{
					//cout << "Inverted Searching: Lambda(" << lambda << ")-Slot(" << slot_i << ") -> ";
					if (!lambdaControl->canAlloc(path,lambda,slot_i))
					{
						//cout << "Used" << endl;
						findedSlots = false;
						break;
					}//else
					//cout << "Free" << endl;
				}
			}

			if (findedSlots)
			{
				lastLambdaFinded = lambda;
				lastSlotFinded = invertedSearch ? (lambdaControl->getTotalSlots() - slot - nSlots) : slot;
				//cout << "Alocou " << nSlots << " slot(s) em Lambda " << lastLambdaFinded << " começando de " << lastSlotFinded << endl;
				return true;
			}
		}
	}
	return false;
}

bool WavelengthAssignmentSubProblem::randomHeuristic(Path &path, unsigned nSlots)
{
	vector<unsigned> lambdaList;        //Aloca os lambdas livres
	unsigned slotList[lambdaControl->getTotalLambdas()];  //Aloca os slots livres
	bool findedSlots;
	for ( int lambda = 0 ; lambda < lambdaControl->getTotalLambdas() ; lambda++){
		for ( int slot = 0 ; slot <= lambdaControl->getTotalSlots() - nSlots ; slot++){
			findedSlots = true;
			for ( int slot_i = slot ; slot_i < slot + nSlots ; slot_i++ ) {
				if(!lambdaControl->canAlloc(path,lambda,slot_i)){
					findedSlots = false;
					break;
				}
			}

			if (findedSlots == true){
				slotList[lambda] = slot;
				break;
			}
		}

		if (findedSlots){
			lambdaList.push_back(lambda);
		}
	}

	if (!lambdaList.empty()) {
		int randLambda = rand() % lambdaList.size();
		lastLambdaFinded = lambdaList[randLambda];
		lastSlotFinded = slotList[lastLambdaFinded];

		return true;
	}

	return false;
}

bool WavelengthAssignmentSubProblem::lossOfCapacityHeuristc(Path &path, vector<Path> &path_int, unsigned nSlots)
{
	vector<Hole> possibilities;

	bool findedSlots;

	for(int lambda = 0; lambda < lambdaControl->getTotalLambdas(); lambda++)
	{
		for(int slot = 0; slot <= lambdaControl->getTotalSlots() - nSlots; slot++)
		{
			findedSlots = true;
			for(int slot_i = slot; slot_i < slot + nSlots; slot_i++ )
			{
				if(!lambdaControl->canAlloc(path,lambda,slot_i))
				{
					findedSlots = false;
					slot = slot_i;
					break;
				}
			}

			if(findedSlots == true)
			{
				Hole hole(lambda, slot);
				possibilities.push_back(hole);
			}
		}
	}

	if(possibilities.empty())
		return false;

	double best_capacity = DBL_MAX;

	lambdaControl->initNextStateNetwork();
	for (unsigned int i = 0; i < possibilities.size(); i += 1)
	{
		double capacity = 0;

		lambdaControl->allocNextStateNetwork(path, possibilities[i].getLambda(), possibilities[i].getSlot(), nSlots);

		for (unsigned int p_i = 0; p_i < path_int.size(); p_i += 1)
		{
			Path p_int = path_int[p_i];
			capacity += lambdaControl->calcRelativesPossibilities(p_int, possibilities[i].getLambda());
		}

		if(capacity < best_capacity)
		{
			best_capacity = capacity;
			lastLambdaFinded = possibilities[i].getLambda();
			lastSlotFinded = possibilities[i].getSlot();
		}

		lambdaControl->disallocNextStateNetwork(path, possibilities[i].getLambda(), possibilities[i].getSlot(), nSlots);
	}

	return true;
}

double WavelengthAssignmentSubProblem::calcPossibilitiesLambdas(Path &path)
{
	int a_score[] = {0,0,0};
	int a_scr=0;

	for(int lambda = 0 ; lambda < lambdaControl->getTotalLambdas() ; lambda++)
	{
		for(int slot = 0 ; slot < lambdaControl->getTotalSlots(); slot++)
		{
			if(lambdaControl->canAlloc(path, lambda, slot))
			{
				a_scr++;
			}
			else
			{
				//		    	a_score[0] += a_scr;
				//				a_score[1] += a_scr/2;
				//				a_score[2] += a_scr/3;

				a_score[0] += a_scr;
				a_score[1] += (a_scr - 1 > 0) ? a_scr - 1 : 0;
				a_score[2] += (a_scr - 2 > 0) ? a_scr - 2 : 0;

				a_scr = 0;
			}
		}
		//		a_score[0] += a_scr;
		//		a_score[1] += a_scr/2;
		//		a_score[2] += a_scr/3;

		a_score[0] += a_scr;
		a_score[1] += (a_scr - 1 > 0) ? a_scr - 1 : 0;
		a_score[2] += (a_scr - 2 > 0) ? a_scr - 2 : 0;

		a_scr=0;
	}

	return (a_score[0] + a_score[1] + a_score[2]);
}
