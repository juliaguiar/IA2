#include <bits/stdc++.h>
#include <iostream>
#include <math.h>
using namespace std;

float entrada[3][64] = {
						{0,0,1,1,1,1,0,0,
						 0,1,1,0,0,1,1,0,
						 1,1,0,0,0,0,1,1,
						 1,1,0,0,0,0,1,1,
						 1,1,0,0,0,0,1,1,
						 0,1,0,0,0,0,1,0,
						 0,0,1,1,1,1,0,0,
						 0,0,0,0,0,0,0,0},
						 
						{0,0,1,1,1,1,0,0,
						 0,1,1,0,0,0,1,0,
						 0,1,1,0,0,0,1,0,
						 0,0,0,0,0,0,1,0,
						 0,0,0,0,0,1,1,0,
						 0,0,0,0,1,1,0,0,
						 0,0,1,1,1,0,0,0,
						 0,1,0,0,0,0,0,0},
						 
						{0,0,0,0,1,1,0,0,
						 0,0,1,1,0,0,0,0,
						 0,1,1,0,0,0,0,0,
						 0,1,0,0,0,0,0,0,
						 0,1,0,1,1,1,1,0,
						 0,1,1,0,0,1,1,0,
						 0,1,1,0,1,1,0,0,
						 0,0,0,1,0,0,0,0}
						};

float peso[64];

float desejada[3] = {0, 9, 6};

float saida, erro, soma;

int i, j;

bool b;

char texto_str[20];

FILE *pont_arq;

void adaline() {
	
//b = false;

	//do {

        //b = true;

        for (int i = 0; i < 3; i++)
		{
			
            cout << "linha de entrada: " << i << endl;

            soma = 0;            
            for (int j = 0; j < 64; j++)
			{
				soma = soma + entrada[i][j] * peso[j];
			}

            soma = soma * (-1);

            saida = 1 / (1 + pow(2.71828, soma)); 

            erro = desejada[i] - saida;

            srand((unsigned int)time((time_t*)NULL));
        	if (erro != 0) 
			{
        		//b = b & false;
        		
        		if (i == 0)
        		{
        			pont_arq = fopen("peso.txt", "a+");
        				
        			if(pont_arq == NULL)
      				{
      					printf("Erro na abertura do arquivo!");
      				}
      					
        			for (int j = 0; j < 64; j++)
        			{
        				peso[j] = rand()%1;
        					
      					fprintf(pont_arq, "%s\n", peso[j]);	
					
        			}
        			fclose(pont_arq);
        		}
        		else
        		{
        			int j = 0;
        			
        			pont_arq = fopen("peso.txt", "w");
        			
        			if(pont_arq == NULL)
      				{
      					printf("Erro na abertura do arquivo!");
      				}
        			
        			while(fgets(texto_str, 20, pont_arq) != NULL)
        			{
        				peso[j] = peso[j] + 0.1 * entrada [i][j] * erro * saida * (1 - saida);
        				
        				fprintf(pont_arq, "%s\n", peso[j]);	
        				
        				j++;
        			}
  					fclose(pont_arq);
        		}
        	}      
			else
			{
				//b = b & true;
			}

    	}
	//} while (!b);
	//cout << "AQUI" << endl;
}

int main() 
{
	float n;
	
	adaline();
    
    j = 0;
    soma = 0;			
	while (scanf("%f", &n) != EOF)
	{
		soma = soma + n * peso[j];
		
		j++;
	}
	
	soma = soma * (-1);
	
	saida = 1 / (1 + pow(2.71828, soma)); 
	
	cout << saida << endl;
}
