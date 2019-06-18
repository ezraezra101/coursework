=begin
quick_sort:
	Invented by C.A.R. "Tony" Hoare in 1960
	Invented supposedly on a bet.

Advantages:
	Worst case running time Theta(n^2), but expected time is Theta(n * log(n))
	Sorts in place
	Works well with virtual memory
=end


#Sorts arr, between min and max (inclusive)
def quick_sort(arr, min, max)
	if min >= max #One element to sort
		#end
	else
		pivot = partition(arr, min, max)

		quick_sort(arr, min, pivot-1)
		quick_sort(arr, pivot+1, max)
	end
end

def partition(arr, min, max)
	pivot = arr[max] #last element is pivot

	before_pivot = min - 1

	#Note: arr[min..before_pivot] is always less than pivot
	# => and arr[before_pivot+1..j-1] is always more than the pivot

	for j in min..max-1 #Step through array (minus pivot)

		if arr[j] <= pivot #Less than pivot?
			before_pivot += 1

			arr[before_pivot], arr[j] = arr[j], arr[before_pivot];
      
			#swap arr[before_pivot] with arr[j]
		end
	end

	arr[before_pivot+1], arr[max] = arr[max], arr[before_pivot+1]
	#swap arr[before_pivot+1] with arr[max] # move pivot to middle

	return before_pivot + 1 #Pivot position
end