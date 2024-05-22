import numpy as np
from links_to_matrix import links_to_matrix

counter=0
def walk(node,visited,matrix,states,size):
    visited[node]=1 
    global counter
    
    
    for i in range(size):
        if visited[i]==0:
            if matrix[node][i]==1 and states[node]==states[i]:
                counter+=1
                walk(i,visited,matrix,states,size)


                    

    
            
    
def largest_cc_same_state(file_links,file_states):
    
    
    matrix=links_to_matrix(file_links)
    
    size=len(matrix)
    
    states=[]
    f=open(file_states)
    
    line=f.readline()
    temp=line.strip().split()
    f.close()
    for elem in temp:
        states.append(float(elem))
        
    visited=np.zeros((size))

    max_size=1
    global counter
    for i in range(size):
        counter=0

        if visited[i]==0:
            walk(i,visited,matrix,states,size)

        if counter>max_size:
            max_size=counter

    return max_size


