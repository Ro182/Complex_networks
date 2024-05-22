import numpy as np
import matplotlib.pyplot as plt


#mean field evolution of states over dynamics

times=1100

n_states=40

runs=10
nodes=400

join=np.zeros((n_states,times))
stds=[]
main_dir=r"/Users/ro/Documents/Tesis_BSc/sc/final/states_evolution"
max_evol=[]

for run in range (runs):
    states=[]
    matrix_evol=np.zeros((n_states,times+1))
    f=open(main_dir+"/0_%d"%(run))
    line=f.readline().strip().split()
    counter=0
    for elem in line:
        if elem not in states:
            states.append(elem)
            matrix_evol[counter][0]=elem
            counter+=1
    
    
    for i in range(times):
        f=open(main_dir+"/%d_%d"%(i,run))
        line=f.readline().strip().split()
        
        for elem in line:
            for j in range(len(matrix_evol)):
                
                if (matrix_evol[j][0]==float(elem)):
                   
                    matrix_evol[j][i+1]+=1/nodes
    
    copy=sorted(matrix_evol, key=lambda a_entry: a_entry[-1]) 
    
    plt.plot(copy[-1])
    max_evol.append(copy[-1])

    for j in range(len(copy)):
        for i in range(1,len(copy[-1])):
            join[j][i-1]+=copy[j][i]/runs 



      
stds=[]
for i in range(1,len(max_evol[0])):
    temp=[]
    for j in range(len(max_evol)):
        temp.append(max_evol[j][i])
    stds.append(np.std(temp))
        
    

plt.figure()
xs=np.linspace(0,1100,1100)
stdsf=[]
xsf=[]
ysf=[]
for i in range(len(join[-1])):
    if (i%10==0):
        xsf.append(xs[i])
        ysf.append(join[-1][i])
        stdsf.append(stds[i])
#plt.errorbar(xs,join[-1],yerr=stds,alpha=0.10,ecolor='black',capsize=5)
print(ysf)
f=open("states_evolution","w")
for elem in ysf:
    f.write("%f "%(elem))
    
plt.errorbar(xsf,ysf,yerr=stdsf,alpha=0.50,ecolor='black',capsize=5)
plt.xlabel("Time step")

plt.ylabel("Percentage of Nodes")
plt.title("Percentage of Nodes with the most adopted state")
plt.figure()
sum_join=np.zeros(times)
for i in range(len(join)-1):
    plt.plot(join[i])
    for j in range(len(join[i])):
        sum_join[j]+=join[i][j]
plt.figure()
plt.plot(sum_join)
plt.show()
