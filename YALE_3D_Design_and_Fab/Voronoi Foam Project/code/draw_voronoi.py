
# This file contains all code related to the faster alternative to eval_structure
# as well as all exporting from those voronoi diagrams.


import matplotlib
matplotlib.use('agg', warn=False) # Generally prefer agg backend...
from scipy.spatial import Voronoi, voronoi_plot_2d
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.collections
from PIL import Image
from mpl_toolkits.mplot3d import Axes3D

import glob
import re
from stl import mesh
from skimage import measure


def within_tau(a,b,z, tau):
    '''Is z between a and b? (inclusive)'''
    return max(a,b)+tau >= z and min(a,b)-tau <= z

def intersect_z(p1, p2,z):
    '''Where the segment between p1 and p2 intersects z (or comes closest to it)'''
    frac = np.clip((z-p2[2])/(p1[2]-p2[2]), 0,1)
    return (
        (1-frac)*p2[0]+frac*p1[0],
        (1-frac)*p2[1]+frac*p1[1]
    )
    

def get_cross_section(vor, z, tau):
    '''Given a voronoi diagram (`vor`), a z value (`z`) and a beam radius (`tau`)
    generates a cross section made of [ellipses, 2-D lines] that approximates a cross section of the voronoi diagram.

    Note: Won't work perfectly when trying to draw a line segment that ends within z+-tau
    '''
    ellipses = []
    lines2d = []
    for simplex in vor.ridge_vertices:
        #vert3 = vor.vertices[simplex] # Each pair in vert3 represents an edge to a cell (with duplicates)
        for p1_i, p2_i in zip(simplex, np.roll(simplex,1)):
            p1, p2 = vor.vertices[[p1_i,p2_i]]
            if p1_i == -1 or p2_i == -1: # Represents points at infinity
                pass
            elif(p1[2] == z and p2[2] == z):
                lines2d.append([p1,p2])
            elif(within_tau(p1[2], p2[2], z, tau)):
                long_axis = np.array(intersect_z(p1,p2,z+tau)) - intersect_z(p1,p2,z-tau)
                center = intersect_z(p1,p2,z)
                long_len = np.linalg.norm(long_axis)
                ellipses.append([
                    center,
                     long_len,# long axis
                     tau*2,# short axis
                     180/3.14159265358979*np.arcsin(long_axis[1]/long_len)# angle (degrees CCW from X-axis)
                    ])
    return [ellipses, lines2d]

def save_cross_section(ellipses, lines, bounds=[-1,-1,-1,1,1,1], voxel_size=1, filename='voronoi.png'):
    dpi = 100
    if(len(lines) != 0):
        print("Ignoring unprintable segments")
    fig = plt.figure(
        frameon=False,
        dpi=dpi)
    fig.set_size_inches((bounds[3]-bounds[0])/voxel_size / dpi, (bounds[4]-bounds[1])/voxel_size / dpi)
    ax = plt.Axes(fig, [0,0,1,1], frame_on=False)
    fig.delaxes(plt.gca())
    ax.set_axis_off()
    fig.add_axes(ax)

    xy, long, short, angle = zip(*ellipses)

    #https://stackoverflow.com/questions/32444037/how-can-i-plot-many-thousands-of-circles-quickly
    coll = matplotlib.collections.EllipseCollection(long, short,angle,
                                                    offsets=np.array(xy), units='xy',
                                                    transOffset=ax.transData,
                                                    linewidths=0,
                                                    facecolors='black',
                                                    antialiased=False)
    ax.add_collection(coll)
    ax.set_xlim(bounds[0], bounds[3])
    ax.set_ylim(bounds[1], bounds[4])
    plt.show()
    plt.savefig(filename, dpi=dpi)

def voxelize(seeds, bounds, tau, voxel_size, filename='voxels_z='):
    vor = Voronoi(seeds)
    for z in np.arange(bounds[2],bounds[2+3], voxel_size):
        print(z)
        [ellipses, lines] = get_cross_section(vor, z, tau)
        save_cross_section(ellipses,lines, bounds, voxel_size=voxel_size, filename=filename + str(z) + '.png')
        plt.close()

def get_slice_filenames(prefix='voxel_renders_3/voxels_z='):
    sorted_images = glob.glob(prefix+'*')
    sorted_images.sort(key=lambda a: float(re.search('=(.+?).png',a).group(1)))
    return sorted_images

def load_voxels(slice_filenames):
    voxels = []
    for image_path in slice_filenames:
        image = np.array(Image.open(image_path).convert('L'))==0
        voxels.append(image)
    voxels = np.array(voxels)
    return voxels


def draw_voxels(voxels):
    '''Very slow voxel drawing function'''
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    ax.voxels(voxels, edgecolor='k')
    plt.show()

def draw_depthmap(voxels):
    depth = np.zeros_like(voxels[0,:,:], dtype=float)
    for i in range(voxels.shape[0]):
         depth[voxels[i,:,:]] = i
    plt.imshow(depth, cmap='gray', interpolation='none')

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
    # Perform some testing
    from gather_seeds import gather_seeds_for_bounds
    def density_func(point):
        extent = np.array([8.0, 2.0, 2.0], dtype=float)  # (mm)
        # grading in x direction
        seed_density_factor = 1
        return (point[0] / extent[0]) * seed_density_factor  # seeds / mm^2

    bounds = np.array([0,0,0, 5, 3, 3])
    tau = 0.1
    coarse_level_length = 4.0
    voxel_size=0.05

    seeds = gather_seeds_for_bounds(bounds, coarse_level_length, density_func)
    voxelize(seeds,bounds,tau,voxel_size)
    z = bounds[2]
    filename="voxels_z=" + str(z) + ".png"

    vor = Voronoi(seeds)
    [ellipses, lines] = get_cross_section(vor, seeds[0][2]+0.3, 0.1)
    save_cross_section(ellipses,lines, bounds, voxel_size=voxel_size, filename=filename)

