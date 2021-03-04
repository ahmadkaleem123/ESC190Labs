import numpy as np

class Node:
    def __init__(self, name):
        self.name = name
        self.connections = []
        self.visited = False


def connect(node1, node2, weight):
    node1.connections.append({"node": node2, "weight": weight})
    node2.connections.append({"node": node1, "weight": weight})


def BFS(node):
    q = [node]
    node.visited = True
    while len(q) > 0:
        cur = q.pop(0) # remove q[0] from q and put it in cur
        #print(cur.name)
       # print(cur.visited)
        for con in cur.connections:
           #print(con["node"].visited)
            if not con["node"].visited:
                q.append(con["node"])
                con["node"].visited = True


################################################################################

def get_all_nodes(node):
    '''Return a list of the nodes in the graph of nodes connected to node
    (N.B., the nodes can be indirectly connected as well)'''
    all_nodes = []
    q = [node]
    node.visited = True
    while len(q) > 0:
        cur = q.pop(0) # remove q[0] from q and put it in cur
        all_nodes.append(cur)
        #print(cur.name)
        for con in cur.connections:
            if not con["node"].visited:
                q.append(con["node"])
                con["node"].visited = True
    return all_nodes
    
################################################################################

def unvisit_all(node):  #Assume that they are all visited
    '''Change all n.visited to False in all the nodes in the graph of nodes
    connected to node. Use BFS to find all the nodes'''


    
    
    q = [node]
    node.visited = True
    while len(q) > 0:
        cur = q.pop(0) # remove q[0] from q and put it in cur
    
        for con in cur.connections:
            #print("in for loop")
            if  con["node"].visited:
                q.append(con["node"])
                con["node"].visited = False

    
  
        
    
    


###############################################################################

def DFS_rec(node):
    '''Print out the names of all nodes connected to node using a
    recursive version of DFS'''
    node.visited = True
    print(node.name)
    for con in node.connections:
        if not con["node"].visited:
            DFS_rec(con["node"])



################################################################################

def DFS_nonrec(node):
    '''Print out the names of all nodes connected to node using a non-recursive
    version of DFS. Make it so that the nodes are printed in the same order
    as in DFS_rec'''
    s = [node]
    node.visited = True
    while len(s) > 0:
        cur = s.pop()
        print(cur.name) 
      
        for con in cur.connections:
           
            if not con["node"].visited:
                s.append(con["node"])
                con["node"].visited = True
    
 


################################################################################
#
# OPTIONAL
#

def dijsktra_slowish(node):
    '''Implement Dijkstra's algorithm as discussed in class (i.e., you don't
    need to use a priority queue'''
    S = [node]
    d = {node.name: 0}

    unexplored = get_all_nodes(node)
    unexplored.remove(node)

    while len(unexplored) > 0:
        mindist = -1#d[n] + neighbours[0].connections["weight"]
        minNode = unexplored[0]
        for n in S:
           
            neighbours = n.connections
            
            for x in range(len(neighbours)):
                if neighbours[x]["node"] in unexplored:
                    if mindist == -1:
                        mindist = d[n.name] + neighbours[x]["weight"]
                        minNode = neighbours[x]["node"]
                    else:
                        if (d[n.name] + neighbours[x]["weight"] < mindist):
                            mindist = d[n.name] + neighbours[x]["weight"]
                            minNode = neighbours[x]["node"]
        S.append(minNode)
        d[minNode.name] = mindist
        unexplored.remove(minNode)
            

            
            

    return d




if __name__ == '__main__':
    TO = Node("TO")
    NYC = Node("NYC")
    DC = Node("DC")
    CDMX = Node("CDMX")
    SF = Node("SF")
    MON = Node("MON")

    connect(TO, NYC, 3)
    connect(TO, SF, 6)
    connect(TO, CDMX, 2)
    connect(NYC, DC, 2)
    connect(SF, DC, 5)
    connect(CDMX, DC, 2)
    connect(SF, MON, 7)


    #L = get_all_nodes(TO)
    #for i in L:
    #    print(i.name)
    #    i.visited=False
        
    #print(L[1].visited)
    #BFS(CDMX)
    #unvisit_all(TO)
   # print(TO.visited)
   # print(NYC.visited)
   # print(SF.visited)
   # print(CDMX.visited)
   # print(DC.visited)
    #DFS_nonrec(DC)
    #DFS_rec(TO)
    # unvisit_all(TO)
    # DFS(TO)
    print(dijsktra_slowish(DC))