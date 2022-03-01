#pragma once

void Opt_Swap_Init_Dfs(int u)
{
	visted[u] = 1;
	for (int i = 0; i < Edge[u].size(); i++)
	{
		int v = Edge[u][i];
		if (!If_Selected[v] || visted[v]) continue;
		Cur_Father[v] = u;
		Opt_Swap_Init_Dfs(v);
	}
}

void Check_Belong_to_Subtree_DFS(int u, int belong)
{
	Belong_to_Subtree[u] = belong;
	for (int i = 0; i < Edge[u].size(); i++)
	{
		int v = Edge[u][i];
		if (If_Selected[v] && !Belong_to_Subtree[v])
			Check_Belong_to_Subtree_DFS(v, belong);
	}
}

bool Check_a_new_Tree(int u, int v)
{
	for (int i = 1; i <= Sub_Tree_Num; i++)
		Sub_Tree_ok[i] = 0;
	for (int i = 0; i < Edge[v].size(); i++)
	{
		int vi = Edge[v][i];
		if (!If_Selected[vi]) continue;
		Sub_Tree_ok[Belong_to_Subtree[vi]] = 1;
	}
	for (int i = 1; i <= Sub_Tree_Num; i++)
		if (Sub_Tree_ok[i] == 0)
			return false;
	return true;
}
/////////////////////////////////////////////////////////////
bool Check_a_new_Tree(int u, int fu, int v)
{
	for (int i = 1; i <= Sub_Tree_Num; i++)
		Sub_Tree_ok[i] = 0;
	for (int i = 0; i < Edge[v].size(); i++)
	{
		int vi = Edge[v][i];
		if (!If_Selected[vi]) continue;
		Sub_Tree_ok[Belong_to_Subtree[vi]] = 1;
	}
	for (int i = 1; i <= Sub_Tree_Num; i++)
		if (Sub_Tree_ok[i] == 0)
			return false;
	return true;
}

//////////////////////////////////////////////////////////////


int Get_SD()
{
	int SD = 0;

	for (int node = Root; node!=0; node=Cur_Solution_Chain[node].nxt)
	{
		int nodej;
		for (int j = 0; j < Edge[node].size(); j++)
		{
			nodej = Edge[node][j];
			if (If_Selected[nodej])
			{
				SD_for_Nodes[node]++;
			}
		}
		if (SD_for_Nodes[node] == 1)
			SD++;
	}
	return SD;
}

int Get_SD_After_Swap(int SD0, int u, int v)
{
	int ans = SD0;
	If_Selected[u] = false;
	for (int i = 0; i < Edge[u].size(); i++)
	{
		int ui = Edge[u][i];
		if (!If_Selected[ui]) continue;
		SD_for_Nodes[ui]--;
		if (SD_for_Nodes[ui] == 0)
			ans--;
		else if (SD_for_Nodes[ui] == 1)
			ans++;
	}

	int sdv = 0;
	for (int i = 0; i < Edge[v].size(); i++)
	{
		int vi = Edge[v][i];
		if (!If_Selected[vi]) continue;
		sdv++;
		if (SD_for_Nodes[vi] + 1 == 1)
			ans++;
		else if (SD_for_Nodes[vi] + 1 == 2)
			ans--;
	}
	if (sdv == 1)
		ans++;


	for (int i = 0; i < Edge[u].size(); i++)
	{
		int ui = Edge[u][i];
		if (!If_Selected[ui]) continue;
		SD_for_Nodes[ui]++;
	}

	If_Selected[u] = true;
	return ans;
}

void Ask_for_Cadidate_Nodes_to_Swap()
{
	Candidate_x_y_to_Swap[0].clear();
	Candidate_x_y_to_Swap[1].clear();
	//Candidate_x_y_to_Swap_SD.clear();

	for (int node = Root; node != 0; node = Cur_Solution_Chain[node].nxt)
	{
		SD_for_Nodes[node] = 0;
	}
	int SD0 = Get_SD();

	for (int u = Root; u != 0; u = Cur_Solution_Chain[u].nxt)
	{
		if (!If_Steiner_Nodes[u]) continue;

		for (int j = Root; j!=0; j= Cur_Solution_Chain[j].nxt)
		{
			Belong_to_Subtree[j] = 0;
		}

		If_Selected[u] = false;

		Sub_Tree_Num = 0;
		for (int node = Root; node !=0; node=Cur_Solution_Chain[node].nxt) {
			if (If_Selected[node] && !Belong_to_Subtree[node])
				Check_Belong_to_Subtree_DFS(node, ++Sub_Tree_Num);
		}


		for (int v = 1; v <= Nodes_Num; v++)
		{
			if (!If_Selected[v] && v != u && degree[v] >= Sub_Tree_Num)
			{
				bool If_a_new_Tree = Check_a_new_Tree(u, v);
				if (If_a_new_Tree)
				{
					int SD_for_Swap = Get_SD_After_Swap(SD0, u, v);
					if (SD_for_Swap <= SD0 + rand() % 5)
					{
						Candidate_x_y_to_Swap[0].push_back(u);
						Candidate_x_y_to_Swap[1].push_back(v);
						//Candidate_x_y_to_Swap_SD.push_back(SD0- SD_for_Swap);
					}
				}
			}
		}
		If_Selected[u] = true;
	}
}

int Get_max_Potential_nodes_for_Swap()
{
	int siz = Candidate_x_y_to_Swap_SD.size();
	Potential_for_Swap[0] = 0;
	for (int i = 1; i <= siz; i++)
		Potential_for_Swap[i] = exp(1.0*Candidate_x_y_to_Swap_SD[i-1]);
	for (int i = 1; i <= siz; i++)
		Potential_for_Swap[i] += Potential_for_Swap[i - 1];
	for (int i = 1; i <= siz; i++)
		Potential_for_Swap[i] = Potential_for_Swap[i] / Potential_for_Swap[siz - 1];
	Potential_for_Swap[siz] = 1.0;
	double P = 1.0*(rand() % 10000 + 1) / 10000;
	for (int i = 1; i <= siz; i++)
	{
		if (Potential_for_Swap[i - 1] < P && P <= Potential_for_Swap[i])
			return i-1;
	}
	return 0;
}




void Ask_for_Cadidate_2_1_Nodes_to_Swap()
{
	Candidate_x_y_z_to_Swap[0].clear();
	Candidate_x_y_z_to_Swap[1].clear();
	Candidate_x_y_z_to_Swap[2].clear();
/*
	for(int node=Root; node!=0;node=Cur_Solution_Chain[node].nxt)
		visted[node] = 0;
	Opt_Swap_Init_Dfs(Root);

	for (int i = 1; i <= Nodes_Num; i++)
	if (!If_Selected[i])
	{
		degree[i] = 0;
		for (int j = 0; j < Edge[i].size(); j++)
		{
			int jj = Edge[i][j];
			if (If_Selected[jj])
				degree[i]++;
		}
	}

	for (int u = Root; u!=0; u=Cur_Solution_Chain[u].nxt)
	{
		if (!If_Steiner_Nodes[u]) continue;

		If_Selected[u] = false;
		int fu = Cur_Father[u];
		if (!If_Steiner_Nodes[fu] || fu == 0)
		{
			If_Selected[u] = true;
			continue;
		}
		If_Selected[fu] = false;

		for (int node = Root; node != 0; node = Cur_Solution_Chain[node].nxt)
		{
			Belong_to_Subtree[node] = 0;
		}
		Sub_Tree_Num = 0;
		for (int node = Root; node != 0; node = Cur_Solution_Chain[node].nxt) {
			if (If_Selected[node] && !Belong_to_Subtree[node])
				Check_Belong_to_Subtree_DFS(node, ++Sub_Tree_Num);
		}

		for (int v = 1; v <= Nodes_Num; v++)
		{
			if (!If_Selected[v] && v != u && v != fu && degree[v] >= Sub_Tree_Num)
			{
				bool If_a_new_Tree = Check_a_new_Tree(u, fu, v);
				if (If_a_new_Tree)
				{
					Candidate_x_y_z_to_Swap[0].push_back(u);
					Candidate_x_y_z_to_Swap[1].push_back(fu);
					Candidate_x_y_z_to_Swap[2].push_back(v);
				}
			}
		}

		If_Selected[u] = true;
		If_Selected[fu] = true;

		/////////////////////////////////////////////////////////////////////
	}
*/
	
	for (int i = 1; i <= Nodes_Num; i++)
	if (!If_Selected[i])
	{
		degree[i] = 0;
		for (int j = 0; j < Edge[i].size(); j++)
		{
			int jj = Edge[i][j];
			if (If_Selected[jj])
				degree[i]++;
		}
	}

	for (int u = Root; u!=0; u=Cur_Solution_Chain[u].nxt)
	if(If_Steiner_Nodes[u])
	for(int i=0;i<Edge[u].size();i++)
	{
		int fu = Edge[u][i];
		if(!If_Selected[fu]||fu<u||!If_Steiner_Nodes[fu])
			continue;
		If_Selected[u] = false;
		If_Selected[fu] = false;

		for (int node = Root; node != 0; node = Cur_Solution_Chain[node].nxt)
		{
			Belong_to_Subtree[node] = 0;
		}
		Sub_Tree_Num = 0;
		for (int node = Root; node != 0; node = Cur_Solution_Chain[node].nxt) {
			if (If_Selected[node] && !Belong_to_Subtree[node])
				Check_Belong_to_Subtree_DFS(node, ++Sub_Tree_Num);
		}

		for (int v = 1; v <= Nodes_Num; v++)
		{
			if (!If_Selected[v] && v != u && v != fu && degree[v] >= Sub_Tree_Num)
			{
				bool If_a_new_Tree = Check_a_new_Tree(u, fu, v);
				if (If_a_new_Tree)
				{
					Candidate_x_y_z_to_Swap[0].push_back(u);
					Candidate_x_y_z_to_Swap[1].push_back(fu);
					Candidate_x_y_z_to_Swap[2].push_back(v);
				}
			}
		}

		If_Selected[u] = true;
		If_Selected[fu] = true;

		/////////////////////////////////////////////////////////////////////
	}
	
}

void Swap_Nodes(int node_to_delete, int node_to_add)
{
	If_Selected[node_to_delete] = false;
	If_Selected[node_to_add] = true;
	
	Cur_Solution_Chain[Cur_Solution_Chain[node_to_delete].pre].nxt = node_to_add;
	Cur_Solution_Chain[Cur_Solution_Chain[node_to_delete].nxt].pre = node_to_add;
	Cur_Solution_Chain[node_to_add].nxt = Cur_Solution_Chain[node_to_delete].nxt;
	Cur_Solution_Chain[node_to_add].pre = Cur_Solution_Chain[node_to_delete].pre;

	//Cur_Solution_Chain[node_to_delete].pre = Cur_Solution_Chain[node_to_delete].nxt = 0;
	
}

void Swap_Nodes_2_1(int node_to_delete1, int node_to_delete2, int node_to_add)
{
	If_Selected[node_to_delete1] = false;
	If_Selected[node_to_delete2] = false;

	Cur_Solution_Sum_of_Nodes -= 1;
	Cur_Solution_Chain[Cur_Solution_Chain[node_to_delete1].pre].nxt = Cur_Solution_Chain[node_to_delete1].nxt;
	Cur_Solution_Chain[Cur_Solution_Chain[node_to_delete1].nxt].pre = Cur_Solution_Chain[node_to_delete1].pre;
	Cur_Solution_Chain[Cur_Solution_Chain[node_to_delete2].pre].nxt = Cur_Solution_Chain[node_to_delete2].nxt;
	Cur_Solution_Chain[Cur_Solution_Chain[node_to_delete2].nxt].pre = Cur_Solution_Chain[node_to_delete2].pre;

	If_Selected[node_to_add] = true;
	Cur_Solution_Chain[node_to_add].pre = Root;
	Cur_Solution_Chain[node_to_add].nxt = Cur_Solution_Chain[Root].nxt;
	Cur_Solution_Chain[Cur_Solution_Chain[Root].nxt].pre = node_to_add;
	Cur_Solution_Chain[Root].nxt = node_to_add;
}
