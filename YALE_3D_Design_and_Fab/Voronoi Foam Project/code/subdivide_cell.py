import numpy
import random
import math

def gen_subcells(center, length):
	'''Generates 3D subcell centers in the cell defined by center and length.'''
	offsets=numpy.array([
		[1,1,1],
		[1,1,-1],
		[1,-1,1],
		[1,-1,-1],
		[-1,1,1],
		[-1,1,-1],
		[-1,-1,1],
		[-1,-1,-1]
	]);
	return (offsets* length/4) + center

def random_sample_in_subcell(center, length):
	return center + length * numpy.array([random.random()-0.5,random.random()-0.5,random.random()-0.5])

def flip(p):
	'''weighted coin flip, returns 1 or 0'''
	return 1 if random.random() <= p else 0

def random_permutation(subcells):
	'''Shuffles the subcells'''
	random.shuffle(subcells)
	return subcells

def subdivide_cell(c, l, rho):
	'''Algorithm 3
	c = center, l = cell side length, rho=density function
	Because of `random`, is not thread safe.
	'''
	random.seed(hash(tuple(c)))
	target_seeds = l**3 * rho(c)
	subcells = random_permutation(gen_subcells(c,l))
	if(target_seeds < 0):
		return [] 

	seeds = []

	if target_seeds <= len(subcells):
		n_seeds = math.floor(target_seeds)+flip(target_seeds%1) # Rounds to an appropriate number of seeds
		seeds = seeds + [random_sample_in_subcell(c2,l/2) for c2 in subcells[:n_seeds]]
	else:
		for c2 in subcells:
			seeds = seeds + subdivide_cell(c2,l/2,rho)

	return seeds

def subdivide_cell_main():

	bounds = [0,0,0,10,10,10]
	extent = numpy.array(bounds[3:6], dtype=float) - bounds[0:3]
	coarse_level_length = 4.0
	tau = 0.5 # beam radius
	voxel_size=0.05

	def density_func(point):
		# grading in x direction
		seed_density_factor = 1
		return (point[0] / extent[0]) * seed_density_factor  # seeds / mm^2


	cell_center = [2,2,2]
	seeds = subdivide_cell(cell_center, coarse_level_length, density_func)
	plot_seeds(seeds, seed_size=10)



if __name__ == '__main__':
	from helpers import *
	subdivide_cell_main()
