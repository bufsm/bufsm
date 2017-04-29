
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

coords_t coord;

void setup() {
  //  pinMode(MODULE_PWR, OUTPUT);
  //  digitalWrite(MODULE_PWR, 0);
  //  pinMode(MODULE_RESET, OUTPUT);
  //  digitalWrite(MODULE_RESET, 0);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);


#ifdef DEBUG
  Serial2.begin(115200);
#endif
  Serial.begin(115200);
  Serial.setTimeout(300);
  Serial.flush();

  //  gprs_powerCycle();

  wait_module_init();

  gps_init();

  gps_get_coordinates(&coord);
  while(! gprs_init());
  //gprs_send_coods(coord);
}

void loop() {
  gps_get_coordinates(&coord);
  if (! gprs_send_coods(coord)){
    while(! gprs_init());
  }
}
