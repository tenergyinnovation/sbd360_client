# tenergy32hub Library

The `tenergy32hub` library is designed for the ESP32-DOIT-DEVKIT-V1 board, providing an interface to interact with various components such as OLED displays, LoRa modules, relays, and sensors. This library simplifies the process of integrating these components into your projects.

## Features

- **OLED Display Management**: Easily initialize and display text or graphics on a 0.91-inch OLED display.
- **LoRa Communication**: Send and receive data using the LoRa module for long-range communication.
- **Relay Control**: Control electrical circuits with a relay.
- **Sensor Integration**: Interface with motion sensors and water leak sensors to detect environmental changes.
- **Potentiometer Reading**: Read values from adjustable potentiometers.

## Installation

1. Download the `tenergy32hub` library from the repository.
2. Place the `tenergy32hub` folder in your Arduino libraries directory (usually found in `Documents/Arduino/libraries`).
3. Restart the Arduino IDE to recognize the new library.

## Usage

### Basic Example

To get started with the `tenergy32hub` library, you can use the basic example provided in the `examples/basic_usage` folder. This example demonstrates how to initialize the library and interact with the components.

### Advanced Example

For more complex applications, refer to the advanced example in the `examples/advanced_usage` folder. This example showcases advanced features and integrations.

## Documentation

Refer to the header files in the `src` directory for detailed documentation on each class and its methods:

- `SBD360Client`: Main class for managing the board components.
- `OLEDDisplay`: Class for handling the OLED display.
- `LoRaModule`: Class for managing LoRa communication.
- `Relay`: Class for controlling the relay.
- `RS485`: Class for RS485 communication.
- `Potentiometer`: Class for reading potentiometer values.
- `MotionSensor`: Class for detecting motion.
- `WaterLeakSensor`: Class for detecting water leaks.

## Contributing

Contributions are welcome! If you have suggestions or improvements, please submit a pull request or open an issue in the repository.

## License

This library is open-source and available under the MIT License.