import timeit
import numpy

coarse_level_length = 4.0  # (mm)
extent = numpy.array([8.0, 2.0, 2.0], dtype=float)  # (mm)
def density_func(point):
    # grading in x direction
    seed_density_factor = 1
    return (point[0] / extent[0]) * seed_density_factor  # seeds / mm^2


def to_time():
    from eval_structure import eval_structure
    eval_structure([1,1,1],coarse_level_length, 0.2, density_func)
timeit.timeit(to_time, number=10)
