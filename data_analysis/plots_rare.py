import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt


#Important to check this code, have all the plots I want#


#ticks for plots 
xticks=[]
yticks=[]
for i in range(11):
    xticks.append("%0.1f"%(i*0.1))
    yticks.append("%0.1f"%((10-i)*0.1))




#direction of data
main_dir=r"/Users/ro/Documents/Tesis_BSc/data_and_plots"
sec_dir=r"/node_dynamics_rewiring_U_vs_Pr_r10_d_5"


#heat map model

divisions=64

heat_map_model=np.zeros((divisions,divisions),dtype=float)

for r in range(divisions):
    for d in range(1,divisions):
        temp_r=r/divisions
        temp_d=d/divisions
        temp_density=(temp_d-temp_d*temp_r)/(temp_d+temp_r-2*temp_d*temp_r)
        heat_map_model[divisions-1-d][r]=temp_density

plt.figure()
ax=sns.heatmap(heat_map_model,vmax=1,vmin=0)
plt.title("Active links density Model")
old_ticks = ax.get_xticks()
new_ticks = np.linspace(np.min(old_ticks), np.max(old_ticks), len(xticks))
ax.set_xticks(new_ticks)
ax.set_xticklabels(xticks)
ax.set_yticks(new_ticks)
ax.set_yticklabels(yticks)


#active links
plt.figure()
f=open(main_dir+sec_dir+"/heat_map_active_links_density","r")
heat_map_active_links_density=[]

for line in f:
    temp_line=[]
    temp=line.strip().split()
    for elem in temp:
        temp_line.append(float(elem))
    heat_map_active_links_density.append(temp_line)
    


ax=sns.heatmap(heat_map_active_links_density,vmax=1,vmin=0)
old_ticks = ax.get_xticks()
new_ticks = np.linspace(np.min(old_ticks), np.max(old_ticks), len(xticks))
ax.set_xticks(new_ticks)
ax.set_xticklabels(xticks)
ax.set_yticks(new_ticks)
ax.set_yticklabels(yticks)

plt.title("Active link density")
f.close()

#S
plt.figure()

f=open(main_dir+sec_dir+"/heat_map_S","r")
heat_map_S=[]

for line in f:
    temp_line=[]
    temp=line.strip().split()
    for elem in temp:
        temp_line.append(float(elem))
    heat_map_S.append(temp_line)
    
ax=sns.heatmap(heat_map_S)
plt.title("S")
f.close()

old_ticks = ax.get_xticks()
new_ticks = np.linspace(np.min(old_ticks), np.max(old_ticks), len(xticks))
ax.set_xticks(new_ticks)
ax.set_xticklabels(xticks)
ax.set_yticks(new_ticks)
ax.set_yticklabels(yticks)



#delta Q

heat_map_delta_Q_dirs=[]
delta=4
for i in range(16):
    heat_map_delta_Q_dirs.append(main_dir+sec_dir+"/heat_map_Q"+"/heat_map_delta_q_d_i_%d_f_%d"%(i*delta,(i+1)*delta))


heat_map_delta_Q_temp=[]
for direction in heat_map_delta_Q_dirs:
    f=open(direction,"r")
    for line in f:
        temp_line=[]
        temp=line.strip().split()
        for elem in temp:
            temp_line.append(float(elem))
        heat_map_delta_Q_temp.append(temp_line)
    f.close()
heat_map_delta_Q=np.zeros((64,64))
for i in range(len(heat_map_delta_Q_temp)):
    for j in range(len(heat_map_delta_Q_temp)):
        heat_map_delta_Q[64-1-i][j]=heat_map_delta_Q_temp[i][j]

plt.figure() 
ax=sns.heatmap(heat_map_delta_Q)
plt.title("\u0394 Q")

old_ticks = ax.get_xticks()
new_ticks = np.linspace(np.min(old_ticks), np.max(old_ticks), len(xticks))
ax.set_xticks(new_ticks)
ax.set_xticklabels(xticks)
ax.set_yticks(new_ticks)
ax.set_yticklabels(yticks)


#delta Q times S
heat_map_delta_Q_times_S=np.zeros((64,64))

for i in range(64):
    for j in range(64):
        heat_map_delta_Q_times_S[i][j]=heat_map_S[j][i]*heat_map_delta_Q[i][j]
        
plt.figure()
ax=sns.heatmap(heat_map_delta_Q_times_S)
plt.title("\u0394 Q x S")

old_ticks = ax.get_xticks()
new_ticks = np.linspace(np.min(old_ticks), np.max(old_ticks), len(xticks))
ax.set_xticks(new_ticks)
ax.set_xticklabels(xticks)
ax.set_yticks(new_ticks)
ax.set_yticklabels(yticks)


#active links model times delta Q
heat_map_active_links_time_delta_Q=np.zeros((64,64))

for i in range(64):
    for j in range(64):
        heat_map_active_links_time_delta_Q[i][j]=heat_map_model[i][j]*heat_map_delta_Q[i][j]

plt.figure()
ax=sns.heatmap(heat_map_active_links_time_delta_Q,vmin=0.0,vmax=0.12)
plt.title("Active link desity (model) x \u0394 Q")

old_ticks = ax.get_xticks()
new_ticks = np.linspace(np.min(old_ticks), np.max(old_ticks), len(xticks))
ax.set_xticks(new_ticks)
ax.set_xticklabels(xticks)
ax.set_yticks(new_ticks)
ax.set_yticklabels(yticks)

#active links times delta Q
heat_map_active_links_time_delta_Q=np.zeros((64,64))

for i in range(64):
    for j in range(64):
        heat_map_active_links_time_delta_Q[i][j]=heat_map_active_links_density[i][j]*heat_map_delta_Q[i][j]

plt.figure()
ax=sns.heatmap(heat_map_active_links_time_delta_Q,vmin=0.0,vmax=0.12)
plt.title("Active link desity x \u0394 Q")

old_ticks = ax.get_xticks()
new_ticks = np.linspace(np.min(old_ticks), np.max(old_ticks), len(xticks))
ax.set_xticks(new_ticks)
ax.set_xticklabels(xticks)
ax.set_yticks(new_ticks)
ax.set_yticklabels(yticks)

plt.show()

