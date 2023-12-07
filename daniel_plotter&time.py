#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Nov 21 19:14:50 2023

@author: tscarioli
"""

import matplotlib.pyplot as plt 
import csv 
  
x = []
y = []
initial_hour = int(input("Enter the hour when testing began: "))
initial_mins = int(input("Enter the minutes when testing began: "))

filepath= " " #include the file path with the data
time_path = " " #include the file path were you want to save the time

def time(secs):
    minutes = secs / 60
    extra_mins = minutes % 60
    hour = (minutes - extra_mins) // 60  # Use floor division to get the hours
    final_minutes = initial_mins + extra_mins

    if final_minutes >= 60:
        extra_hours = final_minutes // 60
        final_hour = initial_hour + hour + extra_hours
        final_minutes = final_minutes % 60
    else:
        final_hour = initial_hour + hour

    final_hour = int(final_hour)
    final_minutes = int(final_minutes)
    final_time = (final_hour, final_minutes)
    return final_time


# reads csv file
with open(filepath, 'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=',')
    for row in lines:
        # appends rows to list
        if row[0] and row[2]:
            x_val = time(int(row[0]))
            x.append(x_val)
            y.append(float(row[3]))

# Format x-axis labels (h, hour) (m, minutes)
x_labels = [f"{h}:{m:02}" for h, m in x] 

plt.scatter(x_labels, y, color='blue', s=1)
plt.xticks(rotation=45, ha="right", ticks=x_labels[::800])
plt.title(label = "45º 11/30",  #edit for desire
          fontsize = 20,
          color= "black")

plt.show()

with open(time_path, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    for item in x_labels:
        writer.writerow([item])