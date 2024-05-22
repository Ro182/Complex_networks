import numpy as np
import matplotlib.pyplot as plt
from largest_cc import largest_cc
from largest_cc_same_state import largest_cc_same_state


#Plots the curve S vs P for a given value of U.


direction="./../src_codes/node_dynamics_rewiring_U/r_10_d_5/"
direction="./../src_codes/node_dynamics_rewiring_U/test_1/"

states_f=[]
links_f=[]
N=100
runs=2
divisions=30
heat_map_data=np.zeros((divisions,divisions))
Ss=[]
xs=np.linspace(0,1,divisions)
for Pr in range(divisions):
    for run in range(runs):
        states_f.append(direction+"states_f_Pr_%d_run%d.dat"%(Pr,run))
        links_f.append(direction+"links_f_MD_Pr_%d_run%d.dat"%(Pr,run))

stds=[]
for Pr in range(divisions):
    temp_largest_cc=0
    temp_stds=[]
    for run in range(runs):
        #temp_largest_cc+=largest_cc(matrix_f[Pr*runs+run])/N
        temp_val=largest_cc_same_state(links_f[Pr*runs+run],states_f[Pr*runs+run])/N
        temp_largest_cc+=temp_val
        temp_stds.append(temp_val)
        
    temp_largest_cc/=runs
    stds.append(np.std(temp_stds))

    Ss.append(temp_largest_cc)
#print(stds)
plt.errorbar(xs,Ss,yerr=stds,alpha=0.5,ecolor='black',capsize=5)
plt.ylim(0,1)
plt.xlim(0,1)
plt.title("S vd Pr U=1")
plt.xlabel("Pr")
plt.ylabel("S")
plt.show()