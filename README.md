# Arduino-based Motion Capture System

## PH 2024 Electronics and Computing Laboratory

### Introduction

Motion capture is the process of recording the motion of a particular subject of interest to digitally replicate that motion in real-time and store it for reference and analysis. This technology is widely used in filmmaking, video game development, medical, sports, and military applications. Traditionally, motion capture involves an array of cameras capturing the motion in 2D, which are then combined to form a 3D representation. However, this approach has drawbacks such as high equipment costs and the need for ample space.

Our project proposes an alternative method by using sensors mounted on the actor to record data directly. This allows for obtaining 3D information instantly with fewer sensors, reducing the need for multiple cameras and extensive setup space. We use the MPU6050 module (accelerometer and gyroscope) and the HMC5983 module (magnetometer) to capture the motion of an arm and forearm system.

### Theory and Technologies

- **MPU6050**: Measures acceleration and angular velocity to calculate the angle of inclination about the X and Y axes.
- **HMC5983**: Measures magnetic field strength in the X, Y, and Z axes to calculate rotation angles.
- **Kalman Filter**: Used to process sensor data and provide the best approximation of the natural angles about each axis of rotation.
- **TCA9548A Multiplexer**: Allows interfacing with multiple sensors that have the same address.
- **Arduino Uno**: Collects sensor data and communicates with a Python program via serial communication.
- **Blender**: 3D computer graphics software used to visualize the captured motion as a virtual stick figure.

### Applications

- Suitable for confined spaces due to minimal equipment and setup.
- Ideal for complex scenes with multiple actors and props as it doesn't require a line of sight to cameras.

### Components

| Component | Quantity |
|-----------|----------|
| TCA9548A I2C Multiplexer | 1 |
| GY-282 - HMC5983 three-axis magnetic field module | 3 |
| MPU6050 (Gyroscope + Accelerometer + Temperature) Sensor Module | 3 |
| Arduino Uno R3 | 1 |
| Wire & tools | - |

### Objectives

The primary objective is to capture the motion of a human arm and forearm system and display it in a virtual world in real-time, demonstrating a prototype of a camera-less motion capture system.

### Installation Instructions

1. **Arduino Setup**:
   - Connect the MPU6050 and HMC5983 modules to the Arduino Uno using the TCA9548A multiplexer.
   - Ensure all connections are secure and the modules are correctly addressed.

2. **Software Setup**:
   - Install the Arduino IDE from [Arduino's official website](https://www.arduino.cc/en/software).
   - Install necessary libraries for MPU6050, HMC5983, and TCA9548A in the Arduino IDE.
   - Upload the provided Arduino sketch to the Arduino Uno.

3. **Python Environment**:
   - Install Python from [Python's official website](https://www.python.org/downloads/).
   - Install necessary Python libraries: `pyserial`, `numpy`, and `blender` using pip:
     ```bash
     pip install pyserial numpy
     ```
   - Ensure Blender is installed and configured to accept Python scripts.

### Usage Instructions

1. **Running the System**:
   - Power on the Arduino and ensure all sensors are functioning.
   - Run the Python script to start capturing data from the Arduino.
   - The Python script processes the sensor data and calculates the rotation angles.
   - The calculated angles are sent to Blender to visualize the motion as a virtual stick figure.

2. **Visualizing Motion**:
   - Open Blender and load the provided Blender project file.
   - Execute the Blender Python script to update the stick figure in real-time based on the captured motion data.

### Contribution Guidelines

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -m 'Add some feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a pull request.

### License

This project is licensed under the MIT License - see the LICENSE file for details.

### Acknowledgements

- [Arduino](https://www.arduino.cc/)
- [Blender](https://www.blender.org/)
