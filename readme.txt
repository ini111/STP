1 reduction:有
2 inital solution
2.1 前 rand() % (Terminals_Nodes_Num / maxdegree(adjacent terminal nodes))个Steiner节点直接放入解集中
2.2 SPH
3 Local Search:
3.1 remove
3.2 swap2-1
3.3 swap1-1(接收交换的条件是SD<=SD0+rand()%5) 
3.4 Max_not_Improve_Num = max(Terminals_Nodes_Num,Edges_Num/5);
