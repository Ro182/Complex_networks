import numpy as np
import matplotlib.pyplot as plt


def model_MD(r,d,N,G,k,time_max,U,n0=0):
    #d is the probability of desconecting from someone with the same state
    # r is the probability for reconecting to someone with the same state
    print("Model_MD")

    #n0 is the initial density of active links 
    
    total_links=k*N/2
    
    if (n0==0):
        n0=k*(N/2)*(G-1)/G
        n0/=total_links
    
    

    
    def formula(r,d,t,n0):
        # is the solution obtained for the evolution of active links
        return n0*np.exp(-2*U*t/N)
        
    ts=np.linspace(0,time_max,1000)
    densities=[]
    
    #here we obtaine the densities
    for t in ts:
        densities.append(formula(r,d,t,n0))
    
    #this part is the plot 
    plt.plot(ts,densities,"r",label="Model")

    plt.xlabel("Time",fontweight='bold')
    plt.ylabel("Density",fontweight='bold')
    
    #this part prints the values of interes 
    print("Total links =%f"%(total_links))
    print("Initial active links= %f"%(densities[0]*total_links))
    print("Final active links= %f"%(densities[-1]*total_links))
    print("Initial density of active links %f"%(n0))
    print("Final density of active links expected= %f"%((d-d*r)/(d+r-2*d*r)))
    print("Final density of active links obtained= %f"%(densities[-1]))


