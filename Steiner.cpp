// Steiner.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)

#include "Init.h"
#include "Reduction.h"
#include "Public_Function.h"
#include "Inital_Solution.h"
#include "Operator_Delete.h"
#include "Operator_Swap.h"
#include "Operator_Swap_keypath.h"
#include "Local_Search.h"



int main(int argc, char* argv[])
{
	Max_Limited_Time = 3600;
	Ask_Swap2=1;
		
	Input_File_Name = argv[1];
	Output_File_Name = argv[2];

	//Input_File_Name = "./instances/hu11u.stp";
	//Output_File_Name = "./results/hc11u.stp";

	cout << Input_File_Name << endl;
	cout << Output_File_Name << endl;


	printf("\n\nNow instance %s is begin:\n", Input_File_Name);

	begin_time = clock();

	System_Init(); 

	//system("pause");
	//return 0;

	printf("Original Nodes Num:%d   Original Edges Num:%d   Original Terminals Num:%d\n", origin_Nodes_Num, origin_Edges_Num, origin_Terminals_Nodes_Num);
	
	Reduction(presolved_nodes, presolved_edges, presolved_terminals,cutnodes);

	printf("Presolved Nodes Num:%d   Presolved Edges Num:%d   Presolved Terminals Num:%d\n", presolved_nodes, presolved_edges, presolved_terminals);
	printf("Cut Nodes Num:%d\n\n", cutnodes);

	Max_not_Improve_Num=Terminals_Nodes_Num;//max(presolved_terminals,Edges_Num/5);

	long long sum = 0, runnum = 0;
	printf("Begin Local Search!\n\n");
	if (Solve_Instance(sum,runnum) == 1)
		printf("Best: %d   Ave:%.2f   LS_num:%ll d\n", Best_Solution.Sum_of_Nodes - 1, 1.0*sum / runnum, runnum);
	else 
		printf("error\n");

	printf("Now instance  %s   is finished\n", Input_File_Name);
	
    system("pause");
	return 0;
}


