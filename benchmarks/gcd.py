import time as Time
import sys

def gcd(a, b):
    while a != b:
        if a < b:
            b = b - a
        else:
            a = a - b
    return a


iterations = 100
mean = 0

for i in range(0, iterations):
    time = Time.time()
    gcd(144, 1836311903)
    mean = mean + (Time.time() - time) * 1000
    sys.stdout.write(f"\r--- {round(i / iterations * 100 + 1)}% ---")

mean = mean / iterations
sys.stdout.write(f"\nMean time: {mean} ms\n")