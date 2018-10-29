#define DEEP_SLEEP


// LoRaWAN NwkSKey, network session key
// This is the default Semtech key, which is used by the prototype TTN network initially.
static const PROGMEM u1_t NWKSKEY[16] = { 0x74, 0xCE, 0x0D, 0x21, 0xB3, 0x8F, 0xEB, 0x44, 0x79, 0xE8, 0x0D, 0x3B, 0xFB, 0xE4, 0xA0, 0xD5 };

// LoRaWAN AppSKey, application session key
// This is the default Semtech key, which is used by the prototype TTN network initially.
static const u1_t PROGMEM APPSKEY[16] = { 0xCC, 0x33, 0xDE, 0xE3, 0x2A, 0x6C, 0x22, 0xE2, 0x6F, 0x81, 0xF7, 0x00, 0x1B, 0xEC, 0x04, 0x84 };

// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = 0x26011777 ; 

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

//static uint8_t mydata[] = "Hi";
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 300;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = A0,
    .dio = {5, 6, 7 },
};

bool GO_DEEP_SLEEP = false;

// If commented out, only the specified channels are used 
// #define LMIC_USE_CH 0,1,2
#define LMIC_USE_CH 0
