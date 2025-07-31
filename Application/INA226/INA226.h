#pragma once

#include <cstdint>
#include <optional>

#include "i2c.h"

/**
 * @class INA226
 * @brief 用于 TI INA226 功率监控芯片的嵌入式 C++ 驱动
 *
 * 本驱动使用 STM32 HAL 库，通过 I2C 进行通信。
 * 它被设计为在定时器中断或主循环中周期性调用，以实现连续监控。
 * 驱动不使用动态内存分配，并使用现代 C++ 特性以提高安全性和可读性。
 */
class INA226 {
public:
	/**
	 * @brief 定义操作的状态码
	 */
	enum class Status : uint8_t {
		OK,           // 操作成功
		Error,        // 通用错误
		ErrorRead,    // HAL 读取操作失败
		ErrorWrite,   // HAL 写入操作失败
		ErrorConfig   // 配置参数无效
	};

	/**
	 * @brief 构造函数
	 * @param i2cHandle 指向已初始化的 HAL I2C 句柄的指针
	 * @param deviceAddress 芯片的 I2C 地址。注意：这是8位地址（7位地址左移一位）
	 */
	INA226(I2C_HandleTypeDef *i2cHandle, uint8_t deviceAddress = (0x40 << 1))
		: _i2cHandle(i2cHandle),
		_deviceAddress(deviceAddress),
		_lastVoltage(std::nullopt),
		_lastCurrent(std::nullopt),
		_lastPower(std::nullopt) { }

	/**
	 * @brief 初始化芯片，写入默认配置
	 * @return 一个 Status 码，指示操作的结果
	 */
	Status Init();

	/**
	 * @brief 更新传感器读数。此函数应被周期性调用（例如，每 100ms）
	 * @return 一个 Status 码，指示操作的结果
	 */
	Status Update();

	/**
	 * @brief 获取最近一次成功更新的电压值
	 * @return 一个包含电压值（伏特）的 std::optional，如果上次读取失败则为空
	 */
	std::optional<float> GetVoltage() const;

	/**
	 * @brief 获取最近一次成功更新的电流值
	 * @return 一个包含电流值（安培）的 std::optional，如果上次读取失败则为空
	 */
	std::optional<float> GetCurrent() const;

	/**
	 * @brief 获取最近一次成功更新的功率值
	 * @return 一个包含功率值（瓦特）的 std::optional，如果上次读取失败则为空
	 */
	std::optional<float> GetPower() const;

private:
	// --- 编译期配置常量 ---
	static constexpr float SHUNT_RESISTANCE = 0.005f;   // 5mΩ
	static constexpr float MAX_EXPECTED_CURRENT = 10.0f; // 10A
	static constexpr uint16_t I2C_TIMEOUT_MS = 20;

	// --- INA226 寄存器地址 ---
	enum class Register : uint8_t {
		Config = 0x00,
		ShuntVoltage = 0x01,
		BusVoltage = 0x02,
		Power = 0x03,
		Current = 0x04,
		Calibration = 0x05,
	};

	// --- 配置寄存器位域常量 ---
	enum class ConvTime : uint16_t {
		CT_140us = 0b000,
		CT_204us = 0b001,
		CT_332us = 0b010,
		CT_588us = 0b011,
		CT_1_1ms = 0b100,
		CT_2_116ms = 0b101,
		CT_4_156ms = 0b110,
		CT_8_244ms = 0b111,
	};
	enum class AvgMode : uint16_t {
		AVG_1 = 0b000,
		AVG_4 = 0b001,
		AVG_16 = 0b010,
		AVG_64 = 0b011,
		AVG_128 = 0b100,
		AVG_256 = 0b101,
		AVG_512 = 0b110,
		AVG_1024 = 0b111,
	};
	enum class OpMode : uint16_t {
		PowerDown = 0b000,
		ShuntAndBusContinuous = 0b111,
	};

	// --- 私有方法 ---
	Status _WriteRegister(Register reg, uint16_t value);
	std::optional<uint16_t> _ReadRegister(Register reg);

	// --- 成员变量 ---
	I2C_HandleTypeDef *_i2cHandle;
	const uint8_t _deviceAddress;

	float _currentLsb = 0.0f;
	float _powerLsb = 0.0f;

	std::optional<float> _lastVoltage;
	std::optional<float> _lastCurrent;
	std::optional<float> _lastPower;
};