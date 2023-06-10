#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 姬轶-ZY2006109

print("How many cities in this question ?")
n = int(input())

print("Please give the distance of each cities ?")
dist = [[0 for i in range(n)] for j in range(n)]
for i in range(n):
    line = input().split(' ')
    for j in range(n):
        dist[j][i] = int(line[j])
'''
for i in range(n):
    for j in range(n):
        print(dist[i][j], end='\t')
    print()
'''

# 构造保存路径与路径长度的数组
sizeLen = 2**(n + 1)
path = [[1 for i in range(n)] for j in range(sizeLen)]
dp = [[-1 for i in range(n)] for j in range(sizeLen)]
# path = np.ones((2**(n + 1), n))
# dp = np.ones((2**(n + 1), n)) * -1


# C(V, vi) = min{C(V-{vi}, vj) + d(vi, vj)}
def TSP(v, reach):
    if dp[v][reach] != -1:
        return dp[v][reach]
    if v == (1 << n):
        return dist[0][reach]
    sumpath = 999999999
    for i in range(n):
        # 取出整数v在二进制表示下的第i位
        if v & (1 << i):
            m = TSP(v & (~(1 << i)), i) + dist[i][reach]
            # 更新最短路径长度，保存路径
            if m < sumpath:
                sumpath = m
                path[v][reach] = i
    dp[v][reach] = sumpath
    return dp[v][reach]


if __name__ == "__main__":
    try:
        # 若只有一个城市
        if n == 1:
            print('最短距离为：0')
            print('最短路径为：1->', end='')
        else:
            # 起点为v1
            reach_point = 0
            # 初始状态所有城市都经过
            s = 2**(n + 1) - 2
            distance = TSP(s, reach_point)
            reach = 0
            print('最短距离为：' + str(distance))
            print('最短路径为：1->', end='')
            for num in range(n - 1):
                reach = int(path[s][reach])
                print(str(reach + 1) + '->', end='')
                # 在路径中去掉该已达城市
                s = s & (~(1 << reach))
    except Exception:
        print('Data Error !\r\n')
    finally:
        n = input()
