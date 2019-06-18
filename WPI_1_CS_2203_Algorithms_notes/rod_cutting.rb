def max(i,j)
	if i > j
		return i
	else
		return j
	end
end

def cut_rod(p, n)
	if n == 0
		return 0
	end
	q = -Float::INFINITY
	for i in 1..n
		q = max(q, p[i] + cut_rod(p,n-i))
	end
	return q
end

$memo = {0=>0}
def memoized_cut_rod(p,n)
	if $memo.has_key?(n)
		return $memo[n]
	end
	q = -Float::INFINITY
	for i in 1..n
		q = max(q, p[i] + memoized_cut_rod(p,n-i))
	end
	return q
end

#Bottom-up version
$memo2 = {0=>0}
def dynamic_programming_cut_rod(p,n)
	for j in 1..n
		q = -Float::INFINITY
		for i in 1..j
			q = max(q,p[i] + $memo2[j-i])
		end
		$memo2[j] = q
	end
	return $memo2[n]
end

prices = [0,1,2,3,8,2,6,7,8,9,10,11]
starting_length = 7

puts cut_rod( prices, starting_length)
puts memoized_cut_rod( prices, starting_length)
puts dynamic_programming_cut_rod( prices, starting_length)
