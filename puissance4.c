#include "puissance4.h"

// Fonction pour créer un nouveau nœud avec une configuration de grille donnée
noeud *creerNoeud(uint16_t plateau[ROWS][COLS], uint16_t colonne)
{
    noeud *nouveauNoeud = (noeud *)malloc(sizeof(noeud));
    if (nouveauNoeud != NULL)
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                nouveauNoeud->plateau[i][j] = plateau[i][j];
            }
        }
        nouveauNoeud->score = 0;
        nouveauNoeud->colonne = colonne;
        nouveauNoeud->enfants = NULL;
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
    enfant->suivant = parent->enfants;
    parent->enfants = enfant;
}

// Fonction pour libérer la mémoire allouée pour les nœuds de l'arbre
void libererArbre(noeud *racine)
{
    if (racine == NULL)
    {
        return;
    }
    noeud *actuel = racine;
    while (actuel != NULL)
    {
        noeud *suivant = actuel->suivant;
        libererArbre(actuel->enfants); // Libérer récursivement les enfants
        free(actuel);
        actuel = suivant;
    }
}

void afficherBanniere()
{
    printf("\n");
    printf("########:: ##::::##:  ####:: ######::: ######::::: ###:::: ##::: ##:: ######:: ########: ##::::::::\n");
    printf("##.... ##: ##:::: ##:. ##:: ##... ##: ##... ##::: ## ##::: ###:: ##: ##... ##: ##.....:: ##::: ##::\n");
    printf("##:::: ##: ##:::: ##:: ##:: ##:::..:: ##:::..::: ##:. ##:: ####: ##: ##:::..:: ##::::::: ##::: ##::\n");
    printf("########:: ##:::: ##:: ##::. ######::. ######:: ##:::. ##: ## ## ##: ##::::::: ######::: ##::: ##::\n");
    printf("##.....::: ##:::: ##:: ##:::..... ##::..... ##: #########: ##. ####: ##::::::: ##...:::: #########:\n");
    printf("##:::::::: ##:::: ##:: ##:: ##::: ##: ##::: ##: ##.... ##: ##:. ###: ##::: ##: ##:::::::...... ##::\n");
    printf("##::::::::. #######:: ####:. ######::. ######:: ##:::: ##: ##::. ##:. ######:: ########::::::: ##::\n");
    printf("..::::::::::.......:::....:::......::::......:::..:::::..::..::::..:::......:::........::::::::..:::\n");

    printf("\nCréé par Ethan Nicolas ^_^ | ethan.bento-nicolas@edu.univ-paris13.fr | 2024\n");

    printf("\n...................................................................................................\n");
}

void afficherPlateau(uint16_t plateau[ROWS][COLS])
{
    printf("\n");
    for (uint16_t i = 0; i < ROWS; i++)
    {
        printf("|");
        for (uint16_t j = 0; j < COLS; j++)
        {
            if (plateau[i][j] == 1)
            {
                printf(" %s●%s |", RED, DEFAULT);
            }
            else if (plateau[i][j] == 2)
            {
                printf(" %s●%s |", YELLOW, DEFAULT);
            }
            else
            {
                printf("   |");
            }
        }
        printf("\n");

        for (uint16_t j = 0; j < COLS; j++)
        {
            printf("+---");
        }
        printf("+\n");
    }
    for (uint16_t j = 0; j < COLS; j++)
    {
        printf("  %d ", j + 1);
    }
    printf("\n\n");
}

// Fonction pour placer un jeton dans la colonne choisie
void placerJeton(uint16_t plateau[ROWS][COLS], uint16_t colonne, uint16_t jeton)
{
    for (uint16_t i = ROWS - 1; i >= 0; i--)
    {
        if (plateau[i][colonne] == 0)
        {
            plateau[i][colonne] = jeton;
            break;
        }
    }
}

// Fonction pour tester si une colonne est pleine
bool colonneEstPleine(uint16_t plateau[ROWS][COLS], uint16_t colonne)
{
    for (uint16_t row = 0; row < ROWS; row++)
    {
        if (plateau[row][colonne] == 0)
        {
            return false;
        }
    }
    return true;
}

// Fonction pour vérifier s'il y a un gagnant
bool verifierGagnant(uint16_t plateau[ROWS][COLS], uint16_t jeton)
{
    // Vérification des lignes
    for (uint16_t i = 0; i < ROWS; i++)
    {
        for (uint16_t j = 0; j <= COLS - 4; j++)
        {
            if (plateau[i][j] == jeton &&
                plateau[i][j + 1] == jeton &&
                plateau[i][j + 2] == jeton &&
                plateau[i][j + 3] == jeton)
            {
                return true;
            }
        }
    }

    // Vérification des colonnes
    for (uint16_t i = 0; i <= ROWS - 4; i++)
    {
        for (uint16_t j = 0; j < COLS; j++)
        {
            if (plateau[i][j] == jeton &&
                plateau[i + 1][j] == jeton &&
                plateau[i + 2][j] == jeton &&
                plateau[i + 3][j] == jeton)
            {
                return true;
            }
        }
    }

    // Vérification des diagonales (de gauche à droite)
    for (uint16_t i = 0; i <= ROWS - 4; i++)
    {
        for (uint16_t j = 0; j <= COLS - 4; j++)
        {
            if (plateau[i][j] == jeton &&
                plateau[i + 1][j + 1] == jeton &&
                plateau[i + 2][j + 2] == jeton &&
                plateau[i + 3][j + 3] == jeton)
            {
                return true;
            }
        }
    }

    // Vérification des diagonales (de droite à gauche)
    for (uint16_t i = 0; i <= ROWS - 4; i++)
    {
        for (uint16_t j = 3; j < COLS; j++)
        {
            if (plateau[i][j] == jeton &&
                plateau[i + 1][j - 1] == jeton &&
                plateau[i + 2][j - 2] == jeton &&
                plateau[i + 3][j - 3] == jeton)
            {
                return true;
            }
        }
    }

    return false;
}

void afficherMenu()
{
    printf("\n+--+--+--+--+--+--+--+--+\n");
    printf("|  MENU :               |\n");
    printf("|  1. Multijoueur       |\n");
    printf("|  2. IA (MiniMax)      |\n");
    printf("|  3. Quitter           |\n");
    printf("+--+--+--+--+--+--+--+--+\n\n");
}

// Initialiser le plateau
void initialiserPlateau(uint16_t plateau[ROWS][COLS])
{
    for (uint16_t i = 0; i < ROWS; i++)
    {
        for (uint16_t j = 0; j < COLS; j++)
        {
            plateau[i][j] = 0;
        }
    }
}

// Scanne le terminal avec test pour éviter les erreurs
void myscanf(uint16_t *valeur, uint16_t min, uint16_t max)
{
    uint16_t check;
    do
    {
        check = scanf("%hu", valeur);

        if (check != 1 || *valeur < min || *valeur > max)
        {
            printf("Invalide, entrez un nombre entre %d et %d.\n", min, max);

            while (getchar() != '\n')
                ; // Eviter la boucle infinie en supprimant les entrées
        }

    } while (check != 1 || *valeur < min || *valeur > max);
}

// Évaluer une suite de 4 cases pour le score
int32_t evaluerSuite(uint16_t case1, uint16_t case2, uint16_t case3, uint16_t case4, uint16_t joueur, uint16_t adversaire)
{
    int32_t score = 0;
    // Si on a 4 jetons joueurs
    if (case1 == joueur && case2 == joueur && case3 == joueur && case4 == joueur)
    {
        // printf("100\n");
        score = 100;
        return score;
    }
    // Si on a 4 jetons adverses
    else if (case1 == adversaire && case2 == adversaire && case3 == adversaire && case4 == adversaire)
    {
        // printf("-95\n");
        score = -95;
        return score;
    }
    // Si on a 3 jetons joueurs et un emplacement vide
    else if ((case1 == joueur && case2 == joueur && case3 == joueur && case4 == 0) || (case1 == 0 && case2 == joueur && case3 == joueur && case4 == joueur))
    {
        // printf("5\n");
        score = 5;
        return score;
    }
    // Si on a 3 jetons adverses et un emplacement vide
    else if ((case1 == adversaire && case2 == adversaire && case3 == adversaire && case4 == 0) || (case1 == 0 && case2 == adversaire && case3 == adversaire && case4 == adversaire))
    {
        // printf("-4\n");
        score = -4;
        return score;
    }
    // Si on a 2 jetons joueurs et 2 emplacements vides
    else if ((case1 == joueur && case2 == joueur && case3 == 0 && case4 == 0) || (case1 == 0 && case2 == joueur && case3 == joueur && case4 == 0) || (case1 == 0 && case2 == 0 && case3 == joueur && case4 == joueur))
    {
        // printf("2\n");
        score = 2;
        return score;
    }
    // Si on a 2 jetons adverses et 2 emplacements vides
    else if ((case1 == adversaire && case2 == adversaire && case3 == 0 && case4 == 0) || (case1 == 0 && case2 == adversaire && case3 == adversaire && case4 == 0) || (case1 == 0 && case2 == 0 && case3 == adversaire && case4 == adversaire))
    {
        // printf("-1\n");
        score = -1;
        return score;
    }
    return score;
}

// Score du plateau avec joueur en positif et adversaire en négatif
int32_t score(uint16_t plateau[ROWS][COLS], uint16_t joueur, uint16_t adversaire)
{
    int32_t valeur = 0;

    for (int16_t i = ROWS - 1; i >= 0; i--)
    {
        for (int16_t j = COLS - 1; j >= 0; j--)
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
            // Vérification des diagonales (de gauche à droite)
            if (i >= 3 && j >= 3)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i - 1][j - 1], plateau[i - 2][j - 2], plateau[i - 3][j - 3], joueur, adversaire);
            }
            // Vérification des diagonales (de droite à gauche)
            if (i >= 3 && j <= COLS - 4)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i - 1][j + 1], plateau[i - 2][j + 2], plateau[i - 3][j + 3], joueur, adversaire);
            }
        }
    }
    return valeur;
}

// Annuler un coup fait dans une colonne
void annulerCoup(uint16_t plateau[ROWS][COLS], uint16_t colonne)
{
    for (uint16_t row = 0; row < ROWS; row++)
    {
        if (plateau[row][colonne] != 0)
        {
            plateau[row][colonne] = 0;
            return;
        }
    }
}

int32_t construireArbre(noeud *racine, uint16_t profondeur, uint16_t joueur, uint16_t adversaire, bool maximiser, int32_t alpha, int32_t beta)
{
    if (profondeur == 0)
    {
        return score(racine->plateau, joueur, adversaire);
    }

    int32_t meilleurScore = maximiser ? INT32_MIN : INT32_MAX;

    for (uint16_t col = 0; col < COLS; col++)
    {
        if (!colonneEstPleine(racine->plateau, col))
        {
            placerJeton(racine->plateau, col, joueur);

            noeud *nouveauNoeud = creerNoeud(racine->plateau, col);
            int32_t scoreEnfant = construireArbre(nouveauNoeud, profondeur - 1, adversaire, joueur, !maximiser, alpha, beta);

            if (maximiser)
            {
                meilleurScore = max(meilleurScore, scoreEnfant);
                alpha = max(alpha, meilleurScore);
            }
            else
            {
                meilleurScore = min(meilleurScore, scoreEnfant);
                beta = min(beta, meilleurScore);
            }

            ajouterEnfant(racine, nouveauNoeud);

            annulerCoup(racine->plateau, col);

            if (beta <= alpha)
                break;
        }
    }

    return meilleurScore;
}

void minimax(uint16_t plateau[ROWS][COLS], uint16_t *colonne, uint16_t joueur, uint16_t adversaire)
{
    noeud *racine = creerNoeud(plateau, 0);

    int32_t meilleurScore = INT32_MIN;

    for (uint16_t col = 0; col < COLS; col++)
    {
        if (!colonneEstPleine(racine->plateau, col))
        {
            placerJeton(racine->plateau, col, joueur);

            noeud *nouveauNoeud = creerNoeud(racine->plateau, col);
            int32_t scoreEnfant = construireArbre(nouveauNoeud, DEPTH - 1, adversaire, joueur, false, INT32_MIN, INT32_MAX);

            if (scoreEnfant > meilleurScore)
            {
                meilleurScore = scoreEnfant;
                *colonne = col;
            }

            ajouterEnfant(racine, nouveauNoeud);

            annulerCoup(racine->plateau, col);
        }
    }

    libererArbre(racine);
}


void JeuIA(uint16_t plateau[ROWS][COLS])
{
    initialiserPlateau(plateau);
    uint16_t colonne;

    // Boucle principale du jeu
    while (true)
    {
        // Demander au joueur de jouer
        printf("Votre tour, entrez le numéro de colonne (1-%d) : ", COLS);

        do
        {
            myscanf(&colonne, 1, COLS);
            if (colonneEstPleine(plateau, colonne - 1))
            {
                printf("Colonne pleine. Choisissez une autre colonne : ");
            }
        } while (colonneEstPleine(plateau, colonne - 1));

        // Placer le jeton du joueur
        placerJeton(plateau, colonne - 1, JOUEUR1);

        afficherPlateau(plateau);

        // Vérifier si le joueur a gagné
        if (verifierGagnant(plateau, JOUEUR1))
        {
            printf("Vous avez gagné !\n");
            break;
        }

        minimax(plateau, &colonne, IA, JOUEUR1);

        printf("\nTour de l'ordinateur : %d\n", colonne + 1);

        placerJeton(plateau, colonne, IA);

        afficherPlateau(plateau);

        if (verifierGagnant(plateau, IA))
        {
            printf("l'IA a gagné !\n");
            break;
        }
    }
}

void Jeu2(uint16_t plateau[ROWS][COLS])
{
    initialiserPlateau(plateau);
    uint16_t colonne;

    // Boucle principale du jeu
    while (true)
    {
        // Demander au joueur 1 de jouer
        printf("Joueur 1, entrez le numéro de colonne (1-%d) : ", COLS);

        do
        {
            myscanf(&colonne, 1, COLS);
            if (colonneEstPleine(plateau, colonne - 1))
            {
                printf("Colonne pleine. Choisissez une autre colonne : ");
            }
        } while (colonneEstPleine(plateau, colonne - 1));

        // Placer le jeton du joueur 1
        placerJeton(plateau, colonne - 1, JOUEUR1);

        // Vérifier si le joueur 1 a gagné
        if (verifierGagnant(plateau, JOUEUR1))
        {
            afficherPlateau(plateau);
            printf("Joueur 1 a gagné !\n");
            break;
        }

        // Afficher le plateau
        afficherPlateau(plateau);

        // Demander au joueur 2 de jouer
        printf("Joueur 2, entrez le numéro de colonne (1-%d) : ", COLS);
        do
        {
            myscanf(&colonne, 1, COLS);
            if (colonneEstPleine(plateau, colonne - 1))
            {
                printf("Colonne pleine. Choisissez une autre colonne : ");
            }
        } while (colonneEstPleine(plateau, colonne - 1));

        // Placer le jeton du joueur 2
        placerJeton(plateau, colonne - 1, JOUEUR2);

        // Vérifier si le joueur 2 a gagné
        if (verifierGagnant(plateau, JOUEUR2))
        {
            afficherPlateau(plateau);
            printf("Joueur 2 a gagné !\n");
            break;
        }

        // Afficher le plateau
        afficherPlateau(plateau);
    }
}

int main()
{
    system("clear");

    srand(time(NULL));

    uint16_t plateau[ROWS][COLS];

    uint16_t choix;

    afficherBanniere();

    do
    {
        afficherMenu();
        printf("Choisissez une option (1-3) : ");
        myscanf(&choix, 1, 3);

        switch (choix)
        {
        case 1:
            printf("Vous avez choisi de jouer à 2 joueurs.\n");
            Jeu2(plateau);
            break;
        case 2:
            printf("Vous avez choisi de jouer contre l'IA.\n");
            JeuIA(plateau);
            break;
        case 3:
            printf("Au revoir!\n");
            break;
        default:
            printf("Choix non valide. Veuillez choisir une option entre 1 et 3.\n");
            break;
        }

    } while (choix != 3);

    return 0;
}
