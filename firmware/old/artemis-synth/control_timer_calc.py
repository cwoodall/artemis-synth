f_cpu = 16e6
target_freq = 120.0 # we want 120 Hz frequency (twice 60 Hz)
timer_res = 1.0/f_cpu # the smallest interval we can get

prescaler = 1024
#find the number of counts we need in our 8-bit timer, so we can figure out prescale
timer_counts = (1.0/target_freq)/(timer_res*prescaler) - 1
print int(timer_counts)
