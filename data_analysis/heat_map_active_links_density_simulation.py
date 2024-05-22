import numpy as np
import matplotlib.pyplot as plt
from active_links_density import active_links_density
import seaborn as sns

direction=r"/Users/ro/Documents/Tesis_BSc/evolution/heat_map"
matrix_i=[]
matrix_f=[]
states_list=[]

runs=10
divisions=20
heat_map_data=np.zeros((divisions,divisions))
curve=[]
for i in range(divisions):
    for j in range(divisions):
        for k in range(runs):
            matrix_f.append(direction+"/m_f_r%d_d%d_run%d"%(i,j,k))
            matrix_i.append(direction+"/m_i_r%d_d%d_run%d"%(i,j,k))
            states_list.append(direction+"/states_r%d_d%d_run%d"%(i,j,k))
for i in range(divisions):
    for j in range(divisions):
        
        temp_density_active_links=0
        for k in range(runs):
            print("r:%d d:%d run:%d"%(i,j,k))
            temp_a_f=active_links_density(matrix_f[i*divisions*runs+j*runs+k], states_list[i*divisions*runs+j*runs+k])

            temp_density_active_links+=temp_a_f
        temp_density_active_links/=runs
        heat_map_data[divisions-1-j][i]=temp_density_active_links
        if(divisions-i==j):
            curve.append(temp_density_active_links)

sns.heatmap(heat_map_data,vmin=0,vmax=1)
plt.show()


plt.figure()
plt.plot(curve)
plt.show()

file=open("heat_map_density","w")
for i in range(len(heat_map_data)):
    for j in range(len(heat_map_data[0])):
        file.write("%f "%(heat_map_data[i][j]))
    file.write("\n")
file.close()
