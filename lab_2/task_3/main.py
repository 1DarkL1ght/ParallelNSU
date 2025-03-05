import subprocess
import csv

import matplotlib.pyplot as plt

def main():
    threads_arr = [1, 2, 4, 7, 8, 16, 20, 40]
    sizes_arr = [20000, 40000]
    threads = dict(zip(threads_arr, range(len(threads_arr))))
    sizes = dict(zip(sizes_arr, range(len(sizes_arr))))
    outputs = [[0 for _ in range(len(threads_arr))] for _ in range(len(sizes))]
    speedups = [[0 for _ in range(len(threads_arr))] for _ in range(len(sizes))]


    for num_threads in threads_arr:
        for size in sizes_arr:
            output = str(subprocess.run(f"main1.exe {num_threads} {size}", 
                capture_output=True, shell=True, text=True, encoding='cp866').stdout)
            
            outputs[sizes[size]][threads[num_threads]] = output

    for num_threads in threads_arr:
        for size in sizes_arr:
            speedups[sizes[size]][threads[num_threads]] = float(outputs[sizes[size]][threads[1]]) / float(outputs[sizes[size]][threads[num_threads]])


    with open('threads_times_1.csv', 'w') as csvfile:
        writer = csv.writer(csvfile, dialect='excel')

        writer.writerow(["", "T1", "T2", "S2", "T4", "S4", "T7", "S7", "T8", "S8", "T16", "S16", "T20", "S20", "T40", "S40"])
        for size in sizes_arr:
            writer.writerow([size, 
                outputs[sizes[size]][threads[1]], 
                outputs[sizes[size]][threads[2]], speedups[sizes[size]][1], 
                outputs[sizes[size]][threads[4]], speedups[sizes[size]][2], 
                outputs[sizes[size]][threads[7]], speedups[sizes[size]][3], 
                outputs[sizes[size]][threads[8]], speedups[sizes[size]][4],
                outputs[sizes[size]][threads[16]], speedups[sizes[size]][5],
                outputs[sizes[size]][threads[20]], speedups[sizes[size]][6],
                outputs[sizes[size]][threads[40]], speedups[sizes[size]][7]
            ])


main()