import subprocess
import csv

# import matplotlib.pyplot as plt

def main(type):
    threads_arr = [1, 2, 4, 7, 8, 16, 20, 40]
    threads = dict(zip(threads_arr, range(len(threads_arr))))
    speedups = [0 for _ in range(len(threads_arr))]
    times = [0 for _ in range(len(threads_arr))]


    for num_threads in threads_arr:
        output = str(subprocess.run(f"./main{type} {num_threads}", 
            capture_output=True, shell=True, text=True, encoding='cp866').stdout)
        
        print(f'Threads = {num_threads}, time = {output}')

        times[threads[num_threads]] = output

    for num_threads in threads_arr:
        speedups[threads[num_threads]] = float(times[threads[1]]) / float(times[threads[num_threads]])


    with open(f'threads_times_3_{type}.csv', 'w') as csvfile:
        writer = csv.writer(csvfile, dialect='excel')
        
        writer.writerow(["T1", "T2", "S2", "T4", "S4", "T7", "S7", "T8", "S8", "T16", "S16", "T20", "S20", "T40", "S40"])
        writer.writerow([ 
            times[threads[1]], 
            times[threads[2]], speedups[1], 
            times[threads[4]], speedups[2], 
            times[threads[7]], speedups[3], 
            times[threads[8]], speedups[4],
            times[threads[16]], speedups[5],
            times[threads[20]], speedups[6],
            times[threads[40]], speedups[7]
        ])


main(1)