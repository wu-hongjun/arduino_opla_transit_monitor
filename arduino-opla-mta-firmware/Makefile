# Makefile for Arduino Opla MTA Firmware
# Requires arduino-cli to be installed

# Board configuration for Arduino Opla (MKR WiFi 1010)
BOARD = arduino:samd:mkrwifi1010
PORT = /dev/cu.usbmodem2101  # MKR WiFi 1010 port

# Project settings
SKETCH = arduino-opla-mta-firmware.ino
BUILD_DIR = build

.PHONY: compile upload monitor clean install-deps list-ports

# Compile the sketch
compile:
	arduino-cli compile --fqbn $(BOARD) --build-path $(BUILD_DIR) .

# Upload to the board
upload: compile
	arduino-cli upload --fqbn $(BOARD) --port $(PORT) --input-dir $(BUILD_DIR)

# Open serial monitor
monitor:
	arduino-cli monitor --port $(PORT) --config baudrate=115200

# Upload and immediately start monitoring
flash: upload
	sleep 2
	$(MAKE) monitor

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Install required dependencies
install-deps:
	arduino-cli core update-index
	arduino-cli core install arduino:samd
	arduino-cli lib install "WiFiNINA"
	arduino-cli lib install "ArduinoHttpClient"
	arduino-cli lib install "Arduino_OplaUI"

# List available ports
list-ports:
	arduino-cli board list

# Help
help:
	@echo "Available commands:"
	@echo "  compile     - Compile the sketch"
	@echo "  upload      - Upload to Arduino Opla"
	@echo "  monitor     - Open serial monitor"
	@echo "  flash       - Upload and start monitoring"
	@echo "  clean       - Clean build files"
	@echo "  install-deps- Install required libraries"
	@echo "  list-ports  - List available serial ports"