/*
 * Reciprocal-contact network-cleaning program.
 * Keeps only reciprocal directed relationships and writes the legacy nodes22.dat
 * and aristes22.dat outputs.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

	int main(void){
	
	int n, *con, *Pi, *Pf, *degree, i, j, k, max, sum, aux;
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
	while(fscanf(input_file, "%d %d", &i, &j)!=EOF){  /*Fill the degree array and assign provisional start/end pointer positions*/
		degree[i]++;};
	printf("C\n");
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
		for(k=Pi[i];k<=Pf[i];k++){
			if(con[k]==j||j==i){/*Remove repeated edges and self-loops*/
				control=false;
				break;};};
		if(control==true){
		Pf[i]++;
		con[Pf[i]]=j;};};
	printf("F\n");
	control=false;
	aux=0;
        for(i=1;i<=max;i++){/*Remove non-reciprocal contacts*/
                for(j=Pi[i];j<=Pf[i];j++){
                        for(k=Pi[con[j]];k<=Pf[con[j]];k++){
                                if(con[k]==i){
                                        control=true;
                                        break;};
                                control=false;};
                        if(control==false){/*the contact is not reciprocal*/
                                con[j]=0;};};
		for(j=Pf[i];j>=Pi[i];j--){
			if(con[j]==0){
				aux++;
				for(k=j;k<Pf[i];k++){
					con[k]=con[k+1];};};};
		Pf[i]=Pf[i]-aux;
		aux=0;};

	printf("E\n");
	aux=0;
	sum=0;
	for(i=1;i<=max;i++){
		if(Pf[i]<Pi[i]){
			aux=aux+degree[i];
			degree[i]=0;};
		for(j=Pi[i];j<=Pf[i];j++){
			con[j-aux]=con[j];};
		Pi[i]=Pi[i]-aux;
                Pf[i]=Pf[i]-aux;
		sum = degree[i] +Pi[i]-Pf[i]-1;
		degree[i]=degree[i]-sum;
		aux=aux+sum;
		sum=0;};
	fclose(input_file);

				


	nodes = fopen("nodes22.dat","w");
	sum=0;
	aux=0;
	for(i=1;i<=max;i++){
		fprintf(nodes, "%d %d %d \n", Pi[i],Pf[i],degree[i]);
		sum=sum+degree[i];
		if(degree[i]>0){
		aux++;};};
	fclose(nodes);
	printf("%d\n %d", sum, aux);
	edges_file=fopen("aristes22.dat","w");
	for(i=1;i<=max;i++){
		for(j=Pi[i];j<=Pf[i];j++){
			control = false;
			for(k=Pi[con[j]];k<=Pf[con[j]];k++){
				if(con[k]==i){
					control=true;
					break;};};
			if(control==true){
			fprintf(edges_file, "%d %d \n", i, con[j]);};};};
	fclose(edges_file),
	free(degree);
	free(con);
	free(Pi);
	free(Pf);

	return 0;}


