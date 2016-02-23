/*---------------------------------------------------------------------------*/
/**
 * \file
 *         A very simple app echoing received packets from SPI Slave on stdin
 *         and backwards to the slave.
 * \author
 *         Rafael de Lucena Valle <rafaeldelucena@gmail.com>
 */
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include "board-spi.h"
#include "ti-lib.h"
#include "dev/leds.h"
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_spi_process, "SPI Hello world process");
AUTOSTART_PROCESSES(&hello_world_spi_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_spi_process, ev, data)
{
  PROCESS_BEGIN();

  static uint8_t in_data, out_data;
  board_spi_open(4000000, BOARD_IOID_LCD_SCK);

  /* GPIO pin configuration, same of the LCD on srf06-cc1310 */
  ti_lib_ioc_pin_type_gpio_input(BOARD_IOID_SPI_MISO);
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_SPI_MOSI);
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_SPI_SCK);
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_LCD_CS);

  /* Default output to clear chip select */
  ti_lib_gpio_pin_write(BOARD_LCD_CS, 1);

  while (1) {
    if (board_spi_read(&in_data, sizeof(uint8_t))) {
      PRINTF("0x%02hhX", in_data);
      out_data = in_data;
      if (!board_spi_write(out_data, sizeof(uint8_t))) {
        leds_on(LEDS_RED);
        PRINTF("\r\n[WRITE ERROR]\r\n");
      }
    } else {
      PRINTF("\r\n[READ ERROR]\r\n");
    }

    PROCESS_PAUSE();
  }

  board_spi_close();

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
