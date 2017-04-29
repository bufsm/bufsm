
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

#define URL "bufsm-dalmago.rhcloud.com"

#define WRITE_TOKEN "TOKEN HERE"
#define GET "GET /linha/1/"WRITE_TOKEN"/"
#define GET2 " HTTP/1.1\r\nHost: bufsm-dalmago.rhcloud.com\r\n\r\n"

//#define MODULE_RESET 6
//#define MODULE_PWR 7
#define BLUE_LED 5
#define GREEN_LED 4
#define RED_LED 3

#define ledOn(led) digitalWrite(led, 1)
#define ledOff(led) digitalWrite(led, 0)
#define ledToggle(led) digitalWrite(led, 1^digitalRead(led))

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
//void gprs_reset();
//void gprs_powerCycle();
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
