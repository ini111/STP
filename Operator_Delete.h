#pragma once

int Delete_book[Max_Nodes_Num + 5];
void Delete_DFS(int u)
{
	Delete_book[u] = 1;
	if (!If_Steiner_Nodes[u])
		Delete_book[u] = 2;
	for (int i = 0; i < Edge[u].size(); i++)
	{
		int v = Edge[u][i];
		if (Delete_book[v] > 0) continue;
		Delete_DFS(v);
		if (Delete_book[v] == 2)
			Delete_book[u] = 2;
	}
}

bool Check_cut_leaves()
{
	for (int u = Root; u != 0; u = Cur_Solution_Chain[u].nxt)
		Delete_book[u] = 0;
	Delete_DFS(Root);
	for (int u = Root; u!=0 ; u=Cur_Solution_Chain[u].nxt)
	{
		if (Delete_book[u] == 1)
			Candidate_Nodes_to_Delete.push_back(u);
	}
	if (Candidate_Nodes_to_Delete.size())
		return true;
	return false;
}


void Tarjan(int i, int Father)
{
	father[i] = Father;
	dfn[i] = low[i] = tim++;
	for (int j = 0; j < Edge[i].size(); ++j)
	{
		int k = Edge[i][j];
		if (!If_Selected[k]) continue;
		if (dfn[k] == -1)
		{
			Tarjan(k, i);
			low[i] = min(low[i], low[k]);
		}
		else if (Father != k)
			low[i] = min(low[i], dfn[k]);
	}
}
void count()
{
	int rootson = 0;
	for (int i = Root; i!=0; i=Cur_Solution_Chain[i].nxt)
	{
		dfn[i] = -1;
		father[i] = 0;
		low[i] = -1;
		is_cut[i] = false;
	}

	tim = 0;
	Tarjan(Root, 0);
	for (int i = Root; i != 0; i = Cur_Solution_Chain[i].nxt)
	{
		if (i == Root) continue;
		int v = father[i];
		if (v == Root)
			rootson++;
		else {
			if (low[i] >= dfn[v])
				is_cut[v] = true;
		}
	}
	if (rootson > 1)
		is_cut[Root] = true;
}
void Ask_for_Cadidate_Nodes_to_Delete_Tarjan()
{
	Candidate_Nodes_to_Delete.clear();
	count();

	for (int u = Root; u!=0; u=Cur_Solution_Chain[u].nxt)
	{
		if (!If_Steiner_Nodes[u]) continue;

		if(!is_cut[u]){
			Candidate_Nodes_to_Delete.push_back(u);
		}
	}
}

void Delete_Node_from_Cur_Tree(int node_to_delete)
{
	If_Selected[node_to_delete] = false;
	Cur_Solution_Sum_of_Nodes--;
	Cur_Solution_Chain[Cur_Solution_Chain[node_to_delete].pre].nxt = Cur_Solution_Chain[node_to_delete].nxt;
	Cur_Solution_Chain[Cur_Solution_Chain[node_to_delete].nxt].pre = Cur_Solution_Chain[node_to_delete].pre;
}