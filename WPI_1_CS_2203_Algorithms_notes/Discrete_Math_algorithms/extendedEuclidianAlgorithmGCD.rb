
#Euclidian algorithm for finding gcd
def euclid(a, b, lsub1 = 1, lsub2 = "Start", qsub1 = "Dummy")
  
  if b == 0
    puts "Inverse: " + lsub1.to_s
    return a
  else
    q = a/b
    r = a % b
    
    if lsub2 == "Start"
      l = 0
    elsif lsub1 == "Start"
      l = 1
    else
      l = lsub2 - lsub1 * qsub1
    end
    
    return euclid(b, r, l, lsub1, q)
  end

end


#Euclidian algorithm for finding inverse modulo aNum
# a^-1 modulo b is this value
def modInverse(a, b, lsub1 = 1, lsub2 = "Start", qsub1 = "Dummy")
  
  if b == 0
    return lsub1
  else
    q = a/b
    r = a % b
    
    if lsub2 == "Start"
      l = 0
    else
      l = lsub2 - lsub1 * qsub1
    end
    
    puts l

    return modInverse(b, r, l, lsub1, q)
  end

end



puts modInverse 23, 120

puts (if 1 == 23 * modInverse(23, 120) % 120 then "Test passes" else "Test fails" end)


puts 1 == 23 * modInverse(23, 120) % 120 ? "Test passes" : "Test fails"



puts "49^-1 modulo 44: " + (modInverse 44, 49).to_s + "\n44^-1 modulo 49: " + (modInverse 49, 44).to_s

puts (44 * -10) % 49


puts 49 * 9 % 44
