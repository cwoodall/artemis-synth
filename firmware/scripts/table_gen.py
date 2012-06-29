print "// Sine Wave"
from math import sin, pi
table = []
for i in range(256):
    table.append(int((sin((2*pi/256)*i)/2+.5)*0XFFF))

out = "uint16_t channelA[] = { "
for elem in range(len(table)):
    out += "%d, " % table[elem]
out += "%d };" % table[-1] 
print out;
print "//Square Wave"
table = []
for i in range(256/2):
    table.append(0xFFF)
    
for i in range(256/2, 256):
    table.append(0x000);

out = "uint16_t channelB[] = { "
for elem in range(len(table)):
    out += "%d, " % table[elem]
out += "%d };" % table[-1] 

print out

print "//Sawtooth Wave"
table = []
for i in range(256):
    table.append(int((float(i)/256)*0xFFF))

out = "uint16_t channelC[] = { "
for elem in range(len(table)):
    out += "%d, " % table[elem]
out += "%d };" % table[-1] 
print out

print "// Triangle Wave"
table = []
for i in range(256/2):
    table.append(int(2*float(i)/256*0xFFF))

for i in range(256/2):
    table.append(int((1-(2*float(i)/256))*0xFFF))

out = "uint16_t channelD[] = { "
for elem in range(len(table)):
    out += "%d, " % table[elem]
out += "%d };" % table[-1] 
print out

