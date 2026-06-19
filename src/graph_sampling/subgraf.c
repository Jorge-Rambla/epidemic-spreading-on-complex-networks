#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define NODOS 50000

	int main(void){
	
	int n, *con, *Pi, *Pf, *grad, i, ori, j, k,  max, suma, Aux, a, Auxi;
	FILE *dades, *subg;
	char dad[20];
	bool control;

	printf("Nom del fitxer d'entrada?\n");
	scanf("%s", dad);
	dades = fopen(dad, "r");
	subg = fopen("subg.dat","w");
	if(dades == NULL||subg==NULL){
		printf("error en obrir el fitxer\n");
		exit(1);};

	n = 0;
	a=0;
	max=0;
	while(fscanf(dades, " %d %d", &i, &j) != EOF){  /*Contem el nombre d'arestes(2n) i el nombre de nodes(max)*/
		n++;
		if(i>max){
			max=i;};
		if(j>max){
			max=j;};};
	printf("La xarxa té %d nodes i %d links \n", max, n);
	con = (int *)malloc((2*n+1)*sizeof(int));  /*Reservem memòria dinàmica pels vectors*/
	Pi = (int *)malloc((max+1)*sizeof(int));
	Pf = (int *)malloc((max+1)*sizeof(int));
	grad = (int *)malloc((max+1)*sizeof(int));

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
	while(fscanf(dades, "%d", &i)!=EOF){  /*Omplim el vector de graus del nodes,assignem les direccions del punter inicial i final(provisionals)*/
		grad[i]++;};
	ori=1;
	for(i=1;i<=max;i++){
		if(grad[i]>grad[ori]){
			ori=i;};};
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

	while(fscanf(dades, "%d %d", &i, &j)!=EOF){  /*Omplim el vector de connexions i modifiquem Pf*/
                control=true;
                for(k=Pi[i];k<=Pf[i];k++){
                        if(con[k]==j||j==i){/*Eliminem possibles repeticions i loops*/
                                control=false;
                                break;};};
                if(control==true){
                Pf[i]++;
                con[Pf[i]]=j;
		Pf[j]++;
                con[Pf[j]]=i;};};





        Aux=0;
        suma=0;
        for(i=1;i<=max;i++){
                for(j=Pi[i];j<=Pf[i];j++){
                        con[j-Aux]=con[j];};
                Pi[i]=Pi[i]-Aux;
                Pf[i]=Pf[i]-Aux;
                suma = grad[i] +Pi[i]-Pf[i]-1;
                grad[i]=grad[i]-suma;
                Aux=Aux+suma;
                suma=0;};
        fclose(dades);
	for(i=1;i<=max;i++){
		suma=suma +grad[i];};

	printf("Després de eliminar repeticions i loops queden %d links \n", suma/2);

	grad[0]=0;
	Aux=0;
	srand(time(NULL));
	while(grad[Aux]==0){
		Aux=rand()%max;};
	a++;
	for(i=Pi[Aux];i<=Pf[Aux];i++){
		fprintf(subg,"%d %d \n", Aux, con[i]);};
	while(a!=NODOS){
		a++;
		Auxi=rand()%grad[Aux];
		Aux=con[Pi[Aux]+Auxi];
		for(i=Pi[Aux];i<=Pf[Aux];i++){
                fprintf(subg,"%d %d \n", Aux, con[i]);};};
		

	fclose(subg);
	free(con);
	free(Pi);
	free(Pf);
	free(grad);

	return 0;}
