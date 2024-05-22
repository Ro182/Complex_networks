

import matplotlib.pyplot as plt
from largest_cc_from_matrix import largest_cc
direction=r"/Users/ro/Documents/Tesis_BSc/evolution/Node_dynamics/u_1_Pr_variates"
matrix_f=[]

N=400
runs=10
divisions=10
plot=[]
curve=[]
for Pr in range(divisions):
    for run in range(runs):
        matrix_f.append(direction+"/m_f_MD_U_40_Pr_%d_run%d"%(Pr,run))



for i in range(divisions):    
    temp_largest_cc=0
    for k in range(runs):
        temp_largest_cc+=largest_cc(matrix_f[i*runs+k])/N
    temp_largest_cc/=runs
    plot.append(temp_largest_cc)



plt.figure()
plt.plot(plot,label="u=1")





direction=r"/Users/ro/Documents/Tesis_BSc/evolution/Node_dynamics/u_0.5_Pr_variates"
matrix_f=[]

N=400
runs=5
divisions=10
plot=[]
curve=[]
for Pr in range(divisions):
    for run in range(runs):
        matrix_f.append(direction+"/m_f_MD_U_20_Pr_%d_run%d"%(Pr,run))



for i in range(divisions):    
    temp_largest_cc=0
    for k in range(runs):
        temp_largest_cc+=largest_cc(matrix_f[i*runs+k])/N
    temp_largest_cc/=runs
    plot.append(temp_largest_cc)



plt.plot(plot,label="u=0.5")




direction=r"/Users/ro/Documents/Tesis_BSc/evolution/Node_dynamics/u_0_Pr_variates"
matrix_f=[]

N=400
runs=5
divisions=10
plot=[]
curve=[]
for Pr in range(divisions):
    for run in range(runs):
        matrix_f.append(direction+"/m_f_MD_U_0_Pr_%d_run%d"%(Pr,run))



for i in range(divisions):    
    temp_largest_cc=0
    for k in range(runs):
        temp_largest_cc+=largest_cc(matrix_f[i*runs+k])/N
    temp_largest_cc/=runs
    plot.append(temp_largest_cc)



plt.plot(plot,label="u=0")
plt.legend()
plt.show()