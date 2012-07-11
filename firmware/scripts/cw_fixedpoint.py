#!/usr/bin/python
import math
class UFixedPoint(object):
    def __init__(self, num, m, n):
        self.mask = 2**(m + n) - 1
        self.k = (1 << (n - 1))
        self.m = m
        self.n = n
        
        temp = int(num * 2**self.n)
        self.value = temp & self.mask

    def __add__(self, other):
        # Currently only allow same Qm.n Floats
        if (self.m == other.m) and (self.n == other.n):
            temp = UFixedPoint(0, self.m, self.n)
            temp.value = (self.value + other.value) & self.mask
            return temp
        else:
            return 0

    def __sub__(self, other):
        if (self.m == other.m) and (self.n == other.n):
            temp = UFixedPoint(0, self.m, self.n)
            temp.value = (self.value - other.value) & self.mask
            return temp
        else:
            return 0

    def __mul__(self, other):
        if (self.m == other.m) and (self.n == other.n):
            temp = UFixedPoint(0, self.m, self.n)
            temp.value = (self.value * other.value) + self.k
            temp.value >>= self.n
            return temp
        else:
            return 0

    def __div__(self,other):
        if (self.m == other.m) and (self.n == other.n):
            temp = UFixedPoint(0, self.m, self.n)
            temp.value = self.value << self.n
            temp.value += (other.value >> 1)
            temp.value = temp.value/other.value
            return temp
        else:
            return 0

    def __float__(self):
        return (float(self.value) / (2**self.n))

    def __str__(self):
        return "%f" % float(self)

    def bin(self):
        [trash, binary] = bin(self.value).split("b")
        len_dif = len(binary) - (self.n + self.m)
        if len_dif < 0:
            zeros = ["0" for i in range(abs(len_dif))]
            binary = ('').join(zeros) + binary
        return "0b" + binary

    def hex(self):
        return hex(self.value)



