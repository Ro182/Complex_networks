import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np
import community as community_louvain



def graph_louvain(file):

    
    
    f=open(file)
    matrix=[]
    for line in f:
        temp_row=[]
        temp=line.strip().split()
        for elem in temp:
            temp_row.append(int(elem))
        matrix.append(temp_row)
    f.close()    
    
    
    matrix_np=np.array(matrix)
    G=nx.from_numpy_matrix(matrix_np)

    partition=community_louvain.best_partition(G)
    pos = nx.spring_layout(G)
    cmap = cm.get_cmap('viridis', max(partition.values()) + 1)
    nx.draw_networkx_nodes(G, pos, partition.keys(), node_size=40,cmap=cmap, node_color=list(partition.values()))
    nx.draw_networkx_edges(G, pos, alpha=0.5)

    
    
