import matplotlib.pyplot as plt
import numpy as np



f=open(r"/Users/ro/Documents/Tesis_BSc/sc/final/node_dynamics_active_link_density_asymptotic_vs_U/active_links_densities")

size=0
densities=[]
stds=[]

for line in f:
    size+=1
    temp_line=line.strip().split()
    temp_values=[]
    for elem in temp_line:
        temp_values.append(float(elem))
    densities.append(np.mean(temp_values))
    stds.append(np.std(temp_values))
    
Us=np.linspace(0,1,size)
plt.figure()
plt.plot(Us,densities)
plt.show()
plt.figure()
plt.errorbar(Us,densities,yerr=stds,alpha=0.5,ecolor='black',capsize=5)
plt.ylim(0,1)
plt.xlim(0,1)
plt.title("Active Link density vs U")
plt.xlabel("U")
plt.ylabel("Active link density")
plt.show()
