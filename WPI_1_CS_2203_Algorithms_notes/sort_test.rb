require_relative "quick_sort"

def print_array(arr)
	print "["
	if arr.length >=2
		for x in 0..arr.length-2
			print arr[x].to_s + ", "
		end
	end
  puts arr[x+1].to_s + "]"
end


def sort_test(fn)

	testcases = [
		[1, 7, 9, 10, 11, 9, 6],
		[1, 7, 9, 10, 11, 9, 6],
		[5,76,2,8,9,3,4,6,44,1],
		[0,1,2,3,4,5,6,7,8,9,10],
		[1,2,3,4,4,5,5,78],
		[5,4,1,78,3,5,2,4]
	]
	puts "Testing " + fn.to_s
	testcases.each do |arr|
		experimental = arr.clone
		control = arr.clone
		
		quick_sort(control,0,control.length-1)
		send(fn, experimental)
		
		if control != experimental
			print "Not a match:\nOriginal:\t"
			print_array arr
			print "Master:\t\t"
			print_array control
			print "Test:\t\t"
			print_array experimental
		end
	end
	puts ""
end

if __FILE__ == $0
	require_relative "selection_sort"
	require_relative "heap_sort"
	require_relative "bubble_sort"
	
	sort_test(:selection_sort)
	sort_test(:heap_sort)
	sort_test(:bubble_sort)
end