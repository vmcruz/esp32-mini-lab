# Changelog

All notable changes to ESP32-Mini-Lab are documented in this file.

This project follows [Semantic Versioning](https://semver.org/).

## 1.0.0 - 2026-10-18

### Added

- Custom PCB designed around the ESP32-C3-MINI-1.
- USB-C interface for power and native USB connectivity.
- 3.3V voltage regulation using the AP2112K LDO.
- 0.96-inch I²C OLED display.
- MicroSD card reader for external storage.
- 5-way D-pad for user input.
- Reset and BOOT buttons.
- Firmware developed using PlatformIO and the Arduino framework.
- Custom 3D-printed enclosure.
- KiCad project files, including schematics and PCB layout.
- PCB manufacturing files, including Gerbers, drill files, BOM, and pick-and-place positions.
- STL model of the enclosure.
- Project documentation and licensing information.

### Tested

- USB connectivity and firmware uploading.
- USB CDC serial output.
- OLED initialization and display output.
- MicroSD card initialization and storage detection.
- Wi-Fi access point and captive portal functionality.
- D-pad input detection.
- Hardware integration and enclosure assembly.

### Known Issues

- Limited cable clearance around the front faceplate.
- Side walls may separate slightly from the faceplate and backplate.
- Screw heads protrude from the enclosure surface.
- External edges and corners could be more ergonomic.

### Notes

- This release represents the first completed and functional hardware prototype.
- The device is powered through USB-C and does not include an internal battery.
- Mechanical improvements and refinements are planned for a future revision.
