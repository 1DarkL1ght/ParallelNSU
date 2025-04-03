import math
import re


def check_sin(filename, arg):
    with open(filename, 'r') as file:
        for line in file:
            match = re.match(r"Id: (\d+), result = ([-\d\.]+)", line)
            if match:
                task_id, expected = match.groups()
                expected = float(expected)
                actual = math.sin(arg)

                if not math.isclose(actual, expected, abs_tol=1e-2):
                    print(f"Ошибка в {filename}: Task {task_id}")


def check_pow(filename, *args):
    with open(filename, 'r') as file:
        for line in file:
            match = re.match(r"Id: (\d+), result = ([-\d\.]+)", line)
            if match:
                task_id, expected = match.groups()
                expected = float(expected)
                actual = math.pow(*args)
                if not math.isclose(actual, expected, abs_tol=1e-2):
                    print(f"Ошибка в {filename}: Task {task_id}")


def check_sqrt(filename, arg):
    with open(filename, 'r') as file:
        for line in file:
            match = re.match(r"Id: (\d+), result = ([-\d\.]+)", line)
            if match:
                task_id, expected = match.groups()
                expected = float(expected)
                actual = math.sqrt(arg)


                if not math.isclose(actual, expected, abs_tol=1e-2):
                    print(f"Ошибка в {filename}: Task {task_id}")


check_sin("sins.txt", 30)
check_pow("pows.txt", 2, 2)
check_sqrt("sqrts.txt", 3)

print("finished")