# Switch Debounce Functionality

## Overview
ใน version 1.8 ของ tenergy32hub library ได้เพิ่มฟังก์ชัน Debounce สำหรับปุ่ม SW1 และ SW2 เพื่อป้องกันปัญหา switch bouncing และเพิ่มความแม่นยำในการอ่านค่าปุ่ม

## Features
- **Switch Debouncing**: ใช้ delay 50ms เพื่อป้องกัน bouncing
- **Press Counting**: นับจำนวนครั้งที่กดปุ่มได้อย่างแม่นยำ
- **Reset Counter**: สามารถรีเซ็ตตัวนับได้
- **Fallback Support**: หากเกิดข้อผิดพลาดจะกลับไปใช้การอ่านแบบปกติ

## Functions

### Basic Switch Reading
```cpp
bool readSW1();    // อ่านสถานะปุ่ม SW1 (debounced)
bool readSW2();    // อ่านสถานะปุ่ม SW2 (debounced)
```

### Press Counting
```cpp
unsigned int getSW1Count();    // อ่านจำนวนครั้งที่กด SW1
unsigned int getSW2Count();    // อ่านจำนวนครั้งที่กด SW2
void resetSW1Count();          // รีเซ็ตตัวนับ SW1
void resetSW2Count();          // รีเซ็ตตัวนับ SW2
```

## Example Usage
```cpp
#include <tenergy32hub.h>

Tenergy32Hub hub;

void setup() {
  Serial.begin(115200);
  hub.begin();
}

void loop() {
  // อ่านสถานะปุ่ม (debounced)
  if (hub.readSW1()) {
    Serial.println("SW1 is pressed!");
  }
  
  // อ่านจำนวนครั้งที่กดปุ่ม
  Serial.printf("SW1 pressed %u times\n", hub.getSW1Count());
  Serial.printf("SW2 pressed %u times\n", hub.getSW2Count());
  
  // รีเซ็ตตัวนับเมื่อกด SW2
  if (hub.readSW2()) {
    hub.resetSW1Count();
    hub.resetSW2Count();
    Serial.println("Counters reset!");
  }
  
  delay(100);
}
```

## Technical Details
- **Debounce Delay**: 50 milliseconds
- **Pull-up**: ใช้ internal pull-up resistor
- **Active Logic**: HIGH = pressed (หลัง debounce processing)
- **Library**: ใช้ Debounce library จาก William Koch

## Pin Definitions
- SW1: GPIO 34
- SW2: GPIO 35

## Notes
- ฟังก์ชัน `count()` จะนับ full press+release cycle
- การกด SW1 หรือ SW2 จะเพิ่มตัวนับ
- สามารถรีเซ็ตตัวนับได้ทุกเมื่อที่ต้องการ
- หากเกิดข้อผิดพลาดในการ initialize Debounce objects ระบบจะกลับไปใช้ digitalRead ปกติ
