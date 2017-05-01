#include "utils.h"

const char *AT_COMMANDS[] = {
  "AT\r\n",
  "ATE0\r\n",
  "AT+IPR=115200\r\n",
  "AT+CREG?\r\n",
  "AT+COPS=0,0\r\n",
  "AT+COPS?\r\n",
  "AT+CGATT=1\r\n",
  "AT+CGDCONT=1, \"IP\", \""APN"\"\r\n",
  "AT+CGACT=1,1\r\n",
  "AT+CIFSR\r\n",
  "AT+CIPSTART=\"TCP\",\""URL"\",80\r\n",
  "AT+CIPSEND=",
  "AT+CIPSHUT\r\n",
  "AT+GPS=1\r\n",
  "AT+GPS=0\r\n",
  "AT+GPSRD=1\r\n",
  "AT+GPSRD=0\r\n"
};

const char *AT_ANS[] = {
  "OK\r\n",
  "OK\r\n",
  "OK\r\n",
  "CREG: 1",
  "OK\r\n",
  "COPS: 0",
  "OK\r\n",
  "OK\r\n",
  "OK\r\n",
  "OK\r\n",
  "OK\r\n",
  "> ",
  "OK\r\n",
  "OK\r\n",
  "OK\r\n",
  "OK\r\n",
  "OK\r\n",
  "$GPVTG"
};

String uart_buffer;
TinyGPS gps;

// unsigned char bufsm_init = 0;
// unsigned char bufsm_gps_valid = 0;
// unsigned char bufsm_connected = 0;
// unsigned char bufsm_data_sent = 0;

void wait_module_init() {
  ledOn(RED_LED);
  uart_buffer = "";

  // wait the module initialization proccess
#ifdef TELIT_SIM
  while (waitFor("CREG: 5\r\n", 0, 15000) != 1) {
#else
  while (waitFor("CREG: 1\r\n", 0, 15000) != 1) {
#endif
    gprs_powerCycle();
    uart_buffer = "";
    Serial.flush();
  }

  uart_buffer = "";

  Serial.print(AT_COMMANDS[DIS_ECHO]);
  waitFor(AT_ANS[DIS_ECHO], 0, 2000);
  uart_buffer = "";

  Serial.print(AT_COMMANDS[SET_BAUD_RATE]);
  waitFor(AT_ANS[SET_BAUD_RATE], 0, 2000);

  uart_buffer = "";
  ledOff(RED_LED);
}

uint8_t gprs_init() {
  do {
    ledToggle(BLUE_LED);
    uart_buffer = "";
    Serial.print(AT_COMMANDS[NETWORK_REGIST]);
  } while (waitFor(AT_ANS[NETWORK_REGIST], 0, 5000) != 1);
  uart_buffer = "";
  ledOn(BLUE_LED);

  //#ifdef DEBUG
  Serial.print(AT_COMMANDS[EN_SHOW_OPERATOR]);
  waitFor(AT_ANS[EN_SHOW_OPERATOR], 0, 10000);
  uart_buffer = "";

  Serial.print(AT_COMMANDS[CHECK_OPERATOR]);
  waitFor(AT_ANS[CHECK_OPERATOR], 0, 7000);
  uart_buffer = "";

  //Serial2.print(AT_COMMANDS[ATTACH]);
  //#endif
  Serial.print(AT_COMMANDS[ATTACH]);
  waitFor(AT_ANS[ATTACH], 0, 20000);
  uart_buffer = "";

#ifdef DEBUG
  Serial2.print(AT_COMMANDS[SET_PDP_CONTEXT]);
#endif
  Serial.print(AT_COMMANDS[SET_PDP_CONTEXT]);
  waitFor(AT_ANS[SET_PDP_CONTEXT], 0, 3000);
  uart_buffer = "";

#ifdef DEBUG
  Serial2.print(AT_COMMANDS[ACTIVATE_PDP_CONTEXT]);
#endif
  Serial.print(AT_COMMANDS[ACTIVATE_PDP_CONTEXT]);
  waitFor(AT_ANS[ACTIVATE_PDP_CONTEXT], "ERROR", 15000);
  uart_buffer = "";

  //#ifdef DEBUG
  //Serial2.print(AT_COMMANDS[GET_IP]);
  Serial.print(AT_COMMANDS[GET_IP]);
  waitFor(AT_ANS[GET_IP], 0, 7000);
  //#endif
  uart_buffer = "";

  uint8_t count = 0;
  do {
    if (count++ >= 5) {
      return 0;
    }
    uart_buffer = "";
#ifdef DEBUG
    Serial2.print(AT_COMMANDS[CONN_TCP]);
#endif
    Serial.print(AT_COMMANDS[CONN_TCP]);
  } while (waitFor(AT_ANS[CONN_TCP], "ERROR", 20000) != 1);

  ledOff(BLUE_LED);
  return 1;
}



void gps_init() {
  Serial.print(AT_COMMANDS[GPS_ON]);
  waitFor(AT_ANS[GPS_ON], 0, 10000);
  uart_buffer = "";
}


uint8_t gprs_send_coods(coords_t value) {
  uart_buffer = "";

  String b = GET + String(value.lat, 8) + "/" + String(value.lng, 8) + GET2;

  Serial.print(AT_COMMANDS[SEND_DATA]);
  Serial.print(b.length());
  Serial.print("\r\n");

  waitFor(AT_ANS[SEND_DATA], 0, 5000);
  uart_buffer = "";

#ifdef DEBUG
  Serial2.print(b);
#endif
  Serial.print(b);

  uint8_t count = 0;
  while (waitFor("200 OK", "ERROR", 17000) != 1) {
    ledOff(GREEN_LED);

    uart_buffer = "";
    Serial.print(AT_COMMANDS[CONN_TCP]);
    if (waitFor(AT_ANS[CONN_TCP], "ERROR", 20000) == 1) {
      break;
    }

    if (count++ >= 3) return 0;
  }

  ledOn(GREEN_LED);

  uart_buffer = "";

  // Serial.print(AT_COMMANDS[CLOSE_TCP]);
  // waitFor("OK\r\n", 0, 10000);

  // uart_buffer = "";

  return 1;
}


void gps_get_coordinates(coords_t *c) {

  uart_buffer = "";
#ifdef DEBUG
  Serial2.print(AT_COMMANDS[GPS_AT_ON]);
#endif
  Serial.print(AT_COMMANDS[GPS_AT_ON]);

  waitFor(AT_ANS[GPS_AT_ON], 0, 5000);
  uart_buffer = "";

  while (1) {
    // Habilita GPS NMEA pela UART
    waitFor(AT_ANS[GPS_ANS], 0, 1200);

    unsigned int i;
    uart_buffer.remove(0, 9); // remove +GPSRD:

    for (i = 0; i < uart_buffer.length(); i++) {
      if (gps.encode(uart_buffer[i])) {

        // retrieves +/- lat/long in 100000ths of a degree
        gps.f_get_position(&c->lat, &c->lng);

#ifdef DEBUG
        Serial2.print("#########>");
        Serial2.print(uart_buffer);
        Serial2.println("<#########");
        Serial2.print("lat: "); Serial2.println(c->lat, 8);
        Serial2.print("lng: "); Serial2.println(c->lng, 8);
        Serial2.println("<#########");
#endif

        // Desabilita GPS NMEA pela UART
        Serial.print(AT_COMMANDS[GPS_AT_OFF]);
        waitFor(AT_ANS[GPS_AT_OFF], 0, 5000);
        uart_buffer = "";
        Serial.flush();

        ledOff(RED_LED);
        return;
      }
    }

    ledToggle(RED_LED);
    uart_buffer = "";

  }
}


int waitFor(const char *ans, const char *error, unsigned int tempo) {
  unsigned long time_counter = millis();

  while (millis() - time_counter < tempo) {
    uart_buffer += Serial.readString();

    if (uart_buffer.indexOf(ans) != -1) {
#ifdef DEBUG
      Serial2.println("Encontrou:");
      Serial2.println(uart_buffer);
      Serial2.println("++++");
#endif

      return 1;
    }

    else if (error != 0 && uart_buffer.indexOf(error) != -1) {
#ifdef DEBUG
      Serial2.print(uart_buffer);
#endif
      return -1;
    }

  }
#ifdef DEBUG
  Serial2.println("Timeout:");
  Serial2.print(uart_buffer);
  Serial2.println("++++");
#endif
  return 0;
}

void gprs_reset() {
  digitalWrite(MODULE_RESET, 1);
  delay(400);
  digitalWrite(MODULE_RESET, 0);
}

void gprs_powerCycle() {
  gprs_reset();

  delay(500);

  digitalWrite(MODULE_PWR, 1);
  delay(2000);
  digitalWrite(MODULE_PWR, 0);
}
