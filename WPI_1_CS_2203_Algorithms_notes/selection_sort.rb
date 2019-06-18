def selection_sort(arr)
	#Any element in the sorted sequence arr[0..i-1] is less than any element in arr[i..end]
	for i in 0..arr.length-2
		#Find minimum of arr[i..arr.length-1]
		minimumLoc = i
		# The value of arr[minimumLoc] <= any element of arr[0..x]
		for x in i..arr.length-1
			if arr[minimumLoc] > arr[x]
				minimumLoc = x
			end
		end
		
		#swap arr[i] and arr[minimumLoc]
		arr[i], arr[minimumLoc] = arr[minimumLoc], arr[i]
		
	end
end