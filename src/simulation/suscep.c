#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define DEL 1
#define GAM 0
#define MAX 1
#define RANG 0.15
#define LIMIT 100
#define simu 5000

	int main(void){
	
	int n, *con, *Pi, *Pf, *grad, **estat, **Ni, **pos, **Ea, i, simulacions, ori, j, k, l, max, suma, gmax, ni, *Ninf, *Nedg, Aux, cepas, a, lamb, recu;
	FILE *dades, *Pc, *Clust, *Knngraf, *Evol, *fases;
	char dad[20];
	double PC, KNN, CLUST, aux, vec[simu+1], vecua[simu+1], aux2, temps, taux, LAM=0, suscep, esp, espcua;
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
	printf("La xarxa té %d nodes i %d links \n", max, n);
	con = (int *)malloc((2*n+1)*sizeof(int));  /*Reservem memòria dinàmica pels vectors*/
	Pi = (int *)malloc((max+1)*sizeof(int));
	Pf = (int *)malloc((max+1)*sizeof(int));
	grad = (int *)malloc((max+1)*sizeof(int));
	Ninf = (int *)malloc((MAX+1)*sizeof(int));
	Nedg = (int *)malloc((MAX+1)*sizeof(int));
	estat = (int **)malloc((MAX+1)*sizeof(int*));
		for(i=0;i<=MAX;i++){
                        estat[i] = (int *)malloc((max+1)*sizeof(int));};
        Ni = (int **)malloc((MAX+1)*sizeof(int*));
		for(i=0;i<=MAX;i++){
                        Ni[i] = (int *)malloc((max+1)*sizeof(int));};

	for(i=1;i<=MAX;i++){
		for(j=1;j<=max;j++){
			estat[i][j]=0;};};
	if(con==NULL || Pi==NULL || Pf==NULL || grad==NULL ||estat ==NULL||Ni==NULL){
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

	pos = (int **)malloc((MAX+1)*sizeof(int*));
                for(i=0;i<=MAX;i++){
                        pos[i] = (int *)malloc((suma+1)*sizeof(int));};
        Ea = (int **)malloc((2*MAX+1)*sizeof(int*));
                for(i=0;i<=2*MAX;i++){
                        Ea[i] = (int *)malloc(((suma/2)+1)*sizeof(int));};
	if(Ea==NULL||pos==NULL){
		printf("problemes de meòria \n");
		exit(3);};
	printf("Després de eliminar repeticions i loops queden %d links \n", suma/2);

	fases=fopen("suscep.dat", "w");

/*	gmax=0; 	
	Pc = fopen("Pc.dat", "w");
	if(Pc==NULL){
		printf("error en obrir el fitxer\n");};
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
                printf("error en obrir el fitxer\n");};

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
                printf("error en obrir el fitxer\n");};
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
	/*Omplim els vector d'estats, nodes infectats i links actius*/
		  /*0=susceptible, 1=infectat, 2=recuperat*/
	srand(time(NULL));
	LAM=1.0;
	for(lamb=1;lamb<=200;lamb++){
		LAM=LAM+0.1;
		recu=0;
	for(simulacions=1;simulacions<=simu;simulacions++){
	for(i=0;i<=MAX;i++){
		Ninf[i]=0; /*Aquesta variable contarà el nombre d'infectats*/
		Nedg[i]=0;}; /*Aquesta contarà el nombre de links actius*/
	temps=0;
	recu=0;
	for(i=1;i<=MAX;i++){
                for(j=1;j<=max;j++){
                        estat[i][j]=0;};};
	i=ori;
/*	printf("%d \n", i);
	free(grad);*/
	estat[1][i]=1; /*aquest serà el primer infectat*/
	Ninf[1]++;
	Ni[1][1]=i;
	Ni[0][0]=1;/*Infectats totals*/
	cepas=1;
	if(cepas!=MAX){
		Ni[0][1]=1; /*Variable per controlar les mutacions*/
	}	else{
		Ni[0][1]=0;
	}
	taux=0;
	for(k=Pi[i];k<=Pf[i];k++){/*{Activem tots els links del primer infectat*/
		Nedg[1]++;
		Nedg[0]++;
		Ea[1][Nedg[1]]=i;
		Ea[2][Nedg[1]]=con[k];
		pos[1][k]=Nedg[1];};
	/*Comencem l'iteració d'events d'infecció-recuperació*/
/*	Evol = fopen("Evol.dat", "w");
        if(Evol==NULL){
                printf("error en obrir el fitxer\n");};Aquí anotem el nombre d'infectats en funció del temps*/
/*	fprintf(Evol, "0  1 \n");*/
	while(Ni[0][0] != 0){
		if(Ni[0][0]>max||Ni[0][0]<0){
			printf("error del programa por nodos en la cepa %d \n",l);
			exit(3);};
		if(cepas>MAX||cepas<0){
            	    printf("error del programa por cepas\n");
                    exit(1);};
	/*	if(taux>=RANG){
                        taux=0;
			fprintf(Evol, "%lf %d ", temps, Ni[0][0]);
			for(i=1;i<=MAX;i++){
                        	fprintf(Evol, "%d ", Ninf[i]);};
			fprintf(Evol, "\n");};*/
		aux = rand()/(double)RAND_MAX;
		temps = temps - log(aux)/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0]); /*Temps en què ocorreix el següent esdeveniment*/
		taux = taux - log(aux)/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0]);
		aux2 = rand()/(double)RAND_MAX;
	        suma = 0;
		if(aux2 < (Nedg[0]*LAM)/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0])){ /*Tenim una infecció provicada per la cepa l*/
			Aux = 1 + rand()%Nedg[0]; /*Agafem un link aleatori com provocador de l'infecció*/
			if(Nedg[0]>2*n|| Nedg[0]<0){
	                	printf("error del programa por aristas\n");
        		        exit(2);};
			for(l=1;l<=cepas;l++){
				if(Aux<=Nedg[l]){
					break;};
				if(Aux>Nedg[l]){
					Aux=Aux-Nedg[l];};};
			Ninf[l]++;
			Ni[0][0]++;
			if(cepas!=MAX){
				Ni[0][1]++;};
			Ni[l][Ninf[l]] = Ea[2*l][Aux];/*Afegim el node susceptible d'aquesta aresta al vector d'infectats*/
			estat[l][Ea[2*l][Aux]] = 1;
			for(i=Pi[Ni[l][Ninf[l]]];i<=Pf[Ni[l][Ninf[l]]];i++){ /*recorrem tots els veins del node infectat*/
				control=true;
				for(a=1;a<=cepas;a++){
					if(estat[a][con[i]] == 1){
						control = false;/*Comprovem que el node no estigui infectat per cap cepa*/
						break;};};
				if(estat[l][con[i]]==0 && control ==true){/*Afegim aresta a la llista si el veí es susceptible*/
					Nedg[l]++;
					Nedg[0]++;
					pos[l][i]=Nedg[l];
					Ea[2*l-1][Nedg[l]]=Ni[l][Ninf[l]];
					Ea[2*l][Nedg[l]]=con[i];};
				
				if((control==false && estat[a][Ni[l][Ninf[l]]]==0)||a==l){/*eliminem l'aresta de la llista si el veí està infectat de la cepa a i el node que he infectat era susceptible*/
					Nedg[a]--;
					Nedg[0]--;
					for(j=Pi[Ea[2*a-1][Nedg[a]+1]];j<=Pf[Ea[2*a-1][Nedg[a]+1]];j++){/*Busquem l'última aresta al vector de posicions pel canvi*/
						if(con[j]==Ea[2*a][Nedg[a]+1]){
							break;};};
					for(k=Pi[con[i]];k<=Pf[con[i]];k++){/*Busquem la posició de l'aresta que volem eliminar*/
						if(con[k]==Ni[l][Ninf[l]]){
							break;};};
					pos[a][j]=pos[a][k];/*intercanviem les posicions*/
					Ea[2*a-1][pos[a][j]] =
					     	Ea[2*a-1][Nedg[a]+1];
					Ea[2*a][pos[a][j]] = Ea[2*a][Nedg[a]+1];};};
			continue;};
		if((aux2 < ((Nedg[0]*LAM) + (Ni[0][0]*DEL))/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0])) && aux2>(Nedg[0]*LAM)/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0])){/*Tenim una recuperació de la cepa l*/
			Aux = 1 + rand()%Ni[0][0]; /*Triem un node infectat aleatòriament per què es recuperi*/
			recu++;
			for(l=1;l<=cepas;l++){
				if(Aux<=Ninf[l]){
					break;};
				if(Aux>Ninf[l]){
					Aux=Aux-Ninf[l];};};
			estat[l][Ni[l][Aux]]=2;
			for(i=Pi[Ni[l][Aux]];i<=Pf[Ni[l][Aux]];i++){/*recorrem tots els veins del node recuperat*/
				control=true;
				for(a=1;a<=cepas;a++){
					if(estat[a][con[i]]==1){
						control=false;
						break;};};
				if(estat[l][con[i]]==0 && control==true){ /*Eliminem un link que abans era actiu*/
					Nedg[l]--;
					Nedg[0]--;
                                        for(j=Pi[Ea[2*l-1][Nedg[l]+1]];j<=Pf[Ea[2*l-1][Nedg[l]+1]];j++){/*Busquem l'última aresta al vector de posicions pel canvi*/
                                                if(con[j]==Ea[2*l][Nedg[l]+1]){
                                                        break;};};
                                        pos[l][j]=pos[l][i];/*intercanviem les posicions*/
                                        Ea[2*l-1][pos[l][j]] = Ea[2*l-1][Nedg[l]+1];
                                        Ea[2*l][pos[l][j]] = Ea[2*l][Nedg[l]+1];
                                        };
				if(control==false && estat[a][Ni[l][Aux]]==0){/*El node recuperat de la cepa l té un veí infectat per la cepa a i és susceptible. Afegim aresta*/
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
			if(cepas!=MAX){
				Ni[0][1]--;};
			if(Ninf[l]==0){/*S'ha extingit una cepa*/
				Ninf[l]=Ninf[cepas];
				Nedg[l]=0;
				Ninf[cepas]=0;
				Nedg[cepas]=0;
				for(j=1;j<=max;j++){
                                        estat[l][j]=estat[cepas][j];
					estat[cepas][j]=0;};
                                for(j=1;j<=Ninf[l];j++){
                                        Ni[l][j]=Ni[cepas][j];
					for(k=Pi[Ni[l][j]];k<=Pf[Ni[l][j]];k++){
						control = true;
						for(a=1;a<=cepas;a++){
							if(estat[a][con[k]]==1){
								control = false;};};
						if(control==true && estat[l][con[k]]==0){
							Nedg[l]++;
							Ea[2*l-1][Nedg[l]]=Ni[l][j];
							Ea[2*l][Nedg[l]]=con[k];
							pos[l][k]=Nedg[l];};};};
				cepas--;
	/*			 printf("%d  %lf \n",cepas, temps);*/
				if(cepas==MAX-1){/*Reactivem la capacitat de mutació de les cepes*/
					Ni[0][1]=Ni[0][0];};};
			continue;};

		if(aux2 > ((Nedg[0]*LAM) + (Ni[0][0]*DEL))/(double)(DEL*Ni[0][0]+GAM*Ni[0][1]+LAM*Nedg[0])){/*Tenim una mutació en un individu infectat amb la cepa l*/
		cepas++;	
		Aux = 1 + rand()%Ni[0][0]; /*Seleccionem aleatòriament el node que muta*/
		for(l=1;l<=cepas;l++){
                                if(Aux<=Ninf[l]){
                                        break;};
                                if(Aux>Ninf[l]){
                                        Aux=Aux-Ninf[l];};};
		estat[cepas][Ni[l][Aux]]=1;
		estat[l][Ni[l][Aux]]=2;/*EL node es recupera de la cepa anterior??*/
		Ninf[cepas]=1;
		Ni[cepas][1]=Ni[l][Aux];
		Nedg[cepas]=0;
		for(j=Pi[Ni[cepas][1]];j<=Pf[Ni[cepas][1]];j++){
			control = true;
			for(a=1;a<=cepas-1;a++){/*Recorrem els veins i mirem que no estiguin infectats per afegir el link*/
				if(estat[a][con[j]]==1){
					control=false;
					break;};};
			if(control==true){/*El veí no està infectat i activem el link per la cepa nova*/
				Nedg[cepas]++;
				Nedg[0]++;
				Ea[cepas*2-1][Nedg[cepas]]=Ni[cepas][1];
				Ea[2*cepas][Nedg[cepas]]=con[j];
				pos[cepas][j]=Nedg[cepas];
				if(estat[l][con[j]]==0){/*Eliminem el link actiu de la cepa anterior*/
					Nedg[l]--;
					Nedg[0]--;
                                        for(k=Pi[Ea[2*l-1][Nedg[l]+1]];k<=Pf[Ea[2*l-1][Nedg[l]+1]];k++){/*Busquem l'última aresta al vector de posicions pel canvi*/
                                                if(con[k]==Ea[2*l][Nedg[l]+1]){
                                                        break;};};
                                        pos[l][k]=pos[l][j];/*intercanviem les posicions*/
                                        Ea[2*l-1][pos[l][k]] = Ea[2*l-1][Nedg[l]+1];
                                        Ea[2*l][pos[l][k]] = Ea[2*l][Nedg[l]+1];
                                        };};};

			printf("%d %lf \n",cepas, temps);
			Ni[l][Aux] = Ni[l][Ninf[l]]; /*substituim el node seleccionat pel últim al vector per eliminar-ho del vector l*/
                        Ninf[l]--;
			if(Ninf[l]==0){/*S'ha extingit una cepa*/
				Ninf[l]=Ninf[cepas];
				Nedg[l]=0;
				for(j=1;j<=max;j++){
					estat[l][j]=estat[cepas][j];
					estat[cepas][j]=0;};
				for(j=1;j<=Ninf[l];j++){
					Ni[l][j]=Ni[cepas][j];
                                        for(k=Pi[Ni[l][j]];k<=Pf[Ni[l][j]];k++){
                                                control = true;
                                                for(a=1;a<=cepas;a++){
                                                        if(estat[a][con[k]]==1){
                                                                control = false;};};
                                                if(control==true && estat[l][con[k]]==0){
                                                        Nedg[l]++;
                                                        Ea[2*l-1][Nedg[l]]=Ni[l][j];
                                                        Ea[2*l][Nedg[l]]=con[k];
                                                        pos[l][k]=Nedg[l];};};};
				cepas--;
				 printf("%d  %lf \n",cepas, temps);
				if(cepas==MAX-1){/*Reactivem la capacitat de mutació de les cepes*/
					Ni[0][1]=Ni[0][0];};};
			if(cepas == MAX){/*Eliminem temporalment la capacitat de mutació*/
				Ni[0][1]=0;};};
		if(temps >LIMIT){
			Ni[0][0]=0;
		/*	printf("El virus no s'extingeix\n");*/};};
	vec[simulacions]=recu;};
	esp=0;
	espcua=0;
	for(i=1;i<=simu;i++){
		esp=esp+vec[i]/(double)simu;
		espcua=espcua+vec[i]*vec[i]/(double)simu;};
	suscep=(espcua-esp*esp)/(double)esp;
	fprintf(fases, "%lf %lf \n", LAM,  suscep);
	
	printf( "%lf %lf \n", LAM, suscep);

/*	if(temps<LIMIT){
		printf("El virus desapareix a temps = %lf \n",temps);};*/		};
	fclose(fases);
	free(con);
	free(Pi);
	free(Pf);
	free(grad);
	free(Ninf);
	free(Nedg);
	for(i=0;i<=MAX;i++){
		free(estat[i]);};
	free(estat);
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
