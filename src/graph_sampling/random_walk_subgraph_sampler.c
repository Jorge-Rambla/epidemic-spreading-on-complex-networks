/*
 * Random-walk-like subgraph sampler.
 * Reads an edge list and writes sampled edges to the legacy subg.dat output.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define TARGET_NODES 50000

	int main(void){
	
	int n, *con, *Pi, *Pf, *degree, i, seed_node, j, k,  max, sum, Aux, a, Auxi;
	FILE *input_file, *subgraph_file;
	char input_filename[20];
	bool control;

	printf("Input file name?\n");
	scanf("%s", input_filename);
	input_file = fopen(input_filename, "r");
	subgraph_file = fopen("subg.dat","w");
	if(input_file == NULL||subgraph_file==NULL){
		printf("error opening file\n");
		exit(1);};

	n = 0;
	a=0;
	max=0;
	while(fscanf(input_file, " %d %d", &i, &j) != EOF){  /*Count the number of directed edge entries and the maximum node id*/
		n++;
		if(i>max){
			max=i;};
		if(j>max){
			max=j;};};
	printf("The network has %d nodes and %d edges \n", max, n);
	con = (int *)malloc((2*n+1)*sizeof(int));  /*Allocate dynamic memory for the arrays*/
	Pi = (int *)malloc((max+1)*sizeof(int));
	Pf = (int *)malloc((max+1)*sizeof(int));
	degree = (int *)malloc((max+1)*sizeof(int));

	if(con==NULL || Pi==NULL || Pf==NULL || degree==NULL){
		printf("Memory allocation error\n");
		exit(2);};
	fclose(input_file);

	input_file = fopen(input_filename, "r");
        if(input_file == NULL){
                printf("error opening file\n");
                exit(1);};

	for(i=0;i<=max;i++){
		degree[i]=0;
		Pi[i]=0;
		Pf[i]=0;};
	for(i=0;i<=2*n;i++){
		con[i]=0;};
	while(fscanf(input_file, "%d", &i)!=EOF){  /*Fill the degree array and assign provisional start/end pointer positions*/
		degree[i]++;};
	seed_node=1;
	for(i=1;i<=max;i++){
		if(degree[i]>degree[seed_node]){
			seed_node=i;};};
	fclose(input_file);
	Pi[1] = 1;
	Pf[1] = 0;
	for(i=2;i<=max;i++){
		Pi[i] = Pi[i-1]+degree[i-1];
		Pf[i] = Pi[i]-1;};


	input_file = fopen(input_filename, "r");
        if(input_file == NULL){
                printf("error opening file\n");
                exit(1);};

	while(fscanf(input_file, "%d %d", &i, &j)!=EOF){  /*Fill the adjacency array and update Pf*/
                control=true;
                for(k=Pi[i];k<=Pf[i];k++){
                        if(con[k]==j||j==i){/*Remove repeated edges and self-loops*/
                                control=false;
                                break;};};
                if(control==true){
                Pf[i]++;
                con[Pf[i]]=j;
		Pf[j]++;
                con[Pf[j]]=i;};};





        Aux=0;
        sum=0;
        for(i=1;i<=max;i++){
                for(j=Pi[i];j<=Pf[i];j++){
                        con[j-Aux]=con[j];};
                Pi[i]=Pi[i]-Aux;
                Pf[i]=Pf[i]-Aux;
                sum = degree[i] +Pi[i]-Pf[i]-1;
                degree[i]=degree[i]-sum;
                Aux=Aux+sum;
                sum=0;};
        fclose(input_file);
	for(i=1;i<=max;i++){
		sum=sum +degree[i];};

	printf("After removing repeated edges and self-loops, %d edges remain \n", sum/2);

	degree[0]=0;
	Aux=0;
	srand(time(NULL));
	while(degree[Aux]==0){
		Aux=rand()%max;};
	a++;
	for(i=Pi[Aux];i<=Pf[Aux];i++){
		fprintf(subgraph_file,"%d %d \n", Aux, con[i]);};
	while(a!=TARGET_NODES){
		a++;
		Auxi=rand()%degree[Aux];
		Aux=con[Pi[Aux]+Auxi];
		for(i=Pi[Aux];i<=Pf[Aux];i++){
                fprintf(subgraph_file,"%d %d \n", Aux, con[i]);};};
		

	fclose(subgraph_file);
	free(con);
	free(Pi);
	free(Pf);
	free(degree);

	return 0;}
