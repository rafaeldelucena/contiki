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
#include <stdio.h>
/*---------------------------------------------------------------------------*/
#define CCxxx0_PARTNUM          0x30
#define CCxxx0_VERSION          0x31
#define READ_BURST              0xC0
/*---------------------------------------------------------------------------*/
PROCESS(part_number_spi_process, "SPI CC1101 part number reader");
AUTOSTART_PROCESSES(&part_number_spi_process);
/*---------------------------------------------------------------------------*/
/* Delay in us */
#define DELAY 10
/*---------------------------------------------------------------------------*/
static void
delay(void)
{
  clock_delay_usec(DELAY);
}
/*---------------------------------------------------------------------------*/
static void
arch_spi_deselect()
{
  ti_lib_gpio_pin_write(BOARD_LCD_CS, 1);
}
/*---------------------------------------------------------------------------*/
static void
arch_spi_select()
{
  ti_lib_gpio_pin_write(BOARD_LCD_CS, 0);
  delay();
}
/*---------------------------------------------------------------------------*/
static uint8_t
arch_spi_write(uint8_t output)
{
  int i;
  uint8_t input;
  input = 0;

  for(i=0; i < 8; i++) {
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
uint8_t
arch_spi_read(void)
{
  return arch_spi_write(0);
}
/*---------------------------------------------------------------------------*/
uint8_t
arch_spi_read_register(uint8_t addr)
{
  printf("[init] %s in [0x%02hhx] ", __PRETTY_FUNCTION__, addr);
  arch_spi_select();
  uint8_t address, resp, data;
  address = addr | READ_BURST;
  resp = arch_spi_write(address);
  printf("w [0x%02hhx] ", resp);
  data = arch_spi_write(0x00);
  arch_spi_deselect();
  printf("out [0x%02hhx] [done]\r\n", data);
  return data;
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(part_number_spi_process, ev, data)
{
  static uint8_t part_number_address, version_number_address;
  static uint8_t part_number, version_number;
  static struct etimer timer;

  PROCESS_BEGIN();
  
  ///* GPIO pin configuration, same of the LCD on srf06-cc1310 */
  ti_lib_ioc_pin_type_gpio_input(BOARD_IOID_SPI_MISO);
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_SPI_MOSI);
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_SPI_SCK);
  ti_lib_ioc_pin_type_gpio_output(BOARD_IOID_LCD_CS);


  /* Default output to clear chip select */
  ti_lib_gpio_pin_write(BOARD_LCD_CS, 1);

  /* The CLK is active low, we set it high when we aren't using it. */
  ti_lib_gpio_pin_clear(BOARD_SPI_SCK);

  part_number_address = CCxxx0_PARTNUM;
  version_number_address= CCxxx0_VERSION;

  etimer_set(&timer, CLOCK_CONF_SECOND);

  while (1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER) {
      part_number = arch_spi_read_register(part_number_address);
      //printf("the part number is: 0x%02hhx \r\n", part_number);
      version_number = arch_spi_read_register(version_number_address);
      //printf("the version number is: 0x%02hhx \r\n", version_number);
      etimer_reset(&timer);
    }
  }


  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
