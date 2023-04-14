import serial
import serial.tools.list_ports as portlist
import os
import keyboard as kb
import numpy as np

BAUD_RATE = 115200
SEP_CHAR = ','
LIMIT = 100
FILE_NAME = "magnetic_raw_data.txt"
readings = np.empty((0, 3))


def get_arduino_port(BAUD_RATE):
    # function to select the available com port for arduino dynamically.
    # warning - On windows operating system this function works perfectly.
    # but on other platforms this function may not work.
    for p in portlist.comports():
        info = p.description.lower()
        print(p)
        if "arduino" in info:
            return serial.Serial(p.device, BAUD_RATE)
    return None


def get_readings(ser, SEP_CHAR: str):
    # read the serial imput and return that as a list.
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


def remove_bad_readings(raw_data, limt):
    # function to remove the the data points that extremely deviate from the normal distribution of data points.
    data = np.empty((0, 3))
    for row in raw_data:
        if True not in ((row > -limt) != (row < limt)):
            data = np.append(data, [row], axis=0)
    return data


# warning - If you are not in Windows operating system, this function may not works
# therefore on other platforms except Windows it is recomended to use manual defining the serial object.
# ser = get_arduino_port(BAUD_RATE)
ser = serial.Serial("/dev/ttyACM0", BAUD_RATE)

if ser != None:
    flush_out(20, SEP_CHAR)
    input("Press enter to start reacording the readings. ")
    print("Start data recording.")
    flush_out(10, SEP_CHAR)

    r = get_readings(ser, SEP_CHAR)
    while r != None:
        print(r)
        readings = np.append(readings, [r], axis=0)
        r = get_readings(ser, SEP_CHAR)
        if kb.is_pressed('esc'):
            break

    # check whether already data file exist and if such file exist then remove that
    if os.path.exists(FILE_NAME):
        os.remove(FILE_NAME)

    # save the readings to a text file
    readings = remove_bad_readings(readings, LIMIT)
    np.savetxt(FILE_NAME, readings, delimiter="\t", fmt='%1.4f')
    print("Done")
else:
    print("No arduino board available.")
