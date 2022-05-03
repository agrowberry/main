import plotly.graph_objects as go
import numpy as np
import pandas as pd
import serial
import time
import re
from collections import deque



def read_pc():	
	mySerial = serial.Serial(port="/dev/cu.usbmodem14101")
	lines = []
	written_to = False
	lines = 0
	odometry_data = pd.DataFrame(columns=["Point", "X", "Y", "Phi"])
	accel_data = pd.DataFrame(columns=["Point", "X", "Y", "Phi"])
	Odometry = True

	with open("results.txt", mode='w') as f:
		while not written_to:
			next_line = mySerial.read_until()
			next_line = str(next_line)
			next_line = next_line[2:-3]
			next_line = next_line.split(sep=":")
			if lines == 31:
				Odometry = False
			elif lines > 0 and Odometry:
				odometry_data.loc[next_line[0], "Point"] = float(next_line[0])
				odometry_data.loc[next_line[0], "X"] = float(next_line[1])
				odometry_data.loc[next_line[0], "Y"] = float(next_line[2])
				odometry_data.loc[next_line[0], "Phi"] = float(next_line[3])
			elif lines > 31:
				Odometry = False
				accel_data.loc[next_line[0], "Point"] = float(next_line[0])
				accel_data.loc[next_line[0], "X"] = float(next_line[1])
				accel_data.loc[next_line[0], "Y"] = float(next_line[2])
				accel_data.loc[next_line[0], "Phi"] = float(next_line[3])

			
			# f.write(next_line[2:-3] + '\r')

			lines += 1
			if lines == 62:
				written_to = True


	odometry_data.to_csv("Odo9.csv")
	accel_data.to_csv("Accel1.csv")

	print("File Written!")

def read_bumper():
	mySerial = serial.Serial(port="/dev/cu.usbmodem14201")
	running = True
	serial_output = deque([])

	while running:
		try:
			cur_line = mySerial.read_until()
			serial_output.append(str(cur_line))
		except:
			running = False
			print("Serial connection terminated...")

	df = pd.DataFrame(columns=["LB", "RB"])

	for line in serial_output:
		line_split = line.split(sep=",")
		print(line_split)
		df.loc[int(line_split[2][:-4]), "LB"] = int(line_split[0][2:])
		df.loc[int(line_split[2][:-4]), "RB"] = int(line_split[1])

	df.to_csv("Bumper_Results.csv")
	print("Bumper Results Saved...")
	

if __name__ == "__main__":
	read_pc()



