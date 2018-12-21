#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<vii> vvii;
typedef vector<int> vi;

vvii graph;
vvii tree;
priority_queue<ii> pq;
vi taken;

void process(int u)
{
	taken[u] = 1;
	int tam = (int) graph[u].size();
	for (int i=0; i < tam; ++i)
	{
		ii v = graph[u][i];
		if (!taken[v.first]) pq.push( ii(-v.second, -v.first) );
	}
}

void mst(int v)
{
	int u, w;
	
	int tam1 = (int) graph.size();
	int tam2 = (int) graph[0].size();

	process(v);
	while (!pq.empty())
	{
		ii front = pq.top(); pq.pop();
		u = -front.second;
		w = -front.first;
		if (!taken[u])
		{
			
			tree[v].push_back( ii(u, w) );
			tree[u].push_back( ii(v, w) );			
			
			process(u);
		}

	}
}


int main()
{
	
	int esp, c;
	int w;
	
	scanf("%d", &esp);
	scanf("%d", &c);
	
	int matriz [esp][c];
	
	for (int i = 0; i < esp; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			scanf("%d", &matriz[i][j]);
		}
	}
	
	
	
	for (int i = 0; i < esp; ++i)
	{
		for (int j = i+1; j < esp; ++j)
		{
			w = 0;
			for (int k1 = 0; k1 < c; ++k1)
			{
				for (int k2 = 0; k2 < c; ++k2)
				{
					if (matriz[i][k1] == matriz[j][k2])
					{
						w++;
					}
				}
			}
			
			graph[i].push_back(ii(j, w));
			graph[j].push_back(ii(i, w));
		}
	}
	
	printf("OIE");
	
	for (int i=0; i <= esp; ++i)
	{
		if (!taken[i])
			mst(i);				
	}
	
	printf("Grafo:");
	int tam1 = (int) tree.size();
	for (int i=0; i < tam1; ++i){
		int tam2 = (int) tree[i].size();
		for (int j=0; j < tam2; ++j)
		{
			ii v = tree[i][j];
		
			printf("[%d, %d]", v.second, v.first);
		}
	}

	return 0;
}
