#ifndef __SW_I2C_DRIVER_H
#define __SW_I2C_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define I2C_SPEED_HZ 100 // 可设置为 100 或 400

typedef enum
{
    ACK = 0,
    NACK
} I2C_ACKTypedef;

typedef enum
{
    I2C_OK = 0,
    I2C_ERROR,
    I2C_NO_ACK_ON_ADDRESS,
    I2C_NO_ACK_ON_REG,
    I2C_NO_ACK_ON_DATA,
    I2C_NO_ACK_ON_READ_ADDRESS
} I2C_StatusTypeDef;

typedef struct
{
    void (*set_scl)(bool level);
    void (*set_sda)(bool level);
    bool (*get_sda)(void);
    void (*delay_us)(uint16_t us);
} sw_i2c_io_interface_t;

// I2C配置和句柄
typedef struct
{
    sw_i2c_io_interface_t io_if;
} sw_i2c_handle_t;

#ifdef __cplusplus
extern "C"
{
#endif

    void SW_I2C_Init(sw_i2c_handle_t *hi2c);
    I2C_StatusTypeDef SW_I2C_Transmit(sw_i2c_handle_t *hi2c, uint8_t addr, uint8_t reg, const uint8_t *data, uint8_t len);
    I2C_StatusTypeDef SW_I2C_Receive(sw_i2c_handle_t *hi2c, uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif // __SW_I2C_DRIVER_H
