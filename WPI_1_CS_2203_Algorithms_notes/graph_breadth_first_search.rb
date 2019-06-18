#Sets the distances of a graph relative to a starting vertex s
# => O( V + E )
def breadth_first_search(graph, s)
	for u in graph.vertices
		if u != s
			u.color = 'white'
			u.dist = Float::INFINITY
			u.parent = nil
		end		
	end
	s.color = 'grey'
	s.dist = 0
	s.parent = nil
	queue = [s]

	while queue != []
		u = queue.shift #pop off first element
		for v_index in graph.adjacent(u)
			
			v = graph.vertices[v_index]
			if v.color == 'white'
				v.color = 'grey'
				v.dist = u.dist + 1
				v.parent = u
				queue << v
			end
		end
		u.color = 'black'
		
	end
end

#An directed graph with unweighted edges
class Graph
	attr_accessor :adjacent, :vertices
	@adjacent
	@vertices
	def initialize(adjacency_lists, vertices)
		@adjacent = adjacency_lists
		@vertices = vertices
	end
	
	#Returns an array of the indicies of the verts adjacent to v
	def adjacent(v)
		if(v.id < @adjacent.length)
			return @adjacent[v.id]
		else
			return []
		end
	end
end

#Vertex in graph
class Vertex
	attr_accessor :color, :dist, :parent, :id
	def initialize(id)
		@id, @color = id, 'white'
		@dist, @parent = Float::INFINITY, nil
	end
end


graph_size = 6
vertices = []
for i in 0..graph_size
	vertices << Vertex.new(i)
end

g = Graph.new([[1,5],[2,3,4],[0,6,1],[4,5],[1,4,2],[6,5,4],[0]], vertices)

breadth_first_search(g, g.vertices[0])

for i in 0..graph_size
	puts g.vertices[i].dist
end