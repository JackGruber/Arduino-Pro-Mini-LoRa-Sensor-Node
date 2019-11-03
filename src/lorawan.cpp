#include "lorawan.h"
#include "settings.h"
#include <hal/hal.h>
#include <SPI.h>
#include "io_pins.h"
#include "powerdown.h"
#include "functions.h"
#include "global.h"

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = PIN_LMIC_NSS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = PIN_LMIC_RST,
    .dio = {PIN_LMIC_DIO0, PIN_LMIC_DIO1, PIN_LMIC_DIO2 },
};

static uint8_t LORA_DATA[4];

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = LORA_TX_INTERVAL;

void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16);}

bool GO_DEEP_SLEEP = false;

void LoRaWANSetup()
{
    Serial.println(F("LoRaWAN_Setup ..."));

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Start job
    LoraWANDo_send(&sendjob);
}


void LoraWANDo_send(osjob_t* j)
{
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        LoraWANGetData();

        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, LORA_DATA, sizeof(LORA_DATA), 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

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
        case EV_JOIN_TXCOMPLETE:
            Serial.println(F("EV_JOIN_TXCOMPLETE"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            #ifndef DISABLE_JOIN
                {
                u4_t netid = 0;
                devaddr_t devaddr = 0;
                u1_t nwkKey[16];
                u1_t artKey[16];
                LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
                Serial.print("netid: ");
                Serial.println(netid, DEC);
                Serial.print("devaddr: ");
                Serial.println(devaddr, HEX);
                Serial.print("artKey: ");
                for (size_t i=0; i<sizeof(artKey); ++i) {
                    Serial.print(artKey[i], HEX);
                }
                Serial.println("");
                Serial.print("nwkKey: ");
                for (size_t i=0; i<sizeof(nwkKey); ++i) {
                    Serial.print(nwkKey[i], HEX);
                }
                Serial.println("");
                }
                // Disable link check validation (automatically enabled
                // during join, but because slow data rates change max TX
                // size, we don't use it in this example.
                LMIC_setLinkCheckMode(0);
            #endif
            break;
        /* This event is defined but not used in the code
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        */
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE"));

            if (LMIC.txrxFlags & TXRX_ACK)
            {
                Serial.println(F("Received ack"));
            }
          
            if (LMIC.dataLen) 
            {
                Serial.print(LMIC.dataLen);
                Serial.println(F(" bytes of payload"));
            }

            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), LoraWANDo_send);
            GO_DEEP_SLEEP = true;
           
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
        /* This event is defined but not used in the code.
        case EV_SCAN_FOUND:
            DisplayPrintln(F("EV_SCAN_FOUND"), LORAWAN_STATE_DISPLAY_LINE);
            break;
        */
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            break;
         default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            break;
    }
}

void LoraWANDo(void)
{
    if(GO_DEEP_SLEEP == true)
    {
      PowerDownTXIntervall();
      GO_DEEP_SLEEP = false;
    }
    else
    {
      os_runloop_once();
    }
}

void LoraWANGetData()
{
    ReadDHTSensor();

    uint8_t  vcc = ( ReadVcc() / 10) - 200;
    
    uint8_t humidity_lora = HUMIDITY;
    
    int16_t temp = (TEMPERATURE * 10);
     
    if ( isnan(TEMPERATURE) )
    { 
      LORA_DATA[2] = 255;    
      LORA_DATA[3] = 255;
    }
    else 
    { 
      LORA_DATA[2] = temp >> 8;
      LORA_DATA[3] = temp & 0xFF;
    }

    if ( isnan(HUMIDITY)) { LORA_DATA[1] = 255; }
    else 
    { 
        int check = HUMIDITY;
        LORA_DATA[1] = humidity_lora;

        // Bit 8 for decimal 1 = 0.5
        if(check > 0.251 && check < 0.751) { LORA_DATA[1] |= (1 << 7); }
        else if(check > 0.751) { LORA_DATA[1] = LORA_DATA[1] + 1; }        
    }
    
    LORA_DATA[0] = vcc;
}