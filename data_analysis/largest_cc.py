import networkx as nx
import numpy as np
from links_to_matrix import links_to_matrix

def largest_cc(file_matrix):

  
    matrix_np=links_to_matrix(file_matrix)
    
    G=nx.from_numpy_array(matrix_np)
    
    largest_cc=[len(c) for c in sorted(nx.connected_components(G), key=len, reverse=True)][0]

    return largest_cc

