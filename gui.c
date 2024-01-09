#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define ROWS 6
#define COLS 7

#define DEPTH 5

typedef struct noeud
{
    char plateau[ROWS][COLS]; // Configuration de la grille
    int colonne;              // Colonne jouée
    int score;                // Score associé à ce nœud
    struct noeud *enfants;    // Liste chaînée des nœuds enfants
    struct noeud *suivant;    // Nœud suivant dans la liste chaînée
} noeud;

GtkWidget *buttons[ROWS][COLS];
GtkWidget *label;

char plateau[ROWS][COLS];
#define JOUEUR1 'X'
#define IA 'O'

int score(char plateau[ROWS][COLS]);

// Fonction pour créer un nouveau nœud avec une configuration de grille donnée
noeud *creerNoeud(char plateau[ROWS][COLS], int colonne)
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
        nouveauNoeud->score = score(plateau);
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

// Fonction pour placer un jeton dans la colonne choisie
bool placerJeton(char plateau[ROWS][COLS], int colonne, char jeton)
{
    for (int i = ROWS - 1; i >= 0; i--)
    {
        if (plateau[i][colonne] == ' ')
        {
            plateau[i][colonne] = jeton;
            return true;
        }
    }
    return false; // Colonne pleine
}

// Fonction pour vérifier s'il y a un gagnant
bool verifierGagnant(char plateau[ROWS][COLS], char jeton)
{
    // Vérification des lignes
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j <= COLS - 4; j++)
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
    for (int i = 0; i <= ROWS - 4; i++)
    {
        for (int j = 0; j < COLS; j++)
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
    for (int i = 0; i <= ROWS - 4; i++)
    {
        for (int j = 0; j <= COLS - 4; j++)
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
    for (int i = 0; i <= ROWS - 4; i++)
    {
        for (int j = 3; j < COLS; j++)
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

// Function to update the game board display
void updateDisplay()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            char label[2] = {plateau[i][j], '\0'};
            gtk_button_set_label(GTK_BUTTON(buttons[i][j]), label);
        }
    }
}

// Function to restart the game
void restartGame(GtkWidget *button, gpointer data)
{
    // Enable all buttons
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            gtk_widget_set_sensitive(buttons[i][j], TRUE);
        }
    }

    // Clear the game board
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            plateau[i][j] = ' ';
        }
    }

    gtk_label_set_text(GTK_LABEL(label), "");

    // Clear the labels on the buttons
    updateDisplay();
}

// Évaluer une suite de 4 cases pour le score
int32_t evaluerSuite(char case1, char case2, char case3, char case4)
{
    int32_t score = 0;
    // Si on a 4 jetons jaunes (couleur IA)
    if (case1 == IA && case2 == IA && case3 == IA && case4 == IA)
    {
        score = 100;
        return score;
    }
    // Si on a 4 jetons rouges (la couleur de l’adversaire)
    else if (case1 == JOUEUR1 && case2 == JOUEUR1 && case3 == JOUEUR1 && case4 == JOUEUR1)
    {
        score = -95;
        return score;
    }
    // Si on a 3 jetons jaunes et un emplacement vide
    else if ((case1 == IA && case2 == IA && case3 == IA && case4 == ' ') || (case1 == ' ' && case2 == IA && case3 == IA && case4 == IA))
    {
        score = 5;
        return score;
    }
    // Si on a 3 jetons rouges et un emplacement vide
    else if ((case1 == JOUEUR1 && case2 == JOUEUR1 && case3 == JOUEUR1 && case4 == 0) || (case1 == 0 && case2 == JOUEUR1 && case3 == JOUEUR1 && case4 == JOUEUR1))
    {
        score = -4;
        return score;
    }
    // Si on a 2 jetons jaunes et 2 emplacements vides
    else if ((case1 == IA && case2 == IA && case3 == ' ' && case4 == ' ') || (case1 == ' ' && case2 == IA && case3 == IA && case4 == ' ') || (case1 == ' ' && case2 == ' ' && case3 == IA && case4 == IA))
    {
        score = 2;
        return score;
    }
    // Si on a 2 jetons rouges et 2 emplacements vides
    else if ((case1 == JOUEUR1 && case2 == JOUEUR1 && case3 == ' ' && case4 == ' ') || (case1 == ' ' && case2 == JOUEUR1 && case3 == JOUEUR1 && case4 == ' ') || (case1 == ' ' && case2 == ' ' && case3 == JOUEUR1 && case4 == JOUEUR1))
    {
        score = -1;
        return score;
    }
    return score;
}

int score(char plateau[ROWS][COLS])
{
    int valeur = 0;

    // Vérification des lignes, colonnes et diagonales
    for (int i = ROWS - 1; i >= 0; i--)
    {
        for (int j = COLS - 1; j >= 0; j--)
        {
            // Vérification des lignes
            if (j >= 3)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i][j - 1], plateau[i][j - 2], plateau[i][j - 3]);
                // printf("v : %d\n", evaluerSuite(plateau[i][j], plateau[i][j - 1], plateau[i][j - 2], plateau[i][j - 3]));
            }

            // Vérification des colonnes
            if (i >= 3)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i - 1][j], plateau[i - 2][j], plateau[i - 3][j]);
                // printf("v : %d\n", evaluerSuite(plateau[i][j], plateau[i - 1][j], plateau[i - 2][j], plateau[i - 3][j]));
            }

            // Vérification des diagonales (de gauche à droite)
            if (i >= 3 && j >= 3)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i - 1][j - 1], plateau[i - 2][j - 2], plateau[i - 3][j - 3]);
                // printf("v : %d\n", evaluerSuite(plateau[i][j], plateau[i - 1][j - 1], plateau[i - 2][j - 2], plateau[i - 3][j - 3]));
            }

            // Vérification des diagonales (de droite à gauche)
            if (i >= 3 && j <= COLS - 4)
            {
                valeur += evaluerSuite(plateau[i][j], plateau[i - 1][j + 1], plateau[i - 2][j + 2], plateau[i - 3][j + 3]);
                // printf("v : %d\n", evaluerSuite(plateau[i][j], plateau[i - 1][j + 1], plateau[i - 2][j + 2], plateau[i - 3][j + 3]));
            }
        }
    }

    return valeur;
}

void annulerCoup(char plateau[ROWS][COLS], int colonne)
{
    for (int row = 0; row < ROWS; row++)
    {
        if (plateau[row][colonne] != ' ')
        {
            plateau[row][colonne] = ' ';
            return;
        }
    }
}

// Fonction pour tester une colonne
bool colonneEstPleine(char plateau[ROWS][COLS], int colonne)
{
    for (int row = 0; row < ROWS; row++)
    {
        if (plateau[row][colonne] == ' ')
        {
            return false;
        }
    }
    return true;
}

void construireArbre(noeud *racine, int profondeur, char joueur)
{
    if (profondeur > DEPTH)
    {
        return;
    }

    for (int col = 0; col < COLS; col++)
    {
        if (!colonneEstPleine(racine->plateau, col))
        {
            // Effectuer le coup
            placerJeton(racine->plateau, col, joueur);

            // printf("Plateau DEPTH %d : \n", profondeur);
            // printf("Score %d : \n", score(racine->plateau));
            // afficherPlateau(racine->plateau);

            // Créer un nouveau nœud pour le coup
            noeud *nouveauNoeud = creerNoeud(racine->plateau, col);

            // Ajouter le nouveau nœud comme enfant
            ajouterEnfant(racine, nouveauNoeud);

            if (!verifierGagnant(nouveauNoeud->plateau, JOUEUR1) && !verifierGagnant(nouveauNoeud->plateau, IA))
            {
                // Appeler récursivement pour le joueur opposé
                construireArbre(nouveauNoeud, profondeur + 1, (joueur == IA) ? JOUEUR1 : IA);
            }

            // Annuler le coup pour revenir à l'état précédent
            annulerCoup(racine->plateau, col);
        }
    }
}

// Fonction pour évaluer le score d'un nœud en utilisant l'algorithme minimax
int32_t evaluerNoeud(noeud *n, int profondeur, bool maximiser)
{
    if (profondeur == 0 || n->enfants == NULL)
    {
        // Si la profondeur maximale est atteinte ou si le nœud est une feuille, retourner le score du nœud
        return n->score;
    }

    // Si c'est le tour de l'IA (maximisation)
    if (maximiser)
    {
        int meilleurScore = INT_MIN;

        // Parcourir les enfants du nœud
        for (noeud *enfant = n->enfants; enfant != NULL; enfant = enfant->suivant)
        {
            int scoreEnfant = evaluerNoeud(enfant, profondeur - 1, false);
            meilleurScore = max(meilleurScore, scoreEnfant);
        }

        return meilleurScore;
    }
    // Si c'est le tour de l'humain (minimisation)
    else
    {
        int pireScore = INT_MAX;

        // Parcourir les enfants du nœud
        for (noeud *enfant = n->enfants; enfant != NULL; enfant = enfant->suivant)
        {
            int scoreEnfant = evaluerNoeud(enfant, profondeur - 1, true);
            pireScore = min(pireScore, scoreEnfant);
        }

        return pireScore;
    }
}

// Fonction pour choisir le meilleur coup à jouer en utilisant l'algorithme minimax
void choisirMeilleurCoup(noeud *racine, int *colonne)
{
    int meilleurScore = INT_MIN;

    // Parcourir les enfants de la racine (les coups possibles)
    for (noeud *enfant = racine->enfants; enfant != NULL; enfant = enfant->suivant)
    {
        int scoreEnfant = evaluerNoeud(enfant, DEPTH, false);

        // Si le score de cet enfant est meilleur, mettre à jour la colonne choisie
        if (scoreEnfant > meilleurScore)
        {
            meilleurScore = scoreEnfant;
            *colonne = enfant->colonne;
        }
    }
    if (meilleurScore == 0)
    {
        *colonne = rand() % 6;
    }
}

void minimax(char plateau[ROWS][COLS], int *colonne, char joueur)
{
    noeud *racine = creerNoeud(plateau, 0);
    construireArbre(racine, 1, joueur);
    choisirMeilleurCoup(racine, colonne);
    libererArbre(racine);
}

// Function to handle button clicks
void onButtonClicked(GtkWidget *button, gpointer data)
{
    int colonne = GPOINTER_TO_INT(data);
    int flag = 0;
    minimax(plateau, &colonne, IA);

    // Check if the column is full
    if (placerJeton(plateau, colonne, IA))
    {
        // Update the display
        updateDisplay();

        // Check for a winner
        if (verifierGagnant(plateau, IA))
        {
            gtk_label_set_text(GTK_LABEL(label), "IA WINS");

            // Disable all buttons to finish the game
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    gtk_widget_set_sensitive(buttons[i][j], FALSE);
                }
            }
            flag = 1;
        }

        // Bot makes a move
        minimax(plateau, &colonne, JOUEUR1);
        placerJeton(plateau, colonne, JOUEUR1);
        updateDisplay();

        // Check for a winner
        if (verifierGagnant(plateau, JOUEUR1) && flag == 0)
        {
            gtk_label_set_text(GTK_LABEL(label), "PLAYER WINS");

            // Disable all buttons to finish the game
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    gtk_widget_set_sensitive(buttons[i][j], FALSE);
                }
            }
        }
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(label),"Column full. Choose another column");
    }
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *grid;

    srand(time(NULL));

    gtk_init(&argc, &argv);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Puissance 4");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a 7x6 grid of buttons
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Add a restart button to enable restarting the game
    GtkWidget *restartButton = gtk_button_new_with_label("Restart");
    g_signal_connect(restartButton, "clicked", G_CALLBACK(restartGame), NULL);
    gtk_grid_attach(GTK_GRID(grid), restartButton, 0, ROWS, COLS, 1);

    label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), label, 0, ROWS + 1, COLS, 1);

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            buttons[i][j] = gtk_button_new_with_label("");
            g_signal_connect(buttons[i][j], "clicked", G_CALLBACK(onButtonClicked), GINT_TO_POINTER(j));
            gtk_grid_attach(GTK_GRID(grid), buttons[i][j], j, i, 1, 1);
        }
    }

    // Initialize the game board
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            plateau[i][j] = ' ';
        }
    }

    // Show all the widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
