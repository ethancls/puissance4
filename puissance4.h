#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// Taille du plateau
#define ROWS 6
#define COLS 7

// Profondeur de l'arbre pour l'algorithme minimax
#define DEPTH 7

#define JOUEUR1 1
#define JOUEUR2 2
#define IA 2

// Couleurs ANSI
#define DEFAULT "\033[0m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define VIOLET "\033[0;34m"

typedef struct noeud
{
    uint16_t plateau[ROWS][COLS]; // Configuration de la grille
    uint16_t colonne;             // Colonne jouée
    int32_t score;                // Score associé à ce nœud
    struct noeud *enfants;        // Liste chaînée des nœuds enfants
    struct noeud *suivant;        // Nœud suivant dans la liste chaînée
} noeud;
