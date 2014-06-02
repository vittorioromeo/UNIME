#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct AlberoImpl
{
	struct AlberoImpl* px;
	struct AlberoImpl* sx;
	struct AlberoImpl* dx;
	int dato;
} Albero;

int albero_Crea(Albero** mA, int mDato)
{
	*mA = (Albero*) malloc(1 * sizeof(Albero));

	if(*mA == NULL) return 1;

	(*mA)->px = (*mA)->sx = (*mA)->dx = NULL;
	(*mA)->dato = mDato;

	return 0;
}

int albero_CreaFiglio(Albero** mA, Albero* mPadre, int mDato)
{
	int risultato = albero_Crea(mA, mDato);
	if(risultato == 0) (*mA)->px = mPadre;
	
	return risultato;
}

void albero_Distruggi(Albero** mA)
{
	free(*mA);
	*mA = NULL;
}

void albero_Distruggi_Ricorsivo(Albero** mA)
{
	if((*mA)->sx != NULL) albero_Distruggi_Ricorsivo(&(*mA)->sx);
	if((*mA)->dx != NULL) albero_Distruggi_Ricorsivo(&(*mA)->dx);

	albero_Distruggi(mA);
}

int albero_Inserisci(Albero* mRadice, int mDato)
{
	Albero* prec = NULL;
	Albero** p = &mRadice;

	while(*p != NULL)
	{
		prec = *p;		

		if(prec->dato == mDato) return 1;
		p = mDato < prec->dato ? &prec->sx : &prec->dx;
	} 

	return albero_CreaFiglio(p, prec, mDato);
}

int albero_Inserisci_Ricorsivo(Albero* mRadice, int mDato)
{
	if(mDato < mRadice->dato) 
	{
		if(mRadice->sx == NULL) 
			return albero_CreaFiglio(&mRadice->sx, mRadice, mDato);
	
		return albero_Inserisci_Ricorsivo(mRadice->sx, mDato);
	}
	else if(mDato > mRadice->dato) 
	{
		if(mRadice->dx == NULL) 
			return albero_CreaFiglio(&mRadice->dx, mRadice, mDato);
		
		return albero_Inserisci_Ricorsivo(mRadice->dx, mDato);
	}

	return 1;
}

Albero* albero_Ricerca(Albero* mA, int mDato)
{
	if(mA == NULL || mDato == mA->dato) return mA;
	return albero_Ricerca(mDato < mA->dato ? mA->sx : mA->dx, mDato);
}

Albero* albero_getMinimo(Albero* mA)
{
	while(mA->sx != NULL) mA = mA->sx;
	return mA;
}

Albero* albero_getMassimo(Albero* mA)
{
	while(mA->dx != NULL) mA = mA->dx;
	return mA;
}

Albero* albero_getSuccessore(Albero* mA)
{
	if(mA->dx != NULL) return albero_getMinimo(mA->dx);
	if(mA->px == NULL) return NULL;
	if(mA == mA->px->sx) return mA->px;

	while(mA->px != NULL && mA != mA->px->sx) mA = mA->px;
	return mA->px;
}

Albero* albero_getPredecessore(Albero* mA)
{
	if(mA->sx != NULL) return albero_getMassimo(mA->sx);
	if(mA->px == NULL) return NULL;
	if(mA == mA->px->dx) return mA->px;

	while(mA->px != NULL && mA != mA->px->dx) mA = mA->px;
	return mA->px;
}

int albero_Rimuovi(Albero* mA)
{
	Albero* padre = mA->px;

	if(padre != NULL)
	{
		Albero** figlioPadre = (padre->sx == mA) ? &(padre->sx) : &(padre->dx);
	
		if(mA->sx == NULL && mA->dx == NULL) *figlioPadre = NULL;
		else if(mA->sx != NULL && mA->dx == NULL) *figlioPadre = mA->sx;
		else if(mA->sx == NULL && mA->dx != NULL) *figlioPadre = mA->dx;
	}

	if(padre == NULL || (mA->sx != NULL && mA->dx != NULL))
	{
		Albero* next = albero_getSuccessore(mA);
		if(next == NULL) next = albero_getPredecessore(mA);
		mA->dato = next->dato;
		
		return albero_Rimuovi(next);
	}

	albero_Distruggi(&mA);
	return 0;
}

int albero_getAltezza(Albero* mRadice)
{
	if(mRadice == NULL) return -1;

	int hSx = albero_getAltezza(mRadice->sx);
	int hDx = albero_getAltezza(mRadice->dx);

	return hSx > hDx ? hSx + 1 : hDx + 1;
}

void stampaX(int mSpazi, const char* mStr)
{
	int i;
	for(i = 0; i < mSpazi; ++i) printf("%s", mStr);
}

int contaCifre(int mNumero)
{
	int risultato = 1;
	
	while(mNumero >= 10)
	{
		mNumero %= 10;
		++risultato;
	}

	return risultato;
}

void albero_AttraversaLiv(Albero* mRadice)
{
	Albero* coda[100];
	int i, spazi = 44, precSpazi = 0, passaggi = 0, livello = 0;
	for(i = 0; i < 100; ++i) coda[i] = NULL;

	coda[0] = mRadice;		
	
	for(i = 0; i < 20; ++i)
	{
		Albero* c = coda[i];
		int numSimboli = (passaggi == 0) ? spazi : precSpazi, cifre;
		const char* simbolo = (passaggi % 2 == 0) ? " " : "-";
		
		if(c != NULL)
		{
			cifre = contaCifre(c->dato);

			stampaX(numSimboli - cifre - 4, simbolo);			
			printf("( %d )", c->dato);

			coda[2 * i + 1] = c->sx;
			coda[2 * i + 2] = c->dx;
		}
		else
		{
			cifre = 1;

			stampaX(numSimboli - cifre, simbolo);
			printf("X");			
		}
			
		if(++passaggi >= pow(2, livello)) 
		{
			++livello;

			printf("\n");
			stampaX(spazi - 1, " ");
			printf("|");
			
			int j;
			for(j = 0; j < passaggi - 1; ++j)
			{
				stampaX(precSpazi - 1, " ");
				printf("|");
			}

			printf("\n");
			
			precSpazi = spazi;
			spazi /= 2;
			passaggi = 0;
		}
	}
}

int main()
{
	Albero* a;

	albero_Crea(&a, 9);	

	albero_Inserisci(a, 5);
	albero_Inserisci(a, 2);
	albero_Inserisci(a, 8);
	albero_Inserisci(a, 12);
	albero_Inserisci(a, 3);
	albero_Inserisci(a, 7);
	albero_Inserisci(a, 10);
	albero_Inserisci(a, 15);
	albero_Inserisci(a, 17);
	albero_Inserisci(a, 4);
	albero_Inserisci(a, 1);
	albero_Inserisci(a, 11);

	albero_AttraversaLiv(a);

	printf("\n\n");
	albero_Rimuovi(albero_Ricerca(a, 5));
	albero_AttraversaLiv(a);

	printf("\nAltezza: %d", albero_getAltezza(a));
 
	albero_Distruggi_Ricorsivo(&a);
	
 	return 0;


/*
	Albero* test = albero_Ricerca(a, 9);
	Albero* testSuccessore = albero_getPredecessore(test);
	
	if(testSuccessore != NULL)
		printf("\nSuccessore di %d: %d", test->dato, testSuccessore->dato);
	else
		printf("\nSuccessore di %d: NULL", test->dato);
*/
}