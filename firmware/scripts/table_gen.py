from math import sin, pi

class WaveTable(object):
    def __init__ (self, samples, max_value, wave_definition):
        self.samples = samples
        self.max_value = max_value
        self.wave_definition = wave_definition
        self.wave = [0 for i in range(samples)]
        
    def generate(self):
        wave = self.wave
        for (i, elem) in enumerate(wave):
            wave[i] = self.wave_definition(i, self.samples, self.max_value)
        self.wave = wave

    def printC(self, type_t, var_name):
        print "%s %s[%i] = {"  % (type_t, var_name, self.samples),
        for i in range(len(self.wave)-1):
            print "%i," % self.wave[i],
        print "%i };" % self.wave[-1]
        
print "// Sine Wave"
sine_wav = WaveTable(256, 0xFFF, lambda x,sam,max: (sin((2*pi/sam)*x)/2 + .5)*max)
sine_wav.generate()
sine_wav.printC("uint16_t", "channelA")
print

print "//Square Wave"
sq_wav = WaveTable(256, 0xFFF, lambda x, sam, max: max if (x < sam/2) else 0)
sq_wav.generate()
sq_wav.printC("uint16_t", "channelB")
print

print "//Sawtooth Wave"
saw_wav = WaveTable(256, 0xFFF, lambda x, sam, max: int((float(x)/sam)*max))
saw_wav.generate()
saw_wav.printC("uint16_t", "channelC")
print

print "// Triangle Wave"
tri_wav = WaveTable(256, 0xFFF, lambda x, sam, max: int(2*float(x)/sam*0xFFF) if (x < (sam/2)) else int((1-(2*float(x-sam/2)/sam))*0xFFF))
tri_wav.generate()
tri_wav.printC("uint16_t", "channelD")
print
