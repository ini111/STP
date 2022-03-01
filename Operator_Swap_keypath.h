#pragma once

queue<int>Inital_Voronoi_que;
int Voronoi_dis[Max_Nodes_Num + 5];
int Voronoi_path[Max_Nodes_Num + 5];
int Voronoi_belong[Max_Nodes_Num + 5];
void Inital_Voronoi_region()
{
	while (!Inital_Voronoi_que.empty())
		Inital_Voronoi_que.pop();

	for (int i = 1; i <= Nodes_Num; i++)
	{
		Voronoi_dis[i] = inf;
		Voronoi_path[i] = 0;
		Voronoi_belong[i] = 0;
	}
	for (int u = Root; u != 0; u = Cur_Solution_Chain[u].nxt)
	{
		Inital_Voronoi_que.push(u);
		Voronoi_dis[u] = 0;
		Voronoi_belong[u] = u;
	}
	while (!Inital_Voronoi_que.empty())
	{
		int u = Inital_Voronoi_que.front();
		Inital_Voronoi_que.pop();
		for (int i = 0; i < Edge[u].size(); i++)
		{
			int v = Edge[u][i];
			if (!Voronoi_belong[v])
			{
				Voronoi_belong[v] = Voronoi_belong[u];
				Voronoi_path[v] = u;
				Voronoi_dis[v] = Voronoi_dis[u] + 1;
				Inital_Voronoi_que.push(v);
			}
		}
	}
}

struct Node_Keyvertex {
	int v, dis;
};
vector<Node_Keyvertex>Tree_with_Keyvertex[Max_Nodes_Num + 5];

int Belong_keyvertex[Max_Nodes_Num + 5];
vector<int>Keyvertex;
bool If_Keyvertex[Max_Nodes_Num + 5];
vector<int>Now_Tree[Max_Nodes_Num + 5];


void Check_Inital_Tree_with_Keyvertex_dfs(int u,int pre)
{
	//cout << "~~~~" << u << " !!! " << pre << endl;
	visted[u] = 1;
	for (int i = 0; i < Edge[u].size(); i++)
	{
		int v = Edge[u][i];
		if (!If_Selected[v] || visted[v])
			continue;
		Now_Tree[u].push_back(v);
		Cur_Father[v] = u;
		Check_Inital_Tree_with_Keyvertex_dfs(v,u);
	}

	if (Now_Tree[u].size() > 1 || !If_Steiner_Nodes[u])
	{
		If_Keyvertex[u] = true;
		Keyvertex.push_back(u);
	}

}



Node_Keyvertex Inital_Tree_with_Keyvertex_dfs(int u,int pre)
{
	Node_Keyvertex res;

	if(If_Keyvertex[u])
	{
		Belong_keyvertex[u] = u;
		for (int i = 0; i < Now_Tree[u].size(); i++)
		{
			int v = Now_Tree[u][i];
			res = Inital_Tree_with_Keyvertex_dfs(v,u);
			Tree_with_Keyvertex[u].push_back(res);
		}
		res.dis = 1;
		res.v = u;
		return res;
	}
	else {
		Belong_keyvertex[u] = Belong_keyvertex[pre];

		int v = Now_Tree[u][0];
		res= Inital_Tree_with_Keyvertex_dfs(v,u);
		res.dis++;
		return res;
	}

}

struct Node_Keyvertex_lca {
	int v, dis, from;
};

Node_Keyvertex_lca Tree_with_Keyvertex_father[Max_Nodes_Num + 5][22];
int Tree_with_Keyvertex_deep[Max_Nodes_Num + 5];
void Tree_with_Keyvertex_dfs(int u)
{
	//cout << u << endl;
	for (int i = 0; i < Tree_with_Keyvertex[u].size(); i++)
	{
		int v = Tree_with_Keyvertex[u][i].v;
		Tree_with_Keyvertex_father[v][0].dis = Tree_with_Keyvertex[u][i].dis;
		Tree_with_Keyvertex_father[v][0].v = u;
		Tree_with_Keyvertex_father[v][0].from = v;
		Tree_with_Keyvertex_deep[v] = Tree_with_Keyvertex_deep[u] + 1;
		Tree_with_Keyvertex_dfs(v);
	}
}

void Inital_Tree_with_Keyvertex()
{
	Keyvertex.clear();
	for (int i = 0; i <= Nodes_Num; i++)
	{
		visted[i] = 0;
		If_Keyvertex[i] = false;
		Now_Tree[i].clear();
		Tree_with_Keyvertex[i].clear();
		Tree_with_Keyvertex_deep[i] = 0;
		for (int j = 0; j <= 20; j++)
			Tree_with_Keyvertex_father[i][j].v = 
			Tree_with_Keyvertex_father[i][j].dis = 
			Tree_with_Keyvertex_father[i][j].from = 0;
	}
	
	//cout << "<" << endl;
	Check_Inital_Tree_with_Keyvertex_dfs(Root,0);
	//cout << ">" << endl;
	//cout << Keyvertex.size() << endl;
	//for (int i = 0; i < Keyvertex.size(); i++)
	//	printf("keyvertex:%d\n", Keyvertex[i]);
	//cout << "<" << endl;
	Node_Keyvertex tmp = Inital_Tree_with_Keyvertex_dfs(Root,0);
	//cout << ">" << endl;

	Tree_with_Keyvertex_deep[Root] = 1;
	Tree_with_Keyvertex_dfs(Root);

	for (int j = 1; j <= 20; j++)
	{
		for (int i = 0; i < Keyvertex.size(); i++)
		{
			int u = Keyvertex[i];
			int v = Tree_with_Keyvertex_father[u][j - 1].v;
			//if (v == 0) break;
			int disu = Tree_with_Keyvertex_father[u][j - 1].dis;
			int disv = Tree_with_Keyvertex_father[v][j - 1].dis;
			
			Tree_with_Keyvertex_father[u][j].v = Tree_with_Keyvertex_father[v][j - 1].v;
			if (disv > disu)
			{
				Tree_with_Keyvertex_father[u][j].dis = disv;
				Tree_with_Keyvertex_father[u][j].from = Tree_with_Keyvertex_father[v][j - 1].from;
			}
			else {
				Tree_with_Keyvertex_father[u][j].dis = disu;
				Tree_with_Keyvertex_father[u][j].from = Tree_with_Keyvertex_father[u][j - 1].from;
			}
		}
	}

//	printf("****%d****%d****\n", Tree_with_Keyvertex_father[5][0].dis, Tree_with_Keyvertex_father[5][0].from);
}

void Ask_Lca_Tree_with_Keyvertex(int x, int y, int &node, int &dis)
{
	if (Tree_with_Keyvertex_deep[x] < Tree_with_Keyvertex_deep[y])
		swap(x, y);
	dis = 0;

	/*
	while(1)
	{
		int xx = Tree_with_Keyvertex_father[x][0].v;
		if (Tree_with_Keyvertex_deep[xx] >= Tree_with_Keyvertex_deep[y])
		{
			if (dis < Tree_with_Keyvertex_father[x][0].dis)
			{
				dis = Tree_with_Keyvertex_father[x][0].dis;
				node = Tree_with_Keyvertex_father[x][0].from;
			}
			x = xx;
		}
		else
			break;
	}

	while(x!=y)
	{
		int xx = Tree_with_Keyvertex_father[x][0].v;
		int yy = Tree_with_Keyvertex_father[y][0].v;
		
		if (dis < Tree_with_Keyvertex_father[x][0].dis)
		{
			dis = Tree_with_Keyvertex_father[x][0].dis;
			node = Tree_with_Keyvertex_father[x][0].from;
		}
		if (dis < Tree_with_Keyvertex_father[y][0].dis)
		{
			dis = Tree_with_Keyvertex_father[y][0].dis;
			node = Tree_with_Keyvertex_father[y][0].from;
		}
		x = xx;
		y = yy;
	}
	*/
	

	
	for (int i = 20; i >= 0; i--)
	{
		int xx = Tree_with_Keyvertex_father[x][i].v;
		if (Tree_with_Keyvertex_deep[xx] >= Tree_with_Keyvertex_deep[y]&&xx!=y)
		{
			if (dis < Tree_with_Keyvertex_father[x][i].dis)
			{
				dis = Tree_with_Keyvertex_father[x][i].dis;
				node = Tree_with_Keyvertex_father[x][i].from;
			}
			x = xx;
		}
	}
	if (Tree_with_Keyvertex_father[x][0].v == y)
		return;

	for (int i = 20; i >= 0; i--)
	{
		int xx = Tree_with_Keyvertex_father[x][i].v;
		int yy = Tree_with_Keyvertex_father[y][i].v;
		if (xx!=yy)
		{
			if (dis < Tree_with_Keyvertex_father[x][i].dis)
			{
				dis = Tree_with_Keyvertex_father[x][i].dis;
				node = Tree_with_Keyvertex_father[x][i].from;
			}
			if (dis < Tree_with_Keyvertex_father[y][i].dis)
			{
				dis = Tree_with_Keyvertex_father[y][i].dis;
				node = Tree_with_Keyvertex_father[y][i].from;
			}
			x = xx;
			y = yy;
		}
	}

	if (x != y)
	{
		if (dis < Tree_with_Keyvertex_father[x][0].dis)
		{
			dis = Tree_with_Keyvertex_father[x][0].dis;
			node = Tree_with_Keyvertex_father[x][0].from;
		}
		if (dis < Tree_with_Keyvertex_father[y][0].dis)
		{
			dis = Tree_with_Keyvertex_father[y][0].dis;
			node = Tree_with_Keyvertex_father[y][0].from;
		}
	}
	
}

vector<int>Candidate_v1_of_Path_to_Swap;
vector<int>Candidate_v2_of_Path_to_Swap;
vector<int>Candidate_node_of_KeyPath_to_Swap;


void Ask_for_Cadidate_Keypath_to_Swap()
{
	Candidate_v1_of_Path_to_Swap.clear();
	Candidate_v2_of_Path_to_Swap.clear();
	Candidate_node_of_KeyPath_to_Swap.clear();

	//cout << "haha" << endl;
	Inital_Voronoi_region();
	//cout << "haha" << endl;
	//cout << "haha" << endl;
	Inital_Tree_with_Keyvertex();
	//cout << "haha" << endl;

	//printf("now4  :: %d\n", Now_Tree[4].size());
	//for (int i = 1; i <= Nodes_Num; i++)
	//	printf("%d  %d\n", i, Belong_keyvertex[i]);

	for (int i = 0; i < Edges_Num; i++)
	{
		int v1 = Edge_x_y[i].x;
		int v2 = Edge_x_y[i].y;
		//if (If_Selected[v1] && If_Selected[v2])
		//	continue;
		//printf("edge %d  %d\n", v1, v2);
		int vv1 = Voronoi_belong[v1];
		int vv2 = Voronoi_belong[v2];
		//printf("vv1:%d  vv2:%d\n", vv1, vv2);
		if (Belong_keyvertex[vv1] == Belong_keyvertex[vv2]|| Belong_keyvertex[vv1]==0)
			continue;
		int x = Belong_keyvertex[vv1];
		int y = Belong_keyvertex[vv2];
		int node, dis;


		Ask_Lca_Tree_with_Keyvertex(x, y, node, dis);

		if (Voronoi_dis[v1] + Voronoi_dis[v2] + 1 < dis)
		{
			//printf("???  %d  %d   %d %d %d\n",If_Selected[v1],If_Selected[v2], Voronoi_dis[v1], Voronoi_dis[v2], dis);
			Candidate_v1_of_Path_to_Swap.push_back(v1);
			Candidate_v2_of_Path_to_Swap.push_back(v2);
			Candidate_node_of_KeyPath_to_Swap.push_back(node);
		}
	}

}

void Swap_KeyPath(int index)
{
	int node = Candidate_node_of_KeyPath_to_Swap[index];
	int v1 = Candidate_v1_of_Path_to_Swap[index];
	int v2 = Candidate_v2_of_Path_to_Swap[index];
	node = Cur_Father[node];
	while (!If_Keyvertex[node])
	{
		If_Selected[node] = false;
		
		Cur_Solution_Sum_of_Nodes--;
		Cur_Solution_Chain[Cur_Solution_Chain[node].pre].nxt = Cur_Solution_Chain[node].nxt;
		Cur_Solution_Chain[Cur_Solution_Chain[node].nxt].pre = Cur_Solution_Chain[node].pre;
		
		node = Cur_Father[node];
	}
	while (!If_Selected[v1])
	{
		If_Selected[v1] = true;
		
		Cur_Solution_Sum_of_Nodes++;
		Cur_Solution_Chain[v1].nxt = Cur_Solution_Chain[Root].nxt;
		Cur_Solution_Chain[v1].pre =Root;

		Cur_Solution_Chain[Root].nxt = v1;
		Cur_Solution_Chain[Cur_Solution_Chain[v1].nxt].pre = v1;
		
		v1 = Voronoi_path[v1];
	}

	while (!If_Selected[v2])
	{
		If_Selected[v2] = true;
		
		Cur_Solution_Sum_of_Nodes++;
		Cur_Solution_Chain[v2].nxt = Cur_Solution_Chain[Root].nxt;
		Cur_Solution_Chain[v2].pre = Root;

		Cur_Solution_Chain[Root].nxt = v2;
		Cur_Solution_Chain[Cur_Solution_Chain[v2].nxt].pre = v2;
		
		v2 = Voronoi_path[v2];
	}

	/*
	int prenode = Root;
	Cur_Solution_Sum_of_Nodes = 1;
	Cur_Solution_Chain[Root].pre = Cur_Solution_Chain[Root].nxt = 0;
	for (int i = 1; i <= Nodes_Num; i++)
	if (If_Selected[i] && i != Root)
	{
		Cur_Solution_Sum_of_Nodes++;
		Cur_Solution_Chain[i].nxt = Cur_Solution_Chain[prenode].nxt;
		Cur_Solution_Chain[i].pre = prenode;
		Cur_Solution_Chain[Cur_Solution_Chain[prenode].nxt].pre = i;
		Cur_Solution_Chain[prenode].nxt = i;
		
		prenode = i;
	}
	*/
}