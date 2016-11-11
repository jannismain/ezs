#include <stdio.h>
#include <cyg/hal/hal_io.h>

#include <pkgconf/io_serial_cortexm_stm32.h>
#include <pkgconf/io_serial.h>

#include <cyg/io/ttyio.h>
#include <cyg/io/io.h>

#include "ezs_serial.h"

#ifdef EZS_DUMB_SERIAL
	// we expect tty0 = UART2 to be configured as polled IO (bufsize=0) in ECOS
	#if (CYGNUM_IO_SERIAL_CORTEXM_STM32_SERIAL1_BUFSIZE != 0)
	#error "buffersize UART2 aka /dev/ser1 must be 0 but it isn't! (see also: EZS_DUMB_SERIAL)"
	#endif
#else // i.e. if !EZS_DUMB_SERIAL
	#if (CYGNUM_IO_SERIAL_CORTEXM_STM32_SERIAL1_BUFSIZE == 0)
	#error "buffersize UART2 aka /dev/ser1 must be nonzero but it isn't! (see also: EZS_DUMB_SERIAL)"
	#endif
#endif


extern "C" {

#ifdef EZS_DUMB_SERIAL
void ezs_serial_init(void) {
	// enable the rxne interrupt generation.

	uint32_t cr1;
	HAL_READ_UINT32(CYGHWR_HAL_STM32_UART2 + CYGHWR_HAL_STM32_UART_CR1, cr1);
	HAL_WRITE_UINT32(CYGHWR_HAL_STM32_UART2 + CYGHWR_HAL_STM32_UART_CR1, cr1 | CYGHWR_HAL_STM32_UART_CR1_RXNEIE);
}
#else // i.e. if !EZS_DUMB_SERIAL
static cyg_io_handle_t handle;

void ezs_serial_init(void) {
	// open /dev/ser1
	printf("lookup = %d\n", cyg_io_lookup("/dev/ser1", &handle));
}
#endif

#ifdef EZS_DUMB_SERIAL
void ezs_serial_putc(uint8_t c) {
	cyg_uint32 stat;
	const CYG_ADDRWORD base = CYGHWR_HAL_STM32_UART2;
	do
	{
		HAL_READ_UINT32(base + CYGHWR_HAL_STM32_UART_SR, stat);
	} while ((stat & CYGHWR_HAL_STM32_UART_SR_TC) == 0);

	HAL_WRITE_UINT32( base + CYGHWR_HAL_STM32_UART_DR, c);
}

uint8_t ezs_serial_getc(void) {
	// get a byte from the data register
	const CYG_ADDRWORD base = CYGHWR_HAL_STM32_UART2; 
	cyg_uint32 c;
	HAL_READ_UINT32( base + CYGHWR_HAL_STM32_UART_DR, c);
	return c;
}

uint8_t ezs_serial_char_available(void) {
	cyg_uint32 stat;
	const CYG_ADDRWORD base = CYGHWR_HAL_STM32_UART2; 
	HAL_READ_UINT32(base + CYGHWR_HAL_STM32_UART_SR, stat);
	
	return (stat & CYGHWR_HAL_STM32_UART_SR_RXNE ) ? 1 : 0;
}
#else
uint8_t ezs_serial_blocking_getc(void) {
	char buf;
	cyg_uint32 len=1;
	cyg_io_read(handle, &buf, &len);
	return buf;
}

void ezs_serial_putc(uint8_t c)
{
	putchar(c);
}
#endif

}
