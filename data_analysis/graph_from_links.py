import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

from links_to_matrix import links_to_matrix

import matplotlib.cm as cm
import community as community_louvain



def graph_louvain(file_links):

    
    plt.figure()

    matrix=links_to_matrix(file_links)
    
    
    matrix_np=np.array(matrix)
    G=nx.from_numpy_array(matrix_np)

    partition=community_louvain.best_partition(G)
    pos = nx.spring_layout(G)
    cmap = cm.get_cmap('viridis', max(partition.values()) + 1)
    nx.draw_networkx_nodes(G, pos, partition.keys(), node_size=40,cmap=cmap, node_color=list(partition.values()))
    nx.draw_networkx_edges(G, pos, alpha=0.5)



def graph(file_states,file_links):

    states=[]
    f=open(file_states)
    
    line=f.readline()
    temp=line.strip().split()
    f.close()
    for elem in temp:
        states.append(float(elem))
    print("Number of nodes %d"%(len(states)))
    
    matrix=links_to_matrix(file_links)
    
    plt.figure()
    
    
    
    matrix_np=np.array(matrix)
    G=nx.from_numpy_array(matrix_np)
    nx.draw(G,with_labels=False,node_color=states,node_size=40)  

main_dir=r"/Users/ro/Documents/Tesis_BSc/sc/final/node_dynamics_rewiring_Smax_vs_U"


file_links=main_dir+"/links_community_f_0"
file_states=main_dir+"/states_community_f_0"
graph(file_states,file_links)
graph_louvain(file_links)
plt.show()