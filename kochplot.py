import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
import math

x, y = 0.0, 0.0
angulo = math.radians(30)

segments = []
total = 2000000

for i in range(total):
    if i.bit_count() % 2 == 0:
        angulo += math.radians(120)
    else:
        x2 = x + 10 * math.cos(angulo)
        y2 = y + 10 * math.sin(angulo)
        segments.append([(x, y), (x2, y2)])
        x, y = x2, y2

fig, ax = plt.subplots(figsize=(8, 8), facecolor="black")
ax.set_facecolor("black")
ax.add_collection(LineCollection(segments, colors="white", linewidths=1))
ax.autoscale()
ax.set_aspect("equal")
ax.axis("off")
plt.show()