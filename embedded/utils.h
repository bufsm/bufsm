
#ifndef __bufsm_utils__
#define __bufsm_utils__

#include <Arduino.h>
#include <TinyGPS.h>
#include <stdint.h>
#include<string.h>

//#define DEBUG
//#define DEBUG_GPS


#define GPS_RESET 7
#define BLUE_LED 10
#define GREEN_LED 9
#define RED_LED 8

#define ledOn(led) digitalWrite(led, 1)
#define ledOff(led) digitalWrite(led, 0)
#define ledToggle(led) digitalWrite(led, 1^digitalRead(led))

#define SerialAT Serial
#ifdef DEBUG
#define SerialDebug Serial
#endif

#define MAX_NULL_RECEIVED 5
#define MAX_INCORRECT_GPS_PARAMETERS 5*7

typedef struct {
  float lat;
  float lng;
} coords_t;


void gps_get_coordinates_ublox(coords_t *c);

void float_to_string(double number, char *res, int afterpoint);
void lat_long_to_bytes(double lat_long, uint8_t *res);
void number_to_byte(double number, uint8_t *res, int afterpoint, int bitsbeforepoint, int bitsafterpoint);

int waitFor(const char *, const char *, unsigned int);

#endif
