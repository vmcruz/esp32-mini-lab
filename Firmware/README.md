# ESP32 Captive Portal Firmware

Configurable captive portal firmware for ESP32, designed to serve custom web layouts and handle incoming requests without requiring firmware recompilation for configuration changes.

The firmware supports **runtime configuration of GPIO pin assignments and portal layouts** through a configuration file stored on a microSD card. Layouts can define their own HTML content, static assets, and URL routes, making it possible to customize the portal's appearance and behavior without modifying or recompiling the firmware.

### Features

- **Configurable layouts:** Serve custom HTML pages and assets from the microSD card.
- **Configurable GPIO pinout:** Set GPIO assignments for peripherals and input controls through a configuration file.
- **Route configuration:** Define URL routes and their associated files and MIME types.
- **Captive portal support:** Redirect clients to the portal when they connect to the ESP32's Wi-Fi access point.
- **MicroSD-based configuration:** Store settings, layouts, and assets externally, allowing changes without reflashing the firmware.
- **Request logging:** Record query parameters received by the captive endpoint to the microSD card.

The goal is to provide a flexible captive portal framework where hardware pin assignments and web content can be adapted to different configurations without recompiling the firmware.

The firmware is developed using **PlatformIO** and the **Arduino framework**.

## Hardware

- **MCU:** ESP32-C3-MINI-1
- **Display:** 0.96-inch OLED, 128×64, SSD1306, I²C
- **Storage:** MicroSD card reader
- **Input:** 5-way D-pad

## Pinout

| Component | Signal | ESP32-C3 GPIO |
| --------- | ------ | ------------: |
| OLED      | SDA    |        GPIO20 |
| OLED      | SCL    |        GPIO21 |
| MicroSD   | CS     |         GPIO5 |
| MicroSD   | MOSI   |         GPIO6 |
| MicroSD   | MISO   |         GPIO7 |
| MicroSD   | SCK    |        GPIO10 |
| D-pad     | Up     |         GPIO0 |
| D-pad     | Left   |         GPIO1 |
| D-pad     | Down   |         GPIO2 |
| D-pad     | Right  |         GPIO3 |
| D-pad     | Press  |         GPIO4 |

## Requirements

- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE](https://platformio.org/)
- A USB data cable compatible with the device

## Getting Started

### 1. Open the Project

Open the `Firmware/` directory in Visual Studio Code with the PlatformIO extension installed.

### 2. Connect the Device

Connect ESP32-Mini-Lab to your computer using its USB-C port.

The ESP32-C3 uses native USB connectivity for firmware uploading and USB CDC serial communication.

### 3. Build the Firmware

Use the PlatformIO **Build** command or run:

```bash
pio run
```

### 4. Flash the Bootstrap Firmware

Before using ESP32-Mini-Lab, **flash the bootstrap firmware to the device**.

The bootstrap firmware initializes the device and prepares it to load its configuration and resources from the microSD card.

Connect the device through USB-C and use PlatformIO to build and upload the bootstrap firmware.

```bash
pio run --target uploadfs
```

### 5. Upload the Firmware

Upload the firmware using PlatformIO:

```bash
pio run --target upload
```

If PlatformIO cannot automatically identify the upload port, specify it in the project's PlatformIO configuration.

### 5. Monitor Serial Output

To inspect firmware logs over USB CDC, run:

```bash
pio device monitor
```

USB CDC must be enabled in the firmware configuration for serial output over the native USB interface.

## Prepare the microSD Card

Format the microSD card with a filesystem supported by the firmware, then copy the configuration file and required resources to the card.

Create a configuration file named `config.txt` at the root of the microSD card with the following contents:

```ini
sd_cs_pin=5
sd_clk_pin=10
sd_miso_pin=7
sd_mosi_pin=6

ssid=Captive Network

i2c_driver=ssd1306
i2c_screen_height=64
i2c_screen_width=128
i2c_sda_pin=20
i2c_scl_pin=21

layout=default

btn_pin_mode=0x05
btn_pin_up=0
btn_pin_down=2
btn_pin_right=3
btn_pin_left=1
btn_pin_press=4
```

The configuration defines the SPI pins for the microSD reader, the Wi-Fi access point SSID, the OLED display settings, the active layout, and the D-pad input pins.

### 1. Configure the Layout

Create the following directory structure at the root of the microSD card:

```text
/layouts
└── default
    ├── index.html
    └── routes.txt
```

The directory name `default` must match the value of the `layout` setting in `config.txt`.

- `index.html` contains the HTML page served by the device.
- `routes.txt` defines the URL routes and the files served for each route.

### 2. Configure Routes

Each entry in `routes.txt` must follow this format:

```text
{path}, {streamed_file}, {mime-type}
```

Each line maps a URL path to a file on the microSD card and its corresponding MIME type.

For example:

```text
/, /layouts/default/index.html, text/html
/some-file, /layouts/default/some-file.mp3, audio/mpeg
```

When a client requests `/`, the device serves `index.html`. Requests to `/some-file` serve the corresponding MP3 file with the `audio/mpeg` MIME type.

#### Reserved Route

`/captive` is a reserved endpoint and must not be configured as a regular route in `routes.txt`.

When query parameters are sent to `/captive`, the firmware records them in a log file on the microSD card.

The log filename follows this format:

```text
captive_logs_{layout_name}.txt
```

For the `default` layout, the filename is:

```text
captive_logs_default.txt
```

Each query parameter is recorded as key-value pair.

For example, a request such as:

```text
/captive?name=example&source=mobile
```

produces log entries containing:

```text
name=example&source=mobile
```

The log file is stored on the microSD card.

## PlatformIO Configuration

The project uses the Arduino framework on the ESP32-C3 platform.

The following build flags enable native USB CDC:

```ini
-DARDUINO_USB_MODE=1
-DARDUINO_USB_CDC_ON_BOOT=1
```

Retain both flags in the relevant build environments.

Application-level debug logging can be configured independently of USB CDC support.

## Example

The `example/` directory contains a basic microSD card configuration, including a sample layout and the files required to get started.

Copy the contents of `example/` to the root of your microSD card, adjust `config.txt` to match your hardware configuration, and insert the card into the ESP32.

Use this example as a starting point for creating custom layouts and configuring the firmware without recompiling it.
