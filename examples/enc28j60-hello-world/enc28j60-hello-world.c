#include "contiki.h"
#include "enc28j60.h"
#include "ti-lib.h"
#include <stdio.h> /* For printf() */
#include <string.h> /* For printf() */
/*---------------------------------------------------------------------------*/
/* We declare the two processes */
PROCESS(enc28j60_hello_world_process, "Enc28j60 Hello World process");

/* We require the processes to be started automatically */
AUTOSTART_PROCESSES(&enc28j60_hello_world_process);
/*---------------------------------------------------------------------------*/
/* Implementation of the first process */
PROCESS_THREAD(enc28j60_hello_world_process, ev, data)
{
  // variables are declared static to ensure their values are kept
  // between kernel calls.
  static int counter;
  static struct etimer timer;
  static uint8_t mac_address[] = {0x00, 0xBE, 0xBA, 0x00, 0xCA, 0xFE};
  static uint8_t new_mac_address[6];
  static uint8_t new_macs[][6] = {
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
    {0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA},
    {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
    {0xBE, 0xBA, 0x00, 0xC0, 0xCA, 0x00},
    {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB},
    {0x9A, 0xB7, 0x35, 0x5D, 0xDB, 0x01},
  };

  PROCESS_BEGIN();

  enc28j60_init(mac_address);

  etimer_set(&timer, CLOCK_CONF_SECOND);

  while (1)
  {
    // wait here for an event to happen
    PROCESS_WAIT_EVENT();

    // if the event is the timer event as expected...
    if(ev == PROCESS_EVENT_TIMER) {
      memset(new_mac_address, 0, 6);
      enc28j60_get_mac_address(new_mac_address);

      printf("MAC: %02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx\r\n",
             new_mac_address[0],
             new_mac_address[1],
             new_mac_address[2],
             new_mac_address[3],
             new_mac_address[4],
             new_mac_address[5]);

      enc28j60_set_mac_address(new_macs[counter % 6]);
      counter++;
      etimer_reset(&timer);
    }

  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
