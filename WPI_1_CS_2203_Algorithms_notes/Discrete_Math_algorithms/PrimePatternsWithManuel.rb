$primes = [2,3,5,7,11,13, 17, 19]
$empty =  [0,0,0,0,0 ,0,  0,  0]


def sequence(total, pows=$empty.clone, digit=0, last = [$empty.clone, 99999999999999999999], individual_names = [])
  

  #Sets pows to the next iteration
  if pows==$empty
    pows[0] = total
  else
    increment_powers(pows, digit)
  end
  
  #Finds non increasing numbers
  if memorable(last, pows)
    
    individual_names << get_id_data(last, pows)
    individual_names.uniq!
    individual_names.sort! {|x,y| arrayNameVal(y) <=> arrayNameVal(x) }
#    print_lots_of_data(last, pows, product)
  end
  
  #puts product
  
  if should_continue(pows)
    individual_names = sequence(total, pows, digit, [pows.clone, primeProduct(pows)], individual_names)
  end
  individual_names
end

#Finds non increasing numbers
def memorable(last, pows)
  #Calculates product
  product = primeProduct(pows)
  
  last[1] <= product
end


#Creates ID data
def get_id_data(last, pows)
  id_data = []
  for i in 0..pows.length-1
    id_data += [(pows[i]-last[0][i])]
  end
  id_data
end

def print_lots_of_data(last, pows, product)
  puts "Last Product: " + last[1].to_s
  printArray(last[0])
  puts "This Product: " + product.to_s
  printArray(pows.each)  
  puts ""
end

#powers for next iteration
def increment_powers(pows, digit)

  #increments power stuffs
  i=0
  while(i < pows.length and pows[i] < 2 )
    i += 1
  end



  if(pows[i] >= 2)
    pows[i] += -2
    pows[i+1] += 1
  end

  if digit != i
    while i > 0
      if pows[i] > 0
        pows[i-1] = 2* pows[i]
        pows[i] = 0
      end
      i += -1
    end
  end
  
  pows
end


#checks if all the possibilities of a number-set of weight something (iterations) is finished
def should_continue(pows)
  i=0
  while pows[i] == 0
    i += 1
  end
  
  pows[i] != 1
end

#Converts list of powers of primes to int
def primeProduct(pows)
  product = 1
  for i in 0..$primes.length-1
    product *=  $primes[i]**pows[i]
  end
  
  product
end

#guess
def printArray(arr)
  print "\t["
  arr.each do |z|
    print z.to_s + ", "
  end
  puts "]"
end

#How we are sorting the elements in the output
# We have no clue if it is at all relevent ;-)
def arrayNameVal(arr)
  val = 1.0
  for i in 0..arr.length-1
    val *= $primes[i]**arr[i]
  end
  val
end


x = sequence(100)

x.each do |z|
  printArray(z)
  #puts arrayNameVal(z).to_f
  
end