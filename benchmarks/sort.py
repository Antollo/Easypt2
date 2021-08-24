import time as Time
import sys
import math
import pathlib

def merge(left, right):
    array = []
    leftIndex = 0
    rightIndex = 0
    while leftIndex < len(left) and rightIndex < len(right):
        if left[leftIndex] < right[rightIndex]:
            array.append(left[leftIndex])
            leftIndex += 1
        else:
            array.append(right[rightIndex])
            rightIndex += 1

    return array + left[leftIndex :] + right[rightIndex :]


def mergeSort(array):
    if len(array) <= 1:
        return array
    pivot = math.floor(len(array) / 2)
    return merge(mergeSort(array[0 : pivot]), mergeSort(array[pivot :]))


file = open(pathlib.Path(__file__).parent.resolve() / 'numbers.txt')
array = [int(line.strip()) for line in file.read().split('\n')]
file.close()


iterations = 100
mean = 0

for i in range(0, iterations):
    time = Time.time()
    mergeSort(array)
    mean = mean + (Time.time() - time) * 1000
    sys.stdout.write(f"\r--- {round(i / iterations * 100 + 1)}% ---")

mean = mean / iterations
sys.stdout.write(f"\nMean time: {mean} ms\n")