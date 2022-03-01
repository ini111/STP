#pragma once
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cmath>
#include<queue>
#include<vector>
#include<string>
#include<cstring>
#include<fstream>
#include<ctime>
#include<stack>

using namespace std;
#define inf 999999999

int Ins_Num;
//char  *Input_File_Name;
//char  *Output_File_Name;
char *Input_File_Name;
char *Output_File_Name;
char input_files[300][50];
char output_files[300][50];
bool Ask_Swap2;

double begin_time;
const int Max_Terminals_Nodes_Num = 200005;
const int Max_space = 1e3;
const int Max_Nodes_Num = 2e6;
const int Max_Edge_Num = 5e6;
int Max_Limited_Time=1;
int limited_time[300];
int Max_not_Improve_Num = 150;

int Max_SD_value;
int Bipartite_Graph;
int H_neibors;
vector<int>all_pair_Steiner_with_H_neibors[3];
/*---------------graph--------------------------------------*/
int origin_Nodes_Num;
int origin_Edges_Num;
int origin_Terminals_Nodes_Num;
vector<int>origin_Edge[Max_Nodes_Num + 5];
int origin_Edge_X[Max_Edge_Num + 5], origin_Edge_Y[Max_Edge_Num + 5];
int origin_Terminals_Nodes[Max_Terminals_Nodes_Num + 5];
vector<int>new_Edge_X;
vector<int>new_Edge_Y;
vector<int>new_add_Edge_X;
vector<int>new_add_Edge_Y;

int presolved_nodes, presolved_edges, presolved_terminals, cutnodes;

int Nodes_Num;
int Edges_Num;
int Terminals_Nodes_Num;
vector<int>Edge[Max_Nodes_Num + 5];
vector<int>Edge_pval[Max_Nodes_Num + 5];
int Terminals_Nodes[Max_Terminals_Nodes_Num + 5];
bool If_Steiner_Nodes[Max_Nodes_Num + 5];
int degree[Max_Nodes_Num + 5];
int important_node[Max_Nodes_Num + 5];
int important_terminal[Max_Nodes_Num + 5];

/*---------------for tree structure-------------------------*/
int Root;
vector<int>Cur_Tree[Max_Nodes_Num + 5];
int Cur_Father[Max_Nodes_Num + 5];

/*---------------to store solution--------------------------*/
struct Node_Solution {
	int Sum_of_Nodes;
	vector<int>Nodes;
};
struct Node_Solution_Chain {
	int pre, nxt;
};
Node_Solution_Chain Cur_Solution_Chain[Max_Nodes_Num + 5];
int Cur_Solution_Sum_of_Nodes;
bool If_Selected[Max_Nodes_Num + 5];
Node_Solution Best_Solution;

/*---------------to store candidate nodes--------------------*/
vector<int>Candidate_Nodes_to_Delete;
vector<int>Candidate_x_y_to_Swap[2];
vector<int>Candidate_x_y_to_Swap_SD;
double Potential_for_Swap[Max_Nodes_Num + 5];

vector<int>Candidate_x_y_z_to_Swap[3];

/*---------------for basic function inital solution----------*/
struct Node_Edge {
	int x, y;
};
vector<Node_Edge>Edge_x_y;
int Random_array[Max_Edge_Num + 5];

int Terminal_Num_of_MST[Max_Nodes_Num + 5];
int Belong[Max_Nodes_Num + 5];
bool Edge_visted[Max_Edge_Num + 5];
bool If_Stroed_Terminal[Max_Edge_Num + 5];
int endpoint;
int preNode;
int Appera_times[Max_Nodes_Num+5];

/*---------------------for Tarjan-----------------------------*/
int father[Max_Nodes_Num + 5];
int dfn[Max_Nodes_Num + 5];
int low[Max_Nodes_Num + 5];
int belong[Max_Nodes_Num + 5];
bool is_cut[Max_Nodes_Num + 5];
int tim;


/*---------------------for check new subtree------------------*/
int Belong_to_Subtree[Max_Nodes_Num + 5];
int Sub_Tree_Num;
int Sub_Tree_ok[Max_Nodes_Num + 5];
int visted[Max_Nodes_Num + 5];
int SD_for_Nodes[Max_Nodes_Num + 5];

/*---------------------for check solution---------------------*/
bool Check_Best_Solution_visted[Max_Nodes_Num], Check_Best_Solution_selected[Max_Nodes_Num];
int Check_Best_Solution_Num;
vector<Node_Edge>Outputedge;
bool Check_Cur_Solution_visted[Max_Nodes_Num], Check_Cur_Solution_selected[Max_Nodes_Num];
int Check_Cur_Solution_Num;



void Read_Instances()
{
	for (int i = 0; i <= Max_Nodes_Num; i++)
	{
		origin_Edge[i].clear();
	}

	ifstream FIC;
	FIC.open(Input_File_Name);
	if (FIC.fail())
	{
		printf("Fail to open the input file %s\n", Input_File_Name);
		return;
	}
	else {
		printf("read instances begin!\n");
	}
	char Str[100];
	while (strcmp(Str, "Nodes"))
		FIC >> Str;
	FIC >> origin_Nodes_Num;
	FIC >> Str >> origin_Edges_Num;

	int x, y, weight;
	for (int i = 0; i < origin_Edges_Num; i++)
	{
		FIC >> Str >> x >> y >> weight;
		origin_Edge_X[i] = x;
		origin_Edge_Y[i] = y;
		origin_Edge[x].push_back(y);
		origin_Edge[y].push_back(x);
	}
	while (strcmp(Str, "Terminals"))
		FIC >> Str;
	FIC >> Str;
	FIC >> origin_Terminals_Nodes_Num;
	for (int i = 0; i < origin_Terminals_Nodes_Num; i++)
	{
		FIC >> Str >> origin_Terminals_Nodes[i];
	}

	printf("\nNodes_Num:%d  Edges_Num:%d   Terminals_Num:%d\nread instances finished!\n", origin_Nodes_Num, origin_Edges_Num, origin_Terminals_Nodes_Num);

}

void Copy_Instances()
{
	for (int i = 0; i <= Max_Nodes_Num; i++)
	{
		Edge[i].clear();
		Edge_pval[i].clear();
		If_Steiner_Nodes[i] = true;
	}
	
	int x, y;
	Nodes_Num = origin_Nodes_Num;
	Edges_Num=origin_Edges_Num;
	for (int i = 0; i < origin_Edges_Num; i++)
	{
		x = origin_Edge_X[i]; y = origin_Edge_Y[i];
		Node_Edge NE;
		NE.x=x; NE.y=y;
		Edge_x_y.push_back(NE);
		Edge[x].push_back(y);
		Edge[y].push_back(x);
		Edge_pval[x].push_back(i);
		Edge_pval[y].push_back(i);
	}

	Terminals_Nodes_Num = origin_Terminals_Nodes_Num;
	for (int i = 0; i < origin_Terminals_Nodes_Num; i++)
	{
		Terminals_Nodes[i] = origin_Terminals_Nodes[i];
		If_Steiner_Nodes[Terminals_Nodes[i]] = false;
	}
}

void Find_rules();

void System_Init()
{
	//srand(time(0));
	srand(0);

	Read_Instances();
	Copy_Instances();

	Find_rules();
}

int numnum[Max_Nodes_Num + 5];
void Find_rules()
{
	int edgenumTT = 0;
	int flagflag=0;
	Bipartite_Graph=0;
	for (int i = 0; i < origin_Edges_Num; i++)
	{
		int x = origin_Edge_X[i], y = origin_Edge_Y[i];
		if (!If_Steiner_Nodes[x] && !If_Steiner_Nodes[y])
			edgenumTT++;
	}
	//printf("Edges that connect two terminals: %d\n", edgenumTT);

	//printf("--------------------------For Steiner----------------------------\n");
	/*
	for (int i = 0; i < Nodes_Num; i++)
		numnum[i] = 0;
	for (int i = 1; i <= Nodes_Num; i++)
	if (If_Steiner_Nodes[i])
	{
		int nu = 0;
		for (int j = 0; j < Edge[i].size(); j++)
		{
			if (!If_Steiner_Nodes[Edge[i][j]])
				nu++;
		}
		numnum[nu]++;
	}
	
	for (int i = 0; i < Nodes_Num; i++)
	if (numnum[i] > 0)
	{
		printf("%d steiners has %d terminal neibors\n", numnum[i],i);
	}
	*/

	//printf("--------------------------For Steiner----------------------------\n");
	for (int i = 0; i < Nodes_Num; i++)
		numnum[i] = 0;
	for (int i = 1; i <= Nodes_Num; i++)
	if (If_Steiner_Nodes[i])
	{
		int nu = 0;
		for (int j = 0; j < Edge[i].size(); j++)
		{
			if (If_Steiner_Nodes[Edge[i][j]])
				nu++;
		}
		numnum[nu]++;
	}
	for (int i = 0; i < Nodes_Num; i++)
	if (numnum[i] > 0)
	{
		//printf("%d steiners has %d steiner neibors\n", numnum[i],i);
		if(i>0)
		flagflag=1;
	}
	/*
	printf("--------------------------For Terminal---------------------------\n");
	for (int i = 0; i < Nodes_Num; i++)
		numnum[i] = 0;
	for (int i = 1; i <= Nodes_Num; i++)
	if (!If_Steiner_Nodes[i])
	{
		int nu = 0;
		for (int j = 0; j < Edge[i].size(); j++)
		{
			if (!If_Steiner_Nodes[Edge[i][j]])
				nu++;
		}
		numnum[nu]++;
	}
	for (int i = 0; i < Nodes_Num; i++)
	if (numnum[i] > 0)
	{
		printf("%d terminals has %d terminal neibors\n", numnum[i], i);
	}

	printf("--------------------------For Terminal---------------------------\n");
	for (int i = 0; i < Nodes_Num; i++)
		numnum[i] = 0;
	for (int i = 1; i <= Nodes_Num; i++)
	if (!If_Steiner_Nodes[i])
	{
		int nu = 0;
		for (int j = 0; j < Edge[i].size(); j++)
		{
			if (If_Steiner_Nodes[Edge[i][j]])
				nu++;
		}
		numnum[nu]++;
	}
	
	for (int i = 0; i < Nodes_Num; i++)
	if (numnum[i] > 0)
	{
		printf("%d terminals has %d steiner neibors\n",  numnum[i],i);
	}
	*/
	

	if(flagflag==0) {
		Bipartite_Graph=1;
		//printf("\n Birnariy Graph\n\n");
	}

	/*
	printf("************************\n");
	for (int i = 0; i < Nodes_Num; i++)
		numnum[i] = 0;

	for(int i=5;i<=Nodes_Num;i++)
	if(If_Steiner_Nodes[i])
	{
		int flag=1;
		for(int j=0;j<Edge[i].size();j++)
		{
			int u=Edge[i][j];
			if(numnum[u]==1)
				flag=0;
		}
		if(!flag) continue;

		printf("%d:",i);
		for(int j=0;j<Edge[i].size();j++)
		{
			int u=Edge[i][j];
			numnum[u]=1;
			if(!If_Steiner_Nodes[u])
				printf("  %d",u);
		}
		printf("\n");
	}
	printf("************************\n");
	*/
}
