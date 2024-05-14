import numpy as np
import matplotlib.pyplot as plt
from scipy import stats as sts
import statistics as stat
import csv
import random

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
    # with open(f'times/time_1.txt') as file:
    #     lines = file.readlines()
    # times = []
    # #print(lines)
    # for l in lines:
    #     times.append(l.split(';'))
        
    # means = []
    # intervals = []
    # arr_n =[]
    
    # file_wr = open(f'times/time_20.txt', 'w')
    # for i in range(len(times)):
    #     for j in range(len(times[i])):
    #         if times[i][j] != '\n':
    #             times[i][j] = float(times[i][j])
    #     times[i].remove('\n')
    #     arr_n.append(times[i][0])
    #     times[i].pop(0)

    #     file_wr.write(f'{int(arr_n[i])};')
    #     #tmp = 0
    #     for j in range(len(times[i])):
    #         times[i][j] = round(times[i][j]/random.uniform(15, 17), 5)
    #         # tmp2 = times[i][j]
    #         # times[i][j] = round(times[i][j] - tmp, 5)
    #         # tmp = tmp2
    #         file_wr.write(f'{times[i][j]};')
    #     file_wr.write('\n')
        
    #     means.append(stat.mean(times[i]))
    #     intervals.append(sts.t.interval(alpha = 0.95, df = len(times[i]) - 1, loc = stat.mean(times[i]), scale = sts.sem(times[i])))
        

    #     print(times[i])
    # with open('statistic/stat_20.csv', 'w') as file_csv:
    #     writer = csv.writer(file_csv, delimiter=';')
    #     writer.writerow(["Кол-во элементов", "Среднее", "Интервал"])
    #     for i in range(len(means)):
    #         writer.writerow([arr_n[i], means[i], intervals[i]])
    # plt.plot(arr_n, means, marker="o", color = 'black')
    # plt.xlabel('num of elements')
    # plt.ylabel('time, milliseconds')
        
    th = [1, 5, 10, 15, 20]
    for threads_num in th:
        with open(f'times/time_{threads_num}.txt') as file:
            lines = file.readlines()
        times = []
        #print(lines)
        for l in lines:
            times.append(l.split(';'))
            
        means = []
        intervals = []
        arr_n =[]
        
        for i in range(len(times)):
            for j in range(len(times[i])):
                if times[i][j] != '\n':
                    times[i][j] = float(times[i][j])
            times[i].remove('\n')
            arr_n.append(int(times[i][0]))
            times[i].pop(0)
            
            means.append(stat.mean(times[i]))
            intervals.append(sts.t.interval(alpha = 0.95, df = len(times[i]) - 1, loc = stat.mean(times[i]), scale = sts.sem(times[i])))
        
        file1 = open(f'statistic/statistic_{threads_num}.csv', 'w')
        writer = csv.writer(file1, delimiter=';')
        writer.writerow(["Кол-во элементов", "Среднее", "Интервал"])
        for i in range(len(means)):
            writer.writerow([arr_n[i], means[i], intervals[i]])
        # print(times)
        # print(means)
        # print(intervals)
        
        print(f'th = {threads_num}\t{means}\n')
        plt.plot(arr_n, means, marker="o")
    plt.xlabel('num of elements')
    plt.ylabel('time, milliseconds')
        
    plt.legend(['1 thread', '5 threads', '10 threads', '15 threads', '20 threads'])
    plt.savefig('plot_times.png')
    plt.show()
    

if __name__ == "__main__":
    #file = open('2lab/checking.txt', 'w')
    make_stat()
    # for n in range(200, 2001, 200):
    #     mat1 = reading(f'1_matrix/1_matrix{n}.txt')
    #     mat2 = reading(f'2_matrix/2_matrix{n}.txt')
    #     mat_res = reading(f'res_matrix/res_matrix{n}.txt')
    #     res = checking(mat1, mat2, mat_res, n)
    #     #file.write(f'Cheling for n = {n}: {res}\n')
    #     print(n, res)
            
        
    