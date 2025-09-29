/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAML10 has 9 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8

//#define DRDY_0 GPIO(GPIO_PORTA, 0)
//#define SPI_CS0 GPIO(GPIO_PORTA, 1)
//#define SPI_MOSI GPIO(GPIO_PORTA, 2)
//#define SPI_SCK GPIO(GPIO_PORTA, 3)
//#define SPI_MISO GPIO(GPIO_PORTA, 5)
#define MB_CFG GPIO(GPIO_PORTA, 27)
#define ResetSensorPin GPIO(GPIO_PORTA, 7)
#define I2C_SDA GPIO(GPIO_PORTA, 16)
#define I2C_SCL GPIO(GPIO_PORTA, 17)
#define USART_TX GPIO(GPIO_PORTA, 8)
#define USART_RX GPIO(GPIO_PORTA, 9)
#define RE GPIO(GPIO_PORTA, 11)
#define DE GPIO(GPIO_PORTA, 10)

#endif // ATMEL_START_PINS_H_INCLUDED

//#define GPIO_PIN_FUNCTION_A 0
//#define GPIO_PIN_FUNCTION_B 1
//#define GPIO_PIN_FUNCTION_C 2
//#define GPIO_PIN_FUNCTION_D 3
//#define GPIO_PIN_FUNCTION_E 4
//#define GPIO_PIN_FUNCTION_F 5
//#define GPIO_PIN_FUNCTION_G 6
//#define GPIO_PIN_FUNCTION_H 7
//#define GPIO_PIN_FUNCTION_I 8
//
//#define DRDY_0 GPIO(GPIO_PORTA, 0)
//#define SPI_CS0 GPIO(GPIO_PORTA, 1)
//#define SPI_MOSI GPIO(GPIO_PORTA, 2)
//#define SPI_SCK GPIO(GPIO_PORTA, 3)
//#define SPI_MISO GPIO(GPIO_PORTA, 5)
//#define MB_CFG GPIO(GPIO_PORTA, 6)
//#define MB_CFG_ext_pull GPIO(GPIO_PORTA, 7)
//#define I2C_SDA GPIO(GPIO_PORTA, 16)
//#define I2C_SCL GPIO(GPIO_PORTA, 17)
//#define USART_TX GPIO(GPIO_PORTA, 22)
//#define USART_RX GPIO(GPIO_PORTA, 23)
//#define RE GPIO(GPIO_PORTA, 24)
//#define DE GPIO(GPIO_PORTA, 25)
