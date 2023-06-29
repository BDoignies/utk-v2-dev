import sys

import matplotlib.pyplot as plt
import numpy as np

def plot_point(file):
    data = np.loadtxt(file)
    
    if data.ndim != 2:
        return
    
    plt.figure()
    
    plt.xlim((0, 1))
    plt.ylim((0, 1))

    plt.gca().axis('off')
    plt.gca().set_aspect('equal')

    plt.scatter(data[:, 0], data[:, 1], s=0.4, color='black')

    plt.savefig(file.replace('.dat', '.png'), bbox_inches='tight')
    plt.close()

def plot_spect(file):
    data = np.loadtxt(file)
    
    N = int(np.round(np.sqrt(data.shape[0])))
    data = data.reshape((N, N))
    plt.figure()

    plt.gca().axis('off')
    plt.gca().set_aspect('equal')
    
    data = np.power(data, 1 / 2.2) # Gamma correction
    plt.imshow(data, cmap='gray')
    plt.savefig(file.replace('.dat', '.png'), bbox_inches='tight')
    
    plt.close()

if __name__ == "__main__":
    pts_file = sys.argv[1]
    spe_file = sys.argv[2]

    print(pts_file, spe_file)
    plot_point(pts_file)
    plot_spect(spe_file)