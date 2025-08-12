# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Arduino Opla Transit Monitor - A firmware application for Arduino MKR WiFi 1010 (Opla IoT Kit) that displays real-time NYC MTA transit information and weather data on a round display with touch button controls.

## Build Commands

```bash
# Compile the firmware
make compile

# Upload to Arduino (default port: /dev/cu.usbmodem2101)
make upload

# Open serial monitor (115200 baud)
make monitor

# Compile, upload, and monitor in one command
make flash

# Install required Arduino libraries
make install-deps

# Clean build artifacts
make clean

# List available serial ports (helpful if default port doesn't work)
make list-ports
```

## Architecture

The codebase follows a modular manager pattern where each component handles a specific responsibility:

1. **Main Loop** (`arduino-opla-mta-firmware.ino`): Initializes managers and coordinates updates
2. **WiFiManager**: Handles multi-network WiFi connectivity with automatic failover
3. **ModeManager**: Coordinates different display modes using a plugin architecture
4. **Display Modes** (inherit from BaseMode):
   - **AmbientDataMode**: Shows temperature, humidity, and light levels with dynamic theming
   - **NYCMTATransitMode**: Displays MTA train arrivals in radial layout
5. **MTAManager**: Fetches and stores NYC MTA train arrival data
6. **LEDManager**: Visual feedback through RGB LEDs for connection status
7. **RadialDisplay**: Library for creating circular UI layouts with rings of various element types

Components communicate through:
- Direct method calls from main loop
- Shared state through getters (e.g., WiFi connection status)
- Button events handled by ModeManager and delegated to active mode
- Mode switching managed centrally with enter/exit lifecycle

## Key Configuration Points

1. **WiFi Credentials** (`arduino-opla-mta-firmware/config.h`):
   - Multiple networks supported with priority ordering
   - Currently has hardcoded credentials that should be secured

2. **API Configuration** (`arduino-opla-mta-firmware.ino`):
   - MTA_PROXY_URL: Needs proper endpoint configuration
   - OPENWEATHER_API_KEY: Requires valid API key
   - Currently using simulated data for testing

3. **Hardware Configuration**:
   - Board: Arduino MKR WiFi 1010
   - Port: Configurable in Makefile (default: `/dev/cu.usbmodem2101`)
   - Touch buttons: TOUCH0-TOUCH4 mapped to mode navigation

## Development Workflow

1. Modify component files (.cpp/.h pairs) in `arduino-opla-mta-firmware/`
2. Run `make compile` to check for compilation errors
3. Use `make flash` to upload and immediately see serial output
4. Serial monitor shows debug information for WiFi connection, API calls, and mode changes

## Testing Approach

- No formal test framework in place
- Testing done through serial output debugging
- Components can be tested independently by commenting out others in main loop
- Use simulated data modes for offline testing

## Important Notes

- The project recently migrated from Arduino Opla to MKR WiFi 1010 board
- WiFi credentials and API keys need to be properly configured before deployment
- The MTA proxy endpoint must be set up separately (not included in repo)
- Serial output at 115200 baud provides extensive debugging information
- Touch button debouncing is handled in the main loop with 200ms delay