# Requirements Document

## Introduction

This feature involves developing firmware for the Arduino Opla that leverages its WiFi capabilities to display real-time MTA transit data and weather information. The system will support multiple operational modes accessible through physical buttons, with visual status indicators using the device's built-in LEDs. The circular display will be optimized to show transit schedules, weather data, and other information in an intuitive format.

## Requirements

### Requirement 1

**User Story:** As a commuter, I want the Arduino Opla to automatically connect to my home/office WiFi networks, so that I can access real-time transit and weather data without manual network configuration.

#### Acceptance Criteria

1. WHEN the device boots up THEN the system SHALL attempt to connect to a hardcoded list of WiFi networks in priority order
2. WHEN WiFi is disconnected THEN the L1 LED SHALL display red color
3. WHEN WiFi is connecting THEN the L1 LED SHALL display yellow color  
4. WHEN WiFi is successfully connected THEN the L1 LED SHALL display green color
5. IF connection to the primary network fails THEN the system SHALL attempt connection to the next network in the list
6. WHEN all networks fail THEN the system SHALL retry the connection sequence after a 30-second delay

### Requirement 2

**User Story:** As a transit user, I want to view real-time MTA train arrival times for specific train lines at specific stations, so that I can plan my commute effectively.

#### Acceptance Criteria

1. WHEN button 1, 2, or 3 is pressed THEN the system SHALL display MTA train data for the configured train/station combination
2. WHEN in MTA mode THEN the circular display SHALL show the next 3 train arrivals for both directions
3. WHEN MTA data is successfully retrieved THEN the L2 LED SHALL display green color
4. WHEN MTA data retrieval fails THEN the L2 LED SHALL display red color
5. WHEN MTA data is being fetched THEN the L2 LED SHALL display yellow color
6. WHEN displaying train data THEN the system SHALL show train line, station name, and arrival times in minutes
7. WHEN train data is older than 2 minutes THEN the system SHALL automatically refresh the data

### Requirement 3

**User Story:** As a user, I want to view current weather conditions including temperature and humidity, so that I can make informed decisions about my day.

#### Acceptance Criteria

1. WHEN button 4 is pressed THEN the system SHALL display current temperature and humidity data
2. WHEN weather data is successfully retrieved THEN the L2 LED SHALL display green color
3. WHEN weather data retrieval fails THEN the L2 LED SHALL display red color
4. WHEN weather data is being fetched THEN the L2 LED SHALL display yellow color
5. WHEN displaying weather data THEN the system SHALL show temperature in Fahrenheit and humidity as percentage
6. WHEN weather data is older than 10 minutes THEN the system SHALL automatically refresh the data

### Requirement 4

**User Story:** As a user, I want to view the likelihood of rain for my current location, so that I can decide whether to bring an umbrella.

#### Acceptance Criteria

1. WHEN button 5 is pressed THEN the system SHALL display rain probability data
2. WHEN rain data is successfully retrieved THEN the L2 LED SHALL display green color
3. WHEN rain data retrieval fails THEN the L2 LED SHALL display red color
4. WHEN rain data is being fetched THEN the L2 LED SHALL display yellow color
5. WHEN displaying rain data THEN the system SHALL show probability as percentage and time window
6. WHEN rain data is older than 15 minutes THEN the system SHALL automatically refresh the data

### Requirement 5

**User Story:** As a user, I want the display to be optimized for the circular screen format, so that information is clearly readable and aesthetically pleasing.

#### Acceptance Criteria

1. WHEN displaying any mode THEN the system SHALL format content to fit the circular display optimally
2. WHEN showing train data THEN the system SHALL arrange arrival times in a circular or radial layout
3. WHEN showing weather data THEN the system SHALL use large, readable fonts appropriate for the circular format
4. WHEN transitioning between modes THEN the system SHALL provide smooth visual transitions
5. WHEN no data is available THEN the system SHALL display an appropriate message centered on the circular display

### Requirement 6

**User Story:** As a user, I want the system to handle network and API failures gracefully, so that the device remains functional even when external services are unavailable.

#### Acceptance Criteria

1. WHEN API calls fail THEN the system SHALL display cached data if available with a "last updated" timestamp
2. WHEN no cached data exists THEN the system SHALL display an appropriate error message
3. WHEN network connectivity is restored THEN the system SHALL automatically resume data fetching
4. WHEN API rate limits are exceeded THEN the system SHALL implement exponential backoff retry logic
5. WHEN system encounters critical errors THEN the system SHALL log errors and attempt graceful recovery

### Requirement 7

**User Story:** As a developer, I want the firmware to be configurable for different train lines and stations, so that the device can be customized for different users' commute patterns.

#### Acceptance Criteria

1. WHEN compiling firmware THEN the system SHALL allow configuration of train lines and stations through compile-time constants
2. WHEN configuring MTA data THEN each of the 3 MTA buttons SHALL be assignable to different train/station combinations
3. WHEN configuring WiFi THEN the system SHALL support up to 5 different WiFi network credentials
4. WHEN configuring location THEN the weather services SHALL use a configurable location for weather and rain data
5. WHEN updating configuration THEN the system SHALL validate train line and station codes against known MTA values