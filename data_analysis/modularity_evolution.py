import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
import community as community_louvain



def modularity_evolution(N,times,file_name,runs):
    modularities=np.zeros(times)
    for k in range(runs):
        f=open(file_name+str(k))
        for i in range(times):
            matrix=[]
            for j in range (N):
                temp_row=[]
                line=f.readline()
                temp=line.strip().split()
                for elem in temp:
                    temp_row.append(int(elem))
                matrix.append(temp_row)
            matrix_np=np.array(matrix)
            G=nx.from_numpy_array(matrix_np)
            partition=community_louvain.best_partition(G)
            modularity=community_louvain.modularity(partition,G)
            modularities[i]+=modularity
        f.close()       
        
    delta_modularities=[]
    for elem in modularities:
        delta_modularities.append((elem-modularities[0])/runs)
    plt.figure()
    plt.plot(delta_modularities)
    plt.xlabel("Time",fontweight="bold")
    plt.ylabel("Delta Q",fontweight="bold")
    plt.ylim(0,0.5)
    plt.show()
runs=10
N=1000
times=50
file_name=r"/Users/ro/Documents/Tesis_BSc/evolution/matrix_representation_evolution"
modularity_evolution(N,times,file_name,runs)