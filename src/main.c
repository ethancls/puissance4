#include "puissance4.h"

void PartieIA(uint8_t plateau[LIGNES][COLONNES])
{
    initialiserPlateau(plateau);
    uint8_t colonne, premier;

    printf("\nVoulez-vous jouer en premier ? 1 : OUI | 0: NON ");
    myscanf(&premier, 0, 1);

    while (true)
    {
        if (premier == 1)
        {
            // Tour Joueur
            minimax(plateau, &colonne, JOUEUR1, IA);
            afficherPlateau(plateau);
            printf("Meilleur coup à jouer : %d\n", colonne + 1);

            printf("\nVotre tour, entrez le numéro de colonne (1-%d) : ", COLONNES);
            do
            {
                myscanf(&colonne, 1, COLONNES);
                if (colonneEstPleine(plateau, colonne - 1))
                {
                    printf("Colonne pleine. Choisissez une autre colonne : ");
                }
            } while (colonneEstPleine(plateau, colonne - 1));

            placerJeton(plateau, colonne - 1, JOUEUR1);

            if (verifierGagnant(plateau, JOUEUR1, colonne - 1))
            {
                afficherPlateau(plateau);
                printf("Vous avez gagné !\n");
                break;
            }
            if (egalite(plateau))
            {
                printf("Match nul !\n");
                break;
            }

            premier = 0;
        }
        else
        {
            // Tour Ordinateur
            minimax(plateau, &colonne, IA, JOUEUR1);
            placerJeton(plateau, colonne, IA);
            afficherPlateau(plateau);
            printf("Tour de l'ordinateur : %d\n", colonne + 1);

            if (verifierGagnant(plateau, IA, colonne))
            {
                printf("l'IA a gagné !\n");
                break;
            }
            if (egalite(plateau))
            {
                printf("Match nul !\n");
                break;
            }

            premier = 1;
        }
    }
}

void PartieVS(uint8_t plateau[LIGNES][COLONNES])
{
    initialiserPlateau(plateau);
    uint8_t colonne;

    while (true)
    {
        // Tour Joueur 1
        afficherPlateau(plateau);

        printf("Joueur 1, entrez le numéro de colonne (1-%d) : ", COLONNES);

        do
        {
            myscanf(&colonne, 1, COLONNES);
            if (colonneEstPleine(plateau, colonne - 1))
            {
                printf("Colonne pleine. Choisissez une autre colonne : ");
            }
        } while (colonneEstPleine(plateau, colonne - 1));

        placerJeton(plateau, colonne - 1, JOUEUR1);

        if (verifierGagnant(plateau, JOUEUR1, colonne - 1))
        {
            afficherPlateau(plateau);
            printf("Joueur 1 a gagné !\n");
            break;
        }

        // Tour Joueur 2
        afficherPlateau(plateau);

        printf("Joueur 2, entrez le numéro de colonne (1-%d) : ", COLONNES);
        do
        {
            myscanf(&colonne, 1, COLONNES);
            if (colonneEstPleine(plateau, colonne - 1))
            {
                printf("Colonne pleine. Choisissez une autre colonne : ");
            }
        } while (colonneEstPleine(plateau, colonne - 1));

        placerJeton(plateau, colonne - 1, JOUEUR2);

        if (verifierGagnant(plateau, JOUEUR2, colonne - 1))
        {
            afficherPlateau(plateau);
            printf("Joueur 2 a gagné !\n");
            break;
        }
        if (egalite(plateau))
        {
            afficherPlateau(plateau);
            printf("Match nul !\n");
            break;
        }
    }
}

int main()
{
    system("clear");

    uint8_t plateau[LIGNES][COLONNES];

    uint8_t choix;

    afficherBanniere();

    do
    {
        afficherMenu();
        printf("Choisissez une option (1-3) : ");
        myscanf(&choix, 1, 3);

        switch (choix)
        {
        case 1:
            PartieVS(plateau);
            break;
        case 2:
            PartieIA(plateau);
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
