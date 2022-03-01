#pragma once

bool LS_Delete()
{
	Ask_for_Cadidate_Nodes_to_Delete_Tarjan();
		//cout << "debug" << endl;
	int siz = Candidate_Nodes_to_Delete.size();
	if(siz>0){
		int tmp_node = rand() % siz;
		Delete_Node_from_Cur_Tree(Candidate_Nodes_to_Delete[tmp_node]);///delete 1 node
		return true;
	}
	return false;
}

bool LS_Swap_Keypath()
{
	Ask_for_Cadidate_Keypath_to_Swap();
	int sizpath=Candidate_v1_of_Path_to_Swap.size();
	if(sizpath>0)
	{
		int tmp_node = rand() % sizpath;
		Swap_KeyPath(tmp_node);
		return true;
	}
	return false;
}

bool LS_Swap2()
{
	int siz1 =0;
	if(Ask_Swap2)
	{
		if(!Bipartite_Graph)
		{
			Ask_for_Cadidate_2_1_Nodes_to_Swap();///swap 2 and 1 ?
			siz1 = Candidate_x_y_z_to_Swap[0].size();
		}
	}
	if (siz1 != 0)
	{
		int tmp_node = rand() % siz1;
		Swap_Nodes_2_1(Candidate_x_y_z_to_Swap[0][tmp_node], Candidate_x_y_z_to_Swap[1][tmp_node], Candidate_x_y_z_to_Swap[2][tmp_node]);
		return true;
	}
	return false;
}

bool LS_Swap1()
{
	Ask_for_Cadidate_Nodes_to_Swap();///swap 1 and 1 ?
	int siz2 = Candidate_x_y_to_Swap[0].size();
	//cout<<siz2<<endl;
	if (siz2 != 0){
		int tmp_node = rand() % siz2;
		Swap_Nodes(Candidate_x_y_to_Swap[0][tmp_node], Candidate_x_y_to_Swap[1][tmp_node]);
		return true;
	}
	return false;
}

void Local_Search()
{
	int num = 0;
	int LSbest = Cur_Solution_Sum_of_Nodes;

	while (clock() - begin_time < 1.0*Max_Limited_Time*CLOCKS_PER_SEC)
	{
		if(!LS_Delete())
		if(!LS_Swap_Keypath())
		if(!LS_Swap2())
		if(!LS_Swap1())
			break;
			
		//cout << Cur_Solution_Sum_of_Nodes << endl;
		//for (int node = Root; node != 0; node = Cur_Solution_Chain[node].nxt)
		//	printf("%d ", node);
		//puts("");
		//printf("flag= %d\n", Check_Cur_Solution());
		if (Cur_Solution_Sum_of_Nodes < LSbest)
		{
			LSbest = Cur_Solution_Sum_of_Nodes;
			num = 0;
		}
		else {
			num++;
			if (num > Max_not_Improve_Num) break;
		}
	}
}


int Solve_Instance(long long &sum, long long &runnum)
{
	Generate_Inital_Solution();

	printf("******%d*********\n", Cur_Solution_Sum_of_Nodes - 1);

	Store_Best_Solution();

	runnum = sum = 0;

	double bestanswer_time;

	long long best_answer_num = 0;
	while (clock() - begin_time < 1.0*Max_Limited_Time*CLOCKS_PER_SEC)
	{
		Generate_Inital_Solution();
		//printf("******%d*********\n", Cur_Solution_Sum_of_Nodes - 1);
		Local_Search();


		if(Cur_Solution_Sum_of_Nodes<0) break;
		sum += 1ll*(Cur_Solution_Sum_of_Nodes - 1);
		runnum++;	

		if (Cur_Solution_Sum_of_Nodes < Best_Solution.Sum_of_Nodes)
		{
			best_answer_num = 1;
			Store_Best_Solution();
			bestanswer_time=(clock()-begin_time)/CLOCKS_PER_SEC;
			printf("%s  time:%.2f s   LS %lld answer:%d        Best answer:%d\n",Input_File_Name, bestanswer_time,runnum, Cur_Solution_Sum_of_Nodes - 1, Best_Solution.Sum_of_Nodes - 1);
		}
		else
			if (Cur_Solution_Sum_of_Nodes == Best_Solution.Sum_of_Nodes)
				best_answer_num++;
	}

	int flag = Check_Best_Solution();

	printf("flag=%d\n", flag);
	sort(Best_Solution.Nodes.begin(), Best_Solution.Nodes.end());
	printf("Nodes:");
	for (int i = 0; i < Best_Solution.Sum_of_Nodes; i++)
	{
		printf("%d ", Best_Solution.Nodes[i]);
	}
	printf("\n");

	printf("The best answer is  %d\n", Best_Solution.Sum_of_Nodes - 1);
	printf("Time to arrive best answer: %.4f\n",bestanswer_time);
	printf("LS num is  %lld        The Sum is  %lld\n", runnum, sum);
	printf("Best Answer Num is: %lld\n", best_answer_num);
	printf("The average answer is  %.2f\n", 1.0*sum / runnum);
	printf("Time is %f\n", (clock() - begin_time) / CLOCKS_PER_SEC);

	FILE *fp;
	fp = fopen(Output_File_Name, "w+");

    fprintf(fp, "******flag=%d******\n\n\n", flag);

	fprintf(fp, "Original Nodes Num:%d   Original Edges Num:%d   Original Terminals Num:%d\n", origin_Nodes_Num, origin_Edges_Num, origin_Terminals_Nodes_Num);
	fprintf(fp, "Presolved Nodes Num:%d   Presolved Edges Num:%d   Presolved Terminals Num:%d\n", presolved_nodes, presolved_edges, presolved_terminals);
	fprintf(fp, "Cut Nodes Num:%d\n\n", cutnodes);


	fprintf(fp, "The best answer is  %d\n", Best_Solution.Sum_of_Nodes - 1);
	fprintf(fp,"Time to arrive best answer: %.4f\n",bestanswer_time);
	fprintf(fp, "LS num is  %lld        The Sum is  %lld\n", runnum, sum);
	fprintf(fp, "Best Answer Num is: %lld\n", best_answer_num);
	fprintf(fp, "The average answer is  %.2f\n", 1.0*sum / runnum);
	fprintf(fp, "Time is %f\n", (clock() - begin_time) / CLOCKS_PER_SEC);

	fprintf(fp, "\n\nNodes:");
	for (int i = 0; i < Best_Solution.Sum_of_Nodes; i++)
	{
		fprintf(fp, "%d ", Best_Solution.Nodes[i]);
	}
	fprintf(fp, "\n\n\n");

	sort(Outputedge.begin(), Outputedge.end(), Outputedgecmp);

	fprintf(fp, "\nEdges:\n");
	for (int i = 0; i < Best_Solution.Sum_of_Nodes - 1; i++)
	{
		fprintf(fp, "%d  %d\n", Outputedge[i].x, Outputedge[i].y);
	}

	fclose(fp);

	return flag;
}
