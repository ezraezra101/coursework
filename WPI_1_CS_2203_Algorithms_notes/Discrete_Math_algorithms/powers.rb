

#Computes x^m in approx O(log(m)) time
def pow(x, m)

  ans = 1

  r = m % 2
  m /= 2

  while(m != 0 || r != 0)
    
    
    if(r==1)
      ans *= x
    end
    
    x *= x
    
  
    r = m % 2
    m /=2
  end

  ans
end


def slow_pow(num, pow)
  if(pow == 0)
    return 1
  else
    return num * slow_pow(num, pow - 1)
  end
end

puts pow 5, 3

puts "Wow... these put out different answers"

puts pow(1.001593894001, 1000)

puts slow_pow(1.001593894001, 1000)