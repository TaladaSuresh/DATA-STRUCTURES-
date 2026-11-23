#include <stdio.h>
#include <limits.h>
#define V 6
#define INF INT_MAX
static void dijkstra(const int w[V][V], int source) {
    int d[V], done[V]={0};
    for (int i=0;i<V;i++) d[i]=INF;
    d[source]=0;
    for (int r=0;r<V;r++) {
        int u=-1;
        for (int i=0;i<V;i++) if (!done[i] && d[i]!=INF && (u==-1 || d[i]<d[u])) u=i;
        if (u==-1) break;
        done[u]=1;
        for (int v=0;v<V;v++) if (!done[v] && w[u][v]>0 && d[u] <= INT_MAX-w[u][v] && d[u]+w[u][v]<d[v]) d[v]=d[u]+w[u][v];
    }
    printf("Distances from %d: ",source); for(int i=0;i<V;i++) if(d[i]==INF) printf("INF "); else printf("%d ",d[i]); printf("\n");
}
static void floyd(int a[V][V]) { for(int k=0;k<V;k++)for(int i=0;i<V;i++)for(int j=0;j<V;j++) if(a[i][k]!=INF && a[k][j]!=INF && a[i][k]+a[k][j]<a[i][j]) a[i][j]=a[i][k]+a[k][j]; }
int main(void) {
    int unreachable[V][V]={{0,4,0,0,0,0},{4,0,2,0,0,0},{0,2,0,0,0,0},{0,0,0,0,3,0},{0,0,0,3,0,0},{0,0,0,0,0,0}};
    printf("Case 1: unreachable vertices (INF means no route)\n"); dijkstra(unreachable,0);
    int neg[V][V]={{0,5,2,0,0,0},{0,0,0,0,0,0},{0,-4,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
    printf("Case 2: negative-edge example\n"); printf("Dijkstra is not valid for negative weights; edge 2->1 (-4) makes route 0->2->1 cost -2, while direct 0->1 costs 5.\n");
    int fw[V][V]; for(int i=0;i<V;i++)for(int j=0;j<V;j++)fw[i][j]=(i==j?0:(neg[i][j]?neg[i][j]:INF)); floyd(fw);
    printf("Floyd-Warshall row 0: "); for(int j=0;j<V;j++)if(fw[0][j]==INF)printf("INF ");else printf("%d ",fw[0][j]); printf("\n");
    printf("Case 3: overflow-safe relaxation uses dist[u] <= INT_MAX - weight before addition.\n");
    printf("Case 4: cycle detection in topological sort\n");
    int edges[4][2]={{0,1},{1,2},{2,3},{3,1}}, indeg[4]={0}, q[4],h=0,t=0,done=0;
    for(int i=0;i<4;i++) indeg[edges[i][1]]++;
    for(int i=0;i<4;i++) if(!indeg[i]) q[t++]=i;
    while(h<t){int u=q[h++];done++;for(int i=0;i<4;i++)if(edges[i][0]==u&&--indeg[edges[i][1]]==0)q[t++]=edges[i][1];}
    printf("%s\n",done==4?"valid DAG":"cycle detected"); return 0;
}
