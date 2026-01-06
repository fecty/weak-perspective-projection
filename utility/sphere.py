import numpy as np
from math import *

N = 20
s = 1
digits = 4

thetaArr = np.linspace(0, 2 * pi, N)
phiArr = np.linspace(-pi / 2, pi / 2, N)

xArr = []
yArr = []
zArr = []

pointsArr = []

for phi in phiArr:
    for theta in thetaArr:
        x = round(s * cos(phi) * cos(theta), digits)
        y = round(s * cos(phi) * sin(theta), digits)
        z = round(s * sin(phi), digits)

        print(f"{{{x}f,{y}f,{z}f}},")

        xArr.append(x)
        yArr.append(y)
        zArr.append(z)
        pointsArr.append((x, y, z))
