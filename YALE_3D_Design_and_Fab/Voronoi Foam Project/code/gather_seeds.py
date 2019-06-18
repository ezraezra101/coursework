import numpy
from subdivide_cell import subdivide_cell
from helpers import *

def gather_seeds(q, l, rho):
	''' Algorithm 2
	Gathers a superset of the seeds (in density field rho)
	that could influence query point q
	'''
	neighbors = two_ring_neighborhood(q,l)
	seeds = []
	for c2 in neighbors:
		seeds = seeds + subdivide_cell(c2,l,rho)

	closest = closest_seed(q,seeds)
	new_neighbors = two_ring_neighborhood(closest, l)
	for c2 in filter_not_in(new_neighbors, neighbors):
		seeds = seeds + subdivide_cell(c2, l, rho)
	return seeds

def gather_seeds_for_bounds(bounds,l,rho):
	'''
	Gathers a superset of the seeds that could influence any point in the bounds.
	Bounds is a list of [xmin, ymin,zmin,xmax,ymax,zmax]
	'''
	def rangeNeighbors(low, high):
		return numpy.arange(low - (low % l)-2*l, high - (high % l)+3*l, l) # +3 because of `%` rounding down
	seeds = []
	for x in rangeNeighbors(bounds[0], bounds[3]):
		for y in rangeNeighbors(bounds[1], bounds[4]):
			for z in rangeNeighbors(bounds[2], bounds[5]):
				seeds = seeds + subdivide_cell((x,y,z), l, rho)
	return seeds