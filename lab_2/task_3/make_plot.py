import csv
import matplotlib.pyplot as plt

def main(type):

    speedups = []
    times = []
    threads_arr = [1, 2, 4, 7, 8, 16, 20, 40]

    with open(f'threads_times_3_{type}.csv', 'r') as csvfile:
        csv_reader = csv.reader(csvfile)
        
        for row in csv_reader:
            if row[0] != 'T1':
                for i in range(2, len(row), 2):
                    speedups.append(float(row[i]))

                times.append(float(row[0]))
                for i in range(1, len(row), 2):
                    times.append(float(row[i]))

            


    fig, (ax1, ax2) = plt.subplots(2, 1)

    ax1.plot(threads_arr[1:], speedups, color='green', label='M=20000', marker='o')
    ax1.plot(threads_arr[1:], threads_arr[1:], color='blue', linestyle='dashed', label='Linear', marker='o')  
    ax1.grid()
    ax1.legend()
    ax1.set_xlabel('Num threads')
    ax1.set_ylabel('Speedup')

    ax2.plot(threads_arr, times, color = 'green', label=f'static, 100, {times[-1]}', marker='o')
    ax2.grid()
    ax2.legend()
    ax2.set_xlabel('Num threads')
    ax2.set_ylabel('Time')

    plt.tight_layout()
    fig.savefig(f"speedups{type}.png")

main(1)