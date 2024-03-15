import numpy as np
import matplotlib.pyplot as plt
from scipy import stats as sts
import statistics as stat

def reading(filename):
    with open(filename) as file:
        lines = file.readlines()
    mat = []
    for l in lines:
        mat.append(l.split('\n')[0])
    for i in range(1, len(mat)):
        mat[i] = mat[i].split(' ')
    n = int(mat[0])
    res_mat = np.zeros((n, n))
    res_mat[0][0] = n
    for i in range(n):
        for j in range(n):
            res_mat[i][j] = int(mat[i+1][j])
    return res_mat

def checking(mat1, mat2, mat_res, n):
    check_mat = mat1.dot(mat2)
    for i in range(n):
        for j in range(n):
            if check_mat[i][j] != mat_res[i][j]:
                return False
    return True

def make_stat():
    with open('times.txt') as file:
        lines = file.readlines()
    times = []
    print(lines)
    for l in lines:
        times.append(l.split(';'))
        
    means = []
    intervals = []
    
    arr_n =[]
    
    for i in range(len(times)):
        for j in range(len(times[i])):
            if times[i][j] != '\n':
                times[i][j] = int(times[i][j])
        times[i].remove('\n')
        arr_n.append(times[i][0])
        times[i].pop(0)
        
        means.append(stat.mean(times[i]))
        intervals.append(sts.t.interval(alpha = 0.95, df = len(times[i]) - 1, loc = stat.mean(times[i]), scale = sts.sem(times[i])))
    
    file1 = open('statistics.txt', 'w')
    for i in range(len(means)):
        file1.write(f'{means[i]} - {intervals[i]}\n')
    print(times)
    print(means)
    print(intervals)
    
    plt.plot(arr_n, means)
    plt.scatter(arr_n, means)
    plt.savefig('plot_times.png')
    plt.show()
    

if __name__ == "__main__":
    file = open('checking.txt', 'w')
    make_stat()
    for n in range(200, 2001, 200):
        mat1 = reading(f'1matrix/1_matrix{n}.txt')
        mat2 = reading(f'2matrix/2_matrix{n}.txt')
        mat_res = reading(f'res_matrix/res_matrix{n}.txt')
        res = checking(mat1, mat2, mat_res, n)
        file.write(f'Cheling for n = {n}: {res}\n')
        print(n, res)
            
        
    