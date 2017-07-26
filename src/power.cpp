#include "power.h"

Power::Power(const Power &power)
{
	*this = power;
}

void Power::setSignal(long double signal)
{
	this->signal = signal;
}

long double Power::getSignal() const
{
	return signal;
}

void Power::setNoise(long double noise)
{
	this->noise = noise;
}

long double Power::getNoise() const
{
	return noise;
}

long double Power::getTotalSignalPlusNoise() const
{
	return signal + noise;
}

long double Power::getOSNR() const
{
    return signal/noise;
}

Power Power::operator+(const Power &power)
{
	return Power(this->getSignal() + power.getSignal() , this->getNoise() + power.getNoise());
}

Power Power::operator*(double factor)
{
	return Power(this->getSignal()*factor, this->getNoise()*factor);
}

std::ostream &operator<<(std::ostream &os, const Power &power)
{
    os << "Power: (" << power.signal << ";" << power.noise << ") OSNR: ";
    if (power.signal&&power.noise)
        os << power.signal/power.noise;
    else
        os <<"-";
    return os;
}
