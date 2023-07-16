# Advanced Data Logger for Home Appliance Monitoring

## Description
This project implements an advanced data logger system using Arduino to monitor and log real-time parameters of various home appliances. The system captures data such as current, voltage, power, power factor, frequency, on/off duration, door on/off status, room temperature, deep chamber temperature, evaporator thrust, RPM, body temperature, dish temperature, and water flow for different appliances like fridge, AC, induction cooker, TV, fan, and washing machine.

## Features
- Real-time Monitoring: The system continuously monitors the parameters of multiple home appliances.
- Data Logging: Captures and logs the data for analysis and tracking over time.
- Parameter Tracking: Measures and logs parameters such as current, voltage, power, power factor, frequency, on/off duration, and more.
- Appliance-Specific Features: Monitors specific parameters for each appliance, such as temperature for fridge and AC, RPM for fan, dish temperature for induction cooker, etc.

## Made for Dr. Mahfujur Rahman, EEE, DUET
This project was developed specifically for Dr. Mahfujur Rahman, Assistant Professor at the Department of Electrical and Electronic Engineering, Dhaka University of Engineering & Technology, Gazipur. It was created as a demonstration of practical implementation and as a contribution to the field of electrical engineering research and education.

## Hardware Requirements
- Arduino board (compatible with the chosen sensors and actuators)
- Sensors and actuators specific to each appliance (current sensor, voltage sensor, temperature sensor, contactless body temperature sensor, flow sensor, etc.)
- Connecting wires
- Power supply

## Software Requirements
- Arduino IDE (Integrated Development Environment)
- Libraries for the specific sensors and actuators used in the project

## Installation and Setup
1. Clone the repository to your local machine using the following command: git clone https://github.com/rezaul-rimon/Power-System-Data-Logger-for-Home-Applience.git
2. Connect the Arduino board to your computer.
3. Open the Arduino IDE.
4. Install the necessary libraries for the sensors and actuators used in the project.
5. Open the Arduino sketch (.ino file) for the specific home appliance you want to monitor.
6. Modify the pin configurations and other parameters if necessary, based on your hardware setup.
7. Upload the sketch to the Arduino board.
8. Repeat steps 5-7 for each home appliance you want to monitor.

## Usage
1. Power on the Arduino board with the connected sensors and actuators.
2. The system will start monitoring the parameters of the selected home appliances.
3. The logged data can be accessed in real-time or stored for later analysis and visualization.
4. Retrieve the logged data from the Arduino board and process it using software tools like Excel, Python, or MATLAB to generate reports, graphs, or perform further analysis.

## Contributing
Contributions to this project are welcome! If you have any suggestions, improvements, or encounter any issues, feel free to open an issue or submit a pull request.


