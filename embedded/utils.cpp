#include "utils.h"

String uart_buffer;
TinyGPS gps;

#define MAX_NULL_RECEIVED 5
#define MAX_INCORRECT_GPS_PARAMETERS 5*7

void gps_get_coordinates_ublox(coords_t *c) {

  SerialAT.flush();
  uart_buffer = "";

  while (1)
  {
    // only 7 arguments are sent by gps, so... try 5 times
    uint8_t incorrect_parameters = 0, uart_null = 0;
    while ( \
            uart_buffer != "GPRMC" && \
            incorrect_parameters < MAX_INCORRECT_GPS_PARAMETERS && \
            uart_null < MAX_NULL_RECEIVED \
          )
    {
      uart_buffer = SerialAT.readStringUntil('$');
      uart_buffer = SerialAT.readStringUntil(',');

#ifdef DEBUG_GPS
      SerialAT.print("Leu: ");
      SerialAT.println(uart_buffer);
#endif

      if (uart_buffer == "")
      {
        uart_null ++;
      }
      else
      {
        uart_null = 0;
      }

      incorrect_parameters++;
    }

    // isn't a null problem?
    if (uart_null < MAX_NULL_RECEIVED)
    {
      // is the correct argument?
      if (incorrect_parameters < MAX_INCORRECT_GPS_PARAMETERS)
      {
#ifdef DEBUG_GPS
        SerialAT.println("Achou");
#endif
        uart_buffer = "$GPRMC,";
        uart_buffer += SerialAT.readStringUntil('$');

        unsigned int i;
        for (i = 0; i < uart_buffer.length(); i++)
        {
          if (gps.encode(uart_buffer[i]))
          {

            unsigned long age;
            // retrieves +/- lat/long in 100000ths of a degree
            gps.f_get_position(&c->lat, &c->lng, &age);

#ifdef DEBUG
            SerialDebug.print("#########>");
            SerialDebug.print(uart_buffer);
            SerialDebug.println("<#########");
            SerialDebug.print("lat: "); SerialDebug.println(c->lat, 8);
            SerialDebug.print("lng: "); SerialDebug.println(c->lng, 8);
            SerialDebug.println("<#########");
#endif

            SerialAT.flush();

            ledOff(RED_LED);
            ledOn(BLUE_LED);
            return;
          }
        }
      }
    }

    // if is unable to get position or valid information
    SerialAT.flush();
    uart_buffer = "";
    ledOn(RED_LED);
    ledOff(BLUE_LED);
  }
}

void float_to_string(double number, char *res, int afterpoint)
{
  // parte inteira em aux
  int32_t aux = (int32_t) number;
  // parte fracionária em aux2
  float aux2 = number - (float) aux;

  // salva a parte inteira do número na string e já coloca o ponto
  sprintf(res, "%d.", aux);

  uint8_t loop;
  for (loop = 0; loop < afterpoint; loop++)
  {
    aux2 = aux2 * 10;
    printf("%f\n", aux2 );
    aux = (int32_t) aux2;
    char strnumber[2];
    sprintf(strnumber, "%d", abs(aux) );
    strcat(res, strnumber);

    aux2 = aux2 - aux;
  }
}

int waitFor(const char *ans, const char *error, unsigned int tempo) {
  unsigned long time_counter = millis();

  while (millis() - time_counter < tempo) {
    uart_buffer += SerialAT.readString();

    if (uart_buffer.indexOf(ans) != -1) {
#ifdef DEBUG
      SerialDebug.println("Encontrou:");
      SerialDebug.println(uart_buffer);
      SerialDebug.println("++++");
#endif

      return 1;
    }

    else if (error != 0 && uart_buffer.indexOf(error) != -1) {
#ifdef DEBUG
      SerialDebug.print(uart_buffer);
#endif
      return -1;
    }

  }
#ifdef DEBUG
  SerialDebug.println("Timeout:");
  SerialDebug.print(uart_buffer);
  SerialDebug.println("++++");
#endif
  return 0;
}
