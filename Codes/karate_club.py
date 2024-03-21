import networkx as nx
import community as community_louvain
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import matplotlib
import numpy as np
import matplotlib._color_data as mcd

""""This code plots a representation of the karate club social network"""

# Here we import the graph of the social network and make a partition
G=nx.karate_club_graph()
partition=community_louvain.best_partition(G)
pos = nx.fruchterman_reingold_layout(G,iterations=100)

#This part chose a color map for the plot
cmap = cm.get_cmap('viridis', max(partition.values()) + 1)


plt.figure()

node_colors=list(partition.values())


# n contains the indexes of each node
# l contains to which of the two main nodes, the other nodes are connected to

n=[]
l=[]


# c contains the colors to be use for the plotting


c=["mediumturquoise","yellow","lawngreen","lightgray"]

for i in range(len(G)):
    n.append(i)
    G.add_node(i,color=c[node_colors[i]])
    if(G.nodes[i]['club']=='Mr. Hi'):
        G.add_node(i,s="s")
        l.append("Mr. Hi")
        
    else:
        G.add_node(i,s="o")
        l.append("John A")
        
labels=dict(zip(n,l))


nodeShapes = set((aShape[1]["s"] for aShape in G.nodes(data = True)))


#For each node class...
for aShape in nodeShapes:
    nodes= [sNode[0] for sNode in filter(lambda x: x[1]["s"]==aShape,G.nodes(data = True))]
    colors=[sNode[1]['color'] for sNode in filter(lambda x: x[1]["s"]==aShape,G.nodes(data = True))]

    #...filter and draw the subset of nodes with the same symbol in the positions that are now known through the use of the layout.
    nodes=nx.draw_networkx_nodes(G,pos,node_shape = aShape, nodelist =nodes,node_color=list(colors),node_size=350, alpha=0.8)
    nodes.set_edgecolor('black')

#Finally, draw the edges between the nodes
nx.draw_networkx_edges(G,pos,alpha=0.5)

nx.draw_networkx_labels(G,pos)
plt.box(False)
plt.show()