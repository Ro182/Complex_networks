
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np


#Also important to check this script, might containt the main plots


dir="/Users/ro/Documents/Tesis_BSc/r/"

divisions=20

heat_map_model=np.zeros((divisions,divisions),dtype=float)
curve=[]

for r in range(1,divisions):
    for d in range(1,divisions):
        temp_r=r/divisions
        temp_d=d/divisions
        temp_density=(temp_d-temp_d*temp_r)/(temp_d+temp_r-2*temp_d*temp_r)
        heat_map_model[divisions-1-d][r]=temp_density
        if (r==divisions-1-d):
            curve.append(temp_density)
plt.figure()
sns.heatmap(heat_map_model,vmax=1,vmin=0,xticklabels=False,yticklabels=False)
plt.title("Active links density Model")
plt.xlabel("r",fontweight="bold")
plt.ylabel("d",fontweight="bold")

plt.show()

plt.figure()
plt.plot(curve,'.')
plt.title("Active links density Model")
plt.xlabel("r=1-d",fontweight="bold")
plt.show()

temp_heat_map_density_data=[]
mode='r'
file=open(dir+"heat_map_density",mode)

for line in file:
    temp_line=line.strip().split()
    temp_row=[]
    for elem in temp_line:
        temp_row.append(float(elem))
        
    temp_heat_map_density_data.append(temp_row) 
    
heat_map_density_data=np.array(temp_heat_map_density_data)
plt.figure()
sns.heatmap(heat_map_density_data,xticklabels=False,yticklabels=False)
plt.title("Density of active links")
plt.xlabel("r",fontweight="bold")
plt.ylabel("d",fontweight="bold")
plt.show()

curve=[]
for i in range(divisions):
    curve.append(heat_map_density_data[i][i])
plt.figure()
plt.plot(curve)
plt.title("Density of active links")
plt.xlabel("r=1-d",fontweight="bold")
plt.show()






temp_heat_map_delta_q_data=[]
mode='r'
file=open(dir+"heat_map_delta_q",mode)

for line in file:
    temp_line=line.strip().split()
    temp_row=[]
    for elem in temp_line:
        temp_row.append(float(elem))
        
    temp_heat_map_delta_q_data.append(temp_row) 
heat_map_delta_q_data=np.empty((divisions,divisions),dtype=float)

for i in range(divisions):
    for j in range(divisions):
        heat_map_delta_q_data[divisions-1-j][i]=temp_heat_map_delta_q_data[divisions-1-i][j]
        
plt.figure()     
sns.heatmap(heat_map_delta_q_data,xticklabels=False,yticklabels=False,vmin=0,vmax=0.5)
plt.title("\u0394 Q")
plt.xlabel("r",fontweight="bold")
plt.ylabel("d",fontweight="bold")
plt.show()


curve=[]
for i in range(divisions):
    curve.append(heat_map_delta_q_data[i][i])
plt.figure()
plt.plot(curve)
plt.title("\u0394 Q")
plt.xlabel("r=1-d",fontweight="bold")
plt.show()



temp_heat_map_S_data=[]
mode='r'
file=open(dir+"heat_map_S",mode)

for line in file:
    temp_line=line.strip().split()
    temp_row=[]
    for elem in temp_line:
        temp_row.append(float(elem))
        
    temp_heat_map_S_data.append(temp_row) 
heat_map_S_data=np.empty((divisions,divisions),dtype=float)

for i in range(divisions):
    for j in range(divisions):
        heat_map_S_data[divisions-1-j][i]=temp_heat_map_S_data[divisions-1-i][j]

plt.figure()
sns.heatmap(heat_map_S_data,xticklabels=False,yticklabels=False,vmin=0,vmax=1)
plt.title("S")
plt.xlabel("r",fontweight="bold")
plt.ylabel("d",fontweight="bold")
plt.show()

curve=[]
for i in range(divisions):
    curve.append(heat_map_S_data[i][i])
plt.figure()
plt.plot(curve)
plt.title("S")
plt.xlabel("r=1-d",fontweight="bold")
plt.show()

heat_map_density_delta_q=np.empty((divisions,divisions),dtype=float)
heat_map_S_delta_q=np.empty((divisions,divisions),dtype=float)

for i in range(divisions):
    for j in range(divisions):
        heat_map_density_delta_q[i][j]=heat_map_density_data[i][j]*heat_map_delta_q_data[i][j]
        heat_map_S_delta_q[i][j]=heat_map_S_data[i][j]*heat_map_delta_q_data[i][j]

plt.figure()
sns.heatmap(heat_map_density_delta_q,xticklabels=False,yticklabels=False)
plt.title("Active links density x \u0394 Q")
plt.xlabel("r",fontweight="bold")
plt.ylabel("d",fontweight="bold")
plt.show()

curve=[]
for i in range(divisions):
    curve.append(heat_map_density_delta_q[i][i])
plt.figure()
plt.plot(curve)
plt.title("Active links density x \u0394 Q")
plt.xlabel("r=1-d",fontweight="bold")
plt.ylim(-0.01,0.12)
plt.show()

plt.figure()
sns.heatmap(heat_map_S_delta_q,xticklabels=False,yticklabels=False)
plt.title("S x \u0394 Q")
plt.xlabel("r",fontweight="bold")
plt.ylabel("d",fontweight="bold")
plt.show()

curve=[]
for i in range(divisions):
    curve.append(heat_map_S_delta_q[i][i])
plt.figure()
plt.plot(curve)
plt.title("S x \u0394 Q")
plt.xlabel("r=1-d",fontweight="bold")
plt.show()






heat_map_model_delta_q=np.empty((divisions,divisions),dtype=float)

for i in range(divisions):
    for j in range(divisions):
        heat_map_model_delta_q[i][j]=heat_map_model[i][j]*heat_map_delta_q_data[i][j]

plt.figure()
sns.heatmap(heat_map_model_delta_q,xticklabels=False,yticklabels=False)
plt.title("Active links density (model) x \u0394 Q")
plt.xlabel("r",fontweight="bold")
plt.ylabel("d",fontweight="bold")

curve=[]
for i in range(divisions):
    curve.append(heat_map_model_delta_q[i][i])
plt.figure()
plt.plot(curve)
plt.title("Active links density(model) x \u0394 Q")
plt.xlabel("r=1-d",fontweight="bold")   
plt.ylim(-0.01,0.12)


heat_map_differece=np.zeros((divisions,divisions),dtype=float)

for i in range(1,divisions-1):
    for j in range(1,divisions-1):
        heat_map_differece[i][j]=np.abs(heat_map_model[i][j]-heat_map_density_data[i][j])

plt.figure()

sns.heatmap(heat_map_differece,xticklabels=False,yticklabels=False)
plt.title("Absolute Error Active links density (model),Active links density")
plt.xlabel("r",fontweight="bold")
plt.ylabel("d",fontweight="bold")

curve=[]
for i in range(divisions):
    curve.append(heat_map_differece[i][i])
plt.figure()
plt.plot(curve)
plt.title("Active links density(model) - Active links density")
plt.xlabel("r=1-d",fontweight="bold")   

plt.show()