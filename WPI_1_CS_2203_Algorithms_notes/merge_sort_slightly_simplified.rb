
def merge(arr, first, center, last)
	arr_left = arr[first..center]
	arr_right = arr[center+1..last]
	
	arr_left << Float::INFINITY
	arr_right<< Float::INFINITY
	
	result = []
	
	for i in 0..last-first
		if((not arr_left.empty?) and arr_left[0] <= arr_right[0])
			result << arr_left.shift
		else
			result << arr_right.shift
		end
	end
	
	for i in first..last
		arr[i] = result[i-first]
	end
end


def merge_sort(arr, start=0, finish=arr.length-1)
	unless(start +1 > finish)
		center = ((finish+start)/2).floor
		arr = merge_sort(arr,start, center)
		arr = merge_sort(arr,center+1, finish)
		merge arr, start, center, finish
	else
		return arr
	end
	return arr
end

p merge_sort([5,2,9,2,6,3,4,6,0,1])
