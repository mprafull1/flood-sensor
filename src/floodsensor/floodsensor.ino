  /*  --------------------------------------------------------------------------------
 *  This Example uses Maxbotix Ultrasonic Sensor and Heltec CuebeCell AB02 dev board
 *  to read the depth and send the distance and battery level over the
 *  LoRaWAN network.
 *
 *  Connections:
 * 
 *    VEXT       -> Maxbotix V+
 *    GND        -> Maxbotix GND
 *    RX2        -> Maxbotix Serial Pin (Pin 5: Serial output)
 *    GPIO 5     -> Maxbotix triggerPin (Pin 4: Ranging start/stop)
 *
 *  -------------------------------------------------------------------------------- */

#include "Floodsense_sensor.h"

#define USE_RG15
#define USE_TIPPING_BUCKET


void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println("Starting");

  #ifdef USE_MAXBOTIX
  setup_maxbotix(2, 150, 7);     // sensor mode 2(Median), 150ms sampling rate (time between readings), 7 readings per measurement
  #endif

  #ifdef USE_RG15
  setup_RG15("Polling");
  #endif

  setup_lorawan(60);             // uplink frequency 60 seconds - controls duty cycle
}

void loop() {
  lorawan_runloop_once();
}