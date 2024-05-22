# Complex_networks
This is the code used for my thesis project

To compile it you need to edit the Makefile to include the location of the lgsl library
make-clean erases all the outputs


In the code you need to edit:
  Code:
    Max_threads -> Number of threads used for running the code in parallel
  Inputs:
    N -> Size of network 
    time -> Number of iteration time steps
    k -> Average number of conections per node
    G -> Number of possible states in the system G<N
    runs -> Number of times to run the code with the same inputs. In order to later have statistic about the outputs.
    d -> Probability of discconection between any given two nodes
    r -> Probability of reconection between any given two nodes
    U -> Threshold value to change node state
    Pr -> Probabiltity for rewiring dynamics
    Pc -> Probability for states change dynamics

  Tesis link: https://repositorio.yachaytech.edu.ec/handle/123456789/313 


Rhe coevolutionary dynamics of the system consists of iterating the following steps:
   1. Choose a node i such that k_i > 0.
   2. With probability Pr apply rewiring process (d,r): (i) select randomly a neighbor j ∈ ν_i ; if g_j = g_i with
      probability d break the link between i and j; if g_j , g_i with probability (1 − d) break the link between i and j.
      (ii) select at random a node l < ν_i; if g_l = g_i, with probability r_c connect i and l; if gl , gi , with probability (1 − r) connect i and l.
   4. With probability Pc = 1 − Pr , choose randomly a node j ∈ ν_i . If If |g_i − g_j| ≤ U, set g_i = g_j. If |g_i − g_j| > U, nothing happens.
      
Step 2 describes the adaptive rewiring process (d,r) that allows the acquisition of new connections. Step 3
specifies the coupling between the processes of rewiring and node dynamics according to the relation Pc = 1 − Pr;
the process of node state change is such that the states of the connected nodes become similar if the threshold
condition is fulfilled. Different coevolution models with this node dynamics can be investigated by considering
different rewiring parameters (d,r) or different coupling functions Pc = f(Pr).
Note that this type of node dynamics based on imitation or copying is absorbing. This means that the number
of states existing in the system decrease in time. A stationary state is reached in a finite time for a finite system.
The total number of links in our network and the total number of possible opinions G are fixed. In the limit of
large system size, the model thus has four parameters: the mean degree hki , the mean number of nodes holding a
particular opinion N/G, the rewiring probability Pr , and the threshold value U. Although the interaction threshold
U is an integer number, it is convenient to express this quantity normalized respect to the number of possible
options G . Then, we define the normalized threshold for interaction as u ≡ U/G, such that u ∈ (0, 1]. We shall fix
<k> and N/G and study the collective behavior of the system in terms of the parameters Pr and u.



Python scripts:
    Main:
	    -heat_map_active_links_density_simulation:Makes the heatmap of active links density for a given (r,d)
        -heat_map_delta_q:Makes de heatmap of delta modularity for a given (r,d)
        -heat_map_S:makes a heat map of S(r,d)
	    -heat_map_MD: Makes a heatmap of S(U,Pr)
    Others:
	    -active_links_denity: Return the density of active links given (file_matrix,file_states)
	    -modularity:    retunrs the modularity of a network given by community louvain
	    -model:         Plots the time evolution of the desity of active links according to the model obtained for a given (r,d)
	    -largest_cc:    Returns the largest connected component in a network 
	    -plot_U_vs_Pr:  Makes a plot of U vs Pr for a given U
	    -reading:       Plots the time evolution of the density of active links of the model and the simulation for a given (r,d)
