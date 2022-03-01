#pragma once

void Get_Random_array(int array_length)
{
	for (int i = 0; i < array_length; i++) {
		Random_array[i] = i;
	}
	int j;
	for (int i = array_length - 1; i >= 0; i--)
	{
		j = rand() % (i + 1);
		swap(Random_array[i], Random_array[j]);
	}
}


void Store_Best_Solution()
{
	Best_Solution.Sum_of_Nodes = Cur_Solution_Sum_of_Nodes;
	Best_Solution.Nodes.clear();
	int node = Root;
	for (int i = 0; i < Cur_Solution_Sum_of_Nodes; i++) {
		Best_Solution.Nodes.push_back(node);
		node = Cur_Solution_Chain[node].nxt;
	}
}

bool Outputedgecmp(const Node_Edge a, const Node_Edge b)
{
	if (a.x == b.x) return a.y < b.y;
	return a.x < b.x;
}

void Check_Best_Solution_DFS(int u)
{
	Check_Best_Solution_Num++;
	Check_Best_Solution_visted[u] = 1;
	for (int i = 0; i < origin_Edge[u].size(); i++)
	{
		int v = origin_Edge[u][i];
		if (Check_Best_Solution_selected[v] && !Check_Best_Solution_visted[v])
		{
			Node_Edge edge;
			edge.x = min(u, v);
			edge.y = max(u, v);
			Outputedge.push_back(edge);
			Check_Best_Solution_DFS(v);
		}
	}
}

int Check_Best_Solution()
{
	Outputedge.clear();
	for (int i = 1; i <= Nodes_Num; i++){
		Check_Best_Solution_visted[i] = Check_Best_Solution_selected[i] = false;
	}
	int numofTeminals=0;
	for (int i = 0; i < Best_Solution.Sum_of_Nodes; i++) {
		Check_Best_Solution_selected[Best_Solution.Nodes[i]] = 1;
		if (!If_Steiner_Nodes[Best_Solution.Nodes[i]])
			numofTeminals++;
	}
	if (numofTeminals != Terminals_Nodes_Num)
		return -1;
	Check_Best_Solution_Num = 0;
	Check_Best_Solution_DFS(Best_Solution.Nodes[0]);
	if (Check_Best_Solution_Num == Best_Solution.Sum_of_Nodes)
		return 1;
	return 0;
}

void Check_Cur_Solution_DFS(int u)
{
	Check_Cur_Solution_Num++;
	Check_Cur_Solution_visted[u] = 1;
	for (int i = 0; i < origin_Edge[u].size(); i++)
	{
		int v = origin_Edge[u][i];
		if (Check_Cur_Solution_selected[v] && !Check_Cur_Solution_visted[v])
			Check_Cur_Solution_DFS(v);
	}
}

int Check_Cur_Solution()
{
	for (int i = 1; i <= Nodes_Num; i++) {
		Check_Cur_Solution_visted[i] = Check_Cur_Solution_selected[i] = false;
	}
	int numofTeminals = 0;
	int node = Root;
	for (int i = 0; i < Cur_Solution_Sum_of_Nodes; i++) {
		Check_Cur_Solution_selected[node] = 1;
		if (!If_Steiner_Nodes[node])
			numofTeminals++;
		node = Cur_Solution_Chain[node].nxt;
	}
	if (numofTeminals != Terminals_Nodes_Num)
		return -1;
	Check_Cur_Solution_Num = 0;
	Check_Cur_Solution_DFS(Root);
	if (Check_Cur_Solution_Num == Cur_Solution_Sum_of_Nodes)
		return 1;
	return 0;
}
