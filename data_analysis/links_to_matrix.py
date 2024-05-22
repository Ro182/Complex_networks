import numpy as np


def links_to_matrix(links_file):
    file=open(links_file,"r")
    top=int(file.readline())

    links=[]
    for line in file:
        temp_line=line.strip().split()
        temp_links=[]
        for elem in temp_line:
            temp_links.append(int(elem))
        links.append(temp_links)
    matrix=np.zeros((top,top))
    for link in links:
        matrix[link[0]][link[1]]=1
        matrix[link[1]][link[0]]=1
    return matrix
