#include "lambda_control.h"

#include <unistd.h>

LambdaControl::LambdaControl(unsigned tNos, unsigned lambda, unsigned slots):matrixAllocation(tNos*tNos*lambda*slots,0),lambdaUsed(tNos*tNos,0),nextStateNetwork(tNos*tNos*lambda*slots,false), actualStateNetwork(tNos*tNos*lambda*slots,false)
{
	//matrixAllocation.reserve(tNos*tNos*lambda, NULL);
	this->tNos = tNos;
	tLambdas = lambda;
	tSlots = slots;
}

unsigned LambdaControl::getMap(unsigned from, unsigned to, unsigned lambda, unsigned slot)
{
	//return from*tNos*tNos + to*tNos + lambda;//<<<ERRADO
	return slot*tLambdas*tNos*tNos + lambda*tNos*tNos + from*tNos + to;
}

bool LambdaControl::canAlloc(const Connection &connection, unsigned lambda, unsigned slot, Connection::TRANSMISSION_TYPE tT)
{
	Path path = connection.getPath();
	path.goStart();
	while ( !path.isEndPath())
	{
		if (matrixAllocation[getMap(path.getActualStep(),path.getNextStep(),lambda,slot)] != NULL &&
            (tT == Connection::BIDIRECTIONAL?(matrixAllocation[getMap(path.getNextStep(),path.getActualStep(),lambda,slot)] != NULL):true ))
			return false;
		path.goAhead();
	}
	return true;
}

bool LambdaControl::canAlloc(const Path &path, unsigned lambda, unsigned slot, Connection::TRANSMISSION_TYPE tT)
{
	Path path_ = path;
	path_.goStart();
	while ( !path_.isEndPath())
	{
		if (matrixAllocation[getMap(path_.getActualStep(),path_.getNextStep(),lambda,slot)] != NULL &&
            (tT == Connection::BIDIRECTIONAL?(matrixAllocation[getMap(path_.getNextStep(),path_.getActualStep(),lambda,slot)] != NULL):true))
			return false;
		path_.goAhead();
	}
	return true;
}

bool LambdaControl::canAllocNextState(const Path &path, unsigned lambda, unsigned slot)
{
	Path path_ = path;
	path_.goStart();
	while ( !path_.isEndPath())
	{
		if (nextStateNetwork[getMap(path_.getActualStep(), path_.getNextStep(), lambda, slot)] == true)
			return false;
		path_.goAhead();
	}
	return true;
}

void LambdaControl::allocConnection(Connection &connection, unsigned lambda, unsigned slot, Connection::TRANSMISSION_TYPE tT)
{
	Path path = connection.getPath();
	path.goStart();
	while ( !path.isEndPath())
	{
		matrixAllocation[getMap(path.getActualStep(),path.getNextStep(),lambda,slot)] = &connection;
		lambdaUsed[path.getActualStep()*tNos + path.getNextStep()]++;

		if (tT == Connection::BIDIRECTIONAL)
		{
            matrixAllocation[getMap(path.getNextStep(),path.getActualStep(),lambda,slot)] = &connection;
		    lambdaUsed[path.getNextStep()*tNos + path.getActualStep()]++;
		}
		path.goAhead();
	}
}

void LambdaControl::disallocConnection(Connection &connection, unsigned slot)
{
    Path path = connection.getPath();
	path.goStart();
	while ( !path.isEndPath())
	{
		matrixAllocation[getMap(path.getActualStep(),path.getNextStep(),connection.getLambda(),slot)] = NULL;
		lambdaUsed[path.getActualStep()*tNos + path.getNextStep()]--;

		if (connection.getTransmissionType() == Connection::BIDIRECTIONAL)
		{
            matrixAllocation[getMap(path.getNextStep(),path.getActualStep(),connection.getLambda(),slot)] = NULL;
		    lambdaUsed[path.getNextStep()*tNos + path.getActualStep()]--;
		}
		path.goAhead();
	}
}

unsigned LambdaControl::getTotalLambdas() const
{
	return tLambdas;
}

unsigned LambdaControl::getTotalSlots() const
{
    return tSlots;
}

unsigned LambdaControl::getTotalNodes() const
{
	return this->tNos;
}

Connection *LambdaControl::getConnection(unsigned from, unsigned to, unsigned lambda, unsigned slot)
{
    return matrixAllocation[getMap(from,to,lambda,slot)];
}

vector<Connection*> *LambdaControl::getMatrixAllocation()
{
	return &(this->matrixAllocation);
}

void LambdaControl::clear()
{
    for (int i = 0 ; i < matrixAllocation.size() ; i++)
    {
        matrixAllocation[i] = 0;
    }
    for (int i = 0 ; i < lambdaUsed.size() ; i++)
    {
        lambdaUsed[i] = 0;
    }
}

unsigned LambdaControl::getTotalLambdasAllocatedIn(unsigned from, unsigned to)//alterar depois
{
    return lambdaUsed[from*tNos + to];
}

unsigned LambdaControl::getTotalLambdasFreeIn(unsigned from, unsigned to)//alterar depois
{
    return tLambdas - lambdaUsed[from*tNos + to];
}

void LambdaControl::getActualSateNetwork()
{
	for(unsigned i=0; i < matrixAllocation.size(); i++)
	{
		actualStateNetwork[i] = (matrixAllocation[i] != NULL);
	}
}

void LambdaControl::initNextStateNetwork()
{
	for(unsigned i=0; i < matrixAllocation.size(); i++)
	{
		nextStateNetwork[i] = (matrixAllocation[i] != NULL);
	}
}

void LambdaControl::allocNextStateNetwork(Path &path, unsigned lambda, unsigned slot, unsigned nSlots)
{
	Path path_ = path;

	path_.goStart();
	while(!path_.isEndPath())
	{
		for(unsigned slot_i = slot; slot_i < slot + nSlots; slot_i++)
		{
			nextStateNetwork[getMap(path_.getActualStep(),path_.getNextStep(),lambda,slot_i)] = true;
		}
		path_.goAhead();
	}
}

void LambdaControl::disallocNextStateNetwork(Path &path, unsigned lambda, unsigned slot, unsigned nSlots)
{
	Path path_ = path;

	path_.goStart();
	while(!path_.isEndPath())
	{
		for(unsigned slot_i = slot; slot_i < slot + nSlots; slot_i++)
		{
			nextStateNetwork[getMap(path_.getActualStep(),path_.getNextStep(),lambda,slot_i)] = false;
		}
		path_.goAhead();
	}
}

unsigned LambdaControl::actualPossibilities(Path &path, unsigned lambda, unsigned nSlots)
{
	unsigned score[] = {0,0,0};
	unsigned scr;

	for(unsigned slot = 0 ; slot < tSlots; slot++)
	{
        scr=0;
        for(unsigned slot_i = slot ; slot_i < tSlots; slot_i++)
        {
        	if(!canAlloc(path, lambda, slot_i))
            {
                slot = slot_i; //otimização
                break;
            }
            else
            {
				scr++;
            }
        }

    	score[0] += scr;
		score[1] += scr/2;
		score[2] += scr/3;
    }

	return score[0] + score[1] + score[2];
}

unsigned LambdaControl::nextPossibilities(Path &path, unsigned lambda, unsigned nSlots)
{
	unsigned score[] = {0,0,0};
	unsigned scr;

	for(unsigned slot = 0 ; slot < tSlots; slot++)
	{
        scr = 0;
        for(unsigned slot_i = slot ; slot_i < tSlots; slot_i++ )
        {
            if(!canAllocNextState(path, lambda, slot_i))
            {
                slot = slot_i;
                break;
            }
            else
            {
            	scr++;
           	}
        }

		score[0] += scr;
		score[1] += scr/2;
		score[2] += scr/3;
    }

	return score[0] + score[1] + score[2];
}

double LambdaControl::calcRelativesPossibilities(Path &path, unsigned lambda)
{
	double a_score[] = {0,0,0};
	double a_scr = 0;

	double n_score[] = {0,0,0};
	double n_scr = 0;

	double r_score[] = {0,0,0};

	for (unsigned lmb = 0; lmb < tLambdas; lmb += 1){
		for(unsigned slot = 0 ; slot < tSlots; slot++){
			if(canAlloc(path, lmb, slot)){
				a_scr++;
		    }else{
				r_score[0] += a_scr;
				r_score[1] += (a_scr - 1 > 0) ? a_scr - 1 : 0;
				r_score[2] += (a_scr - 2 > 0) ? a_scr - 2 : 0;

		    	a_scr = 0;
		   	}
		}
	}

	r_score[0] += a_scr;
	r_score[1] += (a_scr - 1 > 0) ? a_scr - 1 : 0;
	r_score[2] += (a_scr - 2 > 0) ? a_scr - 2 : 0;

	a_scr = 0;

	for(unsigned slot = 0 ; slot < tSlots; slot++)
	{
		if(canAlloc(path, lambda, slot)){
			a_scr++;
	    }else{
			a_score[0] += (a_scr);
			a_score[1] += ((a_scr - 1 > 0) ? a_scr - 1 : 0);
			a_score[2] += ((a_scr - 2 > 0) ? a_scr - 2 : 0);

	    	a_scr = 0;
	   	}

	    if(canAllocNextState(path, lambda, slot)){
			n_scr++;
	    }else{
			n_score[0] += (n_scr);
			n_score[1] += ((n_scr - 1 > 0) ? n_scr - 1 : 0);
			n_score[2] += ((n_scr - 2 > 0) ? n_scr - 2 : 0);

	    	n_scr = 0;
	   	}
	}

	a_score[0] += a_scr;
	a_score[1] += ((a_scr - 1 > 0) ? a_scr - 1 : 0);
	a_score[2] += ((a_scr - 2 > 0) ? a_scr - 2 : 0);

	n_score[0] += n_scr;
	n_score[1] += ((n_scr - 1 > 0) ? n_scr - 1 : 0);
	n_score[2] += ((n_scr - 2 > 0) ? n_scr - 2 : 0);

	double result =  ((a_score[0] + a_score[1] + a_score[2]) - (n_score[0] + n_score[1] + n_score[2]))/(r_score[0] + r_score[1] + r_score[2]);
	return result;
}

unsigned LambdaControl::calcPossibilities(Path &path, unsigned lambda)
{
	int a_score[] = {0,0,0};
	int a_scr=0;

	int n_score[] = {0,0,0};
	int n_scr=0;
	for (unsigned lmbd = 0; lmbd < tLambdas; lmbd++) {
		for(unsigned slot = 0 ; slot < tSlots; slot++) {
			if(canAlloc(path, lmbd, slot)) {
				a_scr++;
		    } else {
				a_score[0] += a_scr;
				a_score[1] += (a_scr - 1 > 0) ? a_scr - 1 : 0;
				a_score[2] += (a_scr - 2 > 0) ? a_scr - 2 : 0;

		    	a_scr = 0;
		   	}

		    if(canAllocNextState(path, lmbd, slot)) {
				n_scr++;
		    } else {
				n_score[0] += n_scr;
				n_score[1] += (n_scr - 1 > 0) ? n_scr - 1 : 0;
				n_score[2] += (n_scr - 2 > 0) ? n_scr - 2 : 0;

		    	n_scr = 0;
		   	}
		}
	}
	
	a_score[0] += a_scr;
	a_score[1] += (a_scr - 1 > 0) ? a_scr - 1 : 0;
	a_score[2] += (a_scr - 2 > 0) ? a_scr - 2 : 0;

	n_score[0] += n_scr;
	n_score[1] += (n_scr - 1 > 0) ? n_scr - 1 : 0;
	n_score[2] += (n_scr - 2 > 0) ? n_scr - 2 : 0;

	return (a_score[0] + a_score[1] + a_score[2]) - (n_score[0] + n_score[1] + n_score[2]);
}

unsigned LambdaControl::calcPossibilities(Path &path, unsigned lambda, unsigned nSlots)
{
	int a_score=0;
	int a_scr=0;

	int n_score=0;
	int n_scr=0;

	for(int slot = 0 ; slot < tSlots; slot++)
	{
		if(canAlloc(path, lambda, slot))
        {
			a_scr++;
        }
        else
        {
			a_score += (a_scr - (nSlots - 1) > 0) ? a_scr - (nSlots - 1) : 0;
        	a_scr = 0;
       	}

        if(canAllocNextState(path, lambda, slot))
        {
			n_scr++;
        }
        else
        {
			n_score += (n_scr - (nSlots - 1) > 0) ? n_scr - (nSlots - 1) : 0;
        	n_scr = 0;
       	}
    }

	a_score += (a_scr - (nSlots - 1) > 0) ? a_scr - (nSlots - 1) : 0;
	n_score += (n_scr - (nSlots - 1) > 0) ? n_scr - (nSlots - 1) : 0;

	return (a_score) - (n_score);
}

vector<Hole> LambdaControl::Possibilities(Path &path, unsigned nSlots)
{
	vector<Hole> possibilities;

	bool findedSlots;
	for(int lambda = 0 ; lambda < getTotalLambdas() ; lambda++)
	{
		for(int slot = 0 ; slot <= getTotalSlots() - nSlots ; slot++)
		{
			findedSlots = true;
			for(int slot_i = slot ; slot_i < slot + nSlots ; slot_i++ )
			{
				if(!canAlloc(path,lambda,slot_i))
				{
					findedSlots = false;
					slot = slot_i;
					break;
				}
			}

			if (findedSlots == true)
			{
				Hole hole(lambda, slot);
				possibilities.push_back(hole);
			}
	    }
	}

	return possibilities;
}
