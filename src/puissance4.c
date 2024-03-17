#include "puissance4.h"

// Initialiser le plateau
void initialiserPlateau(uint8_t plateau[LIGNES][COLONNES])
{
    for (uint8_t i = 0; i < LIGNES; i++)
    {
        for (uint8_t j = 0; j < COLONNES; j++)
        {
            plateau[i][j] = 0;
        }
    }
}

// Fonction pour placer un jeton dans la colonne choisie
void placerJeton(uint8_t plateau[LIGNES][COLONNES], uint8_t colonne, uint8_t jeton)
{
    for (int8_t i = LIGNES - 1; i >= 0; i--)
    {
        if (plateau[i][colonne] == 0)
        {
            plateau[i][colonne] = jeton;
            return;
        }
    }
}

// Annuler un coup fait dans une colonne
void annulerCoup(uint8_t plateau[LIGNES][COLONNES], uint8_t colonne)
{
    for (uint8_t i = 0; i < LIGNES; i++)
    {
        if (plateau[i][colonne] != 0)
        {
            plateau[i][colonne] = 0;
            return;
        }
    }
}

// Fonction pour tester si une colonne est pleine
bool colonneEstPleine(uint8_t plateau[LIGNES][COLONNES], uint8_t colonne)
{
    if (plateau[0][colonne] == 0)
    {
        return false;
    }
    return true;
}

// Fonction pour vérifier s'il y a un gagnant
bool verifierGagnant(uint8_t plateau[LIGNES][COLONNES], uint8_t jeton, uint8_t colonne)
{
    uint8_t ligne = 0;
    for (uint8_t i = 0; i < LIGNES; i++)
    {
        if (plateau[i][colonne] != 0)
        {
            ligne = i;
            break;
        }
    }

    // Vérification de la ligne
    for (uint8_t j = 0; j < 4; j++)
    {
        if (colonne + j - 3 >= 0 && colonne + j < COLONNES)
        {
            if (plateau[ligne][colonne + j] == jeton &&
                plateau[ligne][colonne + j - 1] == jeton &&
                plateau[ligne][colonne + j - 2] == jeton &&
                plateau[ligne][colonne + j - 3] == jeton)
            {
                return true;
            }
        }
    }

    // Vérification de la colonne
    for (uint8_t j = 0; j < 4; j++)
    {
        if (ligne + j - 3 >= 0 && ligne + j < LIGNES)
        {
            if (plateau[ligne + j][colonne] == jeton &&
                plateau[ligne + j - 1][colonne] == jeton &&
                plateau[ligne + j - 2][colonne] == jeton &&
                plateau[ligne + j - 3][colonne] == jeton)
            {
                return true;
            }
        }
    }

    // Vérification des diagonales descendantes
    for (uint8_t j = 0; j < 4; j++)
    {
        if (ligne + j - 3 >= 0 && ligne + j < LIGNES && colonne + j - 3 >= 0 && colonne + j < COLONNES)
        {
            if (plateau[ligne + j][colonne + j] == jeton &&
                plateau[ligne + j - 1][colonne + j - 1] == jeton &&
                plateau[ligne + j - 2][colonne + j - 2] == jeton &&
                plateau[ligne + j - 3][colonne + j - 3] == jeton)
            {
                return true;
            }
        }
    }

    // Vérification des diagonales ascendantes
    for (uint8_t j = 0; j < 4; j++)
    {
        if (ligne + j - 3 >= 0 && ligne + j < LIGNES && colonne - j + 3 < COLONNES && colonne - j >= 0)
        {
            if (plateau[ligne + j][colonne - j] == jeton &&
                plateau[ligne + j - 1][colonne - j + 1] == jeton &&
                plateau[ligne + j - 2][colonne - j + 2] == jeton &&
                plateau[ligne + j - 3][colonne - j + 3] == jeton)
            {
                return true;
            }
        }
    }

    return false;
}

// Vérifier si il y a égalité
bool egalite(uint8_t plateau[LIGNES][COLONNES])
{
    for (uint8_t i = 0; i < COLONNES; i++)
    {
        if (!colonneEstPleine(plateau, i))
            return false;
    }
    return true;
}

// Scanne le terminal avec test pour éviter les erreurs
void myscanf(uint8_t *valeur, uint8_t min, uint8_t max)
{
    uint8_t check;
    do
    {
        check = scanf("%hhu", valeur);
        if (check != 1 || *valeur < min || *valeur > max)
        {
            printf("Invalide, entrez un nombre entre %d et %d.\n", min, max);
            while (getchar() != '\n')
                ;
        }
    } while (check != 1 || *valeur < min || *valeur > max);
}