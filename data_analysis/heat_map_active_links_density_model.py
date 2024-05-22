import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
divisions=256
heat_map=np.zeros((divisions,divisions),dtype=float)
curve=[]

for r in range(1,divisions):
    for d in range(1,divisions):
        temp_r=r/divisions
        temp_d=d/divisions
        temp_density=(temp_d-temp_d*temp_r)/(temp_d+temp_r-2*temp_d*temp_r)
        heat_map[divisions-1-d][r]=temp_density
        if (d==divisions-1-r):
            curve.append(temp_density)

sns.heatmap(heat_map,vmax=1,vmin=0,xticklabels=False,yticklabels=False)
plt.title("Active links density Model")
plt.xlabel("r",fontweight="bold")
plt.ylabel("d",fontweight="bold")
plt.show()

plt.figure()
plt.plot(curve,'.')
plt.title("Active links density Model")
plt.xlabel("r=d",fontweight="bold")
plt.show()