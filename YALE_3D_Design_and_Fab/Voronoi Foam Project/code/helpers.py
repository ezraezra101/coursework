import numpy
import itertools

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

from numba import jit

def plot_seeds(seeds, seed_size = 0.5):
	'''Plot seeds on a matplotlib figure.
	Modified from the supplement to the paper.
	'''
	fig = plt.figure()
	seeds_x = [s[0] for s in seeds]
	seeds_y = [s[1] for s in seeds]
	seeds_z = [s[2] for s in seeds]
	
	ax = fig.add_subplot(111, projection='3d')
	ax.scatter(seeds_x, seeds_y, seeds_z, s=seed_size, color='k')
	plt.show()


def filter_not_in(new, old):
	'''Returns only points that are in new and not in old'''
	old_set = set([tuple(x) for x in old])
	return list(filter(lambda x: tuple(x) not in old_set, new))

def grid_cell_enclosing(q,grid_size):
	return numpy.floor(numpy.array(q)/grid_size + 0.5)*grid_size

def two_ring_neighborhood(q, grid_size):
	'''Given a grid center, find the centers of its two ring neighborhood'''
	nearby = [-2,-1,0,1,2]
	return grid_cell_enclosing(q, grid_size)+grid_size*numpy.array(list(itertools.product(nearby,nearby,nearby)))


def distance(q,s):
	return sum((numpy.array(q)-numpy.array(s))**2)**0.5

def closest_seed(q, seeds):
	dists = [distance(q, s) for s in seeds]
	return seeds[numpy.argmin(dists)]


# cross3 is from Gist that avoids using numpy.cross
# Part of FreeKiteSim Copyright (C) 2013 by Uwe Fechner, Delft University
#https://gist.github.com/ufechner7/98bcd6d9915ff4660a10
@jit(nopython=True)
def cross3(vec1, vec2, result):
    """ Calculate the cross product of two 3d vectors. """
    a1, a2, a3 = float(vec1[0]), float(vec1[1]), float(vec1[2])
    b1, b2, b3 = float(vec2[0]), float(vec2[1]), float(vec2[2])
    result[0] = a2 * b3 - a3 * b2
    result[1] = a3 * b1 - a1 * b3
    result[2] = a1 * b2 - a2 * b1

@jit(nopython=False)
def bisector_line(s1,s2,s3):
    '''Returns a line (pont on line, unit tangent) that is equidistance from these arguments.'''
    s1 = numpy.array(s1)
    s2 = numpy.array(s2)
    s3 = numpy.array(s3)
    axis = numpy.zeros(3)
    cross3(s1-s2, s1-s3, axis)
    if(axis[0]==0 and axis[1] == 0 and axis[2] == 0): # Axis is zero vector
        raise ValueError("Seeds are colinear")
    s_avg = (s1+s2+s3)/3 # Bisector line passes through here
    return (s_avg, axis)

def closest_point(q, line):
	'''Figures out the closest point to q on the line (point on line, tangent)'''
	return numpy.dot(line[1], q-line[0]) * line[1] + line[0] # Closest point on the line to q.
	# http://www.nabla.hr/PC-LinePlaneIn3DSpB6.htm
	# For some basic geometric algebra
	# The value of d describing a plane n*x+d = 0:
	'''
	1. $ \vec n *\vec x + d = 0, \vec x = t*\vec n + \vec s_{avg}$
	2. $ \vec{n} * (t*\vec n + \vec s_{avg}) + d = 0$
	3. $ t * \vec{n^2} + \vec s_{avg} * \vec{n} + d = 0$
	4. $ t + \vec s_{avg} * \vec{n} + d = 0 \text{ because } \vec{n} * \vec{n}$
	5. $ t = -\vec s_{avg} * \vec{n} - d$
	6. We know that $d = -\vec n * \vec q$, so:
	    - $ t = -(\vec s_{avg} * \vec{n} + (-\vec n * \vec q))$
	    - $ t = -\vec{n} * (\vec s_{avg} - \vec q)$
	    - $ t = \vec{n} * (\vec q - \vec s_{avg})$
	7. Now we want to find the point $\vec x$ associated with $t$:
	    - $ t = \vec{n} \cdot (\vec q - \vec s_{avg})$ and $\vec x = t*\vec n + \vec s_{avg}$
	    - $\vec x = (\vec{n} \cdot (\vec q - \vec s_{avg}))*\vec n + \vec s_{avg}$
	'''

