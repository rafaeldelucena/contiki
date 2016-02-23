/*---------------------------------------------------------------------------*/
#include "ti-lib.h"
#include "board-spi.h"
#include "dev/leds.h"
#include <stdio.h>
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_init(void)
{
  board_spi_open(4000000, BOARD_IOID_SPI_ETHERNET_CLK);

  /* GPIO pin configuration */
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_SPI_ETHERNET_CS);

  /* Default output to clear chip select */
  ti_lib_gpio_pin_write(BOARD_SPI_ETHERNET_CS, 1);
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_select(void)
{
  ti_lib_gpio_pin_write(BOARD_SPI_ETHERNET_CS, 0);
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_deselect(void)
{
  ti_lib_gpio_pin_write(BOARD_SPI_ETHERNET_CS, 1);
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_write(uint8_t output)
{
  if (!board_spi_write(&output, sizeof(uint8_t))) {
    leds_on(LEDS_RED);
    printf("[ERROR] %s\r\n", __PRETTY_FUNCTION__);
  }
}
/*---------------------------------------------------------------------------*/
uint8_t
enc28j60_arch_spi_read(void)
{
  uint8_t buf;
  if (!board_spi_read(&buf, sizeof(uint8_t))) {
    leds_on(LEDS_RED);
    printf("[ERROR] %s\r\n", __PRETTY_FUNCTION__);
  }
  return buf;
}
/*---------------------------------------------------------------------------*/
