from cw_fixedpoint import *


a = (UFixedPoint(880, 24, 7)*UFixedPoint(256,24,7))/UFixedPoint(22e3,24,7)
print a.value >> 4
print a.hex()
print a

b = (880.00 * 256.)/22e3
print hex(int(b * 2**7))
