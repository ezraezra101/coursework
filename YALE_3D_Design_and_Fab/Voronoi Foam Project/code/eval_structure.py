from gather_seeds import *
from numba import *
import numpy as np

@jit(nopython=False)
def bisector_line(s1,s2,s3):
    '''Returns a line (pont on line, unit tangent) that is equidistance from these arguments.'''
    s1 = numpy.array(s1)
    s2 = numpy.array(s2)
    s3 = numpy.array(s3)
    axis = numpy.zeros(3)
    cross3(s1-s2, s1-s3, axis)
    # if(axis[0]==0 and axis[1] == 0 and axis[2] == 0): # Axis is zero vector
    #     raise ValueError("Seeds are colinear")
    s_avg = (s1+s2+s3)/3 # Bisector line passes through here
    return (s_avg, axis)


@jit('float32(float64[:], float64[:])', nopython=True)
def distance_numba(a,b):
    d = a-b
    return (d[0]*d[0]+d[1]*d[1]+d[2]*d[2])**0.5

@jit
def closest_point(q, line):
    '''Figures out the closest point to q on the line (point on line, tangent)'''
    return numpy.dot(line[1], q-line[0]) * line[1] + line[0] # Closest point on the line to q.

@jit
def matchGenerator(q, seeds, tau):
    for i in range(1, len(seeds)):
        for j in range(i+1, len(seeds)):
            bl = []
            #try:
            bl = bisector_line(seeds[0],seeds[i],seeds[j])
            #except ValueError as error:
            #    continue; # The seeds are colinear and don't form a bisector line.
            pl = closest_point(q, bl)
            # If the bisector of 3 seeds gets within distance tau of q
            if distance_numba(q, pl) <= tau:
                yield (i, j)
    yield False

def eval_structure(q,l,tau, rho):
    '''Algorithm 1
    Arguments: query_point, coarse cell size, beam radius, density field
    '''
    seeds = list(gather_seeds(q,l, rho))

    seeds.sort(key=lambda x: distance(x,q));
    seeds = np.array(seeds, dtype=np.float64)

    q = np.array(q, dtype=np.float64)
    tau = np.array([tau], dtype=np.float64)[0]

    m = matchGenerator(q, seeds, tau)
    while True:
        x = next(m)
        if(x == False):
            return False
        else:
            i,j = x
            bl = bisector_line((seeds[0]),(seeds[i]),(seeds[j]))
            accept = True
            # Check if point pl is actually within q's cell?
            # TODO make sure it isn't a typo in paper!
            for k in np.arange(1, len(seeds)):
                if(np.linalg.norm(seeds[0]-pl) > np.linalg.norm(seeds[k]-pl)):
                    accept = False
                    break;
            if accept:
                return 1

# Old version without Numba acceleration. Perhaps it is more clear.
"""
from gather_seeds import gather_seeds
from helpers import *
import numpy as np


def ar(p):
    return np.array(p, dtype=np.float32)

def eval_structure(q,l,tau, rho):
	'''Algorithm 1
	Arguments: query_point, coarse cell size, beam radius, density field
	'''
	seeds = list(gather_seeds(q,l, rho))

	seeds.sort(key=lambda x: distance(x,q));
	seeds = np.array(seeds, dtype=np.float32)

	for i in range(1, len(seeds)):
		for j in range(i+1, len(seeds)): # For each pair of seeds (except seeds[0])
			bl = []
			try:
				bl = bisector_line(ar(seeds[0]),ar(seeds[i]),ar(seeds[j]))
			except ValueError as error:
				continue; # The seeds are colinear and don't form a bisector line.
			
			pl = closest_point(q, bl)
			# If the bisector of 3 seeds gets within distance tau of q
			if(distance(q, pl) <= tau): 
				accept = True
				# Check if point pl is actually within q's cell?
				# TODO make sure it isn't a typo in paper!
				for k in np.arange(1, len(seeds)):
					if(np.linalg.norm(seeds[0]-pl) > np.linalg.norm(seeds[k]-pl)):
						accept = False
						break;
				if accept:
					return 1
	return 0
"""