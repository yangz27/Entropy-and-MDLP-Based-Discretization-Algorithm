#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2018/5/3 8:53
# @Author  : WangliLin
# @Site    : 
# @File    : entropy_based_discretization.py
# @Software: PyCharm

from __future__ import division
from math import log
import numpy as np

'''
计算一个数据集的entropy
'''
def entropy(S, start, end, class_types):
    if (start > end):
        return 0.0
    ent = 0.0
    N = end - start + 1
    c_count = {}
    for i in range(class_types):
        c_count[i] = 0
    for i in range(start, end+1):
        c_count[S[i][1]] += 1
    pro_i = 0.0
    for i in range(class_types):
        if c_count[i] > 0:
            pro_i = c_count[i]/N
            ent_i = -(pro_i * log(pro_i, 2))
            ent += ent_i
            # print("class{0} count: {1}/{2} pro: {3} ent: {4}".format(i, c_count[i], N, pro_i, ent_i))
    del c_count
    # print("ent of this dataset:{}".format(ent))
    return ent

'''
计算一个划分的entopy
'''
def entropy_with_T(S, start, end, T, class_types):
    ent1 = entropy(S, start, T, class_types)
    ent2 = entropy(S, T+1, end, class_types)
    N = end - start + 1;
    entST = (T - start + 1) / N * ent1 + (end - (T+1) + 1) / N * ent2;
    print("entST with {0} of this dateset: {1}".format(T, entST))
    return entST

'''
计算二分划分的最佳划分点
'''
def best_cut_point(S, start, end, class_types):
    T = -1
    min_entST = 0
    entST = 0
    for i in range(start, end):
        print("i: {}".format(i))
        while (i < end) & (S[i] == S[i+1]):
            i += 1
        entST = entropy_with_T(S, start, end, i, class_types)
        if T == -1:
            T = i
            min_entST = entST
        elif entST < min_entST:
            T = i
            min_entST = entST
    return T

'''
计算不同的类别数目
'''
def unique_class(S, start, end, class_types):
    if start == end:
        return 1
    c_count = {}
    for i in range(class_types):
        c_count[i] = 0
    for i in range(start, end+1):
        c_count[S[i][1]] += 1
    unique_class = 0
    for i in range(class_types):
        if c_count[i] != 0:
            unique_class += 1
    return unique_class

'''
Recursive Minimal Entropy Patitioning
'''
def partition(S, start, end, Ts, class_types):
    if start >= end:
        return Ts
    N = end - start + 1
    T = best_cut_point(S, start, end, class_types)
    if T == end:
        return Ts
    gainST = entropy(S, start, end, class_types) - entropy_with_T(S, start, end, T, class_types)
    # 统计每部分中类别的数目 + entropy
    k = unique_class(S, start, end, class_types)
    k1 = unique_class(S, start, T, class_types)
    k2 = unique_class(S, T+1, end, class_types)
    entS = entropy(S, start, end, class_types)
    entS1 = entropy(S, start, T, class_types)
    entS2 = entropy(S, T+1, end, class_types)
    # print("----------------------")
    # print(N)
    # print(log(pow(3, k) - 2, 2))
    # print(k * entS - k1 * entS1 - k2 * entS2)
    delta = log(pow(3, k) - 2, 2) - (k * entS - k1 * entS1 - k2 * entS2)
    print("delta: {}".format(delta))
    threshold = (log(N-1, 2) + delta) / N
    # if gainST <= 0:
    print("-----------wwwww--------")
    print("threshold: {0} gainST: {1}".format(threshold, gainST))
    if gainST <= threshold:
    # if len(Ts) >= 2:
        return Ts
    else:
        Ts.append(T)
        print("----------")
        print(Ts)
        partition(S, start, T, Ts, class_types)
        partition(S, T+1, end, Ts, class_types)

def discretization(S, start, end, class_types):
    Ts = []
    partition(S, start, end, Ts, class_types)
    print("-----www-----")
    print(Ts)
    print("划分点为： {}".format(Ts))

def df2list(df, feature, label):
    df = df.sort_values(by=feature)
    list_all = []
    for i in range(df.shape[0]):
        # loc: 具体索引值 iloc： 根据其标签所在位置
        list_all.append([df.iloc[i][feature], df.iloc[i][label]])
    return list_all

if __name__ =='__main__':
    S = [[7,1], [5,1], [3,0], [4,0], [1,1], [2,0], [3,1], [0,1]]
    # ent = entropy(S, 0, 7, 2)
    # print("ent of this dataset:{}".format(ent))
    # entST = entropy_with_T(S, 0, 7, 3, 2)
    # print(entST)
    T = best_cut_point(S, 0, 7, 2)
    print(T)
    discretization(S, 0, 7, 2)