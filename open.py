import numpy as np 
import matplotlib.pyplot as plt 

x = np.linspace(-10, 10, 4000)
y = x**2 

print("Plotting figure...")
plt.figure(figsize = (4,4), dpi = 300)
plt.plot(x,y,color = 'r', lw = 1)
plt.xlabel(r"$x$")
plt.ylabel(r"$y$")
plt.title("Parabola wallahi")
plt.xlim(x.min(),x.max())
plt.ylim(-10,50)
plt.grid()
plt.savefig("wallahi.png")
plt.close()
print("Done!")
