from links_to_matrix import links_to_matrix

def active_links_density(file_links,file_states ):
    
    
    matrix=links_to_matrix(file_links)
    
    total_links=0
    
    for i in range(len(matrix)):
        for j in range(i+1,len(matrix[0])):
            if (matrix[i][j])==1:
                total_links+=1
    
    f=open(file_states)
    states=f.readline().split()
    
    
    active_links=0
    
    for i in range(len(matrix)):
        for j in range(i+1, len(matrix[0])):
            if(matrix[i][j]==1):
                if (states[i]!=states[j]):
                    active_links+=1
    return active_links/total_links

