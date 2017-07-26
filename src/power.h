#ifndef POWER_H
#define POWER_H

#include <iostream>

class Power {

public:
	Power(long double ssignal = 0.0, long double nnoise = 0.0):signal(ssignal),noise(nnoise){}
	Power(const Power &power);
	~Power(){}

	void setSignal(long double signal);
	long double getSignal() const;
	void setNoise(long double noise);
	long double getNoise() const;
	long double getOSNR() const;

	long double getTotalSignalPlusNoise() const;

	Power operator*(double factor);
	Power operator+(const Power &power);

	friend std::ostream &operator<<(std::ostream &os, const Power &power);

private:
	long double signal;
	long double noise;


};

#endif // POWER_H
