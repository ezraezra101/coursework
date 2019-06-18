#Finds the determinant of a matrix (2Dimensional nxn array)
def det(matrix)
  #puts "Matrix:"
  #printMatrix(matrix)
  
  if(matrix.length > 2)
    
    sign = 1
    determinant = 0
    
    for i in 0..matrix.length-1
      
      determinant += sign * matrix[0][i] * det(stripRowCol(matrix, i))
      
     # puts "Determinant:" + determinant.to_s
      
      sign *= -1
    end
    
    determinant
    
  else
    matrix[0][0] * matrix[1][1] - matrix[1][0]*matrix[0][1]
  end
end

#Removes the column of the second argument and the first (index 0) row
def stripRowCol(m, row)
  #Deep copies m to matrix
  matrix = Marshal.load(Marshal.dump(m))

  matrix.delete_at(0)
  
  for i in 0..matrix.length - 1
      matrix[i].delete_at(row)
  end
  
  matrix
end


#Prints a 2D array
def printMatrix(matrix)
  
  for i in 0..matrix.length-1
    print "[ "
    for j in 0..matrix[i].length-1
      print matrix[i][j].to_s + " "
    end
    puts "]"
  end
end


matrix = [[-4, -1, 0],
          [0, -1, -1],
          [3, 4, 0]]

printMatrix(matrix)
puts ""

#printMatrix(stripRowCol(matrix, 2))
puts ""

puts det(matrix)