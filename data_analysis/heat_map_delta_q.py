import numpy as np
import matplotlib.pyplot as plt
from modularity import modularity
import seaborn as sns

direction=r"/Users/ro/Documents/Tesis_BSc/evolution/heat_map"
matrix_i=[]
matrix_f=[]

runs=10
divisions=20
heat_map_data=np.zeros((divisions,divisions))
curve=[]
for r in range(divisions):
    for d in range(divisions):
        for run in range(runs):
            matrix_f.append(direction+"/m_f_r%d_d%d_run%d"%(r,d,run))
            matrix_i.append(direction+"/m_i_r%d_d%d_run%d"%(r,d,run))
 
for r in range(divisions):
    print("%d out of %d"%(r,divisions))
    for d in range(divisions):
        temp_delta_modularity_links=0
        for run in range(runs):
            temp_Q_i=modularity(matrix_i[r*divisions*runs+d*runs+run])
            temp_Q_f=modularity(matrix_f[r*divisions*runs+d*runs+run])
            temp_delta_modularity_links+=temp_Q_f-temp_Q_i
        temp_delta_modularity_links/=runs
        heat_map_data[r][divisions-1-d]=temp_delta_modularity_links
        if(divisions-r==d):
            curve.append(temp_delta_modularity_links)

sns.heatmap(heat_map_data)

plt.figure()
plt.plot(curve)
plt.show()

file=open("heat_map_delta_q","w")
for r in range(len(heat_map_data)):
    for d in range(len(heat_map_data[0])):
        file.write("%f "%(heat_map_data[r][d]))
    file.write("\n")
file.close()