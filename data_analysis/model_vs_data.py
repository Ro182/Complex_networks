

import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
from model import model
from model_MD import model_MD
from modularity import modularity
import community as community_louvain


print("From Simulation")

U=0

f=open(r"/Users/ro/Documents/Tesis_BSc/evolution/Node_dynamics/active_links_evolution/m_i_MD_r_5_d_5_U_%d_Pr_10_Pc_0_run_0"%(U))
f=open(r"/Users/ro/Documents/Tesis_BSc/evolution/matrix_representation_f_community")
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
        if (matrix[i][j]==1 and i!=j):
            total_links+=1

active_links=[]
f=open(r"/Users/ro/Documents/Tesis_BSc/evolution/Node_dynamics/active_links_evolution/active_links")
f=open(r"/Users/ro/Documents/Tesis_BSc/evolution/active_links")

line=f.readline()
temp=line.strip().split()
f.close()
for elem in temp:
    active_links.append(float(elem)/total_links)

plt.figure()
plt.plot(active_links,'.',label="Simulation")
print(active_links)
plt.xlabel("Time",fontweight='bold')
plt.ylabel("Density",fontweight='bold')


# =============================================================================
# matrix_np=np.array(matrix)
# modularity_i=modularity(r"C:\Users\USUARIO\Desktop\tesis\evolution\Node_dynamics\active_links_evolution\m_i_MD_r_5_d_5_U_%d_Pr_0_Pc_10_run_0"%(U))
# modularity_f=modularity(r"C:\Users\USUARIO\Desktop\tesis\evolution\Node_dynamics\active_links_evolution\m_f_MD_r_5_d_5_U_%d_Pr_0_Pc_10_run_0"%(U))
# 
# print("Total links %f"%(total_links))
# print("Initial active links %f"%(active_links[0]*total_links))
# print("Final active links %f"%(active_links[-1]*total_links))
# print("Initial density of active links %f"%(active_links[0]))
# print("Final active links density %f"%(active_links[-1]))
# print ("Initial modularity %f"%(modularity_i))
# print ("Final modularity %f"%(modularity_f))
# print ("Delta modularity %f"%(modularity_f-modularity_i))
# 
# G=nx.from_numpy_matrix(matrix_np)
# 
# print([len(c) for c in sorted(nx.connected_components(G), key=len, reverse=True)][0])
# 
# =============================================================================

k=4
N=400  
G=40
d=0.5
r=0.5
time_max=50
U=0.0

#model_MD(r, d, N, G, k, time_max,U)
model(r,d,N,G,k,time_max,n0=active_links[0])
plt.legend()
plt.ylim(0,1.1)
plt.show()
