#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;
// this is ada's code
#define  STATUS_A 0
#define  STATUS_Z 1
#define  STATUS_AZ 2 

const int maxn=509340;	   //max nodes 
const int maxm=7178415;	   //max edges

int n,m;
vector<int> v[maxn];	//v[i] stores all edges that are connected vertex i 
vector<int> vv[maxn];	//vv[i] stores all vertices that are connected vertex i
int choose[maxn];		//choose [i] represents whether choose vertex i or not 
int Deg[maxn];		//the Deg of a vertex is how many edges that are incident to the vertex are uncovered 
int certain[maxn];     //certain[i] represents whether the state of vertex i is definite 
int edge[maxm][2];		//edge[i][0], edge[i][1] represents two nodes that are connected to edge i
bool wave[maxm][3];		//wave functions of vertices
/*for an edge i that is connected to vertex 1 and vertex 2, there are three status in wave[i]
wave[i][0]: [1]    (we choose vertex 1, but we do not choose vertex 2) 
wave[i][1]: [2]    (we choose vertex 2, but we do not choose vertex 1)
wave[i][2]: [1,2]  (we choose vertex 1 and vertex 2)
*/ 
int Entropy[maxm];	//Entropy[i] represents the entropy of wave[i], which means how many status are available in wave[i]
queue<int> q;// the queue is used to store vertices used for propagating 

void ban(int e,int status); 
/*  
get current time, for timing
*/
int getCurrentTime()  
{  
   struct timeval tv;  
   gettimeofday(&tv,NULL);  
   return tv.tv_sec *1000 + tv.tv_usec/1000;  
} 

/*  
Get the sum of the Degs of the neighboring nodes for a given vertex
*/
long long getNsupport(int node)
{ 
   long long iSum=0;
   for (int i=0; i<vv[node].size();i++)	
   {
   		iSum+=Deg[vv[node][i]]; 
   }
   return iSum;
}

/* 
After choosing some vertices, some more edges may be covered by these vertices  
Therefore, the Deg of every vertex should be changed after covering the edges 
When the maximum degree of a neighbor is 1, the node is automatically banned 
*/
void renewDeg(int node)
{
	Deg[node]=0;
	for(int i=0;i<v[node].size();i++)
	{
		for(int j=0;j<=1;j++)
		{
			int nei=edge[v[node][i]][j]; 
			if(choose[nei]==1) continue;	
			Deg[nei]--;
			if ( Deg[nei]==1 && getNsupport(nei)>1 )
			//if Deg[nei] equals 1, it must be better to choose vertex nei 
			{
				ban(v[node][i],2);
			}
		}
	}
	return;
}
/*
When propagating, we ban illegal status according to elements in the queue
*/
void Propagate()
{
	while(!q.empty())
	{
		int x=q.front();int y=choose[x]; q.pop();
		if(y==1) renewDeg(x);//update Deg of vertices 
		for(int i=0;i<v[x].size();i++)
		{
			int e=v[x][i];
			if(y==1)//choose y
			{
				if(edge[e][0]==x)
				{
					
				}
				if(edge[e][1]==x)
				{
					
				}
			}
			if(y==0)//not choose y
			{
				if(edge[e][0]==x)
				{
					ban(e,0);ban(e,2);
				}
				if(edge[e][1]==x)
				{
					ban(e,1);ban(e,2);
				}
			} 
		}
	}
}

bool hasCovered()
{
	for(int i=1;i<=n;i++) 
	{
		if(Deg[i]>0) return 0;
	}
	return 1;
}
/*
Compared to the solution to Red Maze, I do not use the notion of Entropy to decide. 
Based on ideas in others' essasys, I find the vertex with the smallest degree, and choose all its neighbour vertices
Since I choose all its neighbour vertices, there is no need to choose the vertex with least degree
*/ 
void Observe()
{
	int minDeg=0x3f3f3f3f; int minv=0;
	//priority_queue
	for(int i=1;i<=n;i++)
	{
		if(Deg[i]==0) continue;
		if(Deg[i]<minDeg)
		{
			minDeg=Deg[i];	
			minv=i;
			if(minDeg==1) break;
		} 
	}
	//find the vertex with the smallest Deg, minv  
	if(minv==0) return;	//all edges have been covered  
	
	for(int i=0;i<v[minv].size();i++)//����minv��������и��������ı� 
	{
		for(int j=0;j<=1;j++)
		{
			int Edge=v[minv][i];
			if(edge[Edge][j]==minv) //j ��minv 
			{	
				wave[Edge][2]=false;wave[Edge][j]=false;
				choose[minv]=0;choose[edge[Edge][1-j]]=1;
				q.push(minv);q.push(edge[Edge][1-j]);
			}
		}
	}
	
	return;
}

void ReadGraph()
{
	scanf("%d%d",&n,&m); //
	for(int i=1;i<=m;i++)
	{
		int x,y;scanf("%d%d",&x,&y);
		if(x>y) swap(x,y);
		Deg[x]++;Deg[y]++;
		edge[i][0]=x;edge[i][1]=y;
		v[x].push_back(i);v[y].push_back(i);  
		vv[x].push_back(y);vv[y].push_back(x);
	}    
} 
void PreProcess()
{
	for(int i=1;i<=m;i++) Entropy[i]=3;//initialize the entropy of every edge   
	for(int i=1;i<=m;i++)
	{
		for(int j=0;j<=2;j++)
		{
			wave[i][j]=true;
		}	
	}	
} 
int main()
{
//	freopen("johnson")
	freopen("C2000-9.in","r",stdin);		
	ReadGraph();  
	int start=getCurrentTime();	
	PreProcess(); 
	while(true)
	{
		if(hasCovered()) break; //all edges covered 
		Observe();
		Propagate();
	}
	
	int cnt=0;
	for(int i=1;i<=n;i++)
	{
		if(choose[i]) 
		{
		//	printf("%d ", i);
			cnt++;	
		}
	}
	printf("\n");
	printf("total number=%d\n",cnt);
	printf("seconds uesd=%.3f",(getCurrentTime()-start)*1.0/1000);  
	
	return 0;
} 
