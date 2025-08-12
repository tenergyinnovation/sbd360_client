/***********************************************************************
 * Project      :     Example_SwitchDebounceTest
 * Description  :     Test built-in debounced switch reading and counting for SW1 and SW2
 *                    on tenergy32hub board with internal debounce logic
 * Hardware     :     tenergy32hub         
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     12/08/2025
 * Revision     :     1.0
 * Rev1.0       :     Built-in debounce implementation (no external library)
 * website      :     http://www.tenergyinnovation.co.th
 * Email        :     uten.boonliam@tenergyinnovation.co.th
 * TEL          :     +66 89-140-7205
 ***********************************************************************/
#include <tenergy32hub.h>

Tenergy32Hub hub; // Create hub object

void setup() {
  Serial.begin(115200);
  Serial.println("\r\n**** Example_SwitchDebounceTest ****");
  Serial.println("Testing built-in debounced switch reading and counting");
  
  // Initialize the Tenergy32Hub (will initialize debounce variables automatically)
  if (!hub.begin()) {
    Serial.println("Failed to initialize Tenergy32Hub!");
    while (1);
  }
  
  hub.showLibraryVersion();
  Serial.println("Press SW1 or SW2 to test built-in debounced switch reading");
  Serial.println("SW1: Increment counter, SW2: Reset counters");
  Serial.println("Built-in debounce delay: 50ms");
  Serial.println("----------------------------------------");
}

void loop() {
  static unsigned long lastPrint = 0;
  static bool lastSW1State = false;
  static bool lastSW2State = false;
  
  // Read current switch states (built-in debounced)
  bool currentSW1 = hub.readSW1();
  bool currentSW2 = hub.readSW2();
  bool slideSwitch = hub.readSlideSwitch();
  
  // Check for SW1 press (rising edge detection)
  if (currentSW1 && !lastSW1State) {
    Serial.println("SW1 Pressed! (Built-in Debounced)");
    hub.setBlueLED(true);
    delay(100);
    hub.setBlueLED(false);
  }
  
  // Check for SW2 press (rising edge detection)
  if (currentSW2 && !lastSW2State) {
    Serial.println("SW2 Pressed! (Built-in Debounced) - Resetting counters");
    hub.setRedLED(true);
    hub.resetSW1Count();
    hub.resetSW2Count();
    delay(100);
    hub.setRedLED(false);
  }
  
  // Store previous states for edge detection
  lastSW1State = currentSW1;
  lastSW2State = currentSW2;
  
  // Print status every 2 seconds
  if (millis() - lastPrint > 2000) {
    Serial.println("=== Switch Status (Built-in Debounce) ===");
    Serial.printf("SW1 Current State: %s\r\n", currentSW1 ? "PRESSED" : "RELEASED");
    Serial.printf("SW2 Current State: %s\r\n", currentSW2 ? "PRESSED" : "RELEASED");
    Serial.printf("Slide Switch: %s\r\n", slideSwitch ? "ON" : "OFF");
    Serial.printf("SW1 Press Count: %u\r\n", hub.getSW1Count());
    Serial.printf("SW2 Press Count: %u\r\n", hub.getSW2Count());
    Serial.println("----------------------------------");
    
    // Display on OLED if available
    hub.clearOLED();
    String line1 = "SW Test (Built-in)";
    String line2 = "SW1:" + String(hub.getSW1Count()) + " SW2:" + String(hub.getSW2Count());
    String line3 = slideSwitch ? "Slide:ON" : "Slide:OFF";
    hub.displayOLEDLines(line1.c_str(), line2.c_str(), line3.c_str(), "");
    
    lastPrint = millis();
  }
  
  delay(10); // Small delay for stability
}
