#include <SPI.h>
#include "utils.h"
#include "src/lmic.h"
#include "src/hal/hal.h"


// bUFSM
coords_t coord;
uint8_t flag_sent = 1;

// LoRaWAN NwkSKey, network session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
// App bUFSM node bufsm_01
static const PROGMEM u1_t NWKSKEY[16] = { 0x86, 0x4B, 0xA2, 0x5D, 0x0F, 0x15, 0xBD, 0x50, 0x2F, 0x83, 0x0F, 0xD4, 0x60, 0x6C, 0x95, 0x1C };

// LoRaWAN AppSKey, application session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
// App bUFSM node bufsm_01
static const u1_t PROGMEM APPSKEY[16] = { 0x71, 0xB5, 0x7F, 0x2E, 0xBD, 0x98, 0x6D, 0x38, 0x63, 0x5A, 0x2C, 0x2F, 0x2B, 0x1B, 0x76, 0x4E };

// LoRaWAN end-device address (DevAddr)
// node bufsm_01
static const u4_t DEVADDR = 0x2603188F ; // <-- Change this address for every node!

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

static osjob_t sendjob;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 6,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 5,
    .dio = {2, 3, 4},
};

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
            flag_sent = 1;
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:delay(3000);
            Serial.println(F("Unknown event"));
            break;
    }
}

void do_send(osjob_t* j){

    char latitude[50];
    float_to_string(coord.lat, latitude, 8);
    
    char longitude[50];
    float_to_string(coord.lng, longitude, 8);

    // Mounting JSON
    String mydataStr = "{\"lat\":\""+String(latitude)+"\",\"lng\":\""+String(longitude)+"\"}";

    // Converting String into uint8_t array
    uint8_t mydata[mydataStr.length()+1];
    mydataStr.getBytes(mydata,mydataStr.length()+1);
  
    
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        Serial.println(F("Packet queued"));
    }
}

void setup() {
    // bUFSM config
    pinMode(BLUE_LED, OUTPUT);
    digitalWrite(BLUE_LED, LOW);
    pinMode(GREEN_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);
    pinMode(RED_LED, OUTPUT);
    
    // Set the red led on init
    digitalWrite(RED_LED, HIGH);
    
    pinMode(GPS_RESET, OUTPUT);
    digitalWrite(GPS_RESET, LOW);
    delay(1000);
    digitalWrite(GPS_RESET, HIGH);
  
    Serial.begin(9600);
    Serial.println(F("Starting"));

    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_serxloratSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    #if defined(CFG_us915)
    // NA-US channels 0-71 are configured automatically
    // but only one group of 8 should (a subband) should be active
    // TTN recommends the second sub band, 1 in a zero based count.
    // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
    LMIC_selectSubBand(1);
    #endif

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    /*
    DR_SF12 : DR0
    DR_SF11 : DR1
    DR_SF10 : DR2
    DR_SF9  : DR3
    DR_SF8  : DR4
    DR_SF7  : DR5
    DR_SF8C : DR6
    */
    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF12,14);

    // Init coord
    gps_get_coordinates_ublox(&coord);
}

void loop() {
    if (flag_sent)
    {
      do_send(&sendjob);
      gps_get_coordinates_ublox(&coord);
      flag_sent = 0;
    }
    os_runloop_once();
}
