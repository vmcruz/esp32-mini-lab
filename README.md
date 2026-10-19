# ESP32-Mini-Lab

A compact, portable ESP32-C3 development device built from a custom-designed PCB and a 3D-printed enclosure. It combines a USB-C-powered ESP32-C3-MINI-1, a microSD card reader, an OLED display, and a 5-way D-pad into a small, self-contained hardware platform for experimentation and development.

The project was designed from scratch, including the PCB layout, firmware, and enclosure. It serves as a hands-on platform for exploring embedded development, hardware integration, and portable ESP32 applications.

## Features

- **ESP32-C3-MINI-1** microcontroller.
- **USB-C power and connectivity**, with no internal battery required.
- **0.96-inch I²C OLED display** for status information and interface output.
- **MicroSD card reader** for external storage.
- **5-way D-pad** for directional input and selection.
- **Custom-designed PCB**, manufactured and assembled by PCBWay.
- **Custom 3D-printed enclosure**, designed to house the PCB and its components.
- **PlatformIO + Arduino framework** for firmware development.

## Project Status

**V1 — Completed and functional.**

The first revision has been fabricated, assembled, enclosed, and tested. The core hardware and firmware are operational (see [CHANGELOG.md](CHANGELOG.md)).

A V2 revision is planned to refine the enclosure's ergonomics, cable clearance, screw mounting, and closure mechanism based on real-world testing (see [TODO.md](TODO.md)).

## Goals

- Build a compact, portable platform for ESP32 experimentation.
- Integrate common embedded-development components into a single device.
- Explore custom PCB design, firmware development, and 3D-printed mechanical design.
- Iterate through hardware revisions based on practical use and testing.

## Power

The device is powered through USB-C and is designed to operate from a standard 5 V USB power source. Compatible smartphones may also be used as a power source, potentially eliminating the need to carry a separate power bank.

## Repository Structure

| Directory / File      | Description                                                                                                                        |
| --------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| `Firmware/`           | Complete firmware source code and development configuration.                                                                       |
| `Hardware/Enclosure/` | 3D-printable enclosure model.                                                                                                      |
| `Hardware/KiCad/`     | Complete KiCad project, including schematics and PCB design files.                                                                 |
| `Hardware/PCBWay/`    | PCB manufacturing and assembly files, including Gerbers, drill files, bill of materials (BOM), and pick-and-place positions (POS). |
| `LICENSES/`           | License texts applicable to the project components.                                                                                |

## License

ESP32-Mini-Lab uses different open-source licenses for its components:

- **Firmware:** [MIT License](LICENSES/MIT.txt)
- **PCB and schematics:** [CERN-OHL-P-2.0](LICENSES/CERN-OHL-P-2.0.txt)
- **3D enclosure models:** [CERN-OHL-P-2.0](LICENSES/CERN-OHL-P-2.0.txt)

Please refer to the applicable license for each component.

## Acknowledgements

Special thanks to **[PajoPCB](https://www.youtube.com/@PajoPCB)** for the tutorial that inspired the PCB design of ESP32-Mini-Lab.

Original tutorial: **[**KiCad 9 ESP32-C3 Mini PCB Design | Full Schematic Walkthrough**](https://www.youtube.com/watch?v=-iZbwzr0dS8)** (watch on youtube)

Thanks for sharing your knowledge and making hardware design more accessible to the maker community.
