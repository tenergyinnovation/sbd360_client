# Built-in Switch Debounce Functionality

## Overview
ใน version 1.8 ของ tenergy32hub library ได้เพิ่มฟังก์ชัน Debounce ภายในไลบรารีสำหรับปุ่ม SW1 และ SW2 เพื่อป้องกันปัญหา switch bouncing และเพิ่มความแม่นยำในการอ่านค่าปุ่ม โดยไม่ต้องพึ่งพา external library

## Features
- **Built-in Debouncing**: ใช้ delay 50ms เพื่อป้องกัน bouncing
- **Press Counting**: นับจำนวนครั้งที่กดปุ่มได้อย่างแม่นยำ
- **Reset Counter**: สามารถรีเซ็ตตัวนับได้
- **No External Dependencies**: ไม่ต้องใช้ external library
- **Lightweight**: ใช้ memory น้อย

## Internal Implementation
```cpp
// Debounce variables สำหรับแต่ละปุ่ม
bool _sw1_state, _sw1_lastState, _sw1_lastReading;
unsigned long _sw1_lastDebounceTime;
unsigned int _sw1_count;

// Constants
static const unsigned long DEBOUNCE_DELAY = 50; // 50ms
```

## Functions

### Basic Switch Reading (with debounce)
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

## Debounce Algorithm
1. อ่านค่า digital input
2. ตรวจสอบว่ามีการเปลี่ยนแปลงหรือไม่
3. รีเซ็ต debounce timer หากมีการเปลี่ยนแปลง
4. รอให้ผ่าน DEBOUNCE_DELAY (50ms)
5. อัปเดตสถานะและนับจำนวนครั้งเมื่อกดปุ่ม
6. คืนค่าสถานะปุ่ม (inverted เนื่องจากใช้ pullup)

## Example Usage
```cpp
#include <tenergy32hub.h>

Tenergy32Hub hub;

void setup() {
  Serial.begin(115200);
  hub.begin(); // จะ initialize debounce variables อัตโนมัติ
}

void loop() {
  // อ่านสถานะปุ่ม (debounced)
  if (hub.readSW1()) {
    Serial.println("SW1 is pressed! (debounced)");
    hub.setBlueLED(true);
    delay(100);
    hub.setBlueLED(false);
  }
  
  // อ่านจำนวนครั้งที่กดปุ่ม
  unsigned int sw1Count = hub.getSW1Count();
  unsigned int sw2Count = hub.getSW2Count();
  
  Serial.printf("SW1: %u times, SW2: %u times\n", sw1Count, sw2Count);
  
  // รีเซ็ตตัวนับเมื่อกด SW2
  if (hub.readSW2()) {
    hub.resetSW1Count();
    hub.resetSW2Count();
    Serial.println("Counters reset!");
  }
  
  delay(500);
}
```

## Technical Details
- **Debounce Delay**: 50 milliseconds
- **Input Mode**: INPUT_PULLUP
- **Active Logic**: LOW = pressed (hardware), function returns true when pressed
- **Counting Logic**: เพิ่มตัวนับเมื่อสถานะเปลี่ยนจาก HIGH เป็น LOW
- **Memory Usage**: ~40 bytes per switch (variables only)

## Pin Definitions
- SW1: GPIO 34 (INPUT_PULLUP)
- SW2: GPIO 35 (INPUT_PULLUP)

## Advantages over External Library
1. **No Dependencies**: ไม่ต้องเพิ่ม external library
2. **Lightweight**: ใช้ memory น้อยกว่า
3. **Customizable**: สามารถปรับแต่ง delay ได้ง่าย
4. **Integrated**: ทำงานร่วมกับ library ได้อย่างราบรื่น
5. **No Dynamic Allocation**: ไม่ใช้ new/delete

## Performance
- **CPU Usage**: Very low (เพียงการเปรียบเทียบและการคำนวณเวลา)
- **Memory Usage**: Static variables only (ไม่มี dynamic allocation)
- **Response Time**: 50ms debounce delay
- **Accuracy**: สูงกว่าการอ่านแบบปกติมาก

## Notes
- ฟังก์ชัน `getSW1Count()` และ `getSW2Count()` จะเรียก `readSW1()` และ `readSW2()` อัตโนมัติเพื่ออัปเดตสถานะ
- การนับจะเพิ่มขึ้นเมื่อปุ่มถูกกด (HIGH → LOW transition)
- สามารถรีเซ็ตตัวนับได้ทุกเมื่อที่ต้องการ
- Debounce logic ทำงานแยกกันสำหรับแต่ละปุ่ม
