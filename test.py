import numpy as np
import matplotlib.pyplot as plt
import scipy.special as sp
from scipy.integrate import quad

soundSpeed = 343
radius = 1
maxFrequency = 200
frequency = 1000

frequencies = np.linspace(20, maxFrequency, 100)
angles = np.linspace(-np.pi/2 , np.pi/2 , 180)

def omega(frequency):
   return frequency * 2 * np.pi

def k(omega):
   return omega / soundSpeed

def radiationRatio(k, sphereRadius):
   return (k * sphereRadius) ** 2 / (1 + (k * sphereRadius) ** 2)

radiationRatioList = list()
for frequency in frequencies:
   om = omega(frequency)
   k_ = k(om)
   radiationRatioList.append(radiationRatio(k_, radius))

def gamma(angle):
   return I(angle) * np.sin(angle)

def I(angle):
   om = omega(frequency)
   k_ = k(om)
   alpha = 2 * sp.jv(3, k_ * radius * np.sin(angle))
   beta = k_ * radius * np.sin(angle)
   return (alpha / beta) ** 2

def D(angle):
   i = I(angle)
   d, error = quad(gamma, 0, np.pi)
   print("D: %f, error: %f" % (d, error))
   return i / d

plt.figure()
for index in range(0, len(frequencies)):
   frequency = frequencies[index]
   directivity = list()
   for angle in angles:
      frequency = frequencies[index]
      directivity.append(D(angle))
   directivity = directivity / max(directivity)
   plt.polar(angles, directivity)
path = "./temp/frequencies3order.png"
plt.savefig(path, format='png')


# print(frequencies)
# print(radiationRatioList)
# plt.figure()
# plt.semilogx(frequencies, radiationRatioList)
# plt.grid("True")
# plt.show()


