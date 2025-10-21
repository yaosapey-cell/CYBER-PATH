#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_HAUTEUR 23
#define MAX_LARGEUR 23

// couleur
#define RESET "\033[0m"
#define ROUGE "\033[31m"

// valider la position
int positionValide(char grille[][MAX_LARGEUR], int hauteur, int largeur, int x, int y) {
    if (x < 0 || x >= hauteur || y < 0 || y >= largeur) return 0;
    if (grille[x][y] != '-') return 0;

    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && ny >= 0 && nx < hauteur && ny < largeur && grille[nx][ny] != '-') {
                return 0;
            }
        }
    }
    return 1;
}
// Créer la grille de jeu
void creationGrille(char grille[][MAX_LARGEUR], int *hauteur, int *largeur) {
    *largeur = rand() % 6 + 17;
    *hauteur = rand() % 6 + 17;

    for (int i = 0; i < *hauteur; i++) {
        for (int j = 0; j < *largeur; j++) {
            grille[i][j] = '-';
        }
    }

    // Définir les limites de la grille
    for (int i = 0; i < *largeur; i++) {
        grille[0][i] = '0';
        grille[*hauteur - 1][i] = '0';
    }

    for (int i = 0; i < *hauteur; i++) {
        grille[i][0] = '0';
        grille[i][*largeur - 1] = '0';
    }

    // Ajouter des murs internes
    for (int i = 0; i < 2; i++) {
        int random1;

        random1 = rand() % (*hauteur - 2) + 1;
        grille[random1][1] = '0';

        random1 = rand() % (*hauteur - 2) + 1;
        grille[random1][*largeur - 2] = '0';

        random1 = rand() % (*largeur - 2) + 1;
        grille[1][random1] = '0';

        random1 = rand() % (*largeur - 2) + 1;
        grille[*hauteur - 2][random1] = '0';
    }

    // Placer les cibles
    int ciblesPlacees = 0;
    while (ciblesPlacees < 9) {
        int random1 = rand() % (*hauteur - 2) + 1;
        int random2 = rand() % (*largeur - 2) + 1;
        if (positionValide(grille, *hauteur, *largeur, random1, random2)) {
            grille[random1][random2] = '1';
            ciblesPlacees++;

            int random3 = rand() % 4 + 1;
            if (random3 == 1 && random1 > 1 && random2 > 1) {
                grille[random1-1][random2] = '0';
                grille[random1-1][random2-1] = '0';
                grille[random1][random2-1] = '0';
            } else if (random3 == 2 && random1 < *hauteur - 2 && random2 > 1) {
                grille[random1+1][random2-1] = '0';
                grille[random1][random2-1] = '0';
                grille[random1+1][random2] = '0';
            } else if (random3 == 3 && random1 > 1 && random2 < *largeur - 2) {
                grille[random1-1][random2] = '0';
                grille[random1-1][random2+1] = '0';
                grille[random1][random2+1] = '0';
            } else if (random3 == 4 && random1 < *hauteur - 2 && random2 < *largeur - 2) {
                grille[random1+1][random2] = '0';
                grille[random1+1][random2+1] = '0';
                grille[random1][random2+1] = '0';
            }
        }
    }

    // Placer les robots
    char robots[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        int random1, random2;
        do {
            random1 = rand() % (*hauteur - 2) + 1;
            random2 = rand() % (*largeur - 2) + 1;
        } while (grille[random1][random2] != '-');

        grille[random1][random2] = robots[i];
    }
}

// Effacer l'écran
void effacerEcran() {
    printf("\033[H\033[J");
}

// Afficher la grille avec des couleurs
void afficherGrille(char grille[][MAX_LARGEUR], int hauteur, int largeur) {
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            if (grille[i][j] == '0') {
                printf(ROUGE "0 " RESET); // murs rouge
            } else {
                printf("%c ", grille[i][j]);
            }
        }
        printf("\n");
    }
}

// Déplacer le robot selon l'entrée de l'utilisateur
int deplacerRobot(char grille[][MAX_LARGEUR], int hauteur, int largeur, int startX, int startY, int maxDeplacements) {
    int deplacements = 0;
    int x = startX;
    int y = startY;
    char robot = grille[x][y];
    char direction;

    while (deplacements < maxDeplacements) {
        printf("Il vous reste %d mouvements.\n", maxDeplacements - deplacements);
        printf("Entrez la direction (z=haut, s=bas, q=gauche, d=droite) : ");
        scanf(" %c", &direction);

        // Assurer une entrée valide
        if (direction != 'z' && direction != 's' && direction != 'q' && direction != 'd') {
            printf("Direction invalide ! Veuillez entrer z, s, q ou d.\n");
            continue;
        }

        // Incrémenter le compte des déplacements
        deplacements++;

        switch (direction) {
            case 'z': // Déplacer vers le haut
                while (x > 0 && grille[x-1][y] != '0' && grille[x-1][y] != 'A' && grille[x-1][y] != 'B' && grille[x-1][y] != 'C' && grille[x-1][y] != 'D') {
                    grille[x][y] = '-';
                    x--;
                    if (grille[x][y] == '1') {
                        grille[x][y] = robot;
                        afficherGrille(grille, hauteur, largeur);
                        return deplacements;
                    }
                    grille[x][y] = robot;
                }
                break;
            case 's': // Déplacer vers le bas
                while (x < hauteur - 1 && grille[x+1][y] != '0' && grille[x+1][y] != 'A' && grille[x+1][y] != 'B' && grille[x+1][y] != 'C' && grille[x+1][y] != 'D') {
                    grille[x][y] = '-';
                    x++;
                    if (grille[x][y] == '1') {
                        grille[x][y] = robot;
                        afficherGrille(grille, hauteur, largeur);
                        return deplacements;
                    }
                    grille[x][y] = robot;
                }
                break;
            case 'q': // Déplacer vers la gauche
                while (y > 0 && grille[x][y-1] != '0' && grille[x][y-1] != 'A' && grille[x][y-1] != 'B' && grille[x][y-1] != 'C' && grille[x][y-1] != 'D') {
                    grille[x][y] = '-';
                    y--;
                    if (grille[x][y] == '1') {
                        grille[x][y] = robot;
                        afficherGrille(grille, hauteur, largeur);
                        return deplacements;
                    }
                    grille[x][y] = robot;
                }
                break;
            case 'd': // Déplacer vers la droite
                while (y < largeur - 1 && grille[x][y+1] != '0' && grille[x][y+1] != 'A' && grille[x][y+1] != 'B' && grille[x][y+1] != 'C' && grille[x][y+1] != 'D') {
                    grille[x][y] = '-';
                    y++;
                    if (grille[x][y] == '1') {
                        grille[x][y] = robot;
                        afficherGrille(grille, hauteur, largeur);
                        return deplacements;
                    }
                    grille[x][y] = robot;
                }
                break;
        }

        afficherGrille(grille, hauteur, largeur);
    }

    return deplacements;
}

int main() {
    srand(time(NULL)); // nombres aléatoires

    int hauteur, largeur;
    char grille[MAX_HAUTEUR][MAX_LARGEUR];
    char robots[] = {'A', 'B', 'C', 'D'};

    // Choisir le niveau de difficulté
    int difficulte;
    do {
        printf("Choisissez le niveau de difficulté:\n");
        printf("1: Facile (35 secondes)\n");
        printf("2: Normal (25 secondes)\n");
        printf("3: Difficile (15 secondes)\n");
        printf("Entrez votre choix (1, 2, ou 3): ");
        if (scanf("%d", &difficulte) != 1) {
            while (getchar() != '\n');
            difficulte = 0;
        }
    } while (difficulte < 1 || difficulte > 3);

    // Définir le temps d'affichage en fonction du niveau de difficulté
    int tempsAffichage;
    switch (difficulte) {
        case 1: // Facile
            tempsAffichage = 35; // 35 sec
            break;
        case 2: // Normal
            tempsAffichage = 25; // 25 sec
            break;
        case 3: // Difficile
            tempsAffichage = 15; // 15 sec
            break;
    }

    // Variables de jeu
    int nombreJoueurs;
    do {
        printf("Entrez le nombre de joueurs (nombre entier positif) : ");
        if (scanf("%d", &nombreJoueurs) != 1 || nombreJoueurs <= 0) {
            while (getchar() != '\n'); 
            nombreJoueurs = 0;
        }
    } while (nombreJoueurs <= 0);

    int scoresJoueurs[nombreJoueurs];
    for (int i = 0; i < nombreJoueurs; i++) {
        scoresJoueurs[i] = 0;
    }

    int nombreRounds;
    do {
        printf("Entrez le nombre de rounds (nombre entier positif) : ");
        if (scanf("%d", &nombreRounds) != 1 || nombreRounds <= 0) {
            while (getchar() != '\n'); // Clear invalid input
            nombreRounds = 0;
        }
    } while (nombreRounds <= 0);

    // Créer une grille unique pour tous les rounds
    creationGrille(grille, &hauteur, &largeur);

    for (int round = 1; round <= nombreRounds; round++) {
        printf("Round %d/%d\n", round, nombreRounds);

        // Choisir un robot au hasard pour chaque round
        int indiceRobot = rand() % 4;
        char robotChoisi = robots[indiceRobot];
        int startX = -1, startY = -1;

        // Trouver la position initiale du robot choisi
        for (int i = 0; i < hauteur; i++) {
            for (int j = 0; j < largeur; j++) {
                if (grille[i][j] == robotChoisi) {
                    startX = i;
                    startY = j;
                    break;
                }
            }
            if (startX != -1) break;
        }

        if (startX == -1 || startY == -1) {
            printf("Aucun robot trouvé sur la grille.\n");
            continue;
        }

        // Afficher quel robot nous utilisons pour le round
        printf("Nous jouons avec le robot %c\n", robotChoisi);

        // Afficher la grille pendant le temps spécifié
        afficherGrille(grille, hauteur, largeur);
        printf("Vous avez %d secondes pour mémoriser la grille...\n", tempsAffichage);
        sleep(tempsAffichage); // Pause pendant le temps spécifié

        // Effacer l'écran
        effacerEcran();

        // Chaque joueur estime le nombre de déplacements
        int estimationsJoueurs[nombreJoueurs];
        for (int i = 0; i < nombreJoueurs; i++) {
            do {
                printf("Joueur %d, entrez le nombre de déplacements que vous pensez faire (nombre entier positif) : ", i + 1);
                if (scanf("%d", &estimationsJoueurs[i]) != 1 || estimationsJoueurs[i] <= 0) {
                    while (getchar() != '\n'); // Clear invalid input
                    estimationsJoueurs[i] = 0;
                }
            } while (estimationsJoueurs[i] <= 0);
        }

        // Déterminer le joueur avec l'estimation la plus basse
        int estimationMin = estimationsJoueurs[0];
        int joueurEstimationMin = 0;
        for (int i = 1; i < nombreJoueurs; i++) {
            if (estimationsJoueurs[i] < estimationMin) {
                estimationMin = estimationsJoueurs[i];
                joueurEstimationMin = i;
            }
        }

        printf("Joueur %d estime faire %d déplacements et va jouer.\n", joueurEstimationMin + 1, estimationMin);

        // Réafficher la grille pour le joueur qui fait les déplacements
        effacerEcran();
        printf("Nous jouons avec le robot %c\n", robotChoisi);
        afficherGrille(grille, hauteur, largeur);

        // Déplacer le robot selon l'entrée du joueur et compter les déplacements
        int deplacementsReels = deplacerRobot(grille, hauteur, largeur, startX, startY, estimationMin);

        // Affichage de débogage pour comprendre le nombre de déplacements
        printf("Joueur %d a utilisé %d déplacements.\n", joueurEstimationMin + 1, deplacementsReels);

        // Vérifier si le robot a atteint une cible (est sur une case '1')
        int cibleAtteinte = 0;
        if (grille[startX][startY] == '1') {
            cibleAtteinte = 1;
        }

        // Mettre à jour les scores selon le nouveau système de points
        if (cibleAtteinte) {
            if (deplacementsReels == estimationMin) {
                scoresJoueurs[joueurEstimationMin] += 2; // Atteindre une cible exactement dans les déplacements estimés
            } else if (deplacementsReels < estimationMin) {
                scoresJoueurs[joueurEstimationMin] -= 1; // Atteindre une cible avec moins de déplacements que l'estimation
                if (scoresJoueurs[joueurEstimationMin] < 0) {
                    scoresJoueurs[joueurEstimationMin] = 0; // Le score ne peut pas être inférieur à 0
                }
            }
        } else {
            // Si le joueur n'a pas atteint la cible, les autres joueurs gagnent 1 point
            for (int i = 0; i < nombreJoueurs; i++) {
                if (i != joueurEstimationMin) {
                    scoresJoueurs[i]++; // Les autres joueurs gagnent 1 point si le joueur échoue à atteindre la cible
                }
            }
        }

        // Afficher les scores actuels
        printf("Scores actuels :\n");
        for (int i = 0; i < nombreJoueurs; i++) {
            printf("Joueur %d : %d\n", i + 1, scoresJoueurs[i]);
        }
    }

    // Déterminer le(s) gagnant(s)
    int scoreMax = scoresJoueurs[0];
    for (int i = 1; i < nombreJoueurs; i++) {
        if (scoresJoueurs[i] > scoreMax) {
            scoreMax = scoresJoueurs[i];
        }
    }

    printf("Le(s) gagnant(s) :\n");
    for (int i = 0; i < nombreJoueurs; i++) {
        if (scoresJoueurs[i] == scoreMax) {
            printf("Joueur %d avec un score de %d !\n", i + 1, scoresJoueurs[i]);
        }
    }

    return 0;
}