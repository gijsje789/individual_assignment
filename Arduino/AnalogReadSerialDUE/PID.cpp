#include "PID.h"
#include "Arduino.h"

PID::PID()
{
  
}

PID::PID(int T, float Ki, float Kp, float max, float min, float maxSlope, int scale)
{
	_T = T;
	_Ki = Ki;
	_Kp = Kp;
	_max = max;
	_min = min;
	_maxSlope = maxSlope;
	_scale = scale;
	_integral = 0;
	_prevOut = 0;
}

PID::~PID()
{
}

void PID::setParameters(int T, float Ki, float Kp, float max, float min, float maxSlope, int scale)
{
  _T = T;
  _Ki = Ki;
  _Kp = Kp;
  _max = max;
  _min = min;
  _maxSlope = maxSlope;
  _scale = scale;
  _integral = 0;
  _prevOut = 0;
}

int PID::getControlSignal(int setPoint, int measured)
{
	/* Serial.print(" s ");
	Serial.print(setPoint);
	Serial.print(" m ");
	Serial.print(measured); */
	// Calculate the error
	int e = setPoint - measured;
	// Calculate the proportional term
	int P = _Kp * e;
	
	// Calculate the integral term
	_integral += e * _T;
	int I = _Ki * _integral;
	
	int output = P + I;
	output = ((float)_max/80000.0)*(float)(output);
	
	Serial.print(" e ");
	Serial.print(e);
	Serial.print(" P ");
	Serial.print(P);
	/*Serial.print(" int ");
	Serial.print(_integral);
	Serial.print(" I ");
	Serial.print(I);
	Serial.print(" O ");
	Serial.print(output); */
	Serial.print(' ');
  Serial.print(output);
	if(output < _min)
	{
		output = _min;
	}
	
	if(output > _max)
	{
		output = _max;
	}
	
	// Limit the change of output signal
	if(abs(output - _prevOut) > _maxSlope)
	{
		output = _maxSlope;
	}
	Serial.print(' ');
  Serial.print(output);
	_prevOut = output;
	return output;
}
