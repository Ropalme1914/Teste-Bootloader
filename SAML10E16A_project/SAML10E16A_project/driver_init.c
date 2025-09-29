/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

struct timer_descriptor      TIMER_0;
struct timer_descriptor      TIMER_2;
struct spi_m_sync_descriptor SPI_0;

struct i2c_m_sync_desc I2C_0;

struct wdt_descriptor WDT_0;

/**
 * \brief NVMCTRL initialization function
 *
 * Enables NVMCTRL peripheral, clocks and initializes driver
 */
void FLASH_0_clock_init(void)
{
	hri_mclk_set_AHBMASK_NVMCTRL_bit(MCLK);
	hri_mclk_set_APBBMASK_NVMCTRL_bit(MCLK);
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_0_init(void)
{
	hri_mclk_set_APBCMASK_TC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, CONF_GCLK_TC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	timer_init(&TIMER_0, TC0, _tc_get_timer());
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_2_init(void)
{
	hri_mclk_set_APBCMASK_TC2_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC2_GCLK_ID, CONF_GCLK_TC2_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	timer_init(&TIMER_2, TC2, _tc_get_timer());
}

//void SPI_0_PORT_init(void)
//{
//
	////// Set pin direction to input
	////gpio_set_pin_direction(SPI_MISO, GPIO_DIRECTION_IN);
	////gpio_set_pin_pull_mode(SPI_MISO,
	                       ////// <y> Pull configuration
	                       ////// <id> pad_pull_config
	                       ////// <GPIO_PULL_OFF"> Off
	                       ////// <GPIO_PULL_UP"> Pull-up
	                       ////// <GPIO_PULL_DOWN"> Pull-down
	                       ////GPIO_PULL_OFF);
	////gpio_set_pin_function(SPI_MISO, PINMUX_PA05D_SERCOM0_PAD1);
//
	////gpio_set_pin_level(SPI_MOSI,
	                   ////// <y> Initial level
	                   ////// <id> pad_initial_level
	                   ////// <false"> Low
	                   ////// <true"> High
	                   ////true);
	////// Set pin direction to output
	////gpio_set_pin_direction(SPI_MOSI, GPIO_DIRECTION_OUT);
	////gpio_set_pin_function(SPI_MOSI, PINMUX_PA02D_SERCOM0_PAD2);
//
	////gpio_set_pin_level(SPI_SCK,
	                   ////// <y> Initial level
	                   ////// <id> pad_initial_level
	                   ////// <false"> Low
	                   ////// <true"> High
	                   ////true);
	////// Set pin direction to output
	////gpio_set_pin_direction(SPI_SCK, GPIO_DIRECTION_OUT);
	////gpio_set_pin_function(SPI_SCK, PINMUX_PA03D_SERCOM0_PAD3);
//}

//void SPI_0_CLOCK_init(void)
//{
	//hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	//hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	//hri_mclk_set_APBCMASK_SERCOM0_bit(MCLK);
//}

//void SPI_0_init(void)
//{
	//SPI_0_CLOCK_init();
	//spi_m_sync_init(&SPI_0, SERCOM0);
	//SPI_0_PORT_init();
//}

void I2C_0_PORT_init(void)
{

	gpio_set_pin_pull_mode(I2C_SDA,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(I2C_SDA, PINMUX_PA16C_SERCOM1_PAD0);

	gpio_set_pin_pull_mode(I2C_SCL,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(I2C_SCL, PINMUX_PA17C_SERCOM1_PAD1);
}

void I2C_0_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBCMASK_SERCOM1_bit(MCLK);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();
	i2c_m_sync_init(&I2C_0, SERCOM1);
	I2C_0_PORT_init();
}

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void USART_0_CLOCK_init()
{

	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBCMASK_SERCOM2_bit(MCLK);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void USART_0_PORT_init()
{

	gpio_set_pin_function(USART_TX, PINMUX_PA08D_SERCOM2_PAD0);

	gpio_set_pin_function(USART_RX, PINMUX_PA09D_SERCOM2_PAD1);
	
	//gpio_set_pin_function(USART_TX, PINMUX_PA22D_SERCOM2_PAD0);
//
	//gpio_set_pin_function(USART_RX, PINMUX_PA23D_SERCOM2_PAD1);
}

void WDT_0_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_WDT_bit(MCLK);
}

void WDT_0_init(void)
{
	WDT_0_CLOCK_init();
	wdt_init(&WDT_0, WDT);
}

void system_init(void)
{
	init_mcu();

	//// GPIO on PA00
	//// Set pin direction to input
	//gpio_set_pin_direction(DRDY_0, GPIO_DIRECTION_IN);
	//gpio_set_pin_pull_mode(DRDY_0,
	                       //// <y> Pull configuration
	                       //// <id> pad_pull_config
	                       //// <GPIO_PULL_OFF"> Off
	                       //// <GPIO_PULL_UP"> Pull-up
	                       //// <GPIO_PULL_DOWN"> Pull-down
	                       //GPIO_PULL_UP);
	//gpio_set_pin_function(DRDY_0, GPIO_PIN_FUNCTION_OFF);

	//// GPIO on PA01
	//gpio_set_pin_level(SPI_CS0,
	                   //// <y> Initial level
	                   //// <id> pad_initial_level
	                   //// <false"> Low
	                   //// <true"> High
	                   //true);
	//// Set pin direction to output
	//gpio_set_pin_direction(SPI_CS0, GPIO_DIRECTION_OUT);
	//gpio_set_pin_function(SPI_CS0, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA06
	// Set pin direction to input
	gpio_set_pin_direction(MB_CFG, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(MB_CFG,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);
	gpio_set_pin_function(MB_CFG, GPIO_PIN_FUNCTION_OFF);

	//GPIO on PA07
	gpio_set_pin_level(ResetSensorPin,
	                   //// <y> Initial level
	                   //// <id> pad_initial_level
	                   //// <false"> Low
	                   //// <true"> High
	                   true);
	//// Set pin direction to output
	gpio_set_pin_direction(ResetSensorPin, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(ResetSensorPin, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA
	gpio_set_pin_level(RE,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);
	// Set pin direction to output
	gpio_set_pin_direction(RE, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(RE, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA
	gpio_set_pin_level(DE,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);
	// Set pin direction to output
	gpio_set_pin_direction(DE, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(DE, GPIO_PIN_FUNCTION_OFF);

	FLASH_0_clock_init();
	FLASH_0_init();

	TIMER_0_init();
	TIMER_2_init();

	//SPI_0_init();

	I2C_0_init();

	USART_0_CLOCK_init();
	USART_0_init();
	USART_0_PORT_init();

	WDT_0_init();
}
