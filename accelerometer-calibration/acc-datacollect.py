import serial
import serial.tools.list_ports as portlist
import os
import keyboard as kb
import numpy as np
import time

BAUD_RATE = 115200  # set baud rate for serial communication
# string seperater which use to seperate components of vector readings.
SEP_CHAR = ','
# text file name which store raw accelerometer readings.
FILE_NAME = "acc-raw-data.txt"
readings = np.empty((0, 3))


def get_arduino_port(BAUD_RATE):
    # function to select the available comport for Arduino dynamically.
    # Warning - on windows operating system this function works perfectly.
    # but on other platforms (UNIX based OSs) this function may not work.
    for p in portlist.comports():
        info = p.description.lower()
        print(p)
        if "arduino" in info:
            return serial.Serial(p.device, BAUD_RATE)
    return None


def get_readings(ser, SEP_CHAR: str):
    # read the serial input and return that as a list.
    try:
        ser_reading = ser.readline().decode().strip().split(SEP_CHAR)
        if len(ser_reading) == 3:
            return list(map(float, ser_reading))
        else:
            raise Exception
    except Exception:
        get_readings(ser, SEP_CHAR)


def flush_out(n, SEP_CHAR):
    # take out few readings to 'flush' out bad ones
    for _ in range(n):
        get_readings(ser, SEP_CHAR)
        time.sleep(0.01)


def remove_bad_readings(raw_data, limit):
    # function to remove the the data points that extremely deviate from the normal distribution of data points.
    data = np.empty((0, 3))
    for row in raw_data:
        if True not in ((row > -limit) != (row < limit)):
            data = np.append(data, [row], axis=0)
    return data


# Warning - If you are not in Windows operating system, this function may not works.
# therefore on other platforms (UNIX based OSs) except Windows it is recomended to define the serial object manually.
# ser = get_arduino_port(BAUD_RATE)
ser = serial.Serial("/dev/ttyACM0", BAUD_RATE)

if ser != None:
    print("Now can start record accelerometer readings.")
    print("Press space button to get reading at any angle, but keep accelerometer at steady position.")
    print("Press escape button to leave from data reading mode.")

    while not kb.is_pressed("esc"):
        state = True
        while kb.is_pressed("space"):
            if state:
                flush_out(20, SEP_CHAR)
                state = False
            r = get_readings(ser, SEP_CHAR)
            print(r)
            readings = np.append(readings, [r], axis=0)
            time.sleep(0.01)

    # check whether already data file exist and if such file exist then remove that.
    if os.path.exists(FILE_NAME):
        os.remove(FILE_NAME)

    # save the readings to a text file
    readings = remove_bad_readings(readings, 100)
    np.savetxt(FILE_NAME, readings, delimiter="\t", fmt='%1.4f')
    print("Done")
else:
    print("No arduino board available.")
