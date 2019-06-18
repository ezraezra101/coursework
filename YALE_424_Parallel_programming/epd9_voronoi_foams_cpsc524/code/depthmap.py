import numpy as np
from stl import mesh
from skimage import measure
import matplotlib.pyplot as plt


def load_file(filename, x,y,z):
    voxels = np.ones([z,x,y], dtype=bool)
    j = 0
    k = 0
    with open(filename) as f:
        for k in range(z):
            f.readline()
            for j in range(y):
                line = f.readline()
                for i in range(x):
                    voxels[k,i,j] = line[i] == "#"
    return voxels



def draw_voxels(voxels):
    '''Very slow voxel drawing function'''
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    ax.voxels(voxels, edgecolor='k')
    plt.show()

def draw_depthmap(voxels, filename=None):
    depth = np.zeros_like(voxels[0,:,:], dtype=float)
    for i in range(voxels.shape[0]):
         depth[voxels[i,:,:]] = i
    dpi = voxels.shape[1]/5
    fig = plt.figure(figsize=(5,voxels.shape[2]/dpi), dpi=dpi, frameon=False)
    print(dpi)
    plt.axis('off')
    plt.tick_params(axis='both', left='off', top='off', right='off', bottom='off', labelleft='off', labeltop='off', labelright='off', labelbottom='off')
    plt.imshow(depth, cmap='gray', interpolation='none')
    if(filename != None):
        plt.savefig(filename, bbox_inches='tight', pad_inches=0.0, dpi=dpi)

def save_stl(voxels, filename='foam.stl'):
    verts, faces = measure.marching_cubes(voxels, 0.5)

    # Shamelessly copied from docs:
    # https://pypi.python.org/pypi/numpy-stl
    foam = mesh.Mesh(np.zeros(faces.shape[0], dtype=mesh.Mesh.dtype))
    for i, f in enumerate(faces):
        for j in range(3):
            foam.vectors[i][j] = verts[f[j],:]

    foam.save(filename)


if __name__ == '__main__':
	voxels = load_file("result.txt", 100, 100, 10)
	draw_depthmap(voxels[:,:,:], 'depthmap.png')
	# save_stl(voxels, 'foam.stl')

