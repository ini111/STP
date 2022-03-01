#pragma once

void Update_Instances()
{
	for (int i = 0; i <= origin_Nodes_Num; i++)
	{
		Edge[i].clear();
		Edge_pval[i].clear();
	}
	Edge_x_y.clear();
	new_Edge_X.clear();
	new_Edge_Y.clear();

	int x, y, edgenum = 0;

	for (int i = 0; i < origin_Edges_Num; i++)
	{
		x = origin_Edge_X[i]; y = origin_Edge_Y[i];
		if (x == 0) continue;
		if (!important_node[x] || !important_node[y]) continue;
		if (!If_Steiner_Nodes[x] && !important_terminal[x]) continue;
		if (!If_Steiner_Nodes[y] && !important_terminal[y]) continue;
		new_Edge_X.push_back(x);
		new_Edge_Y.push_back(y);
		Edge[x].push_back(y);
		Edge[y].push_back(x);
		Node_Edge NE;
		NE.x=x; NE.y=y;
		Edge_x_y.push_back(NE);
		Edge_pval[x].push_back(edgenum);
		Edge_pval[y].push_back(edgenum);
		edgenum++;
	}

	for (int i = 0; i < new_add_Edge_X.size(); i++)
	{
		x = new_add_Edge_X[i]; y = new_add_Edge_Y[i];
		if (x == 0) continue;
		if (!important_node[x] || !important_node[y]) continue;
		new_Edge_X.push_back(x);
		new_Edge_Y.push_back(y);

		Edge[x].push_back(y);
		Edge[y].push_back(x);
		Node_Edge NE;
		NE.x=x; NE.y=y;
		Edge_x_y.push_back(NE);
		Edge_pval[x].push_back(edgenum);
		Edge_pval[y].push_back(edgenum);
		edgenum++;
	}
	//printf("***(%d   %d)***\n", sizzz, sizzz2);
	Edges_Num = edgenum;
}

int num;//BCC���� 
int dfs_clock;
int stk[Max_Nodes_Num + 5], top;//ջ 
vector<int>bcc[Max_Nodes_Num + 5];
int tarjan(int u, int fa)
{
	int lowu = dfn[u] = ++dfs_clock;
	for (int i = 0; i < Edge[u].size(); i++)
	{
		int v = Edge[u][i];
		if (!dfn[v])
		{
			stk[++top] = v;//�������ĵ���ջ 
			int lowv = tarjan(v, u);
			lowu = min(lowu, lowv);
			if (lowv >= dfn[u])//�Ǹ���� 
			{
				num++;
				while (stk[top] != v)//�����ջֱ��Ŀ��� 
					bcc[num].push_back(stk[top--]);
				bcc[num].push_back(stk[top--]);//Ŀ����ջ 
				bcc[num].push_back(u);//��Ҫ���˽���ǰ�����bcc 
			}
		}
		else if (v != fa)
			lowu = min(lowu, dfn[v]);
	}
	return lowu;
}


void Inital_Graph_Tarjan()
{	
	for (int i = 0; i < min(2, Terminals_Nodes_Num); i++)
	{
		int x = Terminals_Nodes[i];
		for (int j = i+1; j < Terminals_Nodes_Num; j++)
		{
			int y = Terminals_Nodes[j];
			Edge[x].push_back(y);
			Edge[y].push_back(x);
		}
	}

	for (int i = 1; i <= Nodes_Num; i++)
	{
		dfn[i] = 0;
		low[i] = 0;
		bcc[i].clear();
	}
	dfs_clock = num = 0;

	for (int i = 1; i <= Nodes_Num; i++)//����n����tarjan 
	if (!dfn[i])
	{
		stk[top = 1] = i;
		tarjan(i, i);
	}

	int flag = 0;
	for (int i = 1; i <= num; i++)
	{
		int flagnum = 0;
		for (int j = 0; j < bcc[i].size(); j++)
			if (!If_Steiner_Nodes[bcc[i][j]])
				flagnum++;
		if (flagnum == Terminals_Nodes_Num)
		{
			flag = i;
			break;
		}
	}


	for (int i = 0; i < bcc[flag].size(); i++)
		important_node[bcc[flag][i]] = 2;

	for (int i = 1; i <= Nodes_Num; i++)
	{
		if (important_node[i] == 2)
			important_node[i] = 1;
		else important_node[i] = 0;
	}
	
	
	//cout << Nodes_Num<<"   "<<bcc[flag].size() << endl;

}


int Reduction_belong[Max_Nodes_Num + 5];
int Reduction_find_father(int x)
{
	return Reduction_belong[x] = (x == Reduction_belong[x] ? x : Reduction_find_father(Reduction_belong[x]));
}

void Joint_neibor_Terminals()
{
	
	for (int i = 0; i < Terminals_Nodes_Num; i++)
	{
		Reduction_belong[Terminals_Nodes[i]] = Terminals_Nodes[i];
	}

	for (int i = 0; i < Edges_Num; i++)
	{
		int	x = new_Edge_X[i], y = new_Edge_Y[i];
		if (!If_Steiner_Nodes[x] && !If_Steiner_Nodes[y])
		{
			int fx = Reduction_find_father(x), fy = Reduction_find_father(y);
			if (fx != fy)
				Reduction_belong[fx] = fy;
		}
	}

	for (int ii = 0; ii < Terminals_Nodes_Num; ii++)
	{
		int u = Terminals_Nodes[ii];
		int fu = Reduction_belong[u] = Reduction_find_father(u);
		if (u == fu) continue;

		for (int i = 0; i < Edge[u].size(); i++)
		if (If_Steiner_Nodes[Edge[u][i]])
		{
			int flag = 1;
			for (int j = 0; j < Edge[fu].size(); j++)
			{
				if (Edge[u][i] == Edge[fu][j])
				{
					flag = 0;
					break;
				}
			}
			if (flag)
			{
				Edge[fu].push_back(Edge[u][i]);
				new_add_Edge_X.push_back(fu);
				new_add_Edge_Y.push_back(Edge[u][i]);
			}
		}
		important_terminal[u] = 0;
		new_add_Edge_X.push_back(fu);
		new_add_Edge_Y.push_back(u);
	}
}


void Reduction_Tarjan(int i, int Father)
{
	father[i] = Father;
	dfn[i] = low[i] = tim++;
	for (int j = 0; j < Edge[i].size(); ++j)
	{
		int k = Edge[i][j];
		if (!important_node[k]) continue;
		if (dfn[k] == -1)
		{
			Reduction_Tarjan(k, i);
			low[i] = min(low[i], low[k]);
		}
		else if (Father != k)
			low[i] = min(low[i], dfn[k]);
	}
}
void Reduction_count()
{
	int rootson = 0;
	for (int i = 1; i <= Nodes_Num; i++)
		if (important_node[i])
		{
			dfn[i] = -1;
			father[i] = 0;
			low[i] = -1;
			is_cut[i] = false;
		}

	tim = 0;
	Reduction_Tarjan(Terminals_Nodes[0], 0);
	for (int i = 1; i <= Nodes_Num; i++)
		if (important_node[i])
		{
			if (i == Terminals_Nodes[0]) continue;
			int v = father[i];
			if (v == Terminals_Nodes[0])
				rootson++;
			else {
				if (low[i] >= dfn[v])
					is_cut[v] = true;
			}
		}
	if (rootson > 1)
		is_cut[Terminals_Nodes[0]] = true;
}

bool Check_Cut_Edges_with_2_Steiners_as_Endpoint(int x, int y)
{
	bool flag = false;
	for (int j = 0; j < Edge[x].size(); j++)
	{
		int v = Edge[x][j];
		if (If_Steiner_Nodes[v]) continue;
		visted[v] = 1;
	}

	for (int j = 0; j < Edge[y].size(); j++)
	{
		int v = Edge[y][j];
		if (If_Steiner_Nodes[v]) continue;
		if (visted[v])
		{
			flag = true;
			break;
		}
	}
	for (int j = 0; j < Edge[x].size(); j++)
	{
		int v = Edge[x][j];
		if (If_Steiner_Nodes[v]) continue;
		visted[v] = 0;
	}
	return flag;
}
void Cut_Edges_with_2_Steiners_as_Endpoint()
{
	int x, y;
	for (int i = 1; i <= Nodes_Num; i++)
		visted[i] = 0;
	for (int i = 0; i < origin_Edges_Num; i++)
	{
		x = origin_Edge_X[i]; y = origin_Edge_Y[i];
		if (x == 0) continue;
		if (!important_node[x] || !important_node[y]) continue;
		if (!If_Steiner_Nodes[x] && !important_terminal[x]) continue;
		if (!If_Steiner_Nodes[y] && !important_terminal[y]) continue;

		if (!If_Steiner_Nodes[x] || !If_Steiner_Nodes[y]) continue;

		
		if (Check_Cut_Edges_with_2_Steiners_as_Endpoint(x,y))
		{
			origin_Edge_X[i] = 0;
			origin_Edge_Y[i] = 0;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

queue<int>Voronoi_diagram_que;
int Voronoi_diagram_dis[Max_Nodes_Num + 5];
int Voronoi_diagram_path[Max_Nodes_Num + 5];
int Voronoi_diagram_belong[Max_Nodes_Num + 5];
void Inital_Voronoi_diagram()
{
	while (!Voronoi_diagram_que.empty())
		Voronoi_diagram_que.pop();

	for (int i = 1; i <= Nodes_Num; i++)
	{
		Voronoi_diagram_dis[i] = inf;
		Voronoi_diagram_path[i] = 0;
		Voronoi_diagram_belong[i] = 0;
	}
	for (int i = 1; i <= Nodes_Num; i++)
		if (degree[i] > 2 || !If_Steiner_Nodes[i])
		{
			Voronoi_diagram_que.push(i);
			Voronoi_diagram_dis[i] = 0;
			Voronoi_diagram_belong[i] = i;
		}
	while (!Voronoi_diagram_que.empty())
	{
		int u = Voronoi_diagram_que.front();
		Voronoi_diagram_que.pop();
		for (int i = 0; i < Edge[u].size(); i++)
		{
			int v = Edge[u][i];
			if (!Voronoi_diagram_belong[v])
			{
				Voronoi_diagram_belong[v] = Voronoi_diagram_belong[u];
				Voronoi_diagram_path[v] = u;
				Voronoi_diagram_dis[v] = Voronoi_diagram_dis[u] + 1;
				Voronoi_diagram_que.push(v);
			}
		}
	}
}


queue<int>min_Distance_que;
int min_Distance_dis[Max_space + 5][Max_space + 5];
void Inital_min_Distance()
{
	while (!min_Distance_que.empty())
		min_Distance_que.pop();
	for (int i = 1; i <= Nodes_Num; i++)
	{
		for (int j = 1; j <= Nodes_Num; j++)
			min_Distance_dis[i][j] = inf;
		min_Distance_dis[i][i] = 0;
	}
	for (int i = 0; i < Edges_Num; i++)
	{
		int x = new_Edge_X[i];
		int y = new_Edge_Y[i];
		min_Distance_dis[x][y] = min_Distance_dis[y][x] = 1;
	}
	for (int k = 1; k <= Nodes_Num; k++)
		for (int i = 1; i <= Nodes_Num; i++)
			for (int j = 1; j <= Nodes_Num; j++)
				min_Distance_dis[i][j] = min(min_Distance_dis[i][j], min_Distance_dis[i][k] + min_Distance_dis[k][j]);
	
}
int min_Distance(int start_vectex, int end_vertex)
{
	return min_Distance_dis[start_vectex][end_vertex];
}

void Delete_Path(int start_vectex, int end_vertex)
{
	int now_vertex = start_vectex;
	while (now_vertex != end_vertex)
	{
		important_node[now_vertex] = 0;
		now_vertex = Voronoi_diagram_path[now_vertex];
	}
}

int degreenum[Max_Nodes_Num + 5];
int Inital_Graph_Delete_Path()
{
	int flag = 0;
	//int degree2num = 0;
	//for (int i = 0; i <= Nodes_Num; i++)
	//	degreenum[i] = 0;
	for (int i = 1; i <= Nodes_Num; i++)
	{
		degree[i] = Edge[i].size();
		//	printf("d[%d]:%d\n", i, degree[i]);
			//degreenum[degree[i]]++;
	}

	//for (int i = 1; i <= 6000; i++)
	//	if (degreenum[i] > 0)
	//		printf("degree %d nodes num:%d\n", i, degreenum[i]);


	Inital_Voronoi_diagram();

	//for (int i = 1; i <= Nodes_Num; i++)
	//{
	//	printf("%d:  belong:%d   path:%d   dis:%d\n", i, Voronoi_diagram_belong[i], Voronoi_diagram_path[i], Voronoi_diagram_dis[i]);
	//}


	for (int i = 0; i < Edges_Num; i++)
	{
		int	x = new_Edge_X[i], y = new_Edge_Y[i];
		if (!important_node[x] || !important_node[y]) continue;
		if (!If_Steiner_Nodes[x] && !If_Steiner_Nodes[y]) continue;
		if (Voronoi_diagram_belong[x] == Voronoi_diagram_belong[y]) continue;
		if (Voronoi_diagram_dis[x] + Voronoi_diagram_dis[y] + 1 > min_Distance(Voronoi_diagram_belong[x], Voronoi_diagram_belong[y]))
		{
			//printf("edge(%d,%d)\n", x, y);
			flag = 1;
			Delete_Path(x, Voronoi_diagram_belong[x]);
			Delete_Path(y, Voronoi_diagram_belong[y]);
			degree[Voronoi_diagram_belong[x]]--;
			degree[Voronoi_diagram_belong[y]]--;
		}
	}
	return flag;
}



void Reduction(int &presolved_nodes, int &presolved_edges,int &presolved_terminals,int &cutnodes)///Nodes_Num,Edge[],Terminal nodes,Ndges_Num
{
	new_add_Edge_X.clear();
	new_add_Edge_Y.clear();
	for (int i = 1; i <= Nodes_Num; i++)
	{
		important_node[i] = 1;
		if (!If_Steiner_Nodes[i])
			important_terminal[i] = 1;
		else important_terminal[i] = 0;
	}

	Update_Instances();
	
	Joint_neibor_Terminals();

	Update_Instances();

	Cut_Edges_with_2_Steiners_as_Endpoint();

	Update_Instances();

	Inital_Graph_Tarjan();
	
	Update_Instances();
	//////////////////////////////
	if (Nodes_Num < Max_space)
	{
		Inital_min_Distance();

		for (int i = 1; i <= 10; i++)
		{
			if (!Inital_Graph_Delete_Path())
				break;
			Update_Instances();
		}

		Update_Instances();
	}
	
	

	//for (int i = 1; i <= Nodes_Num; i++)
	//	if (important_node[i])
	//		printf("****%d\n", i);
	
	
	Reduction_count();
	
	int numcut = 0;
	for(int i=1;i<=Nodes_Num;i++)
	if (important_node[i]&&If_Steiner_Nodes[i])
	{
		if (is_cut[i]){
			numcut++;
		}
	}
	//printf("*cutcutcutcut:%d\n", numcut);
	cutnodes = numcut;

	Update_Instances();
	

	presolved_nodes = Nodes_Num, presolved_edges = Edges_Num, presolved_terminals = Terminals_Nodes_Num;
//	printf("\n(%d %d)\n", presolved_nodes, presolved_edges);
	for (int i = 1; i <= Nodes_Num; i++)
	{
		if (!important_node[i])
			presolved_nodes--;
		if (!If_Steiner_Nodes[i] && !important_terminal[i])
		{
			presolved_nodes--;
			presolved_terminals--;
		}
	}
	//printf("\n(%d %d)\n", presolved_nodes, presolved_edges); 
	presolved_edges -= (Terminals_Nodes_Num - presolved_terminals);

}


