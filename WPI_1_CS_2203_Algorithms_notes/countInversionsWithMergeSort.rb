
#sorts an array in ascending order and records the number of inversions
# => 	(number of pairs out of place)
def countInversions(arr, first, last)

	if first >= last
		return 0 #The list is sorted and has no inversions
	else
		center = ((first + last) / 2).floor #Where the array is split for recursion

		inversions = countInversions(arr, first, center) #Inversions on left side
		inversions += countInversions(arr, center+1, last) #Inversions on right side

		inversions += mergeCount(arr, first, center, last) #Inversions between left and right sides
		return inversions
	end
end

# Merges two sorted subsequences of an array (first to center, center+1 to last)
# And counts the number of inversions
def mergeCount(arr, first, center, last)
	inversions = 0

	arr_left = arr[first..center] # Copy of left half
	arr_right = arr[center+1..last] # Copy of right half

	result = [] #Sorted array

	#Add sentinels to the ends of the arrays
	arr_left << Float::INFINITY
	arr_right << Float::INFINITY

	#Merge the two arrays
	for i in 0..last-first
		if( (not arr_left.empty?) and arr_left[0] <= arr_right[0])
			result << arr_left.shift
		else #An element was out of order and needs to be swapped
			result << arr_right.shift
			inversions += 1
		end
	end
	
	#Replace arr[first..last] with result
	for i in first..last
		arr[i] = result[i-first]
	end
	
	return inversions
end

array = [2,3,8,6,1]

puts countInversions(array, 0, array.length - 1)

puts "\n"
puts array