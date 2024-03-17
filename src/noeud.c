#include "puissance4.h"

// Fonction pour créer un nouveau nœud avec une configuration de grille donnée
noeud *creerNoeud(uint8_t plateau[LIGNES][COLONNES], uint8_t colonne)
{
    noeud *nouveauNoeud = (noeud *)malloc(sizeof(noeud));
    if (nouveauNoeud != NULL)
    {
        for (int i = 0; i < LIGNES; i++)
        {
            for (int j = 0; j < COLONNES; j++)
            {
                nouveauNoeud->plateau[i][j] = plateau[i][j];
            }
        }
        nouveauNoeud->joueur = 0;
        nouveauNoeud->colonne = colonne;
        nouveauNoeud->enfant = NULL;
        nouveauNoeud->suivant = NULL;
    }
    else
    {
        printf("Erreur allocation mémoire !\n");
    }
    return nouveauNoeud;
}

// Fonction pour ajouter un nœud enfant à un nœud parent
void ajouterEnfant(noeud *parent, noeud *enfant)
{
    enfant->suivant = parent->enfant; // On fait pointer l'ancien enfant comme suivant du nouveau
    parent->enfant = enfant;          // On actualise l'enfant du parent
}

// Construire l'arbre des coups possibles avec leur score
void construireArbre(noeud *racine, uint8_t profondeur, uint8_t joueur, uint8_t adversaire, bool tourJoueur)
{
    if (profondeur == 0)
    {
        return;
    }

    for (uint8_t col = 0; col < COLONNES; col++)
    {
        if (!colonneEstPleine(racine->plateau, col))
        {
            placerJeton(racine->plateau, col, joueur);

            noeud *nouveauNoeud = creerNoeud(racine->plateau, col);

            if (tourJoueur)
                nouveauNoeud->joueur = joueur;
            else
                nouveauNoeud->joueur = adversaire;

            ajouterEnfant(racine, nouveauNoeud);

            if (!verifierGagnant(nouveauNoeud->plateau, joueur, col) && !verifierGagnant(nouveauNoeud->plateau, adversaire, col) && !egalite(nouveauNoeud->plateau))
            {
                construireArbre(nouveauNoeud, profondeur - 1, adversaire, joueur, true ^ tourJoueur);
            }

            annulerCoup(racine->plateau, col);
        }
    }
}

// Fonction pour libérer la mémoire allouée pour les nœuds de l'arbre
void libererArbre(noeud *racine)
{
    if (racine == NULL)
    {
        return;
    }
    noeud *courant = racine;
    while (courant != NULL)
    {
        noeud *suivant = courant->suivant;
        libererArbre(courant->enfant);
        free(courant);
        courant = suivant;
    }
}
