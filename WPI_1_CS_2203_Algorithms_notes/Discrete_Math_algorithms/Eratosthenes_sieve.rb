#Way of generating primes:
def eratosthenes_sieve(i)
  x = (2..i).collect { |n| n } #x is an array = [2, 3, ... i-1, i]
  
  current = 0
  sqrt_i = Math.sqrt(i)
  
  while current < x.length and x[current] < sqrt_i
    x.reject! {|num| num % x[current] == 0 && num != x[current]}
    current += 1
  end
  x
end

puts eratosthenes_sieve(10000)