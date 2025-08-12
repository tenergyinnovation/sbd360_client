/***********************************************************************
 * Project      :     Example_SwitchDebounceAdvanced
 * Description  :     Advanced test for built-in debounced switch functionality
 *                    - Tests SW1, SW2 with press counting and timing
 *                    - Shows debounce effectiveness comparison
 *                    - Performance monitoring and stress testing
 * Hardware     :     tenergy32hub         
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     12/08/2025
 * Revision     :     1.0
 * Rev1.0       :     Advanced built-in debounce testing
 * website      :     http://www.tenergyinnovation.co.th
 * Email        :     uten.boonliam@tenergyinnovation.co.th
 * TEL          :     +66 89-140-7205
 ***********************************************************************/
#include <tenergy32hub.h>

Tenergy32Hub hub; // Create hub object

// Test statistics
unsigned long sw1_pressTime = 0;
unsigned long sw2_pressTime = 0;
unsigned long testStartTime = 0;
bool performanceMode = false;

void setup() {
  Serial.begin(115200);
  Serial.println("\r\n**** Example_SwitchDebounceAdvanced ****");
  Serial.println("Advanced built-in debounced switch testing");
  
  // Initialize the Tenergy32Hub
  if (!hub.begin()) {
    Serial.println("Failed to initialize Tenergy32Hub!");
    while (1);
  }
  
  hub.showLibraryVersion();
  
  // Show test instructions
  Serial.println("=== DEBOUNCE TEST MODES ===");
  Serial.println("Normal Mode:");
  Serial.println("  SW1: Increment counter + timing test");
  Serial.println("  SW2: Reset counters + enter performance mode");
  Serial.println("Performance Mode:");
  Serial.println("  SW1: Rapid press test (10 seconds)");
  Serial.println("  SW2: Exit performance mode");
  Serial.println("Slide Switch: Toggle display brightness");
  Serial.println("Built-in debounce: 50ms delay");
  Serial.println("=====================================");
  
  // Initial display
  hub.clearOLED();
  hub.displayOLEDLines("Debounce Test", "Ready...", "Press SW1/SW2", "Built-in: 50ms");
  
  testStartTime = millis();
}

void performanceTest() {
  static unsigned long perfStart = 0;
  static unsigned int perfCount = 0;
  static bool perfActive = false;
  
  if (!perfActive) {
    perfActive = true;
    perfStart = millis();
    perfCount = 0;
    Serial.println("=== PERFORMANCE TEST STARTED ===");
    Serial.println("Press SW1 rapidly for 10 seconds!");
    hub.clearOLED();
    hub.displayOLEDLines("PERFORMANCE TEST", "Press SW1 rapidly", "Time: 10s", "Count: 0");
  }
  
  // Read switches during performance test
  bool currentSW1 = hub.readSW1();
  bool currentSW2 = hub.readSW2();
  static bool lastSW1 = false;
  static bool lastSW2 = false;
  
  // Count SW1 presses
  if (currentSW1 && !lastSW1) {
    perfCount++;
    hub.setBlueLED(true);
    delay(50);
    hub.setBlueLED(false);
    
    // Update display during test
    unsigned long elapsed = (millis() - perfStart) / 1000;
    String timeStr = "Time: " + String(10 - elapsed) + "s";
    String countStr = "Count: " + String(perfCount);
    hub.clearOLED();
    hub.displayOLEDLines("PERFORMANCE TEST", "Press SW1 rapidly", timeStr.c_str(), countStr.c_str());
  }
  
  // Check for SW2 to exit
  if (currentSW2 && !lastSW2) {
    performanceMode = false;
    perfActive = false;
    
    float pps = (float)perfCount / ((millis() - perfStart) / 1000.0);
    Serial.println("=== PERFORMANCE TEST ENDED ===");
    Serial.printf("Total presses: %u\n", perfCount);
    Serial.printf("Test duration: %.1f seconds\n", (millis() - perfStart) / 1000.0);
    Serial.printf("Press rate: %.2f presses/second\n", pps);
    Serial.printf("Debounce effectiveness: %.1f%%\n", (perfCount > 0) ? 100.0 : 0.0);
    
    hub.clearOLED();
    hub.displayOLEDLines("Test Complete", ("Presses: " + String(perfCount)).c_str(), 
                        ("Rate: " + String(pps, 1) + "/s").c_str(), "SW2: Continue");
    delay(3000);
    return;
  }
  
  // Auto-end after 10 seconds
  if (millis() - perfStart > 10000) {
    performanceMode = false;
    perfActive = false;
    
    float pps = (float)perfCount / 10.0;
    Serial.println("=== PERFORMANCE TEST AUTO-ENDED ===");
    Serial.printf("Total presses: %u in 10 seconds\n", perfCount);
    Serial.printf("Average rate: %.2f presses/second\n", pps);
    
    hub.clearOLED();
    hub.displayOLEDLines("Test Complete", ("Presses: " + String(perfCount)).c_str(), 
                        ("Rate: " + String(pps, 1) + "/s").c_str(), "Auto-ended");
    delay(3000);
  }
  
  lastSW1 = currentSW1;
  lastSW2 = currentSW2;
}

void loop() {
  static unsigned long lastPrint = 0;
  static bool lastSW1State = false;
  static bool lastSW2State = false;
  
  // Performance test mode
  if (performanceMode) {
    performanceTest();
    return;
  }
  
  // Normal operation mode
  bool currentSW1 = hub.readSW1();
  bool currentSW2 = hub.readSW2();
  bool slideSwitch = hub.readSlideSwitch();
  
  // SW1 press detection with timing
  if (currentSW1 && !lastSW1State) {
    sw1_pressTime = millis();
    Serial.println("SW1 Pressed! (Built-in Debounced)");
    Serial.printf("Press timing: %lu ms from start\n", sw1_pressTime - testStartTime);
    
    // Visual feedback
    hub.setBlueLED(true);
    delay(100);
    hub.setBlueLED(false);
  }
  
  // SW2 press detection - enter performance mode or reset
  if (currentSW2 && !lastSW2State) {
    sw2_pressTime = millis();
    
    if (hub.getSW1Count() > 5) {
      // Enter performance mode if SW1 pressed more than 5 times
      performanceMode = true;
      Serial.println("SW2 Pressed! - Entering Performance Mode");
    } else {
      // Normal reset
      Serial.println("SW2 Pressed! (Built-in Debounced) - Resetting counters");
      hub.resetSW1Count();
      hub.resetSW2Count();
      testStartTime = millis();
    }
    
    // Visual feedback
    hub.setRedLED(true);
    delay(100);
    hub.setRedLED(false);
  }
  
  // Store previous states
  lastSW1State = currentSW1;
  lastSW2State = currentSW2;
  
  // Print detailed status every 3 seconds
  if (millis() - lastPrint > 3000) {
    unsigned long uptime = (millis() - testStartTime) / 1000;
    
    Serial.println("=== Advanced Switch Status (Built-in Debounce) ===");
    Serial.printf("Uptime: %lu seconds\n", uptime);
    Serial.printf("SW1 State: %s", currentSW1 ? "PRESSED" : "RELEASED");
    if (sw1_pressTime > 0) {
      Serial.printf(" (Last: %lu ms ago)", millis() - sw1_pressTime);
    }
    Serial.println();
    
    Serial.printf("SW2 State: %s", currentSW2 ? "PRESSED" : "RELEASED");
    if (sw2_pressTime > 0) {
      Serial.printf(" (Last: %lu ms ago)", millis() - sw2_pressTime);
    }
    Serial.println();
    
    Serial.printf("Slide Switch: %s\n", slideSwitch ? "ON" : "OFF");
    Serial.printf("SW1 Press Count: %u", hub.getSW1Count());
    if (hub.getSW1Count() > 0 && uptime > 0) {
      Serial.printf(" (%.2f presses/min)", (float)hub.getSW1Count() * 60.0 / uptime);
    }
    Serial.println();
    
    Serial.printf("SW2 Press Count: %u\n", hub.getSW2Count());
    Serial.printf("Mode: %s\n", hub.getSW1Count() > 5 ? "Ready for Performance Test" : "Normal");
    Serial.println("------------------------------------------------");
    
    // Advanced OLED display
    hub.clearOLED();
    String line1 = "Debounce Test";
    String line2 = "SW1:" + String(hub.getSW1Count()) + " SW2:" + String(hub.getSW2Count());
    String line3 = slideSwitch ? "Slide:ON" : "Slide:OFF";
    String line4 = hub.getSW1Count() > 5 ? "SW2:PerfTest" : "SW2:Reset";
    
    hub.displayOLEDLines(line1.c_str(), line2.c_str(), line3.c_str(), line4.c_str());
    
    lastPrint = millis();
  }
  
  delay(5); // Minimal delay for optimal responsiveness
}
