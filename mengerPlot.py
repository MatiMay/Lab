import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import ListedColormap
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def crear_lista(f, c):
    lista = np.ones((f, c))
    lista[1, 1] = 0
    queque=[(0, 0), (0, 1), (0, 2), (1, 0), (1, 2), (2, 0), (2, 1), (2, 2)]
    return lista, queque

def expand(lista, queque):
    new_lista = np.zeros((lista.shape[0] * 3, lista.shape[1] * 3))
    for i in range(lista.shape[0]):
        for j in range(lista.shape[1]):
            if lista[i, j] == 1:
                new_lista[i*3:(i+1)*3, j*3:(j+1)*3] = np.ones((3, 3)) 
                if (i, j) not in queque:
                    queque.append((i, j))
    return new_lista

def remove(lista, queque):
    remove_lista = np.copy(lista)
    for i, j in queque:
        remove_lista[i*3+1, j*3+1] = 0
        #pop_index = queque.index((i, j))
        #queque.pop(pop_index)
    #queque.clear()
    return remove_lista

def plot_black_white(matrix):
    cmap = ListedColormap(["black", "white"])
    plt.figure(figsize=(8, 8))
    plt.imshow(matrix, cmap=cmap, vmin=0, vmax=1, interpolation="nearest")
    plt.gca().set_aspect("equal")
    plt.axis("off")
    plt.tight_layout()
    plt.show()
    volume = np.asarray(matrix)

    if volume.ndim != 3:
        raise ValueError("matrix must be a 3D list of lists of lists")

    if not np.isin(volume, [0, 1]).all():
        raise ValueError("matrix must contain only 0s and 1s")

    filled = np.ones(volume.shape, dtype=bool)
    facecolors = np.empty(volume.shape + (4,), dtype=float)
    facecolors[volume == 0] = (0, 0, 0, 0)
    facecolors[volume == 1] = (1, 1, 1, 1)

    fig = plt.figure(figsize=(10, 10))
    ax = fig.add_subplot(111, projection='3d')
    ax.voxels(filled, facecolors=facecolors)

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    fig.patch.set_facecolor('black')
    ax.set_facecolor('black')
    plt.show()

    volume = np.asarray(matrix)
    if volume.ndim != 3:
        raise ValueError("matrix must be a 3D list of lists of lists")
    if not np.isin(volume, [0, 1]).all():
        raise ValueError("matrix must contain only 0s and 1s")
    coords = np.argwhere(volume == 1)
    fig = plt.figure(figsize=(5, 5))
    ax = fig.add_subplot(111, projection='3d')
    if coords.size > 0:
        ax.scatter(coords[:, 0], coords[:, 1], coords[:, 2], c='white', s=4)

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_box_aspect(volume.shape)
    fig.patch.set_facecolor('black')
    ax.set_facecolor('black')
    plt.show()

def plot3d(matrix):
    fig=plt.figure()
    ax=fig.add_subplot(111, projection='3d')
    x, y, z = np.indices((10,10,10))
    voxels= matrix
    ax.voxels(voxels)
    plt.show()

def computar2d(lista, iteraciones, queque):
    for _ in range(iteraciones):
        new_lista = expand(lista, queque)
        removed_lista = remove(new_lista, queque)
        lista = removed_lista
    return lista

def computar3d(lista):
    tamaño = (lista.shape[0])
    lista3d = np.ones((tamaño, tamaño, tamaño))
    for i in range(tamaño):
        for j in range(tamaño):
            for k in range(tamaño):
                if lista[i, j] == 0:
                    lista3d[i, j, k] = 0
                    lista3d[i, k, j] = 0
                    lista3d[k, i, j] = 0
    return lista3d

def MengerSponge(iteraciones):
    lista, queque = crear_lista(3, 3)
    lista= computar2d(lista, iteraciones, queque)
    lista3d = computar3d(lista)

    plot3d(lista3d)

MengerSponge(2)