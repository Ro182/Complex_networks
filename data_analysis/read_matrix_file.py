
import numpy as np
def read_matrix_file(file):
    f=open(file,"r")
    matrix=[]
    for line in f:
        temp_line=[]
        temp=line.strip().split()
        for elem in temp:
            temp_line.append(float(elem))
        matrix.append(temp_line)
    return np.matrix(matrix) 

        
