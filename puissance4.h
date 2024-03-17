#pragma once // Si le fichier .h est appelé dans plusieurs .c on doit #ifndef ou #pragma once pour éviter d'inclure plusieurs fois le .h 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// Macros
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// Taille du plateau
#define LIGNES 6
#define COLONNES 7

// Profondeur de l'arbre pour l'algorithme minimax
#define PROFONDEUR 5

// Jetons
#define JOUEUR1 1
#define JOUEUR2 2
#define IA 2

// Couleurs ANSI
#define DEFAUT "\e[0m"
#define ROUGE "\e[0;31m"
#define JAUNE "\e[0;33m"
#define VIOLET "\e[0;35m"

typedef struct noeud
{
    uint8_t plateau[LIGNES][COLONNES];  // Grille de jeu
    uint8_t joueur;                    // Joueur ayant joué le coup associé au noeud
    uint8_t colonne;                  // Colonne jouée
    struct noeud *enfant;            // Liste chainee d'enfants
    struct noeud *suivant;          // Enfant suivant
} noeud;

// Fonctions d'affichage
void afficherBanniere();
void afficherMenu();
void afficherPlateau(uint8_t plateau[LIGNES][COLONNES]);

// Fonctions utiles au puissance 4
void initialiserPlateau(uint8_t plateau[LIGNES][COLONNES]);
void placerJeton(uint8_t plateau[LIGNES][COLONNES], uint8_t colonne, uint8_t jeton);
void annulerCoup(uint8_t plateau[LIGNES][COLONNES], uint8_t colonne);
bool colonneEstPleine(uint8_t plateau[LIGNES][COLONNES], uint8_t colonne);
bool verifierGagnant(uint8_t plateau[LIGNES][COLONNES], uint8_t jeton, uint8_t colonne);
bool egalite(uint8_t plateau[LIGNES][COLONNES]);
void myscanf(uint8_t *valeur, uint8_t min, uint8_t max);

// Fonctions pour le MiniMax
void minimax(uint8_t plateau[LIGNES][COLONNES], uint8_t *colonne, uint8_t joueur, uint8_t adversaire);
int32_t evaluerNoeud(noeud *n, int32_t alpha, int32_t beta, bool maximiser, uint8_t joueur, uint8_t adversaire);
int32_t evaluerSuite(uint8_t case1, uint8_t case2, uint8_t case3, uint8_t case4, uint8_t joueur, uint8_t adversaire);
int32_t score(uint8_t plateau[LIGNES][COLONNES], uint8_t joueur, uint8_t adversaire);

// Fonctions pour l'arbre
noeud *creerNoeud(uint8_t plateau[LIGNES][COLONNES], uint8_t colonne);
void ajouterEnfant(noeud *parent, noeud *enfant);
void construireArbre(noeud *racine, uint8_t profondeur, uint8_t joueur, uint8_t adversaire, bool tourJoueur);
void libererArbre(noeud *racine);

// Fonctions de partie
void PartieIA(uint8_t plateau[LIGNES][COLONNES]);
void PartieVS(uint8_t plateau[LIGNES][COLONNES]);