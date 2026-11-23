#include <stdio.h>
#include <limits.h>
#define V 20
#define INF INT_MAX
int cost[V][V];
void build_graph(void){for(int i=0;i<V;i++)for(int j=0;j<V;j++)cost[i][j]=0;for(int i=0;i<V-1;i++){cost[i][i+1]=cost[i+1][i]=(i%5)+2;}for(int i=0;i<V-5;i+=5)for(int j=i+5;j<V;j+=5)cost[i][j]=cost[j][i]=4+(i%3);for(int i=0;i<V;i++)for(int j=i+2;j<V;j+=3)if(!cost[i][j])cost[i][j]=cost[j][i]=7+(i+j)%6;}
int prim(int blockedA,int blockedB){int key[V],parent[V],in[V]={0},total=0;for(int i=0;i<V;i++){key[i]=INF;parent[i]=-1;}key[0]=0;for(int k=0;k<V;k++){int u=-1;for(int v=0;v<V;v++)if(!in[v]&&(u==-1||key[v]<key[u]))u=v;if(u==-1||key[u]==INF)return -1;in[u]=1;total+=key[u];for(int v=0;v<V;v++){int w=cost[u][v];if((u==blockedA&&v==blockedB)||(u==blockedB&&v==blockedA))w=0;if(!in[v]&&w>0&&w<key[v]){key[v]=w;parent[v]=u;}}}printf("MST edges: ");for(int v=1;v<V;v++)printf("%d-%d(%d) ",parent[v],v,((parent[v]==blockedA&&v==blockedB)||(parent[v]==blockedB&&v==blockedA))?0:cost[parent[v]][v]);printf("\nTotal cable cost = %d units\n",total);return total;}
int main(void){build_graph();printf("20-hub minimum spanning tree\n");int initial=prim(-1,-1);printf("\nAfter making cable 0-1 unavailable\n");int changed=prim(0,1);printf("\nPlan cost changed by %d units\n",changed-initial);return 0;}
