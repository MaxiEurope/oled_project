# STM32 OLED Display Project

This project is developed for the STM32 microcontroller, interfacing with an OLED display (SSD1351) to visualize analog input from a potentiometer using ADC. The display is controlled via SPI communication, while UART is used for debugging purposes.

## Features

- **Analog Input Visualization:** Reads analog input from a potentiometer using ADC.
- **Periodic Sampling:** The potentiometer's value is captured every 10 seconds using a timer interrupt.
- **PWM Conversion:** The ADC value is converted to the corresponding PWM value.
- **Color Display:** The PWM value is used to generate a color, which is displayed on the OLED screen in RGB565 format.
- **Color Details:** The OLED display shows:
  - The closest named color.
  - The RGB values.
  - The hex value of the color.
- **UART Debugging:** Outputs the ADC value, PWM percentage, and color information via UART for easy monitoring.

## Project Components

- **MCU:** STM32
- **Display:** OLED (SSD1351) with RGB565 color format
- **Communication:**
  - **SPI** for OLED display control.
  - **UART** for serial communication and debugging.
  - **ADC** for reading potentiometer input.
- **Timer Interrupt:** A periodic timer interrupt captures and processes the potentiometer input every 10 seconds.

## OLED SSD1351 Library

This project uses a custom OLED SSD1351 library, developed specifically for this project. You can find the library and its source code at:

- **GitHub:** [OLED SSD1351 Library](https://github.com/MaxiEurope/OLED-SSD1351)

## Setup

1. Connect the potentiometer to an analog pin of the STM32.
2. The SSD1351 OLED display is connected via SPI.
3. UART is set up for debugging at 115200 baud rate.
4. The ADC captures input, and a timer interrupt triggers the processing every 10 seconds.

## Usage

1. Adjust the potentiometer, and the system will capture the input every 10 seconds.
2. The corresponding color is calculated based on the PWM value derived from the potentiometer input.
3. The OLED screen will display:
   - The closest named color.
   - The RGB values of the color.
   - The hex value of the color.
4. Debug information is sent to the serial monitor over UART, displaying the ADC value, PWM percentage, and color details.

## Author

- **Author:** MaxiEurope
- **GitHub:** [MaxiEurope](https://github.com/MaxiEurope)

## License

This project is licensed under the MIT License.

