import networkx as nx
import matplotlib.pyplot as plt
import numpy as np




def graph(file_states,file_matrix):

    states=[]
    f=open(file_states)
    
    line=f.readline()
    temp=line.strip().split()
    f.close()
    for elem in temp:
        states.append(int(elem))
    print("Number of nodes %d"%(len(states)))
    
    
    plt.figure()
    
    
    f=open(file_matrix)
    matrix=[]
    for line in f:
        temp_row=[]
        temp=line.strip().split()
        for elem in temp:
            temp_row.append(int(elem))
        matrix.append(temp_row)
    f.close()    
    
    
    matrix_np=np.array(matrix)
    G=nx.from_numpy_array(matrix_np)
    pos = nx.fruchterman_reingold_layout(G,iterations=100)
    nx.draw(G,pos,with_labels=False,node_color=states)  
    plt.show()    
