#include "Status.hpp"

namespace Status {

    int8_t speedPercentage = 0;       // 转速百分比
    int8_t targetSpeedPercentage = 0; // 目标转速百分比

    // 电机状态
    MotorStatus motorStatus = MotorStatus::PowerOff;

}