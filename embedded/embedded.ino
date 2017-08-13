
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"

coords_t coord;
uint8_t counter;

void setup() {
  counter = 0;
  pinMode(MODULE_PWR, OUTPUT);
  digitalWrite(MODULE_PWR, 0);
  pinMode(MODULE_RESET, OUTPUT);
  digitalWrite(MODULE_RESET, 0);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  gprs_reset();

#ifdef DEBUG
  SerialDebug.begin(115200);
#endif
  SerialAT.begin(115200);
  SerialAT.setTimeout(300);
  SerialAT.flush();

  gprs_powerCycle();
  wait_module_init();
  gps_init();
  gps_get_coordinates(&coord);

  while (! gprs_init()) {
    if (counter++ >= 5) {
      counter = 0;
      gprs_powerCycle();
      wait_module_init();
    }
  }
}

void loop() {
  static uint8_t init_counter  = 0;
  gps_get_coordinates(&coord);

  //gprs_send_coods(&coord);
  if (! gprs_send_coods(&coord)) {

    if (init_counter++ >= 5) {
      init_counter = 0;

      while (! gprs_init()) {

        if (counter++ >= 5) {
          counter = 0;
          gprs_powerCycle();
          wait_module_init();
        }
      }
    }
  }
}
