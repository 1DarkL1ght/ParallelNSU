import csv
import matplotlib.pyplot as plt

small_size_times = []
big_size_times = []
threads_arr = [1, 2, 4, 7, 8, 16, 20, 40]

with open('threads_times_1.csv', 'r') as csvfile:
    csv_reader = csv.reader(csvfile)
    for row in csv_reader:
        if row[0] == '20000':
            for i in range(3, len(row), 2):
                small_size_times.append(float(row[i]))

        if row[0] == '40000':
            for i in range(3, len(row), 2):
                big_size_times.append(float(row[i]))
        


fig, ax = plt.subplots(1, 1)

ax.plot(threads_arr[1:], small_size_times, color='green', label='M=20000', marker='o')
ax.plot(threads_arr[1:], big_size_times, color='red', label='M=40000', marker='o')
ax.plot(threads_arr[1:], threads_arr[1:], color='blue', linestyle='dashed', label='Linear', marker='o')  
ax.grid()
ax.legend()
fig.savefig("speedups1.png")