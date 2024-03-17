#include "puissance4.h"

void minimax(uint8_t plateau[LIGNES][COLONNES], uint8_t *colonne, uint8_t joueur, uint8_t adversaire)
{
    noeud *racine = creerNoeud(plateau, 0);
    construireArbre(racine, PROFONDEUR, joueur, adversaire, true);

    int32_t meilleurScore = INT32_MIN;
    printf("\n");
    int32_t scores[COLONNES];
    for (uint8_t i = 0; i < COLONNES; i++)
    {
        scores[i] = INT32_MAX;
    }

    // Parcourir les enfant de la racine (les coups possibles)
    for (noeud *enfant = racine->enfant; enfant != NULL; enfant = enfant->suivant)
    {
        int32_t scoreEnfant = evaluerNoeud(enfant, INT32_MIN, INT32_MAX, false, joueur, adversaire);

        scores[enfant->colonne] = scoreEnfant;

        if (scoreEnfant > meilleurScore)
        {
            meilleurScore = scoreEnfant;
            *colonne = enfant->colonne;
        }
        if (scoreEnfant == meilleurScore && fabs(enfant->colonne - ceil(COLONNES / 2)) < fabs(*colonne - ceil(COLONNES / 2))) // On joue le coup le plus proche du centre si il y a égalité
        {
            meilleurScore = scoreEnfant;
            *colonne = enfant->colonne;
        }

    }
    for (uint8_t i = 0; i < COLONNES; i++)
    {
        if(scores[i] == INT32_MAX)
            printf("    ");
        else if (scores[i] < 0)
        {
            if (scores[i] < -9)
                if (scores[i] < -99)
                    printf("%s%d%s", VIOLET, scores[i], DEFAUT);
                else
                    printf(" %s%d%s", VIOLET, scores[i], DEFAUT);
            else
                printf(" %s%d%s ", VIOLET, scores[i], DEFAUT);
        }
        else
        {
            if (scores[i] > 9)
                if (scores[i] > 99)
                    printf(" %s%d%s", VIOLET, scores[i], DEFAUT);
                else
                    printf("  %s%d%s", VIOLET, scores[i], DEFAUT);
            else
                printf("  %s%d%s ", VIOLET, scores[i], DEFAUT);
        }
    }
    libererArbre(racine);
}

// Fonction pour évaluer le score d'un nœud en utilisant l'algorithme minimax avec élagage alphabeta
int32_t evaluerNoeud(noeud *n, int32_t alpha, int32_t beta, bool maximiser, uint8_t joueur, uint8_t adversaire)
{
    if (n->enfant == NULL)
    {
        if (n->joueur == joueur)
            return score(n->plateau, joueur, adversaire);
        else if (n->joueur == adversaire)
            return score(n->plateau, adversaire, joueur);
    }

    // Si c'est le tour de l'IA (maximisation)
    if (maximiser)
    {
        int32_t score = INT32_MIN;

        for (noeud *enfant = n->enfant; enfant != NULL; enfant = enfant->suivant)
        {
            int32_t scoreEnfant = evaluerNoeud(enfant, alpha, beta, false, joueur, adversaire);
            score = max(score, scoreEnfant);

            if (score > beta)
                break;
            alpha = max(alpha, score);
        }

        return score;
    }
    // Si c'est le tour de l'humain (minimisation)
    else
    {
        int32_t score = INT32_MAX;

        for (noeud *enfant = n->enfant; enfant != NULL; enfant = enfant->suivant)
        {
            int32_t scoreEnfant = evaluerNoeud(enfant, alpha, beta, true, joueur, adversaire);
            score = min(score, scoreEnfant);

            if (score < alpha)
                break;
            beta = min(beta, score);
        }

        return score;
    }
}

// Score du plateau avec joueur en positif et adversaire en négatif
int32_t score(uint8_t plateau[LIGNES][COLONNES], uint8_t joueur, uint8_t adversaire)
{
    int32_t valeur = 0;

    for (int8_t i = LIGNES - 1; i >= 0; i--)
    {
        for (int8_t j = COLONNES - 1; j >= 0; j--)
        {
            // Vérification des lignes
            if (j >= 3)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i][j - 1], plateau[i][j - 2], plateau[i][j - 3], joueur, adversaire);
            }
            // Vérification des colonnes
            if (i >= 3)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i - 1][j], plateau[i - 2][j], plateau[i - 3][j], joueur, adversaire);
            }
            // Vérification des diagonales ascendantes
            if (i >= 3 && j >= 3)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i - 1][j - 1], plateau[i - 2][j - 2], plateau[i - 3][j - 3], joueur, adversaire);
            }
            // Vérification des diagonales descendantes
            if (i >= 3 && j <= COLONNES - 4)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i - 1][j + 1], plateau[i - 2][j + 2], plateau[i - 3][j + 3], joueur, adversaire);
            }
        }
    }
    return valeur;
}

int32_t evaluerSuite(uint8_t case1, uint8_t case2, uint8_t case3, uint8_t case4, uint8_t joueur, uint8_t adversaire)
{
    int32_t score = 0;
    uint8_t J = 0;
    uint8_t A = 0;

    case1 == joueur ? J++ : 0;
    case2 == joueur ? J++ : 0;
    case3 == joueur ? J++ : 0;
    case4 == joueur ? J++ : 0;

    case1 == adversaire ? A++ : 0;
    case2 == adversaire ? A++ : 0;
    case3 == adversaire ? A++ : 0;
    case4 == adversaire ? A++ : 0;

    if (J == 4)
    {
        score = 100;
        return score;
    }
    else if (A == 4)
    {
        score = -95;
        return score;
    }
    else if (J == 3 && A == 0)
    {
        score = 5;
        return score;
    }
    else if (J == 0 && A == 3)
    {
        score = -4;
        return score;
    }
    else if (J == 2 && A == 0)
    {
        score = 2;
        return score;
    }
    else if (J == 0 && A == 2)
    {
        score = -1;
        return score;
    }
    return score;
}
