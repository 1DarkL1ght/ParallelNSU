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
    final_data = [[0 for _ in range(len(threads_arr) * 2 - 1)] for _ in range(len(sizes))]


    for num_threads in threads_arr:
        for size in sizes_arr:
            output = str(subprocess.run(f"main.exe {num_threads} {size}", 
                capture_output=True, shell=True, text=True, encoding='cp866').stdout)
            
            outputs[sizes[size]][threads[num_threads]] = output
            print(f"nthreads = {num_threads}, size = {size}, output = {output}")

    for num_threads in threads_arr:
        for size in sizes_arr:
            speedups[sizes[size]][threads[num_threads]] = float(outputs[sizes[size]][threads[1]]) / float(outputs[sizes[size]][threads[num_threads]])


    print(f"test = {outputs[sizes[20000]][threads[1]]}, {outputs[sizes[40000]][threads[1]]}")

    with open('threads_times.csv', 'w') as csvfile:
        writer = csv.writer(csvfile, dialect='excel')

        writer.writerow(["", "T1", "T2", "S2", "T4", "S4", "T7", "S7", "T8", "S8", "T16", "S16", "T20", "S20", "T40", "S40"])
        
        writer.writerow([20000, 
            outputs[sizes[20000]][threads[1]], 
            outputs[sizes[20000]][threads[2]], speedups[sizes[20000]][1], 
            outputs[sizes[20000]][threads[4]], speedups[sizes[20000]][2], 
            outputs[sizes[20000]][threads[7]], speedups[sizes[20000]][3], 
            outputs[sizes[20000]][threads[8]], speedups[sizes[20000]][4],
            outputs[sizes[20000]][threads[16]], speedups[sizes[20000]][5],
            outputs[sizes[20000]][threads[20]], speedups[sizes[20000]][6],
            outputs[sizes[20000]][threads[40]], speedups[sizes[20000]][7]
        ])

        writer.writerow([40000, 
            outputs[sizes[40000]][threads[1]], 
            outputs[sizes[40000]][threads[2]], speedups[sizes[40000]][1], 
            outputs[sizes[40000]][threads[4]], speedups[sizes[40000]][2], 
            outputs[sizes[40000]][threads[7]], speedups[sizes[40000]][3], 
            outputs[sizes[40000]][threads[8]], speedups[sizes[40000]][4],
            outputs[sizes[40000]][threads[16]], speedups[sizes[40000]][5],
            outputs[sizes[40000]][threads[20]], speedups[sizes[40000]][6],
            outputs[sizes[40000]][threads[40]], speedups[sizes[40000]][7]
        ])

main()