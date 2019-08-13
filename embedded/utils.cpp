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

// transforma um number double em um array de bytes
// o array deve ser do tamanho (1 + bitsbeforepoint + bitsafterpoint)
// o 1o bit eh o bit indicativo de sinal (0 positivo, 1 negativo)
// afterpoint: numero de casas depois da virgula
// bitsbeforepoint: numero de bits reservados para o numero antes da virgula
// bitsafterpoint:numero de bits reservados para o numero depois da virgula
void number_to_byte(double number, uint8_t *res, int afterpoint, int bitsbeforepoint, int bitsafterpoint) {
  // int bitsbeforepoint = 8;
  // int bitsafterpoint = 27;

  // parte inteira em aux
  int64_t aux = (int64_t) number;
  int64_t result = 0;
  // parte fracionária em aux2
  double aux2 = number - (int32_t) aux;

  // pega o valor das casas decimais
  int64_t fpoint = (int64_t)abs(aux2 * pow(10, afterpoint));
  if((abs(aux) << (63 - bitsbeforepoint)) >> (63 - bitsbeforepoint) != abs(aux) ) {
    printf("Numero antes da virgula e maior que a quantidade de bits requisitada\n");
  }
  if((fpoint << (63 - bitsafterpoint)) >> (63 - bitsafterpoint) != fpoint ) {
    printf("Numero depois da virgula e maior que a quantidade de bits requisitada\n");
  }

  // se negativo, seta primeiro bit 1
  if(aux < 0) {
      result = (int64_t)1 << (bitsbeforepoint + bitsafterpoint);
  }
  // seta o valor inteiro
  result = result | ((int64_t)abs(aux) << bitsafterpoint);
  // seta o resto como as casas decimais
  result = result | (int64_t)abs(aux2 * pow(10, 8));

  int numBytes = (int)((1 + bitsbeforepoint + bitsafterpoint) / 8);
  if((1 + bitsbeforepoint + bitsafterpoint) % 8 != 0) {
    numBytes++;
  }

  // sem memcpy porque pode ser big/little endian
  for(int i = 0; i < numBytes; i++) {
    res[i] = (result >> (numBytes - 1 - i) * 8) & 0xFF;
  }
}

// transforma latitude/longitude (double) em 5 bytes (36 bits uteis)
// formato: 4 bits 0, 1 bit indicativo de sinal, 8 bits de parte inteira, 27 bits de parte decimal
void lat_long_to_bytes(double lat_long, uint8_t *res) {
  // +180 a -180 e +90 a -90 estao sempre abaixo de 8 bits (1 byte)
  // 8 digitos eh sempre abaixo de 27 bits
  number_to_byte(lat_long, res, 8, 8, 27);
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
