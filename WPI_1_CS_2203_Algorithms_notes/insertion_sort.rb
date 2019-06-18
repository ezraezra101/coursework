def insertion_sort(arr)
	for j in 1..arr.length-1
		key = arr[j]
		# Insert arr[j] into the sorted arr[0..j-1]
		i = j-1
		while i >= 0 and arr[i] > key
			puts "Scootching"
			arr[i+1] = arr[i]
			i += -1
		end
		arr[i+1] = key
	end
end


array = [1,3,7,3,1,67,3,6,4,2,1,0,5,9,53,32]
insertion_sort array
puts array

array = [2,3,8,6,1]

insertion_sort array