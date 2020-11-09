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
float n = 0.1;

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

void findNewW(int i){
    w[O[i]][0] += n*(D[i][0] - w[O[i]][0]);
    w[O[i]][1] += n*(D[i][1] - w[O[i]][1]);
}

float diaspora(){
    float d = 0;
    for(int i = 0; i < N; i++){
        d += sqrt(pow(D[i][0] - w[O[i]][0],2) + pow(D[i][1] - w[O[i]][1],2));
    }
    return d;
}

void lvq(){
    int t = 0;
    initW();
    copyW();
	printf("palia kentra: \n");
    for(int i = 0; i < M; i++){
        printf("%f %f\n",w[i][0], w[i][1]);
    }
    wOld[0][0] = 100;
    while(checkFinish() == 0){
        copyW();
        for(int i = 0; i < N; i++){
            findNewGroup(i);
            findNewW(i);
        }
        n = 0.95*n;
        t++;
    }
    printf("t=%d\n\n",t);
	printf("nea kentra: \n");
    for(int i = 0; i < M; i++){
        printf("%f %f\n",w[i][0], w[i][1]);
    }
    printf("Diaspora= %f\n",diaspora());
}

int main(){
    srand(time(NULL));
    readFromFile();
    lvq();
    return 0;
}
