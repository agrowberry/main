import plotly.graph_objects as go
import numpy as np
import pandas as pd
import serial
import time
from collections import deque
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation




mySerial = serial.Serial(port="/dev/cu.usbmodem14201")

first_line = mySerial.read_until()

fig = plt.figure()
ax = fig.add_subplot(111)
plt.set_aspect('equal', adjustable='box')


df = pd.DataFrame(columns=["x", "y"])


x = deque([])
y = deque([])


while True:
    next_line = mySerial.read_until()
    if next_line != first_line:
        xy_split = str(next_line).split(":")
        x_item = float(xy_split[0][2:])
        y_item = float(xy_split[1])
        if len(x) < 100:
            x.append(x_item)
            y.append(y_item)
        else:
            x.append(x_item)
            y.append(y_item)
            x.popleft()
            y.popleft()

        first_line = next_line
        line1.set_ydata(y)
        line1.set_xdata(x)
        plt.plot(x, y, 'r-')
        plt.pause(0.0001)

        
        