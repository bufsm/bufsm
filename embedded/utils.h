
#ifndef __bufsm_utils__
#define __bufsm_utils__

#include <Arduino.h>
#include <TinyGPS.h>


#define TELIT_SIM
// #define DEBUG

#ifdef TELIT_SIM
#define APN "internetm2m.air.com"
#else
#define APN "zap.vivo.com.br"
#endif

#define URL "iot.eclipse.org"

#define GPS_PRECISION (1E5)

const byte MQTT_CONNECT[] = {0x10, 19, 0x00, 0x06, 0x4d, 0x51, 0x49, 0x73, 0x64, 0x70, 0x03, 0x02, 0x00, 0x3c, 0x00, 5, 'B', 'U', 'F', 'S', 'M'}; // Protocol version: 3, clientID = BUFSM
#define MQTT_PUBLISH_FIRST_BYTE 0x31 // QOS 0, Retain
#define MQTT_TOPIC "b123"

#define MODULE_RESET 6
#define MODULE_PWR 7
#define BLUE_LED 5
#define GREEN_LED 4
#define RED_LED 3

#define ledOn(led) digitalWrite(led, 1)
#define ledOff(led) digitalWrite(led, 0)
#define ledToggle(led) digitalWrite(led, 1^digitalRead(led))

#define SerialAT Serial
#ifdef DEBUG
#define SerialDebug Serial
#endif

typedef struct {
  float lat;
  float lng;
} coords_t;

void wait_module_init();
uint8_t gprs_init();
void gps_init();
// void gprs_connect();
uint8_t gprs_send_coods(coords_t);
void gps_get_coordinates(coords_t*);
void gprs_reset();
void gprs_powerCycle();
int waitFor(const char *, const char *, unsigned int);


enum at_command {
  AT = 0,
  DIS_ECHO,
  SET_BAUD_RATE,
  NETWORK_REGIST,
  EN_SHOW_OPERATOR,
  CHECK_OPERATOR,
  ATTACH,
  SET_PDP_CONTEXT,
  ACTIVATE_PDP_CONTEXT,
  GET_IP,
  CONN_TCP,
  SEND_DATA,
  CLOSE_TCP,
  GPS_ON,
  GPS_OFF,
  GPS_AT_ON,
  GPS_AT_OFF,
  GPS_ANS
};

#endif
