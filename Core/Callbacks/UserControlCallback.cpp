#include <algorithm>

#include "Button_Shared.hpp"
#include "EC11_Shared.hpp"
#include "Status.hpp"

static void EC11ClockwiseCallback(void) {
	if (Status::motorStatus == Status::MotorStatus::Running || Status::motorStatus == Status::MotorStatus::Stopped) {
		Status::targetSpeedPercentage = std::min(Status::targetSpeedPercentage + 5, 100);
	}
}

static void EC11CounterClockwiseCallback(void) {
	if (Status::motorStatus == Status::MotorStatus::Running) {
		Status::targetSpeedPercentage = std::max(Status::targetSpeedPercentage - 5, 0);
		if (Status::targetSpeedPercentage == 0) {
			Status::motorStatus = Status::MotorStatus::Stopped;
		}
	}
}

static void MotorButtonShortPressCallback(void) {
	if (Status::motorStatus == Status::MotorStatus::Running) {
		Status::motorStatus = Status::MotorStatus::Decelerating;
	} else if (Status::motorStatus == Status::MotorStatus::Shutdown) {
		Status::motorStatus = Status::MotorStatus::Stopped;
	}
}

void RegisterUserControlCallbacks(void) {
	ec11.RegisterClockwiseCallback(EC11ClockwiseCallback);
	ec11.RegisterCounterClockwiseCallback(EC11CounterClockwiseCallback);
	motorButton.RegisterShortPressCallback(MotorButtonShortPressCallback);
}