from mrjob.job import MRJob

# Run with `python mr_matrix_multiplication.py input_matrices.data`

#A.shape = (3, 2)
#B.shape = (2, 3)
A_WIDTH = 2
A_HEIGHT = 3
B_WIDTH = 3
B_HEIGHT = A_WIDTH

C_HEIGHT = A_HEIGHT
C_WIDTH = B_WIDTH

class MRMatrixMultiply(MRJob):

    def mapper(self, _, matrix_point):
        # matrix_point is in the form of ('Matrix', row, col, value)
        matrix_point = matrix_point.split(',')
        matrix_name, row, col, value = matrix_point
        matrix_name = matrix_name.encode('utf-8')
        row = int(row)
        col = int(col)
        value = float(value)
        if(matrix_name == 'A'):
            for i in range(1,C_WIDTH+1):
                yield ('C', row, i), ((matrix_name, row, col), value)
        else: # matrix_point[0] == 'B'
            for i in range(1, C_HEIGHT+1):
                yield ('C', i, col), ((matrix_name, row, col), value)

    def reducer(self, key, values):
        inputs = dict()
        for value in values:
            inputs[tuple(value[0])] = value[1]
        value = 0
        for i in range(1, A_WIDTH+1):
            value += float(inputs[('A',key[1],i)]) * float(inputs[('B', i, key[2])])

        # Produces ('C', row, col), value
        yield ("_", key + [value])


if __name__ == '__main__':
    MRMatrixMultiply.run()
