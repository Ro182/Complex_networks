import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from largest_cc import largest_cc
direction=r"/Users/ro/Documents/Tesis_BSc/evolution/heat_map"
matrix_f=[]

N=400
runs=10
divisions=20
heat_map_data=np.zeros((divisions,divisions))
curve=[]
for r in range(divisions):
    for d in range(divisions):
        for run in range(runs):
            matrix_f.append(direction+"/m_f_r%d_d%d_run%d"%(r,d,run))
            
for r in range(divisions):
    print("%d out of %d"%(r,divisions))    
    for d in range(divisions):    
        temp_largest_cc=0
        for run in range(runs):
            temp_largest_cc+=largest_cc(matrix_f[r*divisions*runs+d*runs+run])/N
        temp_largest_cc/=runs

        heat_map_data[r][divisions-1-d]=temp_largest_cc
        if(divisions-r==d):
            curve.append(temp_largest_cc)

sns.heatmap(heat_map_data,vmin=0,vmax=1)

file=open("heat_map_S","w")
for r in range(len(heat_map_data)):
    for d in range(len(heat_map_data[0])):
        file.write("%f "%(heat_map_data[r][d]))
    file.write("\n")
file.close()

plt.figure()
plt.plot(curve)

plt.show()