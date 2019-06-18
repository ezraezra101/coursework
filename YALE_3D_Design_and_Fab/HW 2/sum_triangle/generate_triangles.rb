# Short script that helps me generate inputs sum_triangle_triangle

def generate_triangle()
	x_range = 0..5
	y_range = 0..5
	points = []
	points += [rand(x_range), rand(y_range)]

	points += [rand(x_range), rand(y_range)]
	# ensure point2's uniqueness
	# not strictly in y_range... but that's not too important for these triangles
	points[3] += 1 if(points[3] == points[1] and points[2] == points[0])

	# Make sure that point3 isn't colinear
	# Here I'm creating an isoceles right triangle.
	vector = [points[0] - points[2], points[1] - points[3]]
	points += [points[0]+vector[1], points[1]-vector[0]]

	points
end

(0..10000).each {
	generate_triangle.each { |p| print p; print " "}
	print "  "
	generate_triangle.each { |p| print p; print " "}
	print "\n"
}
