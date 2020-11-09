#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 500
#define M 7


float D[N][2];
float w[M][2];
float wOld[M][2];
int O[N];

void readFromFile(){
    FILE *infile;
	infile = fopen("points.txt","r");
    if(infile == NULL){
        printf("Error on file\n");
        exit(0);
    }
    for(int i = 0; i < N; i++){
        fscanf(infile,"%f %f", &D[i][0], &D[i][1]);
    }
    fclose(infile);
}

void initW(){
    int k, j;

    for(int i = 0; i < M; i++){
        do{
			k = rand()%N;
            w[i][0] = D[k][0];
            w[i][1] = D[k][1];
            for(j = 0; j < i; j++){
                if(w[i][0] == w[j][0] && w[i][1] == w[j][1]){
                    break;
                }
            }

        }while(j < i);
    }
}

void copyW(){
    for(int i = 0; i < M; i++){
        wOld[i][0] = w[i][0];
        wOld[i][1] = w[i][1];
    }
}

int checkFinish(){
    for(int i = 0; i < M; i++){
        if(fabs(wOld[i][0] - w[i][0]) > 0.0001 || fabs(wOld[i][1] - w[i][1]) > 0.0001){
            return 0;
        }
    }
    return 1;
}

void findNewGroup(int n){
    float dMin, d;
    dMin = sqrt(pow(D[n][0] - w[0][0],2) + pow(D[n][1] - w[0][1],2));
    O[n] = 0;
    for(int i = 1; i < M; i++){
        d = sqrt(pow(D[n][0] - w[i][0],2) + pow(D[n][1] - w[i][1],2));
        if(d < dMin){
            dMin = d;
            O[n] = i;
        }
    }
}

void findNewW(){
    int counter[M];
    for(int i = 0; i < M; i++){
        w[i][0] = 0;
        w[i][1] = 0;
        counter[i] = 0;
    }
    for(int i = 0; i < N; i++){
        w[O[i]][0] += D[i][0];
        w[O[i]][1] += D[i][1];
        counter[O[i]] ++;
    }
    for(int i = 0; i < M; i++){
        w[i][0] /= (float)counter[i];
        w[i][1] /= (float)counter[i];
    }
}

float diaspora(){   #Variance
    float d = 0;

    for(int i = 0; i < N; i++){
        d += sqrt(pow(D[i][0] - w[O[i]][0],2) + pow(D[i][1] - w[O[i]][1],2));
    }
    return d;
}

void kmeans(){
    int t = 0;
    initW();
    copyW();
	printf("old centers: \n");
    for(int i = 0; i < M; i++){
        printf("%f %f\n",w[i][0], w[i][1]);
    }
    wOld[0][0] = 100;
    while(checkFinish() == 0){
        copyW();
        for(int i = 0; i < N; i++)
            findNewGroup(i);
        findNewW();
        t++;
    }
    printf("t=%d\n\n",t);
	printf("new centers: \n");
    for(int i = 0; i < M; i++){
        printf("%f %f\n",w[i][0], w[i][1]);
    }
    printf("Variance = %f\n",diaspora());
}

int main(){
    srand(time(NULL));
    readFromFile();
    kmeans();
    return 0;
}
