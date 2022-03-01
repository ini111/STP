
int pval[Max_Edge_Num + 5];
queue<int>Inital_Solution_que;
int Inital_Solution_que_book[Max_Nodes_Num + 5];
int Inital_Solution_dis[Max_Nodes_Num + 5];
int Inital_Solution_path[Max_Nodes_Num + 5];
void Inital_Solution_BFS_SPFA()
{
	//cout << "dfs " << u << endl;

	while (!Inital_Solution_que.empty())
	{
		int u = Inital_Solution_que.front();
		Inital_Solution_que.pop();
		Inital_Solution_que_book[u] = 0;

		for (int i = 0; i < Edge[u].size(); i++)
		{
			int v = Edge[u][i];
			//cout << v << endl;
			if (Inital_Solution_dis[v] > Inital_Solution_dis[u] + pval[Edge_pval[u][i]])
			{
				//	cout << v << endl;
				Inital_Solution_dis[v] = Inital_Solution_dis[u] + pval[Edge_pval[u][i]];
				Inital_Solution_path[v] = u;
				if (!Inital_Solution_que_book[v])
				{
					Inital_Solution_que_book[v] = 1;
					Inital_Solution_que.push(v);
				}
			}
		}
	}
}

void Inital_Solution_BFS(int solved_Nodes_Num)
{
	for (int i = 0; i < Edges_Num; i++)
		pval[i] = 1;//rand() % 200 + 100;

	for (int i = 1; i <= Nodes_Num; i++) {
		Inital_Solution_dis[i] = inf;
		Inital_Solution_path[i] = 0;
		Inital_Solution_que_book[i] = 0;
	}
	while (!Inital_Solution_que.empty())
		Inital_Solution_que.pop();
	Inital_Solution_que.push(Root);
	Inital_Solution_dis[Root] = 0;
	Inital_Solution_que_book[Root] = 1;

	int solved_Selected_Num = 1;
	while (solved_Selected_Num < solved_Nodes_Num)
	{
		//cout << Terminals_Selected_Num << endl;
		Inital_Solution_BFS_SPFA();

		int min_node = 0, min_dis = inf;
		for (int i = 0; i < solved_Nodes_Num; i++)
		{
			int u = Terminals_Nodes[i];
			if (!If_Selected[u]&& Inital_Solution_dis[u]< min_dis)
			{
				min_node = u;
				min_dis = Inital_Solution_dis[u];
			}
		}

		int u = min_node;
		while (!If_Selected[u])
		{
			If_Selected[u] = true;
			Cur_Solution_Sum_of_Nodes++;
			Cur_Solution_Chain[u].pre = preNode;
			Cur_Solution_Chain[u].nxt = Cur_Solution_Chain[preNode].nxt;
			Cur_Solution_Chain[preNode].nxt = u;
			preNode = u;

			Inital_Solution_dis[u] = 0;
			Inital_Solution_que.push(u);

			u = Inital_Solution_path[u];
		}

		solved_Selected_Num++;
	}
}
int Inital_Solution_Greedy_Degree();
void Generate_Inital_Solution()//// If_Selected,Cur_Solution_Sum_of_Nodes,Cur_Solution_Chain
{
	for (int i = 1; i <= Nodes_Num; i++) {
		If_Selected[i] = false;
		Inital_Solution_dis[i] = inf;
	}
	for(int i=Terminals_Nodes_Num-1;i>0;i--)
	{
		swap(Terminals_Nodes[i],Terminals_Nodes[rand()%i]);
	}

	Root = Terminals_Nodes[rand() % Terminals_Nodes_Num];
	//cout << Root << endl;
	Cur_Solution_Chain[Root].nxt = Cur_Solution_Chain[Root].pre = 0;
	preNode = Root;
	If_Selected[Root] = true;
	Cur_Solution_Sum_of_Nodes = 1;
	//cout << "*****" << endl;
	int solved_Nodes_Num = Terminals_Nodes_Num;
	//solved_Nodes_Num=Inital_Solution_Greedy_Degree();
	//cout << "*****" << endl;
	Inital_Solution_BFS(solved_Nodes_Num);
	//cout << "*****" << endl;
}

int Belong_Component[Max_Nodes_Num + 5];
struct Greedy_Degree_queue {
	int u, degree;
	bool operator <(const Greedy_Degree_queue &a)const {
		return degree < a.degree;
	}
};
priority_queue<Greedy_Degree_queue>Greedy_Degree_que;
void update_adjacent_steiner_degree(int u)
{
	for (int j = 0; j < Edge[u].size(); j++)
	{
		int v = Edge[u][j];
		if (If_Steiner_Nodes[v])
			degree[v]--;
	}
}
int Inital_Solution_Greedy_Degree()
{
	int maxdegree = 0;
	for (int i = 1; i <= Nodes_Num; i++)
	{
		degree[i] = 0;
		if (If_Steiner_Nodes[i])
		{
			for (int j = 0; j < Edge[i].size(); j++)
			{
				if (!If_Steiner_Nodes[Edge[i][j]])
					degree[i]++;
			}
			maxdegree = max(maxdegree, degree[i]);
			
		}
		Belong_Component[i] = i;
	}
	while (!Greedy_Degree_que.empty())
		Greedy_Degree_que.pop();
	
	Get_Random_array(Nodes_Num);
	
	for (int ii = 0; ii < Nodes_Num; ii++)
	{
		int i = Random_array[ii] + 1;
		if (If_Steiner_Nodes[i])
		{
			Greedy_Degree_queue node;
			node.degree = degree[i];
			node.u = i;
			Greedy_Degree_que.push(node);
		}
	}
	if(maxdegree==0)
		return Terminals_Nodes_Num;
	int Iter = rand() % (Terminals_Nodes_Num / maxdegree)+Terminals_Nodes_Num;
	Iter=min(Iter,Nodes_Num);
	int It = Terminals_Nodes_Num;
	while(It<Iter)
	{
		//cout<<It<<endl;
		Greedy_Degree_queue node;
		node = Greedy_Degree_que.top();
		int u = node.u;
		if (node.degree <= 1) break;
		Greedy_Degree_que.pop();
		if (node.degree > degree[u])
		{
			node.degree = degree[u];
			Greedy_Degree_que.push(node);
			continue;
		}
		Terminals_Nodes[It++] = u;
		for (int i = 0; i < Edge[u].size(); i++)
		{
			int v = Edge[u][i];
			if (If_Steiner_Nodes[v]) continue;
			if (Belong_Component[v]==v)
			{
				Belong_Component[v] = u;
				update_adjacent_steiner_degree(v);
			}
		}
	}
	return It;
}
