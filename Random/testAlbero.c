#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

struct AlberoImpl;
typedef struct AlberoImpl Albero;

struct AlberoImpl
{
    Albero* px;
    Albero* sx;
    Albero* dx;
    int dato;
};

// Alloca memoria per un nodo, lo crea ed inizializza
// Restituisce 1 in caso di errore
// Il puntatore passato conterrà l'indirizzo del nodo creato
int albero_Crea(Albero** mA, int mDato)
{
    *mA = (Albero*)malloc(1 * sizeof(Albero));

    if(*mA == NULL) return 1;

    (*mA)->px = (*mA)->sx = (*mA)->dx = NULL;
    (*mA)->dato = mDato;

    return 0;
}

// Alloca memoria per un nodo, lo crea ed inizializza
// Setta il padre del nodo creato ad mPadre
// Restituisce 1 in caso di errore
int albero_CreaFiglio(Albero** mA, Albero* mPadre, int mDato)
{
    assert(mPadre != NULL);

    int risultato = albero_Crea(mA, mDato);
    if(risultato == 0) (*mA)->px = mPadre;

    return risultato;
}

// Dealloca memoria per un nodo e setta il puntatore
// passato a NULL
void albero_Distruggi(Albero** mA)
{
    assert(*mA != NULL);

    free(*mA);
    *mA = NULL;
}

// Dealloca ricorsivamente memoria per un albero,
// settando il puntatore a NULL
void albero_Distruggi_Ricorsivo(Albero** mA)
{
    if(*mA == NULL) return;

    albero_Distruggi_Ricorsivo(&(*mA)->sx);
    albero_Distruggi_Ricorsivo(&(*mA)->dx);
    albero_Distruggi(mA);
}

// Inserisce un nodo in maniera ordinata in un albero
// Restituisce 1 in caso di errore
int albero_Inserisci(Albero* mRadice, int mDato)
{
    Albero* prec;
    Albero** p = &mRadice;

    // Finchè non viene trovato un puntatore di un nodo
    // esistente che punta ad un nodo NULL, p continua a
    // scendere, seguito da prec, nell'albero, mantenendo
    // l'ordine dei dati
    // Se viene trovato un dato già esistente, restituisce 1
    while(*p != NULL)
    {
        prec = *p;

        if(prec->dato == mDato) return 1;
        p = mDato < prec->dato ? &prec->sx : &prec->dx;
    }

    return albero_CreaFiglio(p, prec, mDato);
}

// Versione ricorsiva della funzione di sopra
int albero_Inserisci_Ricorsivo(Albero* mRadice, int mDato)
{
    if(mRadice == NULL) return albero_Crea(&mRadice, mDato);

    Albero** p = &mRadice->sx;

    if(mDato == mRadice->dato) return 1;
    if(mDato > mRadice->dato) p = &mRadice->dx;

    if(*p == NULL) return albero_CreaFiglio(p, mRadice, mDato);
    return albero_Inserisci_Ricorsivo(*p, mDato);
}

// Cerca un nodo di dato mDato, se lo trova restituisce un
// puntatore ad esso, altrimenti restituisce NULL
Albero* albero_Ricerca(Albero* mA, int mDato)
{
    assert(mA != NULL);

    if(mA == NULL || mDato == mA->dato) return mA;
    return albero_Ricerca(mDato < mA->dato ? mA->sx : mA->dx, mDato);
}

// Scende continuamente a sinistra, restituendo il puntatore al
// nodo di valore minimo o NULL
Albero* albero_getMinimo(Albero* mA)
{
    while(mA->sx != NULL) mA = mA->sx;
    return mA;
}

// Scende continuamente a destra, restituendo il puntatore al
// nodo di valore massimo o NULL
Albero* albero_getMassimo(Albero* mA)
{
    while(mA->dx != NULL) mA = mA->dx;
    return mA;
}

Albero* albero_getFiglioSx(Albero* mA)
{
    assert(mA != NULL);
    return mA->sx;
}
Albero* albero_getFiglioDx(Albero* mA)
{
    assert(mA != NULL);
    return mA->dx;
}

typedef Albero* (*FPAl)(Albero*);

Albero* albero_getSuccPred(Albero* mA, FPAl mFnMinMax, FPAl mFnF1, FPAl mFnF2)
{
    assert(mA != NULL);

    // Se ha un figlio F1, restituisce il min/max tra
    // i maggioranti/minoranti
    Albero* figlioF1 = (*mFnF1)(mA);
    if(figlioF1 != NULL) return (*mFnMinMax)(figlioF1);

    // Se non ha figlio F1 e non ha padre, restituisce NULL
    if(mA->px == NULL) return NULL;

    // Se il nodo passato è il figlio F2 di suo padre,
    // suo padre è il successore - restituiscilo
    if(mA == (*mFnF2)(mA->px)) return mA->px;

    // Altrimenti risale l'albero finchè non trova un nodo
    // che è il figlio F2 di suo padre - se non esiste
    // restituisce NULL
    while(mA->px != NULL && mA != (*mFnF2)(mA->px)) mA = mA->px;
    return mA->px;
}

Albero* albero_getSuccessore(Albero* mA)
{
    return albero_getSuccPred(
        mA, &albero_getMinimo, &albero_getFiglioDx, &albero_getFiglioSx);
}

Albero* albero_getPredecessore(Albero* mA)
{
    return albero_getSuccPred(
        mA, &albero_getMassimo, &albero_getFiglioSx, &albero_getFiglioDx);
}

// Rimuove un nodo da un albero, assume che il nodo faccia parte di un albero
void albero_Rimuovi(Albero* mA)
{
    assert(mA != NULL);

    Albero* padre = mA->px;

    // Se il nodo da rimuovere ha un padre
    if(padre != NULL)
    {
        // Se mA è il figlio sx del padre, figlioPadre punterà all'indirizzo
        // del figlio sx del padre di mA, altrimenti figlioPadre punterà
        // all'indirizzo del figlio dx del padre di mA
        Albero** figlioPadre = (padre->sx == mA) ? &(padre->sx) : &(padre->dx);

        // Se mA non ha figli, *figlioPadre sarà settato a NULL
        // Se mA ha solo figlio sx, *figlioPadre sarà settato al figlio sx
        // Se mA ha solo figlio dx, *figlioPadre sarà settato al figlio dx
        if(mA->sx == NULL && mA->dx == NULL)
            *figlioPadre = NULL;
        else if(mA->sx != NULL && mA->dx == NULL)
            *figlioPadre = mA->sx;
        else if(mA->sx == NULL && mA->dx != NULL)
            *figlioPadre = mA->dx;
    }

    // Altrimenti, se il padre non esiste (cioè mA è la radice), oppure
    // mA ha due figli
    if(padre == NULL || (mA->sx != NULL && mA->dx != NULL))
    {
        // Troviamo o il predecessore o il successore di mA
        Albero* next = albero_getSuccessore(mA);
        if(next == NULL) next = albero_getPredecessore(mA);

        // Settiamo il dato contenuto in mA con il dato contenuto
        // nel suo successore/predecessore
        mA->dato = next->dato;

        // Rimuoviamo ricorsivamente sul successore/predecessore
        albero_Rimuovi(next);
        return;
    }

    // Deallochiamo la memoria per il nodo cancellato
    albero_Distruggi(&mA);
}

// Restituisce l'altezza di un albero
int albero_getAltezza(Albero* mRadice)
{
    // Se l'albero è vuoto (senza radice), restituisce -1
    if(mRadice == NULL) return -1;

    // Altrimenti restituisce l'altezza maggiore tra il sottoalbero
    // sx o dx, calcolandola ricorsivamente
    int hSx = albero_getAltezza(mRadice->sx);
    int hDx = albero_getAltezza(mRadice->dx);
    return hSx > hDx ? hSx + 1 : hDx + 1;
}

void stampaX(int mSpazi, const char* mStr)
{
    for(; mSpazi >= 0; --mSpazi) printf("%s", mStr);
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

            for(; passaggi > 1; --passaggi)
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
}

/*
Albero* albero_getSuccessore(Albero* mA)
{
    // Se ha un figlio destro, restituisce il minore tra
    // i maggioranti
    if(mA->dx != NULL) return albero_getMinimo(mA->dx);

    // Se non ha figlio destro e non ha padre, restituisce
    // NULL
    if(mA->px == NULL) return NULL;

    // Se il nodo passato è il figlio sinistro di suo padre,
    // suo padre è il successore - restituiscilo
    if(mA == mA->px->sx) return mA->px;

    // Altrimenti risale l'albero finchè non trova un nodo
    // che è il figlio sinistro di suo padre - se non esiste
    // restituisce NULL
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
*/