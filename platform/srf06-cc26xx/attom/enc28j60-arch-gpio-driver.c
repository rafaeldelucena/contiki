/*---------------------------------------------------------------------------*/
#include "clock.h"
#include "ti-lib.h"
#include "dev/leds.h"
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/* Delay in us */
#define DELAY 10
/*---------------------------------------------------------------------------*/
inline static void
delay(void)
{
  clock_delay_usec(DELAY);
}
/*---------------------------------------------------------------------------*/
static uint8_t
enc28j60_arch_spi_write_byte(uint8_t output)
{
  int i;
  uint8_t input;
  input = 0;

  for(i = 0; i < 8; i++) {
    /* Write data on MOSI pin */
    if(output & 0x80) {
      ti_lib_gpio_pin_write(BOARD_SPI_MOSI, 1);
    } else {
      ti_lib_gpio_pin_clear(BOARD_SPI_MOSI);
    }
    output <<= 1;

    /* Set clock high  */
    ti_lib_gpio_pin_write(BOARD_SPI_SCK, 1);
    delay();

    /* Read data from MISO pin */
    input <<= 1;
    if(ti_lib_gpio_pin_read(BOARD_SPI_MISO) != 0) {
      input |= 0x1;
    }

    /* Set clock low */
    ti_lib_gpio_pin_clear(BOARD_SPI_SCK);
    delay();
  }
  return input;
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_init(void)
{
  leds_on(LEDS_YELLOW);
  /* GPIO pin configuration */

  ti_lib_ioc_pin_type_gpio_input(BOARD_IOID_SPI_MISO);
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_SPI_MOSI);
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_SPI_SCK);
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_SPI_ETHERNET_CS);
//TODO: reset pin must be used???

  /* Default output to clear chip select */
  ti_lib_gpio_pin_write(BOARD_SPI_ETHERNET_CS, 1);

  /* The CLK is active low, we set it high when we aren't using it. */
  ti_lib_gpio_pin_clear(BOARD_SPI_SCK);
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_select(void)
{
  leds_on(LEDS_RED);
  ti_lib_gpio_pin_write(BOARD_SPI_ETHERNET_CS, 0);
  delay();
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_deselect(void)
{
  leds_off(LEDS_RED);
  ti_lib_gpio_pin_write(BOARD_SPI_ETHERNET_CS, 1);
}
/*---------------------------------------------------------------------------*/
uint8_t
enc28j60_arch_spi_read(void)
{
  leds_toggle(LEDS_GREEN);
  return enc28j60_arch_spi_write_byte(0);
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_write(uint8_t output)
{
  leds_toggle(LEDS_BLUE);
  enc28j60_arch_spi_write_byte(output);
}
/*---------------------------------------------------------------------------*/
