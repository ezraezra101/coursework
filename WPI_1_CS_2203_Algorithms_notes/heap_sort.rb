#Turns an array into a heap with the max element in position arr[0]
# => O(n)
def build_max_heap(arr)
	arr.heap_size = arr.length
	
	for i in ((arr.length() -1) / 2).downto(0) #For the first half
		#If this was for the full arr.length().downto(0), it would be fine
		# => Second half is already max_heapified, because they have no children
		max_heapify(arr, i)
	end
end

#If everything but arr[i] is in proper heap order, make it into a heap
#Recurrence: T(n) ≤ T(2n/3) + theta(1)
# => With master theorem: T(n) = O(log n)
def max_heapify(arr, i) #i is the temporary root
	l = left i
	r = right i
	if l < arr.heap_size and arr[l] > arr[i]
		largest = l
	else
		largest = i
	end
	
	if r < arr.heap_size and arr[r] > arr[largest]
		largest = r
	end
	
	#If the root isn't in the right place, float it down & recurse
	if largest != i
		arr[i], arr[largest] = arr[largest], arr[i]
		max_heapify(arr, largest)
	end
end

#Does heap_sort
# => O(n log(n))
def heap_sort(arr)
	build_max_heap(arr)
	#Put the largest (first) item at the end, put that value at the front, rebuild the heap, and repeat
	#Invariant:
	# => any element in the sorted subsequence arr[i+1...end] >= any element in arr[0..i]
	for i in (arr.length-1).downto(1)
		arr[0], arr[i] = arr[i], arr[0] #Swap first element with smallest sorted element
		arr.heap_size += -1
		max_heapify(arr, 0)
	end
end

#Allowing heaps to remember their size separately from their length
class Array
	@heap_size
	attr_reader :heap_size
	attr_writer :heap_size
end

#Location of parent of this element in heap
def parent(i)
	return ((i+1)/2).floor-1 #could be simpler
end
#Location of left child in heap
def left(i)
	return 2*i + 1
end
#Location of right child in heap
def right(i)
	return 2 * i + 2
end
