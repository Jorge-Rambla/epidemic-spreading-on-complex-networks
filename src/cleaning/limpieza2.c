#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

	int main(void){
	
	int n, *con, *Pi, *Pf, *grad, i, j, k, max, suma, aux;
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
	while(fscanf(dades, "%d %d", &i, &j)!=EOF){  /*Omplim el vector de graus del nodes,assignem les direccions del punter inicial i final(provisionals)*/
		grad[i]++;};
	printf("C\n");
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
		for(k=Pi[i];k<=Pf[i];k++){
			if(con[k]==j||j==i){/*Eliminem possibles repeticions i loops*/
				control=false;
				break;};};
		if(control==true){
		Pf[i]++;
		con[Pf[i]]=j;};};
	printf("F\n");
	control=false;
	aux=0;
        for(i=1;i<=max;i++){/*Eliminem contactes no recíprocs*/
                for(j=Pi[i];j<=Pf[i];j++){
                        for(k=Pi[con[j]];k<=Pf[con[j]];k++){
                                if(con[k]==i){
                                        control=true;
                                        break;};
                                control=false;};
                        if(control==false){/*el contacte no és recíproc*/
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
	suma=0;
	for(i=1;i<=max;i++){
		if(Pf[i]<Pi[i]){
			aux=aux+grad[i];
			grad[i]=0;};
		for(j=Pi[i];j<=Pf[i];j++){
			con[j-aux]=con[j];};
		Pi[i]=Pi[i]-aux;
                Pf[i]=Pf[i]-aux;
		suma = grad[i] +Pi[i]-Pf[i]-1;
		grad[i]=grad[i]-suma;
		aux=aux+suma;
		suma=0;};
	fclose(dades);

				


	nodes = fopen("nodes22.dat","w");
	suma=0;
	aux=0;
	for(i=1;i<=max;i++){
		fprintf(nodes, "%d %d %d \n", Pi[i],Pf[i],grad[i]);
		suma=suma+grad[i];
		if(grad[i]>0){
		aux++;};};
	fclose(nodes);
	printf("%d\n %d", suma, aux);
	aristes=fopen("aristes22.dat","w");
	for(i=1;i<=max;i++){
		for(j=Pi[i];j<=Pf[i];j++){
			control = false;
			for(k=Pi[con[j]];k<=Pf[con[j]];k++){
				if(con[k]==i){
					control=true;
					break;};};
			if(control==true){
			fprintf(aristes, "%d %d \n", i, con[j]);};};};
	fclose(aristes),
	free(grad);
	free(con);
	free(Pi);
	free(Pf);

	return 0;}


