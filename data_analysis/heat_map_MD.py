
import seaborn as sns
import matplotlib.pyplot as plt
from largest_cc import largest_cc
import numpy as np

#Couldn't find the folder Node_dynamics_Rewiring_r_vs_Pr

direction=r"/Users/ro/Documents/Tesis_BSc/sc/Node_dynamics_Rewiring_r_vs_Pr"
matrix_f=[]

N=400
runs=3
divisions_Pr=9
divisions_U=10
curve=[]

heat_map_S=np.zeros((divisions_U,divisions_Pr),dtype=float)
for U in range(divisions_U):
    for Pr in range(divisions_Pr):
        for run in range(runs):
            matrix_f.append(direction+"/m_f_MD_U_%d_Pr_%d_run%d"%(U,Pr,run))

for U in range(divisions_U):
    for Pr in range(divisions_Pr):    
        print(U,Pr)
        temp_largest_cc=0
        for run in range(runs):
            temp_largest_cc+=largest_cc(matrix_f[U*divisions_Pr*runs+Pr*runs+run])/N
        temp_largest_cc/=runs
        heat_map_S[divisions_U-1-U][Pr]=temp_largest_cc

sns.heatmap(heat_map_S,vmax=1,vmin=0,xticklabels=False,yticklabels=False)

file=open("heat_map_MD_float_d50_r50","w")
for i in range(len(heat_map_S)):
    for j in range(len(heat_map_S[0])):
        file.write("%f "%(heat_map_S[i][j]))
    file.write("\n")
file.close()

plt.figure()
