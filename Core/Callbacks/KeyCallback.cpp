#include "gpio.h"

#include "Button_Shared.hpp"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (displayButton.HandleInterrupt(GPIO_Pin)) {
        return;
    }

    if (motorButton.HandleInterrupt(GPIO_Pin)) {
        return;
    }

    if (ec11Button.HandleInterrupt(GPIO_Pin)) {
        return;
    }
}