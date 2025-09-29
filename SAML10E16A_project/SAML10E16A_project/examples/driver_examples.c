/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

#ifndef NVMCTRL_ROW_SIZE
#define NVMCTRL_ROW_SIZE (NVMCTRL_PAGE_SIZE * NVMCTRL_ROW_PAGES)
#endif
static uint8_t flash_src_data[FLASH_PAGE_SIZE];
static uint8_t flash_chk_data[FLASH_PAGE_SIZE];

/**
 * Example of using FLASH_0 to read and write buffer.
 */
void FLASH_0_example(void)
{
	uint32_t i;

	/* Target address.
	 * Make sure the address is reasonable to your application.
	 * It might a address in NVM memory of FLASH, DATAFLASH, AUX FLASH.
	 */
	uint32_t target_addr = FLASH_ADDR + FLASH_SIZE - NVMCTRL_ROW_SIZE;

	/* Initialize source data */
	for (i = 0; i < FLASH_PAGE_SIZE; i++) {
		flash_src_data[i] = i;
	}

	/* Erase row in flash */
	FLASH_0_erase_row(target_addr);

	/* Write data to flash */
	FLASH_0_write_page(target_addr, flash_src_data, FLASH_PAGE_SIZE);

	/* Read data from flash */
	FLASH_0_read(target_addr, flash_chk_data, FLASH_PAGE_SIZE);

	/* Check data */
	for (i = 0; i < FLASH_PAGE_SIZE; i++) {
		if (flash_src_data[i] != flash_chk_data[i]) {
			while (1)
				; /* Error happen */
		}
	}
}

static struct timer_task TIMER_0_task1, TIMER_0_task2;

/**
 * Example of using TIMER_0.
 */
static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_0_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_0_example(void)
{
	TIMER_0_task1.interval = 100;
	TIMER_0_task1.cb       = TIMER_0_task1_cb;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_0_task2.interval = 200;
	TIMER_0_task2.cb       = TIMER_0_task2_cb;
	TIMER_0_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_add_task(&TIMER_0, &TIMER_0_task2);
	timer_start(&TIMER_0);
}

static struct timer_task TIMER_2_task1, TIMER_2_task2;

/**
 * Example of using TIMER_2.
 */
static void TIMER_2_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_2_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_2_example(void)
{
	TIMER_2_task1.interval = 100;
	TIMER_2_task1.cb       = TIMER_2_task1_cb;
	TIMER_2_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_2_task2.interval = 200;
	TIMER_2_task2.cb       = TIMER_2_task2_cb;
	TIMER_2_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_2, &TIMER_2_task1);
	timer_add_task(&TIMER_2, &TIMER_2_task2);
	timer_start(&TIMER_2);
}

/**
 * Example of using SPI_0 to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_0[12] = "Hello World!";

void SPI_0_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_0, &io);

	spi_m_sync_enable(&SPI_0);
	io_write(io, example_SPI_0, 12);
}

void I2C_0_example(void)
{
	struct io_descriptor *I2C_0_io;

	i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_0_io);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, 0x12, I2C_M_SEVEN);
	io_write(I2C_0_io, (uint8_t *)"Hello World!", 12);
}

/*
 * \Write data to usart interface
 *
 * \param[in] buf Data to write to usart
 * \param[in] length The number of bytes to write
 *
 * \return The number of bytes written.
 */
static uint32_t USART_0_write(const uint8_t *const buf, const uint16_t length)
{
	uint32_t offset = 0;

	ASSERT(buf && length);

	while (!USART_0_is_byte_sent())
		;
	do {
		USART_0_write_byte(buf[offset]);
		while (!USART_0_is_byte_sent())
			;
	} while (++offset < length);

	return offset;
}

/*
 * \Read data from usart interface
 *
 * \param[in] buf A buffer to read data to
 * \param[in] length The size of a buffer
 *
 * \return The number of bytes read.
 */
static uint32_t USART_0_read(uint8_t *const buf, const uint16_t length)
{
	uint32_t offset = 0;

	ASSERT(buf && length);

	do {
		while (!USART_0_is_byte_received())
			;
		buf[offset] = USART_0_read_byte();
	} while (++offset < length);

	return offset;
}

/**
 * Example of using USART_0 to write the data which received from the usart interface to IO.
 */
void USART_0_example(void)
{
	uint8_t data[2];

	if (USART_0_read(data, sizeof(data)) == 2) {
		USART_0_write(data, 2);
	}
}

/**
 * Example of using WDT_0.
 */
void WDT_0_example(void)
{
	uint32_t clk_rate;
	uint16_t timeout_period;

	clk_rate       = 1000;
	timeout_period = 4096;
	wdt_set_timeout_period(&WDT_0, clk_rate, timeout_period);
	wdt_enable(&WDT_0);
}
