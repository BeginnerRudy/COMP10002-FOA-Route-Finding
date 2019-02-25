#include<stdio.h> 
#include<stdlib.h>

#define MAX 999 /* the street cannot go through */
#define PATHDIR 4 /* 4 directions. N E S W */
#define MAXTARGET 10 /* maximum number of targets */
#define RIGHT 0 
#define UP 1 
#define LEFT 2 
#define DOWN 3

/*---------------------------------------------------------------------*/
/*                     Author: Qilu Yu (784128)                        */

void readInput(int *row,int *col,int *allLine,int **map,char **index,
	int *notUsedNum,int *totalCost,char **target,int *targetNum);

void resultS1(int *col,int *row,int *notUsedNum,int *totalCost,
	int *targetNum,char **target);

void PathS2(int b,int e,int *dist,int* path,int *col);

void showS2(int *targetNum,char **target,char *start,
	int *dist,int *path,int *col);

void ShortestPath_DJ(int **G,char *start,int *dist,
	int *path,int *col,int *allLine);

void resultS2(int *allLine,int **mapEdges,int **map,
	char **target,int *targetNum,int*col);

void showS3(int *dist,int *path,int *col,int *row,int *allLine);

void resultS3(int *targetNum,int *allLine,int **mapEdges,
	char **target,int *col,int *row);

/*---------------------------------------------------------------------*/

void readInput(int *row,int *col,int *allLine,int **map,char **index,
	int *notUsedNum,int *totalCost,char **target,int *targetNum)
{  
	int i,j;
	for(i=0;i<*allLine;i++){
		map[i]=(int *)malloc(PATHDIR *sizeof(int));	
		index[i]=(char *)malloc(PATHDIR *sizeof(char));			
	}

	for(i=0;i<*allLine;i++){
		scanf("%s",index[i]);
		for(j=0;j<PATHDIR;j++){
			scanf("%d",&map[i][j]);	
			if(map[i][j]==MAX){
				*notUsedNum=*notUsedNum+1;
			}else{
				*totalCost+=map[i][j];
			}
		}
	}
	while(1){
		target[*targetNum]=(char *)malloc(PATHDIR*sizeof(char));
		scanf("%s",target[*targetNum]);

		if(target[*targetNum][0]>='0' && target[*targetNum][0]<='9')
			/* make sure the input char is numerical */
		{
			*targetNum=*targetNum+1;
		}
		if(feof(stdin))
			break;
	}
}
/*---------------------------------------------------------------------*/

void resultS1(int *col,int *row,int *notUsedNum,int *totalCost,
	int *targetNum,char **target){
	printf("S1: grid is %d x %d, and has %d intersections\n",*col,
		*row,(*row)*(*col));
	printf("S1: of %d possibilities, %d of them cannot be used\n",
		(*row)*(*col)*PATHDIR,*notUsedNum);
	printf("S1: total cost of remaining possibilities is %d seconds\n",
		*totalCost);
	printf("S1: %d grid locations supplied, first one is %s, last one is %s\n",
		*targetNum,target[0],target[*targetNum-1]);

}
/*---------------------------------------------------------------------*/
void PathS2(int b,int e,int *dist,int* path,int *col){
	char r='a',c='0';
	if(e!=b){
		PathS2(b,path[e],dist,path,col);
		r+=e/(*col);
		c+=e%(*col);
		printf("S2: \t  then to %c%c, cost of %d\n",c,r,dist[e]);
	}
}
/*---------------------------------------------------------------------*/

void showS2(int *targetNum,char **target,char *start,int *dist,
	int *path,int *col)
{
	int n;
	for(n=1;n<*targetNum;n++){
		int i=(target[n][1]-'a')*(*col)+(target[n][0]-'0');
		int v=(start[1]-'a')*(*col)+(start[0]-'0');
		printf("S2: start at grid %s, cost of 0\n",target[0]);
		PathS2(v,i,dist,path,col);
	}	
}

/*---------------------------------------------------------------------*/

void ShortestPath_DJ(int **G,char *start,int *dist,int *path,
	int *col,int *allLine)
{  
	int v=(start[1]-'a')*(*col)+(start[0]-'0');
	int i,j,k,min;  
	int *final=	(int *)malloc((*allLine)*sizeof(int));
	/* to indicate if the best path was found for this point */

	for(i=0;i<*allLine;i++)  
	{
		
		dist[i]=G[v][i];
		/* initialize */

		if(dist[i]<MAX) 
		{
			path[i]=v;/* update the path by storing the grid coordinates*/ 

		}
		final[i]=0;/* not found */
	}  

	dist[v]=0; /* initialize */
	final[v]=1; 

	for(j=0;j<*allLine-1;j++)
	{  
		min=MAX;  
		for(i=0;i<*allLine;i++)  
		{
			if(dist[i]<min && final[i]==0)  
			{  
				min=dist[i]; /* sotre the min distance */
				k=i;  /* sotre the position */
			} 
		}

		final[k]=1; 

		for(i=0;i<*allLine;i++)  {
			/* go through each point and the path currently found */   
			if(dist[i]>=dist[k]+G[k][i] && final[i]==0)  
			{  
				dist[i]=dist[k]+G[k][i];  
				path[i]=k;				
			}  
		}
	}
	free(final);
} 
/*---------------------------------------------------------------------*/

void resultS2(int *allLine,int **mapEdges,int **map,
	char **target,int *targetNum,int*col)
{
	int i,j;
	for(i=0;i<*allLine;i++){
		mapEdges[i]=(int *)malloc((*allLine)*sizeof(int));
		for(j=0;j<*allLine;j++){
			mapEdges[i][j]=MAX;/* initialize */
		}         
	} 
	for(i=0;i<*allLine;i++)  
	{  
		if(i+1<*allLine){
			mapEdges[i][i+1]=map[i][RIGHT];
		}

		if(i-*col>=0){
			mapEdges[i][i-*col]=map[i][UP];
		}

		if(i-1>=0){		
			mapEdges[i][i-1]=map[i][LEFT];
		}

		if(i+*col<*allLine){
			mapEdges[i][i+*col]=map[i][DOWN];
		}
	} 

	int *dist=(int *)malloc((*allLine)*sizeof(int)); 
	/* dist store the min distance from v to other points*/
	int *path=(int *)malloc((*allLine)*sizeof(int));

	ShortestPath_DJ(mapEdges,target[0],dist,path,col,allLine); 
	showS2(targetNum,target,target[0],dist,path,col);
	free(dist);
	free(path);

}

/*---------------------------------------------------------------------*/

void showS3(int *dist,int *path,int *col,int *row,int *allLine)
{
	int i,j;
	printf("S3:");
	for(i=0;i<*col;i++){
		printf("%9d",i);
	}
	printf("\n");
	printf("S3:   +----+");
	for(i=1;i<(*col);i++){
		printf("--------+");
	}
	printf("\n");

	for(i=0;i<(*row);i++){
		printf("S3: %c |  ",'a'+i);	
		for(j=0;j<*col;j++){
			printf("%3d",dist[i*(*col)+j]);
			if(path[i*(*col)+j]==i*(*col)+j+1){
				printf(" <<<< ");/* go left */				
			}else if(i*(*col)+j+1<*allLine && path[i*(*col)+j+1]
				==i*(*col)+j){
				printf(" >>>> ");/* go right */				
			}else{
				printf("%6c",' ');			
			}
		}
		printf("\n");
		if(i==(*row)-1){
			break;
		}
		int k=0;
		while(k++<2)
		{
			printf("S3:   |  ");
			for(j=0;j<(*col);j++){
				if(path[i*(*col)+j]==(i+1)*(*col)+j)
				{
					printf("%3c",'^');
					/* go up */	
				}else if((i+1)*(*col)+j<*allLine && path[(i+1)*(*col)+j]
					==i*(*col)+j){						
					printf("%3c",'v');
					/* go down */	
				}else{
					printf("%3c",' ');
				}
				printf("%6c",' ');
			}
			printf("\n");
		}
	}
}
/*---------------------------------------------------------------------*/

void resultS3(int *targetNum,int *allLine,int **mapEdges,
	char **target,int *col,int *row)
{
	int **dist=(int **)malloc((*targetNum)*sizeof(int*));
	int **path=(int **)malloc((*targetNum)*sizeof(int*));
    int i,j;
	for(i=0;i<*targetNum;i++){
		dist[i]=(int *)malloc((*allLine)*sizeof(int));
		path[i]=(int *)malloc((*allLine)*sizeof(int));
		/* calculate the best path for each point */

		ShortestPath_DJ(mapEdges,target[i],dist[i],path[i],col,allLine); 
	}

	int *minPath=(int *)malloc((*allLine)*sizeof(int));
	int *minDist=(int *)malloc((*allLine)*sizeof(int));

	for(i=0;i<*allLine;i++){
		minPath[i]=path[0][i];
		minDist[i]=dist[0][i];


		for(j=1;j<*targetNum;j++){
			if(minDist[i]>=dist[j][i]){
				minPath[i]=path[j][i];
				minDist[i]=dist[j][i];
			}
		}	
	}
	showS3(minDist,minPath,col,row,allLine);

	for(i=0;i<*targetNum;i++){

		free(dist[i]);
		free(path[i]);
	}
	free(dist);
	free(path);
	free(minPath);
	free(minDist);
}
/*---------------------------------------------------------------------*/

int main(int argc,char*args[]){                 

	int row=0,col=0;
	int i;
	int **map=NULL;
	char **index=NULL;
	int allLine=0;
	char **target=NULL;
	int targetNum=0;
	int notUsedNum=0;
	int totalCost=0;
	int **mapEdges=NULL;

	scanf("%d %d",&col,&row);
	allLine=(row)*(col);

	map=(int **)malloc((allLine)*sizeof(int*));
	index=(char **)malloc((allLine)*sizeof(char*));
	target=(char **)malloc(MAXTARGET*sizeof(char*));

	readInput(&row,&col,&allLine,map,index,&notUsedNum,&totalCost,target,
		&targetNum);
	resultS1(&col,&row,&notUsedNum,&totalCost,&targetNum,target);
	mapEdges=(int **)malloc((allLine)*sizeof(int*));
	printf("\n");
	resultS2(&allLine,mapEdges,map,target,&targetNum,&col);
	printf("\n");
	resultS3(&targetNum,&allLine,mapEdges,target,&col,&row);

	for(i=0;i<allLine;i++){
		free(mapEdges[i]);
		free(map[i]);	
		free(index[i]);	

	}
	for(i=0;i<targetNum;i++)
	{
		free(target[i]);	
	}

	free(mapEdges);
	free(map);
	free(index);	
	free(target);	
	return 0;
}
/*---------------------------------------------------------------------*/
/*                         algorithms are fun!!!!!!                    */