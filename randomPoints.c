#include <stdio.h>
#include <stdlib.h>

#define N 3000
#define M 100

float points[N][2];

void createRandomPoints(float min, float max){
    int i, j;
    for(i = 0; i < N; i++){
        do{
            points[i][0] = (rand()/(float)RAND_MAX)*(max-min) + min;
            points[i][1] = (rand()/(float)RAND_MAX)*(max-min) + min;

            for (j = 0; j < i; j++){
                if(points[i][0] == points[j][0] && points[i][1] == points[j][1]){
                    break;
                }
            }
            //when j == i means that it is a duplicate point
        }while(j < i);
    }
}

void createRandomPointsS2(float minx, float maxx, float miny, float maxy){
    int i, j;
    for(i = 0; i < M; i++){
        do{
            points[i][0] = (rand()/(float)RAND_MAX)*(maxx-minx) + minx;
            points[i][1] = (rand()/(float)RAND_MAX)*(maxy-miny) + miny;

            for (j = 0; j < i; j++){
                if(points[i][0] == points[j][0] && points[i][1] == points[j][1]){
                    break;
                }
            }
            //when j == i means it is a new point
        }while(j < i);
    }
}

void writeToFile(FILE *outfileTrain, FILE *outfileTest){
    int i;

    for(i = 0; i < N/2; i++){
        fprintf(outfileTrain,"%f %f\n", points[i][0], points[i][1]);
    }

    for(i = N/2; i < N; i++){
        fprintf(outfileTest,"%f %f\n", points[i][0], points[i][1]);
    }
}

void writeToFileS2(FILE *outfile){
    int i;

    for(i = 0; i < M; i++){
        fprintf(outfile,"%f %f\n", points[i][0], points[i][1]);
    }

}
int main(){
    FILE *outfileTest, *outfileTrain, *outfile;

    srand(10);

    outfileTest = fopen("test.txt", "w");
    outfileTrain = fopen("train.txt", "w");

    if(outfileTest == NULL || outfileTrain == NULL){
        printf("Error on files\n");
        exit(0);
    }

    createRandomPoints(0,2);
    writeToFile(outfileTrain, outfileTest);
    createRandomPoints(-2,0);
    writeToFile(outfileTrain, outfileTest);

    fclose(outfileTest);
    fclose(outfileTrain);


    //s2

    outfile = fopen("points.txt", "w");
    if(outfile == NULL){
        printf("Error on files");
        exit(0);
    }

    createRandomPointsS2(-0.3, 0.3, -0.3, 0.3);
    writeToFileS2(outfile);

    createRandomPointsS2(-1.1, -0.5, 0.5, 1.1);
    writeToFileS2(outfile);

    createRandomPointsS2(-1.1, -0.5,-1.1, -0.5);
    writeToFileS2(outfile);

    createRandomPointsS2(0.5, 1.1, -1.1, -0.5);
    writeToFileS2(outfile);

    createRandomPointsS2(0.5, 1.1, 0.5, 1.1);
    writeToFileS2(outfile);

    fclose(outfile);
}
