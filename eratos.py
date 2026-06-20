import math 
import numpy as np
import matplotlib.pyplot  as plt

N = 1000
sqN = int(math.sqrt(N))

numlist = list(range(2,N,1))

# Sieve of eratosthenes
for i in range(2,sqN,1):
    for n in numlist:
        if (n%i == 0) and (i != n):
            numlist.remove(n)

# Printing
print(f"Prime numbers from 0 to {N}")
for i in range(0,len(numlist)):
    print(numlist[i])

# Plotting
plt.figure(figsize = (4,4), dpi = 150)
plt.plot(
        list(range(len(numlist))),
        numlist, 
        color = "r", 
        lw = 1
    )
plt.xlim(0, len(numlist))
plt.ylim(0, None)
plt.show()







