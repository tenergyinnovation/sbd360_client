/***********************************************************************
 * Project      :     Example_SwitchBasic
 * Description  :     Basic switch testing with built-in debounce
 *                    Simple example for beginners to understand debounce functionality
 * Hardware     :     tenergy32hub         
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     12/08/2025
 * Revision     :     1.0
 * Rev1.0       :     Basic switch example with built-in debounce
 * website      :     http://www.tenergyinnovation.co.th
 * Email        :     uten.boonliam@tenergyinnovation.co.th
 * TEL          :     +66 89-140-7205
 ***********************************************************************/
#include <tenergy32hub.h>

Tenergy32Hub hub; // Create hub object

void setup() {
  Serial.begin(115200);
  Serial.println("\r\n**** Basic Switch Test ****");
  Serial.println("Simple switch testing with built-in debounce");
  
  // Initialize the board
  if (!hub.begin()) {
    Serial.println("Failed to initialize board!");
    while (1);
  }
  
  Serial.println("Ready! Press SW1 or SW2 to test");
  Serial.println("SW1: Blue LED, SW2: Red LED");
}

void loop() {
  // Read switches (automatically debounced)
  bool sw1_pressed = hub.readSW1();
  bool sw2_pressed = hub.readSW2();
  
  // Control LEDs based on switch states
  hub.setBlueLED(sw1_pressed);   // Blue LED follows SW1
  hub.setRedLED(sw2_pressed);    // Red LED follows SW2
  
  // Print press counts every 2 seconds
  static unsigned long lastTime = 0;
  if (millis() - lastTime > 2000) {
    Serial.printf("SW1 pressed: %u times\n", hub.getSW1Count());
    Serial.printf("SW2 pressed: %u times\n", hub.getSW2Count());
    
    // Display on OLED
    hub.clearOLED();
    hub.displayOLEDLines("Switch Test", 
                        ("SW1: " + String(hub.getSW1Count())).c_str(),
                        ("SW2: " + String(hub.getSW2Count())).c_str(),
                        "Built-in Debounce");
    
    lastTime = millis();
  }
  
  delay(10);
}
