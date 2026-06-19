#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

	int main(void){
	
	int n, *con, *Pi, *Pf, *grad, i, j, k, nodos, max, suma, aux;
	FILE *dades, *nodes, *aristes;
	char dad[20];
	bool control;

	printf("Nom del fitxer d'entrada?\n");
	scanf("%s", dad);
	dades = fopen(dad, "r");
	if(dades == NULL){
		printf("error en obrir el fitxer\n");
		exit(1);};

	n = 0;
	max=0;
	while(fscanf(dades, " %d %d", &i, &j) != EOF){  /*Contem el nombre d'arestes(2n) i el nombre de nodes(max)*/
		n++;
		if(i>max){
			max=i;};
		if(j>max){
			max=j;};};
	con = (int *)malloc((2*n+1)*sizeof(int));  /*Reservem memòria dinàmica pels vectors*/
	Pi = (int *)malloc((max+1)*sizeof(int));
	Pf = (int *)malloc((max+1)*sizeof(int));
	grad = (int *)malloc((max+1)*sizeof(int));

	printf("A\n");
	if(con==NULL || Pi==NULL || Pf==NULL || grad==NULL){
		printf("Problemes de memòria\n");
		exit(2);};
	fclose(dades);

	dades = fopen(dad, "r");
        if(dades == NULL){
                printf("error en obrir el fitxer\n");
                exit(1);};

	for(i=0;i<=max;i++){
		grad[i]=0;
		Pi[i]=0;
		Pf[i]=0;};
	for(i=0;i<=2*n;i++){
		con[i]=0;};
	printf("B\n");
	while(fscanf(dades, "%d", &i)!=EOF){  /*Omplim el vector de graus del nodes,assignem les direccions del punter inicial i final(provisionals)*/
		grad[i]++;};
	printf("%d\n", grad[5935]);
	fclose(dades);
	Pi[1] = 1;
	Pf[1] = 0;
	for(i=2;i<=max;i++){
		Pi[i] = Pi[i-1]+grad[i-1];
		Pf[i] = Pi[i]-1;};


	dades = fopen(dad, "r");
        if(dades == NULL){
                printf("error en obrir el fitxer\n");
                exit(1);};

	printf("D\n");
	while(fscanf(dades, "%d %d", &i, &j)!=EOF){  /*Omplim el vector de connexions i modifiquem Pf*/
		control=true;
			if(grad[i]>1400||grad[j]>1400){/*Eliminem possibles repeticions i loops*/
				control=false;};
			for(k=Pi[i];k<=Pf[i];k++){
                        if(con[k]==j||j==i){/*Eliminem possibles repeticions i loops*/
                                control=false;
                                break;};};
		if(control==true){
		Pf[i]++;
		con[Pf[i]]=j;
		Pf[j]++;
		con[Pf[j]]=i;};};
	printf("E\n");
	aux=0;
	suma=0;
	for(i=1;i<=max;i++){
		for(j=Pi[i];j<=Pf[i];j++){
			con[j-aux]=con[j];};
		Pi[i]=Pi[i]-aux;
                Pf[i]=Pf[i]-aux;
		suma = grad[i] +Pi[i]-Pf[i]-1;
		grad[i]=grad[i]-suma;
		aux=aux+suma;
		suma=0;};
	fclose(dades);
	nodes = fopen("nodes.dat","w");
	for(i=1;i<=max;i++){
		fprintf(nodes, "%d %d %d \n", Pi[i],Pf[i],grad[i]);
		suma=suma+grad[i];};
	suma=suma/2;
	fclose(nodes);
	aristes=fopen("aristes.dat","w");
	for(i=1;i<=max;i++){
		for(j=Pi[i];j<=Pf[i];j++){
			fprintf(aristes, "%d %d \n",i, con[j]);};};
	printf("%d\n", grad[5935]);
	nodos=0;
	for(i=1;i<=max;i++){
		if(grad[i]!=0){
			nodos++;};};
	printf("%d\n", nodos);
	fclose(aristes),
	free(grad);
	free(con);
	free(Pi);
	free(Pf);

	return 0;}


