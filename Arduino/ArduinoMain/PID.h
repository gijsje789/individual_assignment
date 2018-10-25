#ifndef _PID_H_
#define _PID_H_

/** Based on the C++ controller example by bradley219 (https://gist.github.com/bradley219/5373998) **/

class PID
{
	public:
    PID();
		PID(int T, float Kp, float Ki, float Kd, float max, float min, float maxSlope, int scale);
		~PID();
    void setParameters(int T, float Kp, float Ki, float Kd, float max, float min, float maxSlope, int scale);
		int getControlSignal(int setPoint, int measured);
	private:
		int _T; 		// Sampling time in ms.
		float _Kp; 		// Proportional gain
		float _Ki;		// Integral gain
    float _Kd;    // Differential gain
		int _max;		// Maximum output signal
		int _min;		// Minimum output signal
		int _maxSlope;	// Maximum change of control signal
		int _integral;	// The integral value
		int _prevOut; 	// The previous output signal
		int _scale;		// The scaling used to prevent float calculations.
    int _error_prev; // Previous error for derivative control.
};

#endif
