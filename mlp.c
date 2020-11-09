#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define d 2
#define K 3
#define H1 8
#define H2 5
#define type 0
#define N 3000
#define L 1

float D[N][d], t[N][K];
float Y0[d], w01[H1][d+1], D0[d], de01[H1][d+1],
    Y1[H1], w12[H2][H1+1], u1[H1], D1[H1], de12[H2][H1+1],
    Y2[H2], w23[K][H2+1], u2[H2], D2[H2], de23[K][H2+1],
    Y3[K], u3[K], D3[K];
float n = 0.01;

void forward_pass(float *x, int d1, float *y, int K1){
    Y0[0] = x[0];
    Y0[1] = x[1];
    for(int i = 0; i < H1; i++){
        u1[i] = 0;
        for(int j = 1; j < d+1; j++){
            u1[i] = u1[i] + w01[i][j]*Y0[j-1];
        }
        u1[i] = u1[i] + w01[i][0];

        if(type == 0){
            Y1[i] = 1.0/(1.0+exp(-u1[i]));
        }
        else{
            Y1[i] = tanh(u1[i]);
        }
    }
    for(int i = 0; i < H2; i++){
        u2[i] = 0;
        for(int j = 1; j < H1+1; j++){
            u2[i] = u2[i] + w12[i][j]*Y1[j-1];
        }
        u2[i] = u2[i] + w12[i][0];
        if(type == 0){
            Y2[i] = 1.0/(1.0+exp(-u2[i]));
        }
        else{
            Y2[i] = tanh(u2[i]);
        }
    }
    for(int i = 0; i < K; i++){
        u3[i] = 0;
        for(int j = 1; j < H2+1; j++){
            u3[i] = u3[i] + w23[i][j]*Y2[j-1];
        }
        u3[i] = u3[i] + w23[i][0];
        Y3[i] = 1.0/(1.0+exp(-u3[i]));
    }
    for(int i = 0; i < K; i++){
        y[i] = Y3[i];
    }
}

void backprop(float *x, int d1, float *t, int K1){
    float y[K],sum;

    forward_pass(x, d, y, K);

    for(int i = 0; i < K; i++){
        D3[i] = y[i]*(1-y[i])*(y[i] - t[i]);
    }
    for(int i = 0; i < H2; i++){
        sum = 0;
        for(int j = 1; j < K+1; j++){
            sum += w23[j][i]*D3[j-1];
        }
        if(type == 0){
            D2[i] = sum*Y2[i]*(1-Y2[i]);
        }
        else{
            D2[i] = 1.0 - pow(tanh(u2[i]),2);
        }
    }
    for(int i = 0; i < H1; i++){
        sum = 0;
        for(int j = 1; j < H2+1; j++){
            sum += w12[j][i]*D2[j-1];
        }
        if(type == 0){
            D1[i] = sum*Y1[i]*(1-Y1[i]);
        }
        else{
            D1[i] = 1.0 - pow(tanh(u1[i]),2);
        }
    }

    for(int i = 0; i < H1; i++){
        for(int j = 1; j < d+1; j++){
            de01[i][j] += D1[i]*Y0[j-1];
        }
        de01[i][0] += D1[i];
    }
    for(int i = 0; i < H2; i++){
        for(int j = 1; j < H1+1; j++){
            de12[i][j] += D2[i]*Y1[j-1];
        }
        de12[i][0] += D2[i];
    }
    for(int i = 0; i < K; i++){
        for(int j = 1; j < H2+1; j++){
            de23[i][j] += D3[i]*Y2[j-1];
        }
        de23[i][0] += D3[i];
    }
}

void init(){
    for(int i = 0; i < H1; i++){
        for(int j = 0; j < d+1; j++){
            w01[i][j] = 2*(rand()/(float)RAND_MAX) -1;
        }
    }
    for(int i = 0; i < H2; i++){
        for(int j = 0; j < H1+1; j++){
            w12[i][j] = 2*(rand()/(float)RAND_MAX) -1;
        }
    }
    for(int i = 0; i < K; i++){
        for(int j = 0; j < H2+1; j++){
            w23[i][j] = 2*(rand()/(float)RAND_MAX) -1;
        }
    }
}

void update(){
    for(int i = 0; i < H1; i++){
        for(int j = 0; j < d+1; j++){
            w01[i][j] -= n*de01[i][j];
        }
    }
    for(int i = 0; i < H2; i++){
        for(int j = 0; j < H1+1; j++){
            w12[i][j] -= n*de12[i][j];
        }
    }
    for(int i = 0; i < K; i++){
        for(int j = 0; j < H2+1; j++){
            w23[i][j] -= n*de23[i][j];
        }
    }
}

void initDE(){
    for(int i = 0; i < H1; i++){
        for(int j = 0; j < d+1; j++){
            de01[i][j] = 0;
        }
    }

    for(int i = 0; i < H2; i++){
        for(int j = 0; j < H1+1; j++){
            de12[i][j] = 0;
        }
    }
    for(int i = 0; i < K; i++){
        for(int j = 0; j < H2+1; j++){
            de23[i][j] = 0;
        }
    }
}
void gradientDescent(){
    int counter = 0;
    float E = 0, oldE = 0;
    int i,j;
    float y[K];

    init();
    while(counter < 500 || fabs(E - oldE) > 0.00001){

        initDE();
        for(int i = 0; i < N; i++){
            backprop(D[i], d, t[i], K);
            if(i%L == 0){
                update();
                initDE();
            }
        }
        counter ++;
        update();
        oldE = E;
        E = 0;
        for(int i = 0; i < N; i++){
            forward_pass(D[i], d, y, K);
            for(int j = 0; j < K; j++){
                E += pow(y[j] - t[i][j],2);
            }
        }
        E = E/2.0;
        printf("Epoxh %d: E= %f, fabs= %f\n",counter,E,fabs(E - oldE));
    }

}
void readFromFile(char *filename){
    FILE *infile;
    infile = fopen(filename,"r");
    if(infile == NULL){
        printf("Error on file\n");
        exit(0);
    }
    for(int i = 0; i < N; i++){
        fscanf(infile,"%f %f", &D[i][0], &D[i][1]);
        for(int j = 0; j < K; j++){
            t[i][j] = 0;
        }
        if(pow(D[i][0] - 1, 2) + pow(D[i][1] - 1, 2) <= 0.16 || pow(D[i][0] + 1, 2) + pow(D[i][1] + 1, 2) <= 0.16){
            t[i][0] = 1;
        }
        else if(pow(D[i][0] - 1, 2) + pow(D[i][1] - 1, 2) > 0.16 &&  pow(D[i][0] - 1, 2) + pow(D[i][1] - 1, 2) < 0.64){
            t[i][1] = 1;
        }
        else if(pow(D[i][0] + 1, 2) + pow(D[i][1] + 1, 2) > 0.16 &&  pow(D[i][0] + 1, 2) + pow(D[i][1] + 1, 2) < 0.64){
            t[i][1] = 1;
        }
        else{
            t[i][2] = 1;
        }
    }
    fclose(infile);
}

void testMLP(){
    int correct = 0;
    float y[K];
    int pos;
    FILE *outfile1, *outfile2;

    outfile1 = fopen("correct.txt","w");
    outfile2 = fopen("noncorrect.txt","w");
    if(outfile1 == NULL || outfile2 == NULL){
		printf("Error on file\n");
        exit(0);
    }
    for(int i = 0; i < N; i++){
        forward_pass(D[i], d, y, K);
        pos = 0;
        for(int j = 1; j < K; j++){
            if(y[pos] < y[j])
                pos = j;
        }
        if(t[i][pos] == 1){
            correct++;
            fprintf(outfile1, "%f %f\n",D[i][0], D[i][1]);
        }
        else{
            fprintf(outfile2, "%f %f\n",D[i][0], D[i][1]);
        }
    }
    fclose(outfile1);
    fclose(outfile2);
    printf("Ability= %f\n", correct/(float)N);
    printf("Counter = %d\n", correct);
}
int main(){
    readFromFile("train.txt");
    gradientDescent();
    readFromFile("test.txt");
    testMLP();
    return 0;
}
