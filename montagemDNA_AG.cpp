#include <bits/stdc++.h>
#include <iostream>
#include <time.h>
#include <cstring>
using namespace std;

typedef vector<string> vs;

vs seq;

int populacao[10][100];
int popancestrais[5][100];
int popdescendentes[50][100];

int n;
int ind;
int melhor = -100;

void inicia_populacao()
{
	int random, a;
	
	srand((unsigned int)time((time_t*)NULL));
	for (int i=0; i<10; i++)
	{
    	for (int j=0; j < n; j++)
		{
			a = 9 - seq[j].size();
			random = rand() % a;
			populacao[i][j] = random;
		}
	}
}

int adap(int cromossomo[100])
{
	int nota = 0;
	
	int max = cromossomo[0], ind_max = 0;
	int min = cromossomo[0], ind_min = 0;
	
	for (int i=1; i < n; i++)
	{
		if(cromossomo[i] > max)
		{
			max = cromossomo[i];
			ind_max = i;
		}
		
		if(cromossomo[i] < min)
		{
			min = cromossomo[i];
			ind_min = i;
		}
	}
	
	
	for (int i = min; i < max+seq[ind_max].size(); i++)
	{
		for (int a=0; a < n; a++)
		{
			for (int b=a+1; b < n; b++)
			{
				if (cromossomo[a] <= i && cromossomo[b] <= i && cromossomo[a]+seq[a].size() > i && cromossomo[b]+seq[b].size() > i)
				{
					if (seq[a][i-cromossomo[a]] == seq[b][i-cromossomo[b]])
					{
						nota++;
					} else {				
						nota=nota-2;
					}
				}
			}
		}
	}
	
	return nota;
}

void selecao()
{
	int P[10], aux, k=5;
	bool b[5];
	
	for (int i=0;i<10;i++)
  	{
  		P[i]=adap(populacao[i]);
	}
	
	for (int i=0;i<=8;i++)
  	{
    	for (int j=i+1;j<=9;j++)
    	{
      		if (P[i]>P[j])
      		{
      			aux=P[i];
      			P[i]=P[j];
      			P[j]=aux;
      		}
		}
	}
	
	for (int i=0;i<5;i++)
  	{
    	for (int j=0;j<10;j++)
    	{
    		for (int a=0;a<5;a++)
    		{
    			b[a] = true;
    			for (int c=0;c<n;c++)
    			{
    				if (populacao[j][c] != popancestrais[a][c])
    					b[a] = false;
    			}
    		}
    		
      		if (P[k]==adap(populacao[j]) && !b[0] && !b[1] && !b[2] && !b[3] && !b[4])
	  		{
      			for (int c=0;c<n;c++)
    				popancestrais[i][c] = populacao[j][c];
	  			k++;
	  			break;
	  		}
    	}
	}
}

void cruzamento(int a, int b)
{
	int random;
	int aux;
	int n2 = n-1;
	
	srand((unsigned int)time((time_t*)NULL));
	do
  	{
  		random=rand()%n2;
  	}
	while(random==0);
	
	for(int k=0;k<=random;k++)
	{
		popdescendentes[ind][k] = popancestrais[a][k];
  		popdescendentes[ind+1][k] = popancestrais[b][k];
	}
	for(int k=random+1;k<n;k++)
	{
		popdescendentes[ind+1][k] = popancestrais[a][k];
  		popdescendentes[ind][k] = popancestrais[b][k];
	}
}

void mutacao()
{
	int r, a, random;
	int n2 = n-1;
	
	srand((unsigned int)time((time_t*)NULL));
	for (int i = 0; i < 5; ++i)
	{
		
		for(int j = 0; j < n; ++j)
			popdescendentes[ind][j] = popancestrais[i][j];
		
		r=rand()%n2;
		
		a = 9 - seq[r].size();
		random = rand() % a;
		
		popdescendentes[ind][r] = random;
		
		ind++;
	}		
}


void substituicao()
{
	int P[10];
	int G[25];
	int pop_aux[10][n];
	int aux, ind_pop = 9, ind_des = 24; 
	
	for (int i=0;i<10;i++)
  	{
  		P[i]=adap(populacao[i]);
	}
	
	for (int i=0;i<=8;i++)
  	{
    	for (int j=i+1;j<=9;j++)
    	{
      		if (P[i]>P[j])
      		{
      			aux=P[i];
      			P[i]=P[j];
      			P[j]=aux;
      			
      			for (int c=0;c<n;c++)
      			{
      				aux = populacao[i][c];
      				populacao[i][c] = populacao[j][c];
      				populacao[j][c] = aux;
      			}
      		}
		}
	}
	
	for (int i=0;i<25;i++)
  	{
  		G[i]=adap(popdescendentes[i]);
	}
	
	for (int i=0;i<=23;i++)
  	{
    	for (int j=i+1;j<=24;j++)
    	{
      		if (G[i]>G[j])
      		{
      			aux=G[i];
      			G[i]=G[j];
      			G[j]=aux;
      			
      			for (int c=0;c<n;c++)
      			{
      				aux = popdescendentes[i][c];
      				popdescendentes[i][c] = popdescendentes[j][c];
      				popdescendentes[j][c] = aux;
      			}
      		}
		}
	}
	
	for (int i=0;i<10;i++)
  	{
  		if(P[ind_pop] >= G[ind_des])
  		{
  			for (int c=0;c<n;c++)
  				pop_aux[i][c] = populacao[ind_pop][c];
  			
			ind_pop--;
  		}
  		else
  		{
  			for (int c=0;c<n;c++)
  				pop_aux[i][c] = popdescendentes[ind_des][c];
  			
			ind_des--;
  		}
  	}
  	
  	for (int j = 0; j < 10; ++j)
		for (int i = 0; i < n; ++i)
			populacao[j][i] = pop_aux[j][i];
	
}

int main() 
{
	string str;
	int aux, a, count=0;
	int vetor[n];
	
	seq.clear();
	
	cin >> n;

	for (int i = 0; i < n; ++i)
	{
		cin >> str;
		seq.push_back(str);
	}
	
	inicia_populacao();
	
	for (int Y=0;Y<10000;Y++)
	{
		ind = 0;
		
		selecao();
		
		for (int i = 0; i < 5; ++i)
			for (int j = i+1; j < 5; ++j)
			{
				cruzamento(i, j);
				ind = ind+2;
			}
			
		mutacao();
		
		substituicao();
		
		aux = adap(populacao[0]);
		if(aux == melhor)
		{
			count++;
		} else {
			melhor = aux;
			count = 0;
		}
		
		if(count >= 5)
		{
			if (melhor == 8)
				break;
		}
	}
	
	for (int i = 0; i < n; ++i)
		cout << populacao[0][i] << " ";

}
