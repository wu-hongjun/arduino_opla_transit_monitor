# Implementation Plan

- [x] 1. Set up project structure and core configuration
  - Create Arduino sketch directory structure with separate files for each major component
  - Define system configuration constants for WiFi credentials, MTA stations, and API keys
  - Set up Arduino libraries dependencies (WiFi, HTTP client, display, JSON parsing)
  - _Requirements: 1.1, 7.1, 7.2, 7.3, 7.4_

- [ ] 2. Implement WiFi connection management
  - Create WiFiManager class with automatic network connection and failover logic
  - Implement connection status monitoring with retry mechanisms
  - Add support for multiple hardcoded WiFi credentials with priority ordering
  - Write unit tests for WiFi connection logic using mock network responses
  - _Requirements: 1.1, 1.2, 1.3, 1.4, 1.5, 1.6_

- [ ] 3. Implement LED status indication system
  - Create LEDManager class to control L1 and L2 RGB LEDs
  - Implement color mapping for WiFi status (red/yellow/green) on L1 LED
  - Implement color mapping for data status (red/yellow/green) on L2 LED
  - Add smooth color transition animations between status changes
  - Write unit tests for LED status logic and color mappings
  - _Requirements: 1.2, 1.3, 1.4, 2.3, 2.4, 2.5, 3.2, 3.3, 3.4, 4.2, 4.3, 4.4_

- [ ] 4. Create HTTP client and API base classes
  - Implement HTTPClient wrapper with timeout and error handling
  - Create base APIClient class with common functionality for all API calls
  - Add request throttling and rate limiting mechanisms
  - Implement exponential backoff retry logic for failed requests
  - Write unit tests for HTTP client error handling and retry logic
  - _Requirements: 6.4, 6.3_

- [ ] 5. Implement MTA API client and data parsing
  - Create MTAAPIClient class to fetch real-time train data from MTA GTFS-RT feeds
  - Implement GTFS-RT protobuf parsing to extract train arrival information
  - Add data validation and error handling for malformed API responses
  - Create TrainArrival data structure and parsing methods
  - Write unit tests for MTA data parsing with mock API responses
  - _Requirements: 2.1, 2.6, 2.7, 6.1, 6.2_

- [ ] 6. Implement weather API client and data parsing
  - Create WeatherAPIClient class to fetch current weather and rain probability
  - Implement JSON parsing for weather API responses (OpenWeatherMap format)
  - Add location-based weather data fetching using configured coordinates
  - Create WeatherData data structure and parsing methods
  - Write unit tests for weather data parsing with mock API responses
  - _Requirements: 3.1, 3.5, 3.6, 4.1, 4.5, 4.6_

- [ ] 7. Implement data caching system
  - Create DataCache class to store and manage cached API responses
  - Implement timestamp-based cache validation and expiration logic
  - Add cache persistence across system resets using EEPROM or flash storage
  - Implement cache fallback when API calls fail
  - Write unit tests for cache management and expiration logic
  - _Requirements: 6.1, 6.2, 2.7, 3.6, 4.6_

- [ ] 8. Create circular display rendering engine
  - Implement DisplayManager class for the Arduino Opla's circular TFT display
  - Create base rendering methods for text, shapes, and layouts optimized for circular format
  - Implement coordinate transformation utilities for circular display positioning
  - Add font management and text sizing for optimal readability
  - Write unit tests for display coordinate calculations and layout logic
  - _Requirements: 5.1, 5.4_

- [ ] 9. Implement MTA data display layouts
  - Create renderMTAData method to display train arrivals in circular/radial layout
  - Implement train line color coding and station name display
  - Add arrival time formatting (minutes) with directional indicators
  - Create loading and error state displays for MTA mode
  - Write unit tests for MTA display layout calculations
  - _Requirements: 2.1, 2.6, 5.1, 5.2, 5.5_

- [ ] 10. Implement weather data display layouts
  - Create renderWeatherData method for temperature and humidity display
  - Implement large, readable font rendering for temperature values
  - Add humidity percentage display with appropriate formatting
  - Create renderRainData method for rain probability display
  - Write unit tests for weather display formatting and layout
  - _Requirements: 3.1, 3.5, 4.1, 4.5, 5.1, 5.3, 5.5_

- [ ] 11. Implement button input handling system
  - Create ButtonHandler class with debouncing and press detection
  - Implement button-to-mode mapping for all 5 physical buttons
  - Add button press event routing to appropriate mode handlers
  - Implement button state management and event queuing
  - Write unit tests for button debouncing and event handling logic
  - _Requirements: 2.1, 3.1, 4.1_

- [ ] 12. Create mode management system
  - Implement ModeManager class to coordinate between different operational modes
  - Create mode transition logic and state management
  - Implement mode-specific update loops and data refresh timing
  - Add mode persistence to remember last active mode across resets
  - Write unit tests for mode transitions and state management
  - _Requirements: 2.1, 3.1, 4.1, 2.7, 3.6, 4.6_

- [ ] 13. Integrate MTA modes with complete data flow
  - Wire MTA API client to mode manager for buttons 1, 2, and 3
  - Implement automatic data refresh based on configured intervals
  - Add error handling and fallback to cached data for MTA modes
  - Integrate LED status updates with MTA data fetching states
  - Write integration tests for complete MTA mode workflows
  - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7_

- [ ] 14. Integrate weather modes with complete data flow
  - Wire weather API client to mode manager for buttons 4 and 5
  - Implement automatic data refresh for weather and rain probability
  - Add error handling and fallback to cached data for weather modes
  - Integrate LED status updates with weather data fetching states
  - Write integration tests for complete weather mode workflows
  - _Requirements: 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6_

- [ ] 15. Implement main application loop and system initialization
  - Create main Arduino setup() function with hardware initialization
  - Implement main loop() function coordinating all system components
  - Add system startup sequence with WiFi connection and initial data fetch
  - Implement graceful error recovery and system reset capabilities
  - Write integration tests for complete system startup and operation
  - _Requirements: 1.1, 6.5, 6.3_

- [ ] 16. Add comprehensive error handling and logging
  - Implement system-wide error logging with serial output for debugging
  - Add graceful degradation when external services are unavailable
  - Implement memory monitoring and garbage collection
  - Add watchdog timer for system recovery from hangs or crashes
  - Write tests for error scenarios and recovery mechanisms
  - _Requirements: 6.1, 6.2, 6.3, 6.4, 6.5_

- [ ] 17. Create configuration validation and compile-time checks
  - Implement validation for MTA train line and station code configurations
  - Add compile-time checks for required API keys and WiFi credentials
  - Create configuration documentation with valid MTA station codes
  - Add configuration examples for common NYC transit scenarios
  - Write tests for configuration validation logic
  - _Requirements: 7.1, 7.2, 7.3, 7.4, 7.5_

- [ ] 18. Optimize performance and memory usage
  - Profile memory usage during normal operation and optimize data structures
  - Implement efficient string handling to minimize heap fragmentation
  - Optimize display rendering for smooth updates and transitions
  - Add power management features for battery operation if applicable
  - Write performance tests and benchmarks for critical system components
  - _Requirements: 5.4, 6.5_