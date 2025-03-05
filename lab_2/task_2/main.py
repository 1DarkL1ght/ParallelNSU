import subprocess
import csv

# import matplotlib.pyplot as plt

def main():
    threads_arr = [1, 2, 4, 7, 8, 16, 20, 40]
    steps_arr = [20000000, 40000000]
    threads = dict(zip(threads_arr, range(len(threads_arr))))
    steps = dict(zip(steps_arr, range(len(steps_arr))))
    outputs = [[0 for _ in range(len(threads_arr))] for _ in range(len(steps))]
    speedups = [[0 for _ in range(len(threads_arr))] for _ in range(len(steps))]


    for num_threads in threads_arr:
        for step in steps_arr:
            output = str(subprocess.run(f"./main {num_threads} {step}", 
                capture_output=True, shell=True, text=True, encoding='cp866').stdout)
            
            outputs[steps[step]][threads[num_threads]] = output

    for num_threads in threads_arr:
        for step in steps_arr:
            speedups[steps[step]][threads[num_threads]] = float(outputs[steps[step]][threads[1]]) / float(outputs[steps[step]][threads[num_threads]])


    with open('threads_times.csv', 'w') as csvfile:
        writer = csv.writer(csvfile, dialect='excel')

        writer.writerow(["", "T1", "T2", "S2", "T4", "S4", "T7", "S7", "T8", "S8", "T16", "S16", "T20", "S20", "T40", "S40"])
        for step in steps_arr:
            writer.writerow([step, 
                outputs[steps[step]][threads[1]], 
                outputs[steps[step]][threads[2]], speedups[steps[step]][1], 
                outputs[steps[step]][threads[4]], speedups[steps[step]][2], 
                outputs[steps[step]][threads[7]], speedups[steps[step]][3], 
                outputs[steps[step]][threads[8]], speedups[steps[step]][4],
                outputs[steps[step]][threads[16]], speedups[steps[step]][5],
                outputs[steps[step]][threads[20]], speedups[steps[step]][6],
                outputs[steps[step]][threads[40]], speedups[steps[step]][7]
            ])


main()