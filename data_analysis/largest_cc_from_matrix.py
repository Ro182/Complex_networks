import networkx as nx
import numpy as np
from links_to_matrix import links_to_matrix

def largest_cc(file_matrix):
    f=open(file_matrix)
    matrix=[]
    for line in f:
        temp_row=[]
        temp=line.strip().split()
        for elem in temp:
            temp_row.append(int(elem))
        matrix.append(temp_row)
    matrix_np=np.array(matrix)
    G=nx.from_numpy_array(matrix_np)
    
    largest_cc=[len(c) for c in sorted(nx.connected_components(G), key=len, reverse=True)][0]

    return largest_cc

