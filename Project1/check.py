import numpy as np
import matplotlib.pyplot as plt
#from scipy import stats as sts
import statistics as stat

def reading(filename):
    with open(filename) as file:
        lines = [int(line.rstrip()) for line in file]
    print(lines)

def checking():
    print()

if __name__ == "__main__":
    reading('1matrix/1_matrix200.txt')