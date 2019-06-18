def bubble_sort(arr)
	for i in 0..arr.length-2
		for j in (arr.length-1).downto(i+1)
			if arr[j] < arr[j-1]
				arr[j],arr[j-1] = arr[j-1],arr[j]
			end
		end
	end
	return arr
end