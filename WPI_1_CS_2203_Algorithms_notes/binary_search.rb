#Returns index or nil
def binary_search(arr, v)
	range_min = 0
	range_max = arr.length - 1
	index = arr.length/2
	while (range_min + 1 < range_max)
		puts "Range_min:" + range_min.to_s
		puts "Range_max:" + range_max.to_s
		if arr[index] < v
			range_min = index
		else
			range_max = index
		end
		
		index = (range_min + range_max)/2
		
		if arr[range_min] == v
			return range_min
		elsif arr[range_max] == v
			return range_max
		end
	end
end


puts binary_search([0,1,2,3,4,5,6,7,8,9,10] , 0)