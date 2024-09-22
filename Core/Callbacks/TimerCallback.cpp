#include "tim.h"

#include "Button_Shared.hpp"
#include "Motor_Shared.hpp"
#include "SSD1306_Shared.hpp"
#include "Status.hpp"
#include "strings.hpp"

void UpdateScreen(void) {
    ssd1306.Clear();
    ssd1306.SetCursor(0, 0);
    ssd1306.WriteString("Speed: ", SSD1306Fonts::Font_11x18);

    char speedStr[5] = { 0 };
    auto length = int8ToString(Status::speedPercentage, speedStr);
    speedStr[length] = '%';
    ssd1306.WriteString(speedStr, SSD1306Fonts::Font_11x18);

    ssd1306.UpdateScreen();
}

static uint16_t timer3_scaler = 0; // 0 ~ 999

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) { // 1ms
        // Buttons
        displayButton.Tick();
        motorButton.Tick();
        ec11Button.Tick();

        // 100ms
        if (timer3_scaler % 100 == 0) {

            // 电机缓慢减速逻辑
            if (Status::motorStatus == Status::MotorStatus::Decelerating) {
                if (Status::targetSpeedPercentage == 0) {
                    Status::motorStatus = Status::MotorStatus::Shutdown;
                } else {
                    Status::targetSpeedPercentage = Status::targetSpeedPercentage - 1;
                }
            }

            // 电机实际速度控制
            Status::speedPercentage = Status::targetSpeedPercentage * 0.1 + Status::speedPercentage * 0.9;
            motor.SetSpeed(static_cast<uint16_t>(Status::speedPercentage) * 40); // 0~100 -> 0~4000

            // 更新屏幕显示
            UpdateScreen();

        }




        // 分频器归零
        if (++timer3_scaler >= 1000) {
            timer3_scaler = 0;
        }
    }
}