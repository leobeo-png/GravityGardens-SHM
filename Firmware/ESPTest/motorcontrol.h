#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <vector>

enum MotorControlMode {
	Stopped,
	Running,
	Calibrating,
};

class MotorControl {
public:
	MotorControl();
	MotorControl(int rpmsensorpin, int speedpin);

	void requestCalibration();
	void update(long ms);

	void requestRpm(int rpm);
private:
	// Setup
	void setupSensorfun();
	int _sensorRpmPin;
	int _motorSpeedPin;

	// Update and mode selection
	void motorTick(long ms);
	void calibrateTick(long ms);
	MotorControlMode _mode;

	// Calibration & calibration settings
	int _maxCalibrationSpeed;
	bool _isCalibrated;
	std::vector<int, float> _calibrationMap; 

	// Motor settings
	int _requestedRPM;

	// Sensor data
	long _previousRpmTick;
	float _currentRPM;
};

#endif // MOTORCONTROL_H
