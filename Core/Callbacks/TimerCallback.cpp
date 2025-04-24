#include "tim.h"

#include "Button_Shared.h"
#include "EC11_Shared.h"
#include "Motor_Shared.h"
#include "SSD1306_Shared.h"
#include "Status.h"
#include "strings.h"

static uint16_t targetFlashingScaler = 0;

void UpdateScreen(void) {
	// Power Off
	if (Status::motorStatus == Status::MotorStatus::PowerOff) {
		ssd1306.Clear();
		ssd1306.SetCursor(14, 23);
		ssd1306.WriteString("Power Off", SSD1306Fonts::Font_11x18);
		ssd1306.UpdateScreen();
		return;
	}

	char speedStr[8] = { 0 };

	// 目标速度百分比
	ssd1306.Clear();
	// ssd1306.SetCursor(0, 0);
	// ssd1306.WriteString("Target:", SSD1306Fonts::Font_11x18);

	auto length = int8ToString(Status::targetSpeedPercentage, speedStr);
	auto x = (128 - (length * 11)) / 2 - 1;
	ssd1306.SetCursor(x, 23);

	speedStr[length] = '%';

	// fill the rest of the string with spaces
	for (auto i = length + 1; i < 8; i++) {
		speedStr[i] = 0;
	}

	// X0 时常亮；X5 时 600ms 闪烁一次；X10 时 200ms 闪烁一次
	if (Status::speedChangeRate == Status::SpeedChangeRate::X0) {
		ssd1306.WriteString(speedStr, SSD1306Fonts::Font_11x18);
	} else if (Status::speedChangeRate == Status::SpeedChangeRate::X5) {
		if (targetFlashingScaler % 12 < 6) {
			ssd1306.WriteString(speedStr, SSD1306Fonts::Font_11x18);
		}
	} else if (Status::speedChangeRate == Status::SpeedChangeRate::X10) {
		if (targetFlashingScaler % 4 < 2) {
			ssd1306.WriteString(speedStr, SSD1306Fonts::Font_11x18);
		}
	}
	if (++targetFlashingScaler >= 24) {
		targetFlashingScaler = 0;
	}

	// // 输出速度百分比
	// ssd1306.SetCursor(0, 20);
	// ssd1306.WriteString("Speed:", SSD1306Fonts::Font_11x18);

	// length = int8ToString(Status::speedPercentage, speedStr);
	// speedStr[length] = '%';
	// ssd1306.WriteString(speedStr, SSD1306Fonts::Font_11x18);

	// // 显示状态
	// ssd1306.SetCursor(0, 40);
	// ssd1306.WriteString("Status: ", SSD1306Fonts::Font_11x18);

	// length = uint8ToString(static_cast<uint8_t>(Status::motorStatus), speedStr);
	// ssd1306.WriteString(speedStr, SSD1306Fonts::Font_11x18);

	ssd1306.UpdateScreen();
}

static uint16_t timer3_scaler = 0; // 0 ~ 999

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) { // 1ms
		// Buttons and EC11
		displayButton.Tick();
		motorButton.Tick();
		ec11Button.Tick();
		ec11.UpdateRotation();

		// 50ms
		if (timer3_scaler % 50 == 0) {

			// 电机缓慢减速逻辑
			if (Status::motorStatus == Status::MotorStatus::Decelerating) {
				if (Status::targetSpeedPercentage == 0) {
					Status::motorStatus = Status::MotorStatus::Shutdown;
					motor.Stop();
				} else {
					Status::targetSpeedPercentage = Status::targetSpeedPercentage - 1;
				}
			}

			// 电机实际速度控制
			if (Status::speedPercentage < Status::targetSpeedPercentage) {
				Status::speedPercentage = std::min(Status::targetSpeedPercentage, static_cast<int8_t>(Status::speedPercentage + 1));
			} else if (Status::speedPercentage > Status::targetSpeedPercentage) {
				Status::speedPercentage = std::max(Status::targetSpeedPercentage, static_cast<int8_t>(Status::speedPercentage - 1));
			}


			// 更新屏幕显示
			UpdateScreen();

		}

		// 800ms
		if (timer3_scaler == 800) {
			motor.SetSpeed(static_cast<uint16_t>(Status::speedPercentage) * 40); // 0~100 -> 0~4000
		}


		// 分频器归零
		if (++timer3_scaler >= 1000) {
			timer3_scaler = 0;
		}
	}
}
