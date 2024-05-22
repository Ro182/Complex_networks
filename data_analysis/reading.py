import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
from model import model
from modularity import modularity
import community as community_louvain


print("From Simulation")


f=open(r"/Users/ro/Documents/Tesis_BSc/evolution/matrix_representation_i")
matrix=[]
for line in f:
    temp_row=[]
    temp=line.strip().split()
    for elem in temp:
        temp_row.append(int(elem))
    matrix.append(temp_row)
f.close()    

total_links=0

for i in range(len(matrix)):
    for j in range(i+1,len(matrix[0])):
        if (matrix[i][j])==1:
            total_links+=1

active_links=[]
f=open(r"/Users/ro/Documents/Tesis_BSc/evolution/active_links")

line=f.readline()
temp=line.strip().split()
f.close()
for elem in temp:
    active_links.append(float(elem)/total_links)


plt.plot(active_links,'.',label="Simulation")
plt.ylim(0,1)
plt.xlabel("Time",fontweight='bold')
plt.ylabel("Density",fontweight='bold')


matrix_np=np.array(matrix)
modularity_i=modularity(r"/Users/ro/Documents/Tesis_BSc/evolution/matrix_representation_i")
modularity_f=modularity(r"/Users/ro/Documents/Tesis_BSc/evolution/matrix_representation_f")

print("Total links %f"%(total_links))
print("Initial active links %f"%(active_links[0]*total_links))
print("Final active links %f"%(active_links[-1]*total_links))
print("Initial density of active links %f"%(active_links[0]))
print("Final active links density %f"%(active_links[-1]))
print ("Initial modularity %f"%(modularity_i))
print ("Final modularity %f"%(modularity_f))
print ("Delta modularity %f"%(modularity_f-modularity_i))

G=nx.from_numpy_array(matrix_np)

print([len(c) for c in sorted(nx.connected_components(G), key=len, reverse=True)][0])


k=4
N=400  
G=40
d=0.5
r=0.5
time_max=50
model(r,d,N,G,k,time_max)
plt.legend()
plt.show()


