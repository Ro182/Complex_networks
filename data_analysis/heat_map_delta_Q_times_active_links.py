import numpy as np
import matplotlib.pyplot as plt
from modularity import modularity
import seaborn as sns
from active_links_density import active_links_density

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
    print("%d out of %d"%(i,divisions))
    for j in range(divisions):
    
        temp_delta_modularity_links=0
        temp_delta_active_links=0
        for k in range(runs):
            temp_Q_i=modularity(matrix_i[i*divisions*runs+j*runs+k])
            temp_Q_f=modularity(matrix_f[i*divisions*runs+j*runs+k])
            temp_a_f=active_links_density(matrix_f[i*divisions*runs+j*runs+k], states_list[i*divisions*runs+j*runs+k])
            
            temp_delta_modularity_links+=temp_Q_f-temp_Q_i
            temp_delta_active_links+=temp_a_f
            
        temp_delta_modularity_links/=runs
        temp_delta_active_links/=runs
        
        heat_map_data[i][divisions-1-j]=temp_delta_modularity_links*temp_delta_active_links
        if(divisions-i==j):
            curve.append(temp_delta_modularity_links*temp_delta_active_links)

sns.heatmap(heat_map_data)
plt.show()

plt.figure()
plt.plot(curve)
plt.show()

    