# Some basic unit tests, by no means exhaustive.


import unittest

from gather_seeds import *
from helpers import *
import numpy
import collections

class RandomTest(unittest.TestCase):
	def test_random(self):
		return
	# TODO implement

class TestHelpers(unittest.TestCase):
	def test_distance(self):
		self.assertEqual(distance([3,4,5],[0,0,5]), 5)

	def test_filter_not_in(self):
		new = [ numpy.array([i,i,i]) for i in range(10)]
		old = [ numpy.array([i,i,i]) for i in range(2,12)]
		self.assertTrue(numpy.array_equal(filter_not_in(new,old), [[0,0,0],[1,1,1]]))

	def test_bisector_line(self):
		s1 = [1,0,0]
		s2 = [0,0,0]
		s3 = [0.0,1.0,0.0]
		bl = bisector_line(s1,s2,s3)
		# Direction should be in the z direction
		self.assertEqual(True, bl[1][0] == 0 and bl[1][1] == 0 and bl[1][2] != 0)

	def test_two_ring_neighborhood(self):
		# Simply tests for duplicates... (I had a typo that generated duplicates...)
		from gather_seeds import gather_seeds
		neighbors = two_ring_neighborhood([0,0,0],1)
		neighbors = [tuple(n) for n in neighbors]
		dups = [(count, item) for item, count in collections.Counter(neighbors).items() if count > 1]
		self.assertEqual(dups, [])


if __name__ == '__main__':
	unittest.main()