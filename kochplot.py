import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
import math

def compute_segments(iterations):
    segments = []
    x,y = 0.0, 0.0
    angulo = math.radians(120)
    for i in range(iterations):
        if i.bit_count() % 2 == 0:
            angulo += math.radians(60)
        else:
            x2 = x + 10 * math.cos(angulo)
            y2 = y + 10 * math.sin(angulo)
            segments.append([(x, y), (x2, y2)])
            x, y = x2, y2
    return segments

def plot(segments):
    fig, ax = plt.subplots(figsize=(8, 8), facecolor="black")
    ax.set_facecolor("black")
    ax.add_collection(LineCollection(segments, colors="white", linewidths=1))
    ax.autoscale()
    ax.set_aspect("equal")
    ax.axis("off")
    plt.show()

def koch_plot(iterations):
    segments = compute_segments(iterations)
    plot(segments)

koch_plot(20000)