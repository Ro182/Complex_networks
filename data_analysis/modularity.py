import networkx as nx
import numpy as np
from links_to_matrix import links_to_matrix

import community as community_louvain

def modularity(file):
     
    matrix_np=links_to_matrix(file)
    G=nx.from_numpy_array(matrix_np)

    partition=community_louvain.best_partition(G)
    modularity=community_louvain.modularity(partition,G)
    return modularity
    
