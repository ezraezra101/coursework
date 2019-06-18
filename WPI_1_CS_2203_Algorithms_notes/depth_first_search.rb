#Note: this hasn't been tested

#Performs a depth first search on a graph
# => Runs in Theta(V + E) time.
def depth_first_search(graph)
	#Initialize all nodes
	for u in graph.verticies
		u.color = 'white'
		u.parent = nil
	end
	
	$time = 0 #global timestamp
	
	for u in graph.verticies
		if u.color == 'white'
			#visiting every unvisited node
			depth_first_search_visit(graph, u)
		end
	end
end
	

def depth_first_search_visit(graph, u)
	$time += 1
	u.d = $time # 'white' u has been discovered
	u.color = 'grey'
	
	for v in g.adjacentp[u] #explore edge (u, v)
		if v.color == 'white'
			v.parent = u #newly discovered
			depth_first_search_visit(graph, v)
		end
	end
	u.color = 'black'
	$time += 1
	u.f = $time
end