/*
 * Preliminary network-cleaning program.
 * Removes repeated edges and self-loops, filters edges attached to high-degree nodes,
 * and writes the legacy nodes.dat and aristes.dat outputs.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

	int main(void){
	
	int n, *con, *Pi, *Pf, *degree, i, j, k, node_count, max, sum, aux;
	FILE *input_file, *nodes, *edges_file;
	char input_filename[20];
	bool control;

	printf("Input file name?\n");
	scanf("%s", input_filename);
	input_file = fopen(input_filename, "r");
	if(input_file == NULL){
		printf("error opening file\n");
		exit(1);};

	n = 0;
	max=0;
	while(fscanf(input_file, " %d %d", &i, &j) != EOF){  /*Count the number of directed edge entries and the maximum node id*/
		n++;
		if(i>max){
			max=i;};
		if(j>max){
			max=j;};};
	con = (int *)malloc((2*n+1)*sizeof(int));  /*Allocate dynamic memory for the arrays*/
	Pi = (int *)malloc((max+1)*sizeof(int));
	Pf = (int *)malloc((max+1)*sizeof(int));
	degree = (int *)malloc((max+1)*sizeof(int));

	printf("A\n");
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
	printf("B\n");
	while(fscanf(input_file, "%d", &i)!=EOF){  /*Fill the degree array and assign provisional start/end pointer positions*/
		degree[i]++;};
	printf("%d\n", degree[5935]);
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

	printf("D\n");
	while(fscanf(input_file, "%d %d", &i, &j)!=EOF){  /*Fill the adjacency array and update Pf*/
		control=true;
			if(degree[i]>1400||degree[j]>1400){/*Remove repeated edges and self-loops*/
				control=false;};
			for(k=Pi[i];k<=Pf[i];k++){
                        if(con[k]==j||j==i){/*Remove repeated edges and self-loops*/
                                control=false;
                                break;};};
		if(control==true){
		Pf[i]++;
		con[Pf[i]]=j;
		Pf[j]++;
		con[Pf[j]]=i;};};
	printf("E\n");
	aux=0;
	sum=0;
	for(i=1;i<=max;i++){
		for(j=Pi[i];j<=Pf[i];j++){
			con[j-aux]=con[j];};
		Pi[i]=Pi[i]-aux;
                Pf[i]=Pf[i]-aux;
		sum = degree[i] +Pi[i]-Pf[i]-1;
		degree[i]=degree[i]-sum;
		aux=aux+sum;
		sum=0;};
	fclose(input_file);
	nodes = fopen("nodes.dat","w");
	for(i=1;i<=max;i++){
		fprintf(nodes, "%d %d %d \n", Pi[i],Pf[i],degree[i]);
		sum=sum+degree[i];};
	sum=sum/2;
	fclose(nodes);
	edges_file=fopen("aristes.dat","w");
	for(i=1;i<=max;i++){
		for(j=Pi[i];j<=Pf[i];j++){
			fprintf(edges_file, "%d %d \n",i, con[j]);};};
	printf("%d\n", degree[5935]);
	node_count=0;
	for(i=1;i<=max;i++){
		if(degree[i]!=0){
			node_count++;};};
	printf("%d\n", node_count);
	fclose(edges_file),
	free(degree);
	free(con);
	free(Pi);
	free(Pf);

	return 0;}


