import matplotlib.pyplot as plt
import numpy as np

# use https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#igrfwmm to find magnetic fieled stringth according to the location.
# these must be decide according to the magnetic intensity of the readings at the location and variation of the data.
EARTH_MAG_STRENGTH = 41.0337    # 41.0463
MAG_READING_FILE = "mag-raw-data.txt"

# Ainverse = np.array([[0.919380, 0.016652, 0.014108],
#              [0.016652, 0.947940, 0.000166],
#              [0.014108, 0.000166, 0.913351]])
# b = np.array([[2.187339],
#               [2.873407],
#               [-12.222010]])

Ainverse = np.array([[0.918885, 0.016650, 0.014657],
             [0.016650, 0.948416, -0.000023],
             [0.014657, -0.000023, 0.913989]])
b = np.array([[2.188976],
              [2.871479],
              [-12.204237]])


def draw_circle(r, x0=0, y0=0, color='b'):
    # function to draw a cicle that center located in (x0, y0)
    theta = np.linspace(0, 2 * np.pi, 200)
    a = r * np.cos(theta) + x0
    b = r * np.sin(theta) + y0
    plt.plot(a, b, color)


def get_calibrated_data(raw_data, Ainverse, b):
    # function to Genere calibrated magnetometer values.
    calibrated_data = np.empty((0, 3))
    for row in raw_data:
        row = row.reshape(3, 1)
        calibrated_reading = np.matmul(Ainverse, row - b).reshape(1, 3)
        calibrated_data = np.append(
            calibrated_data, calibrated_reading, axis=0)
    return calibrated_data


def make_2d_plot(fig_num, x, y, calib_x, calib_y, circle_r, xlabel, ylabel, title,
                 legend: list, raw_color='r', calib_color='b', circle_color='g'):
    plt.figure(fig_num)
    plt.plot(x, y, f'x{raw_color}')  # draw Y vs X
    plt.plot(calib_x, calib_y, f'x{calib_color}')  # draw claibrated Y vs X
    # draw the circle that shows the magnitude of Earth's Mag. field strength
    draw_circle(circle_r, color=circle_color)
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.legend(legend)
    plt.grid()
    plt.axis('equal')
    plt.tight_layout()


raw_data = np.genfromtxt(MAG_READING_FILE, delimiter='\t')
calibrated_data = get_calibrated_data(raw_data, Ainverse, b)

# plot 3D plot for raw data and calibrated data
fig = plt.figure(1, figsize=(6, 6))
ax = fig.add_subplot(111, projection='3d')
ax.plot(raw_data[:, 0], raw_data[:, 1],
        raw_data[:, 2], 'xr')  # draw 3D plot
ax.plot(calibrated_data[:, 0], calibrated_data[:, 1],
        calibrated_data[:, 2], 'xb')  # draw claibrated 3D plot
ax.set_title("Magnetometer readings")
ax.set_xlabel('X (uT)')
ax.set_ylabel('Y (uT)')
ax.set_zlabel('Z (uT)')
ax.legend(["Raw data", "Calibrated Data"])
ax.grid()
ax.axis('equal')
plt.tight_layout()

make_2d_plot(2, raw_data[:, 0], raw_data[:, 1],
             calibrated_data[:, 0], calibrated_data[:,
                                                    1], EARTH_MAG_STRENGTH, 'X (uT)', 'Y (uT)', "Y vs X",
             ["Raw data", "Calibrated Data", "Magnitude of Earth Magnetic field strength"])
make_2d_plot(3, raw_data[:, 1], raw_data[:, 2],
             calibrated_data[:, 1], calibrated_data[:,
                                                    2], EARTH_MAG_STRENGTH, 'Y (uT)', 'Z (uT)', "Z vs Y",
             ["Raw data", "Calibrated Data", "Magnitude of Earth Magnetic field strength"])
make_2d_plot(4, raw_data[:, 0], raw_data[:, 2],
             calibrated_data[:, 0], calibrated_data[:,
                                                    2], EARTH_MAG_STRENGTH, 'X (uT)', 'Z (uT)', "Z vs X",
             ["Raw data", "Calibrated Data", "Magnitude of Earth Magnetic field strength"])

plt.show()
