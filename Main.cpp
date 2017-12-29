#include <cstdio>
#include <iostream>
#include "BCube.h"
using namespace std;
#define BW 1000
int main(){
	/*
	BCube* bcube  = new BCube(4,1,1000,1000);
	bcube->ConstructBCube(1600);
	printf("Construct OK\n");
	bcube->ParaSync();
	**/
	int para_num;
	int p = 0;
	int s = 0;
	int n = 3;
	int k = 1;
	int N = 9;
	int **sendMatrix = new int*[N];
	int i = 0;
	int j = 0;
	for(i = 0; i<N;i++){
		sendMatrix[i] = new int[N];
	}
	Utils::getScatterMatrix(p, s, n, k, N, sendMatrix, para_num);
	//Utils::getGatherMatrix(p, s, n, k, N, sendMatrix, para_num);
	printf("para_num = %d\n",para_num );
	for(i = 0; i<N; i++){
		for(j = 0; j < N; j++){
			printf("%d ", sendMatrix[i][j] );
		}
		printf("\n");
	}


}