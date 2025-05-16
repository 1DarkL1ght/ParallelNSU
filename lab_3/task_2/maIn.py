import math
import re


def check_sin(filename):
    with open(filename, 'r') as file:
        for line in file:
            match = re.match(r"Task #(\d+): sin\(([-\d\.]+)\) = ([-\d\.]+)", line)
            if match:
                task_id, arg, expected = match.groups()
                expected = float(expected)
                arg = float(arg)
                actual = math.sin(arg)

                if not math.isclose(actual, expected, abs_tol=1e-2):
                    print(f"Ошибка в {filename}: Task {task_id}")


def check_pow(filename):
    with open(filename, 'r') as file:
        for line in file:
            match = re.match(r"Task #(\d+): pow\(([-\d\.]+), ([-\d\.]+)\) = ([-\d\.]+)", line)
            if match:
                task_id, arg1, arg2, expected = match.groups()
                # print(arg1, arg2, expected)
                expected = float(expected)
                arg1, arg2 = float(arg1), float(arg2)
                actual = math.pow(arg1, arg2)
                if not math.isclose(actual, expected, abs_tol=1e-2):
                    print(f"Ошибка в {filename}: Task {task_id}")


def check_sqrt(filename):
    with open(filename, 'r') as file:
        for line in file:
            match = re.match(r"Task #(\d+): sqrt\(([-\d\.]+)\) = ([-\d\.]+)", line)
            if match:
                task_id, arg, expected = match.groups()
                expected = float(expected)
                arg = float(arg)
                actual = math.sqrt(arg)

                if not math.isclose(actual, expected, abs_tol=1e-2):
                    print(f"Ошибка в {filename}: Task {task_id}")

                    
def check_sqare(filename):
    with open(filename, 'r') as file:
        for line in file:
            match = re.match(r"Task #(\d+): \(([-\d\.]+)\)\^2 = ([-\d\.]+)", line)
            if match:
                task_id, arg, expected = match.groups()
                expected = float(expected)
                arg = float(arg)
                # print(arg, expected)
                actual = arg ** 2


                if not math.isclose(actual, expected, abs_tol=1e-2):
                    print(f"Ошибка в {filename}: Task {task_id}")


check_sin("sin.txt")
check_pow("pow.txt")
check_sqrt("sqrt.txt")
check_sqare("square.txt")

print("finished")