/*
 * Mutation-rate scan for estimating survival/endemic probabilities.
 * Scientific parameters are defined below as compile-time constants.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define DEL 1
#define MAX 30
#define RANG 0.15
#define LIMIT 200
#define node_count 43305
#define LAM 5
#define simulations 30
#define gamma_steps 20
#define gamma_step 0.1
#define gamma_start 1.6

	int main(void){

	int n, total_degree, *con, cien, extin, *Pi, simu, *Pf, *degree, **state, **Ni, **pos, **Ea, i, seed_node, j, k, l, max, sum, gmax, ni, *Ninf, *Nedg, Aux, strains, a, lamb, gamm;
	FILE *input_file, *Pc, *Clust, *Knngraf, *Evol, *fases;
	char input_filename[20];
	double PC, KNN, CLUST, aux, aux2, sim_time, event_time_increment, GAM=0;
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
	printf("The network has %d nodes and %d edges \n", max, n);
	con = (int *)malloc((2*n+1)*sizeof(int));  /*Allocate dynamic memory for the arrays*/
	Pi = (int *)malloc((max+1)*sizeof(int));
	Pf = (int *)malloc((max+1)*sizeof(int));
	degree = (int *)malloc((max+1)*sizeof(int));
	Ninf = (int *)malloc((MAX+1)*sizeof(int));
	Nedg = (int *)malloc((MAX+1)*sizeof(int));
	state = (int **)malloc((MAX+1)*sizeof(int*));
		for(i=0;i<=MAX;i++){
                        state[i] = (int *)malloc((max+1)*sizeof(int));};
        Ni = (int **)malloc((MAX+1)*sizeof(int*));
		for(i=0;i<=MAX;i++){
                        Ni[i] = (int *)malloc((max+1)*sizeof(int));};

	for(i=1;i<=MAX;i++){
		for(j=1;j<=max;j++){
			state[i][j]=0;};};
	if(con==NULL || Pi==NULL || Pf==NULL || degree==NULL ||state ==NULL||Ni==NULL){
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
	Aux=0;
	for(i=1;i<=max;i++){
		sum=sum +degree[i];
		if(degree[i]>0){
		Aux++;};};
	total_degree=sum;
	pos = (int **)malloc((MAX+1)*sizeof(int*));
                for(i=0;i<=MAX;i++){
                        pos[i] = (int *)malloc((sum+1)*sizeof(int));};
        Ea = (int **)malloc((2*MAX+1)*sizeof(int*));
                for(i=0;i<=2*MAX;i++){
                        Ea[i] = (int *)malloc(((sum/2)+1)*sizeof(int));};
	if(Ea==NULL||pos==NULL){
		printf("memory allocation error \n");
		exit(3);};
	printf("After cleaning the network, %d nodes and %d edges remain \n", Aux, sum/2);

	fases=fopen("fases2.dat", "w");
	seed_node=1;
        for(i=1;i<=max;i++){
                if(degree[i]>degree[seed_node]){
                        seed_node=i;};};

/*	gmax=0;
	Pc = fopen("Pc.dat", "w");
	if(Pc==NULL){
		printf("error opening file\n");};
	ni=0;
	for(i=1;i<=max;i++){
		if(grad[i]>gmax){
			gmax=grad[i];};};
	aux=0;
	for(i=0;i<=gmax;i++){
		for(j=1;j<=max;j++){
			if(grad[j]>=i){
				ni++;};
			if(grad[j]==i){
				aux++;};};
		PC=ni/(double)max;
		ni=0;
		if(aux != 0 || i==0){
			fprintf(Pc, "%d   %f \n", i, PC);};
		aux=0;};
	fclose(Pc);

	Knngraf = fopen("Knngraf.dat", "w");
	KNN=0;
        if(Knngraf==NULL){
                printf("error opening file\n");};

	l=0;
	aux=0;
	for(i=1;i<=gmax;i++){
		for(j=1;j<=max;j++){
			if(grad[j]==i){
				l++;
				for(k=Pi[j];k<=Pf[j];k++){
					KNN = KNN + grad[con[k]];};
				KNN = KNN/(double)i;
				aux = aux+KNN;
				KNN=0;};};
		aux=aux/(double)l;
		if(l!=0){
			fprintf(Knngraf, "%d   %f \n", i, aux);};
		aux=0;
		l=0;};
	fclose(Knngraf);


	Clust = fopen("Clust.dat", "w");
	CLUST=0;
        if(Clust==NULL){
                printf("error opening file\n");};
	aux=0;
	aux2=0;
for(ni=1;ni<=gmax;ni++){
	for(i=1;i<=max;i++){
		if(grad[i]==ni){
		aux++;
		for(j=Pi[i];j<=Pf[i];j++){
			for(k=Pi[con[j]];k<=Pf[con[j]];k++){
				for(l=Pi[con[k]];l<=Pf[con[k]];l++){
					if(i==con[l]){
						CLUST++;
						break;};};};};
		if(CLUST !=0){
			CLUST = CLUST/(double)(grad[i]*(grad[i]-1));};
		aux2 = aux2 + CLUST;
		CLUST=0;};};
	if(aux!=0){
		aux=aux2/(double)aux;
		fprintf(Clust, " %d %f \n", ni, aux);
		aux=0;
		aux2=0;};};
	fclose(Clust);*/
	/*Initialize state, infected-node, and active-edge arrays*/
		  /*0=susceptible, 1=infected, 2=recovered*/
	srand(time(NULL));
	for(gamm=0;gamm<=gamma_steps;gamm++){
		GAM=(gamma_start+gamm*gamma_step)/(double)node_count;
		extin=0;
		cien=0;
	for(simu=1;simu<=simulations;simu++){
	for(i=0;i<=MAX;i++){
		Ninf[i]=0; /*This variable counts the number of infected nodes*/
		Nedg[i]=0;}; /*This variable counts active edges*/
	sim_time=0;
	for(i=1;i<=MAX;i++){
                for(j=1;j<=max;j++){
                        state[i][j]=0;};};
	i=seed_node;
/*	printf("%d \n", i);
	free(grad);*/
	state[1][i]=1; /*this is the first infected node*/
	Ninf[1]++;
	Ni[1][1]=i;
	Ni[0][0]=1;/*Total infected nodes*/
	strains=1;
	if(strains!=MAX){
		Ni[0][1]=1; /*Variable used to control mutations*/
	}	else{
		Ni[0][1]=0;
	}
	event_time_increment=0;
	for(k=Pi[i];k<=Pf[i];k++){/*{Activate all edges of the first infected node*/
		Nedg[1]++;
		Nedg[0]++;
		Ea[1][Nedg[1]]=i;
		Ea[2][Nedg[1]]=con[k];
		pos[1][k]=Nedg[1];};
	/*Start the infection-recovery event iteration*/
/*	Evol = fopen("Evol.dat", "w");
        if(Evol==NULL){
                printf("error opening file\n");};Record the number of infected nodes over time here*/
/*	fprintf(Evol, "0  1 \n");*/
	while(Ni[0][0] != 0){
		if(Ni[0][0]>max||Ni[0][0]<0){
			printf("program error: invalid infected-node count in strain %d \n",l);
			exit(3);};
		if(strains>MAX||strains<0){
		        printf("program error: invalid strain count\n");
                    exit(1);};
	/*	if(taux>=RANG){
                        taux=0;
			fprintf(Evol, "%lf %d ", sim_time, Ni[0][0]);
			for(i=1;i<=MAX;i++){
	    fprintf(Evol, "%d ", Ninf[i]);};
			fprintf(Evol, "\n");};*/
		aux = rand()/(double)RAND_MAX;
		sim_time = sim_time - log(aux)/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0]); /*Temps en què ocorreix el següent esdeveniment*/
		event_time_increment = event_time_increment - log(aux)/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0]);
		aux2 = rand()/(double)RAND_MAX;
	        sum = 0;
		if(aux2 < (Nedg[0]*LAM)/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0])){ /*An infection event caused by strain l occurs*/
			Aux = 1 + rand()%Nedg[0]; /*Agafem un link aleatori com provocador de l'infecció*/
			if(Nedg[0]>2*n|| Nedg[0]<0){
		    printf("program error: invalid active-edge count\n");
	        exit(2);};
			for(l=1;l<=strains;l++){
				if(Aux<=Nedg[l]){
					break;};
				if(Aux>Nedg[l]){
					Aux=Aux-Nedg[l];};};
			Ninf[l]++;
			Ni[0][0]++;
			if(strains!=MAX){
				Ni[0][1]++;};
			Ni[l][Ninf[l]] = Ea[2*l][Aux];/*Add the susceptible endpoint of this edge to the infected-node array*/
			state[l][Ea[2*l][Aux]] = 1;
			for(i=Pi[Ni[l][Ninf[l]]];i<=Pf[Ni[l][Ninf[l]]];i++){ /*iterate over all neighbors of the infected node*/
				control=true;
				for(a=1;a<=strains;a++){
					if(state[a][con[i]] == 1){
						control = false;/*Check that the node is not infected by any strain*/
						break;};};
				if(state[l][con[i]]==0 && control ==true){/*Add the edge to the list if the neighbor is susceptible*/
					Nedg[l]++;
					Nedg[0]++;
					pos[l][i]=Nedg[l];
					Ea[2*l-1][Nedg[l]]=Ni[l][Ninf[l]];
					Ea[2*l][Nedg[l]]=con[i];};

				if((control==false && state[a][Ni[l][Ninf[l]]]==0)||a==l){/*remove the edge from the list if the neighbor is infected by strain a and the newly infected node was susceptible*/
					Nedg[a]--;
					Nedg[0]--;
					for(j=Pi[Ea[2*a-1][Nedg[a]+1]];j<=Pf[Ea[2*a-1][Nedg[a]+1]];j++){/*Find the last edge in the position array for the swap*/
						if(con[j]==Ea[2*a][Nedg[a]+1]){
							break;};};
					for(k=Pi[con[i]];k<=Pf[con[i]];k++){/*Find the position of the edge to remove*/
						if(con[k]==Ni[l][Ninf[l]]){
							break;};};
					pos[a][j]=pos[a][k];/*intercanviem les posicions*/
					Ea[2*a-1][pos[a][j]] =
						Ea[2*a-1][Nedg[a]+1];
					Ea[2*a][pos[a][j]] = Ea[2*a][Nedg[a]+1];};};
			continue;};
		if((aux2 < ((Nedg[0]*LAM) + (Ni[0][0]*DEL))/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0])) && aux2>(Nedg[0]*LAM)/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0])){/*A recovery event occurs in strain l*/
			Aux = 1 + rand()%Ni[0][0]; /*Choose a random infected node to recover*/
			for(l=1;l<=strains;l++){
				if(Aux<=Ninf[l]){
					break;};
				if(Aux>Ninf[l]){
					Aux=Aux-Ninf[l];};};
			state[l][Ni[l][Aux]]=2;
			for(i=Pi[Ni[l][Aux]];i<=Pf[Ni[l][Aux]];i++){/*iterate over all neighbors of the recovered node*/
				control=true;
				for(a=1;a<=strains;a++){
					if(state[a][con[i]]==1){
						control=false;
						break;};};
				if(state[l][con[i]]==0 && control==true){ /*Remove an edge that was previously active*/
					Nedg[l]--;
					Nedg[0]--;
                                        for(j=Pi[Ea[2*l-1][Nedg[l]+1]];j<=Pf[Ea[2*l-1][Nedg[l]+1]];j++){/*Find the last edge in the position array for the swap*/
                                                if(con[j]==Ea[2*l][Nedg[l]+1]){
                                                        break;};};
                                        pos[l][j]=pos[l][i];/*intercanviem les posicions*/
                                        Ea[2*l-1][pos[l][j]] = Ea[2*l-1][Nedg[l]+1];
                                        Ea[2*l][pos[l][j]] = Ea[2*l][Nedg[l]+1];
                                        };
				if(control==false && state[a][Ni[l][Aux]]==0){/*The node recovered from strain l has a neighbor infected by strain a and is susceptible. Add edge*/
					Nedg[a]++;
					Nedg[0]++;
					for(j=Pi[con[i]];j<=Pf[con[i]];j++){
						if(con[j]==Ni[l][Aux]){
							break;};};
					pos[a][j]=Nedg[a];
                                        Ea[2*a-1][Nedg[a]]=con[i];
                                        Ea[2*a][Nedg[a]]=Ni[l][Aux];};};

			Ni[l][Aux] = Ni[l][Ninf[l]]; /*substituim el node seleccionat pel últim al vector*/
                        Ninf[l]--;
			Ni[0][0]--;
			if(strains!=MAX){
				Ni[0][1]--;};
			if(Ninf[l]==0){/*A strain has gone extinct*/
				Ninf[l]=Ninf[strains];
				Nedg[l]=0;
				Ninf[strains]=0;
				Nedg[strains]=0;
				for(j=1;j<=max;j++){
                                        state[l][j]=state[strains][j];
					state[strains][j]=0;};
                                for(j=1;j<=Ninf[l];j++){
                                        Ni[l][j]=Ni[strains][j];
					for(k=Pi[Ni[l][j]];k<=Pf[Ni[l][j]];k++){
						control = true;
						for(a=1;a<=strains;a++){
							if(state[a][con[k]]==1){
								control = false;};};
						if(control==true && state[l][con[k]]==0){
							Nedg[l]++;
							Ea[2*l-1][Nedg[l]]=Ni[l][j];
							Ea[2*l][Nedg[l]]=con[k];
							pos[l][k]=Nedg[l];};};};
				strains--;
	/*			 printf("%d  %lf \n",strains, sim_time);*/
				if(strains==MAX-1){/*Reactivate the mutation capacity of the strains*/
					Ni[0][1]=Ni[0][0];};};
			continue;};

		if(aux2 > ((Nedg[0]*LAM) + (Ni[0][0]*DEL))/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0])){/*A mutation event occurs in an individual infected with strain l*/
		strains++;
		Aux = 1 + rand()%Ni[0][0]; /*Seleccionem aleatòriament el node que muta*/
		for(l=1;l<=strains;l++){
                                if(Aux<=Ninf[l]){
                                        break;};
                                if(Aux>Ninf[l]){
                                        Aux=Aux-Ninf[l];};};
		state[strains][Ni[l][Aux]]=1;
		state[l][Ni[l][Aux]]=2;/*The node recovers from the previous strain*/
		Ninf[strains]=1;
		Ni[strains][1]=Ni[l][Aux];
		Nedg[strains]=0;
		for(j=Pi[Ni[strains][1]];j<=Pf[Ni[strains][1]];j++){
			control = true;
			for(a=1;a<=strains-1;a++){/*Iterate over neighbors and check that they are not infected before adding the edge*/
				if(state[a][con[j]]==1){
					control=false;
					break;};};
			if(control==true){/*The neighbor is not infected, so activate the edge for the new strain*/
				Nedg[strains]++;
				Nedg[0]++;
				Ea[strains*2-1][Nedg[strains]]=Ni[strains][1];
				Ea[2*strains][Nedg[strains]]=con[j];
				pos[strains][j]=Nedg[strains];
				if(state[l][con[j]]==0){/*Remove the active edge from the previous strain*/
					Nedg[l]--;
					Nedg[0]--;
                                        for(k=Pi[Ea[2*l-1][Nedg[l]+1]];k<=Pf[Ea[2*l-1][Nedg[l]+1]];k++){/*Find the last edge in the position array for the swap*/
                                                if(con[k]==Ea[2*l][Nedg[l]+1]){
                                                        break;};};
                                        pos[l][k]=pos[l][j];/*intercanviem les posicions*/
                                        Ea[2*l-1][pos[l][k]] = Ea[2*l-1][Nedg[l]+1];
                                        Ea[2*l][pos[l][k]] = Ea[2*l][Nedg[l]+1];
                                        };};};

		/*	printf("%d %lf \n",strains, sim_time);*/
			Ni[l][Aux] = Ni[l][Ninf[l]]; /*replace the selected node with the last vector element to remove it from vector l*/
                        Ninf[l]--;
			if(Ninf[l]==0){/*A strain has gone extinct*/
                                Ninf[l]=Ninf[strains];
                                Nedg[l]=Nedg[strains];
                                Ninf[strains]=0;
                                Nedg[strains]=0;
                                for(j=1;j<=max;j++){
                                        state[l][j]=state[strains][j];
                                        state[strains][j]=0;};
                                for(j=1;j<=Ninf[l];j++){
                                        Ni[l][j]=Ni[strains][j];}
                                for(j=1;j<=Nedg[l];j++){
                                        Ea[2*l-1][j]=Ea[2*strains-1][j];
                                        Ea[2*l][j]=Ea[2*strains][j];};
                                for(j=1;j<=total_degree;j++){
                                        pos[l][j]=pos[strains][j];};

				strains--;
		/*		 printf("%d  %lf \n",strains, sim_time);*/
				if(strains==MAX-1){/*Reactivate the mutation capacity of the strains*/
					Ni[0][1]=Ni[0][0];};};
			if(strains == MAX){/*Temporarily disable mutation capacity*/
				Ni[0][1]=0;};};
		if(sim_time >LIMIT){
			Ni[0][0]=0;
			extin++;
		/*	printf("El virus no s'extingeix\n");*/};};
	if(sim_time>100){
		cien++;};};
	fprintf(fases, "%.15f %.15f %.15lf \n", gamma_start+gamm*gamma_step, cien/(double)simulations, extin/(double)simulations);
	printf("%.15f %.15f %.15lf \n", gamma_start+gamm*gamma_step, cien/(double)simulations, extin/(double)simulations);
/*	if(sim_time<LIMIT){
		printf("The virus disappears at sim_time = %lf \n",sim_time);};*/		};
	free(con);
	free(Pi);
	free(Pf);
	free(degree);
	free(Ninf);
	free(Nedg);
	for(i=0;i<=MAX;i++){
		free(state[i]);};
	free(state);
	for(i=0;i<=MAX;i++){
		free(Ni[i]);};
	free(Ni);
	for(i=0;i<=MAX;i++){
		free(pos[i]);};
	free(pos);
	for(i=0;i<=2*MAX;i++){
		free(Ea[i]);};
	free(Ea);

	return 0;}
