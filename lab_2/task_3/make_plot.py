import csv
import matplotlib.pyplot as plt

def main(type):

    speedups = []
    times = []
    threads_arr = [1, 2, 4, 7, 8, 16, 20, 40]

    with open(f'threads_times_{type}.csv', 'r') as csvfile:
        csv_reader = csv.reader(csvfile)
        for row in csv_reader:
            for i in range(3, len(row), 2):
                speedups.append(float(row[i]))

            times.append(float(row[1]))
            for i in range(2, len(row), 2):
                times.append(float(row[i]))

            


    fig, (ax1, ax2) = plt.subplots(1, 1)

    ax1.plot(threads_arr[1:], speedups, color='green', label='M=20000', marker='o')
    ax1.plot(threads_arr[1:], threads_arr[1:], color='blue', linestyle='dashed', label='Linear', marker='o')  
    ax1.grid()
    ax1.legend()

    ax2.plot(threads_arr[1:], times, color = 'green', label='Variant2', marker='0')
    ax2.grid()
    ax2.legend()
    fig.savefig(f"speedups{type}.png")

main(2)