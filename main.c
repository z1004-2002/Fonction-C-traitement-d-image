#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define C_PI 3.141592653589793
#define MAX_THETA 180

/*-------------------------------------------
//? DEFINITION DE LA STRUCTURE DE DONNEES QUI VA RECEVOIR L'IMAGE
//? la largeur, la hauteur, la valeur maximale d'un pixel et le table des valeurs des pixels
---------------------------------------------*/
typedef struct
{
    int largeur;
    int hauteur;
    int max_val;
    unsigned char *data;
} ImagePGM;

/*-------------------------------------------
//? FONCTIONS PERMETTANT D'OBTENIR LE NUMÉRO DE LIGNE ET DE COLONNE
---------------------------------------------*/
//! Numéro de la ligne
int get_numero_ligne(int n, int largeur)
{
    return (n - n % largeur) / largeur;
}
//! numéro de la colonne
int get_numero_colonne(int n, int largeur)
{
    return n % largeur;
}
//! position dans le fichier
int get_position(int i, int j, int largeur)
{
    return i * largeur + j;
}

/*-------------------------------------------
//? FONCTIONS DE LECTURE DE L'IMAGE
---------------------------------------------*/
ImagePGM *lecture(const char *nom_fichier)
{
    /*-------------------------------------------
    //? LECTURE DU FICHIER QUI CONTIENT L'IMAGE
    ---------------------------------------------*/
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier)
    {
        perror("cannot open");
        return NULL;
    }

    /*-------------------------------------------
    //? LIBERATION DE L'ESPACE MEMOIRE POUR CONTENIR LES INFOS DE L'IMAGE
    ---------------------------------------------*/
    ImagePGM *image = malloc(sizeof(ImagePGM));
    if (!image)
    {
        fclose(fichier);
        perror("cannot allocate memory");
        return NULL;
    }

    /*-------------------------------------------
    //? LECTURE DE L'ENTETE DU FICHIER PGM
    ---------------------------------------------*/
    char format[3];
    fscanf(fichier, "%2s", format);
    if (strcmp(format, "P5") != 0)
    {
        fprintf(stderr, "Format non pris en charge: %s\n", format);
        free(image);
        fclose(fichier);
        return NULL;
    }

    /*-------------------------------------------
    //? LECTURE DES INFORMATION SUR LE FICHIER TEL QUE
    //? LA LONGUEUR, LA LARGEUR, ET LA VALEUR MAX D'UN PIXEL
    ---------------------------------------------*/
    fscanf(fichier, "%d %d", &image->largeur, &image->hauteur);
    fscanf(fichier, "%d", &image->max_val);
    //?LECTURE DE LA LIGNE VIDE
    fgetc(fichier);

    /*-------------------------------------------
    //? LIBERATION DE L'ESPACE MEMOIRE POUR CONTENIR LES DONNEES DE L'IMAGE
    ---------------------------------------------*/
    image->data = malloc(image->largeur * image->hauteur);
    if (!image->data)
    {
        free(image);
        fclose(fichier);
        perror("ne peut pas allouer la mémoire à l'image");
        return NULL;
    }
    /*-------------------------------------------
    //? LECTURE RÉCUPÉRATION DE L'IMAGE
    ---------------------------------------------*/
    fread(image->data, sizeof(unsigned char), image->largeur * image->hauteur, fichier);
    fclose(fichier);

    return image;
}

/*-------------------------------------------
//? FONCTION D'ENREGISTREMENT DE L'IMAGE MODIFIÉE
---------------------------------------------*/
void enregister_pgm(const char *nom_fichier, ImagePGM *image)
{
    FILE *fichier = fopen(nom_fichier, "w");
    if (!fichier)
    {
        perror("cannot open");
        return;
    }

    fprintf(fichier, "P5\n%d %d\n%d\n", image->largeur, image->hauteur, image->max_val);
    fwrite(image->data, sizeof(unsigned char), image->largeur * image->hauteur, fichier);
    fclose(fichier);
}
/*-------------------------------------------
//? FONCTION DE CRÉATION D'IMAGE NOIR
---------------------------------------------*/
ImagePGM *init_image_pgm(int hauteur, int largeur, int max_val)
{
    /*-------------------------------------------
    //? LIBERATION DE L'ESPACE MEMOIRE POUR CONTENIR LES INFOS DE L'IMAGE
    ---------------------------------------------*/
    ImagePGM *image_noir = malloc(sizeof(ImagePGM));
    if (!image_noir)
    {
        perror("cannot allocate memory");
        return NULL;
    }
    //? RÉCUPÉRATION DES PARAMÈTRES DE L'IMAGE
    image_noir->hauteur = hauteur;
    image_noir->largeur = largeur;
    image_noir->max_val = max_val;

    /*-------------------------------------------
    //? LIBERATION DE L'ESPACE MEMOIRE POUR CONTENIR LES DONNEES DE L'IMAGE
    ---------------------------------------------*/
    image_noir->data = malloc(image_noir->largeur * image_noir->hauteur);
    if (!image_noir->data)
    {
        free(image_noir);
        perror("ne peut pas allouer la mémoire à l'image");
        return NULL;
    }
    for (int i = 0; i < image_noir->largeur * image_noir->hauteur; i++)
    {
        image_noir->data[i] = 0;
    }
    return image_noir;
}

/*-------------------------------------------
//? FONCTION DE LIBÉRATION DE L'ESPACE MÉMOIRE UTILISÉ POUR MANIPULER L'IMAGE
---------------------------------------------*/
void liberer_une_image(ImagePGM *image)
{
    if (image)
    {
        free(image->data);
        free(image);
    }
}
/*-------------------------------------------
//? FONCTION DE SOMME DE DEUX IMAGES
---------------------------------------------*/
ImagePGM *somme_images(ImagePGM *image1, ImagePGM *image2)
{
    if (image1->hauteur != image2->hauteur || image1->largeur != image2->largeur)
    {
        printf("\nles hauteurs ou les largeurs de vos deux images ne correspondent pas\n");
        return NULL;
    }
    /*-------------------------------------------
    //? LIBERATION DE L'ESPACE MEMOIRE POUR CONTENIR LES INFOS DE L'IMAGE
    ---------------------------------------------*/
    ImagePGM *somme = malloc(sizeof(ImagePGM));
    if (!somme)
    {
        perror("cannot allocate memory");
        return NULL;
    }
    //? RÉCUPÉRATION DES PARAMÈTRES DE L'IMAGE
    somme->hauteur = image1->hauteur;
    somme->largeur = image1->largeur;
    somme->max_val = image1->max_val;

    /*-------------------------------------------
    //? LIBERATION DE L'ESPACE MEMOIRE POUR CONTENIR LES DONNEES DE L'IMAGE
    ---------------------------------------------*/
    somme->data = malloc(somme->largeur * somme->hauteur);
    if (!image1->data)
    {
        free(somme);
        perror("ne peut pas allouer la mémoire à l'image");
        return NULL;
    }
    for (int i = 0; i < somme->largeur * somme->hauteur; i++)
    {
        int n = image1->data[i] + image2->data[i];
        if (n > somme->max_val)
        {
            somme->data[i] = somme->max_val;
        }
        else
        {
            somme->data[i] = n;
        }
    }
    return somme;
}

/*-------------------------------------------
//? FONCTION DE DIFFÉRENCE DE DEUX IMAGES
---------------------------------------------*/
ImagePGM *difference_images(ImagePGM *image1, ImagePGM *image2)
{
    if (image1->hauteur != image2->hauteur || image1->largeur != image2->largeur)
    {
        printf("\nles hauteurs ou les largeurs de vos deux images ne correspondent pas\n");
        return NULL;
    }
    /*-------------------------------------------
    //? LIBERATION DE L'ESPACE MEMOIRE POUR CONTENIR LES INFOS DE L'IMAGE
    ---------------------------------------------*/
    ImagePGM *somme = malloc(sizeof(ImagePGM));
    if (!somme)
    {
        perror("cannot allocate memory");
        return NULL;
    }
    //? RÉCUPÉRATION DES PARAMÈTRES DE L'IMAGE
    somme->hauteur = image1->hauteur;
    somme->largeur = image1->largeur;
    somme->max_val = image1->max_val;

    /*-------------------------------------------
    //? LIBERATION DE L'ESPACE MEMOIRE POUR CONTENIR LES DONNEES DE L'IMAGE
    ---------------------------------------------*/
    somme->data = malloc(somme->largeur * somme->hauteur);
    if (!image1->data)
    {
        free(somme);
        perror("ne peut pas allouer la mémoire à l'image");
        return NULL;
    }
    for (int i = 0; i < somme->largeur * somme->hauteur; i++)
    {
        int n = image1->data[i] - image2->data[i];
        if (n < 0)
        {
            somme->data[i] = 0;
        }
        else
        {
            somme->data[i] = n;
        }
    }
    return somme;
}

/*-------------------------------------------
//? FONCTION D'AUGMENTATION DE LA LUMINOSITÉ
---------------------------------------------*/
ImagePGM *modifier_luminosite(ImagePGM *image, int facteur)
{
    ImagePGM *intensity_image = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    for (int i = 0; i < image->largeur * image->hauteur; i++)
    {
        int nouvelle = image->data[i] + facteur;
        if (nouvelle > image->max_val)
            nouvelle = image->max_val;
        if (nouvelle < 0)
            nouvelle = 0;
        intensity_image->data[i] = (unsigned char)nouvelle;
    }
    return intensity_image;
}

/*-------------------------------------------
//? FONCTION D'AMÉLIORATION DU CONTRASTE
---------------------------------------------*/
ImagePGM *modification_basique_du_contraste(ImagePGM *image)
{
    ImagePGM *contrast_image = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    //! DÉTERMINATION DU MIN ET DU MAX
    int min = image->data[0];
    int max = image->data[0];
    for (int i = 0; i < image->hauteur * image->largeur; i++)
    {
        if (max < image->data[i])
            max = image->data[i];
        if (image->data[i] < min)
            min = image->data[i];
    }

    //! APPLICATION DE LA FONCTION DE MODIFICATION DU CONTRAST
    for (int i = 0; i < image->hauteur * image->largeur; i++)
    {
        contrast_image->data[i] = (unsigned char)image->max_val * (image->data[i] - min) / (max - min);
    }

    return contrast_image;
}

/*-------------------------------------------
//? HISTOGRAMME D'UNE IMAGE
---------------------------------------------*/
int *histogramme(ImagePGM *image)
{
    int *hist = malloc(image->max_val * sizeof(int));
    //* initialisationde l'histogramme
    for (int i = 0; i < image->max_val; i++)
    {
        hist[i] = 0;
    }
    //*remplissage de l'histogramme
    for (int i = 0; i < image->largeur * image->hauteur; i++)
    {
        hist[image->data[i]] += 1;
    }
    return hist;
}

/*-------------------------------------------
//? FONCTION D'APPLANISSEMENT DE L'HISTOGRAMME
---------------------------------------------*/
ImagePGM *egaliser_histogramme(ImagePGM *image)
{
    ImagePGM *hist_equal = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    //*Etape 1 : Calcul de l'histogramme
    int *hist = histogramme(image);

    //* Etape 2 : Normalisation de l'histogramme
    float *nouvel_histogram = malloc(image->max_val * sizeof(float));
    for (int i = 0; i < image->max_val; i++)
    {
        nouvel_histogram[i] = hist[i] / ((float)(image->largeur * image->hauteur));
    }

    //*Etape 3 : Densité de probabilité normalisé

    float *densite = malloc(image->max_val * sizeof(float));
    //* initialisation
    for (int i = 0; i < image->max_val; i++)
    {
        densite[i] = 0.0;
    }
    for (int i = 0; i < image->max_val; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            densite[i] += nouvel_histogram[j];
        }
    }

    //*Etape 4 : Transformation des niveaux de gris de l'image
    for (int i = 0; i < image->hauteur * image->largeur; i++)
    {
        hist_equal->data[i] = (unsigned char)(densite[image->data[i]] * 255);
    }

    return hist_equal;
}

/*-------------------------------------------
//? SEULLAGE D'UNE IMAGE (transformation en image binaire)
---------------------------------------------*/
ImagePGM *seuillage(ImagePGM *image, int seuil)
{
    if (seuil < 0 || seuil > image->max_val)
        return NULL;

    //?initialisation des images
    ImagePGM *image_binaire = init_image_pgm(image->hauteur, image->largeur, image->max_val);

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        if (image->data[n] < seuil)
        {
            image_binaire->data[n] = 0;
        }
        else
        {
            image_binaire->data[n] = image_binaire->max_val;
        }
    }

    return image_binaire;
}

/*-------------------------------------------
//? FONCTION DE LISSAGE(Moyenneur)
---------------------------------------------*/
ImagePGM *filtre_moyenneur(ImagePGM *image)
{
    float filtre[3][3] = {{1.0 / 9, 1.0 / 9, 1.0 / 9}, {1.0 / 9, 1.0 / 9, 1.0 / 9}, {1.0 / 9, 1.0 / 9, 1.0 / 9}};

    //?initialisation des images
    ImagePGM *image_lisse = init_image_pgm(image->hauteur, image->largeur, image->max_val);

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);

        if (i < image->hauteur && i + 1 < image->hauteur && i + 2 < image->hauteur && j < image->largeur && j + 1 < image->largeur && j + 2 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i, j + 2, image->largeur);
            int c = get_position(i + 1, j, image->largeur);
            int d = get_position(i + 1, j + 1, image->largeur);
            int e = get_position(i + 1, j + 2, image->largeur);
            int f = get_position(i + 2, j, image->largeur);
            int g = get_position(i + 2, j + 1, image->largeur);
            int h = get_position(i + 2, j + 2, image->largeur);

            image_lisse->data[n] = abs(image->data[n] * filtre[0][0] + image->data[a] * filtre[0][1] + image->data[b] * filtre[0][2] + image->data[c] * filtre[1][0] + image->data[d] * filtre[1][1] + image->data[e] * filtre[1][2] + image->data[f] * filtre[2][0] + image->data[g] * filtre[2][1] + image->data[h] * filtre[2][2]);
        }
    }

    return image_lisse;
}

/*-------------------------------------------
//? FONCTION DE LISSAGE(Gaussien)
---------------------------------------------*/
ImagePGM *filtre_gaussien(ImagePGM *image)
{
    float filtre[3][3] = {{1.0 / 16, 1.0 / 8, 1.0 / 16}, {1.0 / 8, 1.0 / 4, 1.0 / 8}, {1.0 / 16, 1.0 / 8, 1.0 / 16}};

    //?initialisation des images
    ImagePGM *image_lisse = init_image_pgm(image->hauteur, image->largeur, image->max_val);

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);

        if (i < image->hauteur && i + 1 < image->hauteur && i + 2 < image->hauteur && j < image->largeur && j + 1 < image->largeur && j + 2 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i, j + 2, image->largeur);
            int c = get_position(i + 1, j, image->largeur);
            int d = get_position(i + 1, j + 1, image->largeur);
            int e = get_position(i + 1, j + 2, image->largeur);
            int f = get_position(i + 2, j, image->largeur);
            int g = get_position(i + 2, j + 1, image->largeur);
            int h = get_position(i + 2, j + 2, image->largeur);

            image_lisse->data[n] = abs(image->data[n] * filtre[0][0] + image->data[a] * filtre[0][1] + image->data[b] * filtre[0][2] + image->data[c] * filtre[1][0] + image->data[d] * filtre[1][1] + image->data[e] * filtre[1][2] + image->data[f] * filtre[2][0] + image->data[g] * filtre[2][1] + image->data[h] * filtre[2][2]);
        }
    }

    return image_lisse;
}

/*-------------------------------------------
//? FONCTION DE DETECTION DES CONTOURS(Prewitt)
---------------------------------------------*/
ImagePGM *filtre_prewitt(ImagePGM *image)
{
    int Gx[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};

    //?initialisation des images
    ImagePGM *imageX = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    ImagePGM *imageY = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);

        if (i < image->hauteur && i + 1 < image->hauteur && i + 2 < image->hauteur && j < image->largeur && j + 1 < image->largeur && j + 2 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i, j + 2, image->largeur);
            int c = get_position(i + 1, j, image->largeur);
            int d = get_position(i + 1, j + 1, image->largeur);
            int e = get_position(i + 1, j + 2, image->largeur);
            int f = get_position(i + 2, j, image->largeur);
            int g = get_position(i + 2, j + 1, image->largeur);
            int h = get_position(i + 2, j + 2, image->largeur);
            imageX->data[n] = abs(image->data[n] * Gx[0][0] + image->data[a] * Gx[0][1] + image->data[b] * Gx[0][2] + image->data[c] * Gx[1][0] + image->data[d] * Gx[1][1] + image->data[e] * Gx[1][2] + image->data[f] * Gx[2][0] + image->data[g] * Gx[2][1] + image->data[h] * Gx[2][2]);
            imageY->data[n] = abs(image->data[n] * Gy[0][0] + image->data[a] * Gy[0][1] + image->data[b] * Gy[0][2] + image->data[c] * Gy[1][0] + image->data[d] * Gy[1][1] + image->data[e] * Gy[1][2] + image->data[f] * Gy[2][0] + image->data[g] * Gy[2][1] + image->data[h] * Gy[2][2]);
        }
    }

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        if (imageX->data[n] + imageY->data[n] > imageX->max_val)
        {
            imageX->data[n] = imageX->max_val;
        }
        else
        {
            imageX->data[n] = imageX->data[n] + imageY->data[n];
        }
    }

    return imageX;
}

/*-------------------------------------------
//? FONCTION DE DETECTION DES CONTOURS(Sobel)
---------------------------------------------*/
ImagePGM *filtre_sobel(ImagePGM *image)
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //?initialisation des images
    ImagePGM *imageX = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    ImagePGM *imageY = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);

        if (i < image->hauteur && i + 1 < image->hauteur && i + 2 < image->hauteur && j < image->largeur && j + 1 < image->largeur && j + 2 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i, j + 2, image->largeur);
            int c = get_position(i + 1, j, image->largeur);
            int d = get_position(i + 1, j + 1, image->largeur);
            int e = get_position(i + 1, j + 2, image->largeur);
            int f = get_position(i + 2, j, image->largeur);
            int g = get_position(i + 2, j + 1, image->largeur);
            int h = get_position(i + 2, j + 2, image->largeur);
            imageX->data[n] = abs(image->data[n] * Gx[0][0] + image->data[a] * Gx[0][1] + image->data[b] * Gx[0][2] + image->data[c] * Gx[1][0] + image->data[d] * Gx[1][1] + image->data[e] * Gx[1][2] + image->data[f] * Gx[2][0] + image->data[g] * Gx[2][1] + image->data[h] * Gx[2][2]);
            imageY->data[n] = abs(image->data[n] * Gy[0][0] + image->data[a] * Gy[0][1] + image->data[b] * Gy[0][2] + image->data[c] * Gy[1][0] + image->data[d] * Gy[1][1] + image->data[e] * Gy[1][2] + image->data[f] * Gy[2][0] + image->data[g] * Gy[2][1] + image->data[h] * Gy[2][2]);
        }
    }

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        if (imageX->data[n] + imageY->data[n] > imageX->max_val)
        {
            imageX->data[n] = imageX->max_val;
        }
        else
        {
            imageX->data[n] = imageX->data[n] + imageY->data[n];
        }
    }

    return imageX;
}

/*-------------------------------------------
//? FONCTION DE DETECTION DES CONTOURS(Robert)
---------------------------------------------*/
ImagePGM *filtre_robert(ImagePGM *image)
{
    int Gx[2][2] = {{0, 1}, {-1, 0}};
    int Gy[2][2] = {{1, 0}, {0, -1}};

    //?initialisation des images
    ImagePGM *imageX = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    ImagePGM *imageY = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);
        if (i < image->hauteur && i + 1 < image->hauteur && j < image->largeur && j + 1 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i + 1, j, image->largeur);
            int c = get_position(i + 1, j + 1, image->largeur);
            imageX->data[n] = abs(image->data[n] * Gx[0][0] + image->data[a] * Gx[0][1] + image->data[b] * Gx[1][0] + image->data[c] * Gx[1][1]);
            imageY->data[n] = abs(image->data[n] * Gy[0][0] + image->data[a] * Gy[0][1] + image->data[b] * Gy[1][0] + image->data[c] * Gy[1][1]);
        }
    }

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        if (imageX->data[n] + imageY->data[n] > imageX->max_val)
        {
            imageX->data[n] = imageX->max_val;
        }
        else
        {
            imageX->data[n] = imageX->data[n] + imageY->data[n];
        }
    }

    return imageX;
}

/*-------------------------------------------
//? FONCTION DE DETECTION DES CONTOURS(Usage de l'approximation discret du LAPLACIEN)
---------------------------------------------*/
ImagePGM *filtre_laplacien(ImagePGM *image)
{
    float laplacien[3][3] = {{1, 1, 1}, {1, -8, 1}, {1, 1, 1}};
    float laplacien2[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};

    //?initialisation des images
    ImagePGM *image_lisse = init_image_pgm(image->hauteur, image->largeur, image->max_val);

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);

        if (i < image->hauteur && i + 1 < image->hauteur && i + 2 < image->hauteur && j < image->largeur && j + 1 < image->largeur && j + 2 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i, j + 2, image->largeur);
            int c = get_position(i + 1, j, image->largeur);
            int d = get_position(i + 1, j + 1, image->largeur);
            int e = get_position(i + 1, j + 2, image->largeur);
            int f = get_position(i + 2, j, image->largeur);
            int g = get_position(i + 2, j + 1, image->largeur);
            int h = get_position(i + 2, j + 2, image->largeur);
            int valeur = abs(image->data[n] * laplacien[0][0] + image->data[a] * laplacien[0][1] +
                             image->data[b] * laplacien[0][2] + image->data[c] * laplacien[1][0] +
                             image->data[d] * laplacien[1][1] + image->data[e] * laplacien[1][2] +
                             image->data[f] * laplacien[2][0] + image->data[g] * laplacien[2][1] +
                             image->data[h] * laplacien[2][2]);

            image_lisse->data[n] = valeur;
        }
    }

    return image_lisse;
}



/*-------------------------------------------
//? FONCTION DE DETECTION DES CONTOURS AVEC SEUIL(Prewitt)
---------------------------------------------*/
ImagePGM *filtre_prewitt_seuil(ImagePGM *image, int seuil)
{
    int Gx[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};

    //?initialisation des images
    ImagePGM *imageX = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    ImagePGM *imageY = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);

        if (i < image->hauteur && i + 1 < image->hauteur && i + 2 < image->hauteur && j < image->largeur && j + 1 < image->largeur && j + 2 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i, j + 2, image->largeur);
            int c = get_position(i + 1, j, image->largeur);
            int d = get_position(i + 1, j + 1, image->largeur);
            int e = get_position(i + 1, j + 2, image->largeur);
            int f = get_position(i + 2, j, image->largeur);
            int g = get_position(i + 2, j + 1, image->largeur);
            int h = get_position(i + 2, j + 2, image->largeur);
            imageX->data[n] = abs(image->data[n] * Gx[0][0] + image->data[a] * Gx[0][1] + image->data[b] * Gx[0][2] + image->data[c] * Gx[1][0] + image->data[d] * Gx[1][1] + image->data[e] * Gx[1][2] + image->data[f] * Gx[2][0] + image->data[g] * Gx[2][1] + image->data[h] * Gx[2][2]);
            imageY->data[n] = abs(image->data[n] * Gy[0][0] + image->data[a] * Gy[0][1] + image->data[b] * Gy[0][2] + image->data[c] * Gy[1][0] + image->data[d] * Gy[1][1] + image->data[e] * Gy[1][2] + image->data[f] * Gy[2][0] + image->data[g] * Gy[2][1] + image->data[h] * Gy[2][2]);
        }
    }

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        if (imageX->data[n] + imageY->data[n] > seuil)
        {
            imageX->data[n] = imageX->max_val;
        }
        else
        {
            imageX->data[n] = 0;
            // imageX->data[n] = imageX->data[n] + imageY->data[n];
        }
    }

    return imageX;
}

/*-------------------------------------------
//? FONCTION DE DETECTION DES CONTOURS AVEC SEUIL(Sobel)
---------------------------------------------*/
ImagePGM *filtre_sobel_seuil(ImagePGM *image, int seuil)
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //?initialisation des images
    ImagePGM *imageX = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    ImagePGM *imageY = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);

        if (i < image->hauteur && i + 1 < image->hauteur && i + 2 < image->hauteur && j < image->largeur && j + 1 < image->largeur && j + 2 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i, j + 2, image->largeur);
            int c = get_position(i + 1, j, image->largeur);
            int d = get_position(i + 1, j + 1, image->largeur);
            int e = get_position(i + 1, j + 2, image->largeur);
            int f = get_position(i + 2, j, image->largeur);
            int g = get_position(i + 2, j + 1, image->largeur);
            int h = get_position(i + 2, j + 2, image->largeur);
            imageX->data[n] = abs(image->data[n] * Gx[0][0] + image->data[a] * Gx[0][1] + image->data[b] * Gx[0][2] + image->data[c] * Gx[1][0] + image->data[d] * Gx[1][1] + image->data[e] * Gx[1][2] + image->data[f] * Gx[2][0] + image->data[g] * Gx[2][1] + image->data[h] * Gx[2][2]);
            imageY->data[n] = abs(image->data[n] * Gy[0][0] + image->data[a] * Gy[0][1] + image->data[b] * Gy[0][2] + image->data[c] * Gy[1][0] + image->data[d] * Gy[1][1] + image->data[e] * Gy[1][2] + image->data[f] * Gy[2][0] + image->data[g] * Gy[2][1] + image->data[h] * Gy[2][2]);
        }
    }

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        if (imageX->data[n] + imageY->data[n] > seuil)
        {
            imageX->data[n] = imageX->max_val;
        }
        else
        {
            // imageX->data[n] = imageX->data[n] + imageY->data[n];
            imageX->data[n] = 0;
        }
    }

    return imageX;
}

/*-------------------------------------------
//? FONCTION DE DETECTION DES CONTOURS AVEC SEUIL(Robert)
---------------------------------------------*/
ImagePGM *filtre_robert_seuil(ImagePGM *image, int seuil)
{
    int Gx[2][2] = {{0, 1}, {-1, 0}};
    int Gy[2][2] = {{1, 0}, {0, -1}};

    //?initialisation des images
    ImagePGM *imageX = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    ImagePGM *imageY = init_image_pgm(image->hauteur, image->largeur, image->max_val);
    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);
        if (i < image->hauteur && i + 1 < image->hauteur && j < image->largeur && j + 1 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i + 1, j, image->largeur);
            int c = get_position(i + 1, j + 1, image->largeur);
            imageX->data[n] = abs(image->data[n] * Gx[0][0] + image->data[a] * Gx[0][1] + image->data[b] * Gx[1][0] + image->data[c] * Gx[1][1]);
            imageY->data[n] = abs(image->data[n] * Gy[0][0] + image->data[a] * Gy[0][1] + image->data[b] * Gy[1][0] + image->data[c] * Gy[1][1]);
        }
    }

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        if (imageX->data[n] + imageY->data[n] > seuil)
        {
            imageX->data[n] = imageX->max_val;
        }
        else
        {
            imageX->data[n] = 0;
            // imageX->data[n] = imageX->data[n] + imageY->data[n];
        }
    }

    return imageX;
}

/*-------------------------------------------
//? FONCTION DE DETECTION DES CONTOURS AVEC SEUIL(Usage de l'approximation discret du LAPLACIEN)
---------------------------------------------*/
ImagePGM *filtre_laplacien_seuil(ImagePGM *image, int seuil)
{
    float laplacien[3][3] = {{1, 1, 1}, {1, -8, 1}, {1, 1, 1}};
    float laplacien2[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};

    //?initialisation des images
    ImagePGM *image_lisse = init_image_pgm(image->hauteur, image->largeur, image->max_val);

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);

        if (i < image->hauteur && i + 1 < image->hauteur && i + 2 < image->hauteur && j < image->largeur && j + 1 < image->largeur && j + 2 < image->largeur)
        {
            int a = get_position(i, j + 1, image->largeur);
            int b = get_position(i, j + 2, image->largeur);
            int c = get_position(i + 1, j, image->largeur);
            int d = get_position(i + 1, j + 1, image->largeur);
            int e = get_position(i + 1, j + 2, image->largeur);
            int f = get_position(i + 2, j, image->largeur);
            int g = get_position(i + 2, j + 1, image->largeur);
            int h = get_position(i + 2, j + 2, image->largeur);
            int valeur = abs(image->data[n] * laplacien[0][0] + image->data[a] * laplacien[0][1] +
                             image->data[b] * laplacien[0][2] + image->data[c] * laplacien[1][0] +
                             image->data[d] * laplacien[1][1] + image->data[e] * laplacien[1][2] +
                             image->data[f] * laplacien[2][0] + image->data[g] * laplacien[2][1] +
                             image->data[h] * laplacien[2][2]);

            image_lisse->data[n] = (valeur >= seuil) ? image->max_val : 0;
        }
    }

    return image_lisse;
}

/*-------------------------------------------
//? FONCTION D'IMPLÉMENTATION DE LA MÉTHODE DE HOUGH
---------------------------------------------*/
ImagePGM *hough_transform(ImagePGM *image1,int seuil)
{
    ImagePGM *image = filtre_prewitt_seuil(image1,seuil);


    int diag = sqrt((image->hauteur * image->hauteur) + (image->largeur * image->largeur));
    int indLig,X;
    ImagePGM *imageVote = init_image_pgm(diag, 181, 255);

    // Application de Hough pour construction de l'image de vote
    for (int i = 0; i < image->hauteur; i++)
    {
        for (int j = 0; j < image->largeur; j++)
        {
            int pixel_value = image->data[get_position(i, j, image->largeur)];
            if (pixel_value >= 240)
            {
                for (int theta = 0; theta < 181; theta++)
                {
                    indLig = i * cos(theta * C_PI / 180) + j * sin(theta * C_PI / 180);
                    if (indLig >= 0 && indLig < diag)
                    {
                        imageVote->data[get_position(indLig, theta, 181)]++;
                    }
                }
            }
        }
    }

    // Initialisation de l'image contenant la droite contour
    ImagePGM *imageDroite = init_image_pgm(image->hauteur, image->largeur, image->max_val);

    // Construction de l'image droite
    for (int rho = 0; rho < diag; rho++)
    {
        for (int theta = 0; theta < 181; theta++)
        {
            if (imageVote->data[get_position(rho, theta, 181)] >= 108)
            {
                for (int j = 0; j < image->largeur; j++)
                {
                    X = (rho - (j * sin(theta * C_PI / 180))) / cos(theta * C_PI / 180);
                    if (X >= 0 && X < image->hauteur)
                    {
                        imageDroite->data[get_position(X, j, image->largeur)] = 255;
                    }
                }
            }
        }
    }

    // Construction de l'image finale en sommant l'image originale et l'image droite
    ImagePGM *imageFinale = somme_images(image1, imageDroite);

    // Libération des ressources intermédiaires
    liberer_une_image(imageVote);
    liberer_une_image(imageDroite);

    return imageFinale;
}


/*-------------------------------------------
//? FONCTION ZOOM IN (reduction de la taille d'une image)
---------------------------------------------*/
ImagePGM *zomm_in(ImagePGM *image)
{
    float polynome[2][2] = {{1.0 / 4, 1.0 / 4}, {1.0 / 4, 1.0 / 4}};

    //?initialisation des images
    ImagePGM *small_image = init_image_pgm(image->hauteur / 2, image->largeur / 2, image->max_val);

    for (int n = 0; n < small_image->largeur * small_image->hauteur; n++)
    {
        int j = get_numero_colonne(n, small_image->largeur);
        int i = get_numero_ligne(n, small_image->largeur);

        if (2 * i < image->hauteur && 2 * i + 1 < image->hauteur && 2 * j < image->largeur && 2 * j + 1 < image->largeur)
        {
            int a = get_position(2 * i, 2 * j, image->largeur);
            ;
            int b = get_position(2 * i, 2 * j + 1, image->largeur);
            int c = get_position(2 * i + 1, 2 * j, image->largeur);
            int d = get_position(2 * i + 1, 2 * j + 1, image->largeur);

            small_image->data[n] = abs(image->data[a] * polynome[0][0] + image->data[b] * polynome[0][1] + image->data[c] * polynome[1][0] + image->data[d] * polynome[1][1]);
        }
    }

    return small_image;
}

/*-------------------------------------------
//? FONCTION ZOOM OUT (augmentation de la taille d'une image)
---------------------------------------------*/
ImagePGM *zomm_out(ImagePGM *image)
{
    float polynome[2][2] = {{1.0 / 4, 1.0 / 4}, {1.0 / 4, 1.0 / 4}};

    //?initialisation des images
    ImagePGM *big_image = init_image_pgm(image->hauteur * 2, image->largeur * 2, image->max_val);

    for (int n = 0; n < image->largeur * image->hauteur; n++)
    {
        int j = get_numero_colonne(n, image->largeur);
        int i = get_numero_ligne(n, image->largeur);

        if (2 * i < big_image->hauteur && 2 * i + 1 < big_image->hauteur && 2 * j < big_image->largeur && 2 * j + 1 < big_image->largeur)
        {
            int a = get_position(2 * i, 2 * j, big_image->largeur);
            ;
            int b = get_position(2 * i, 2 * j + 1, big_image->largeur);
            int c = get_position(2 * i + 1, 2 * j, big_image->largeur);
            int d = get_position(2 * i + 1, 2 * j + 1, big_image->largeur);

            big_image->data[a] = image->data[n];
            big_image->data[b] = image->data[n];
            big_image->data[c] = image->data[n];
            big_image->data[d] = image->data[n];
        }
    }

    return big_image;
}

/*-------------------------------------------
//? FONCTION D'IMPLÉMENTATION DE LA MÉTHODE DE OTSU
---------------------------------------------*/
ImagePGM *binaire_otsu(ImagePGM *image)
{
    int seuil = 1;
    float *var_intra_classe = malloc(256 * sizeof(float));
    int *h = histogramme(image);

    for (int i = 1; i <= image->max_val; i++)
    {
        float m1 = 0.0;
        float p1 = 0.0;
        float m2 = 0.0;
        float p2 = 0.0;
        float var1 = 0.0;
        float var2 = 0.0;

        int aux = 0;
        for (int j = 0; j <= i - 1; j++)
        {
            aux += h[j];
        }

        p1 = aux * 1.0 / (image->largeur * image->hauteur);
        m1 = aux * 1.0 / (i);

        aux = 0;
        for (int j = i; j <= 255; j++)
        {
            aux += h[j];
        }
        p2 = aux * 1.0 / (image->largeur * image->hauteur * 1.0);
        m2 = aux * 1.0 / (256 - i);

        for (int j = 0; j <= i - 1; j++)
        {
            var1 += ((h[j] - m1) * (h[j] - m1));
        }

        for (int j = i; j <= 255; j++)
        {
            var2 += ((h[j] - m2) * (h[j] - m2));
        }
        var1 /= i;
        var2 /= (256 - i);
        var_intra_classe[i] = p1 * var1 + p2 * var2;
    }

    for (int i = 1; i <= 255; i++)
    {
        if (var_intra_classe[i] > var_intra_classe[seuil])
        {
            seuil = i;
        }
    }
    // printf("%d", seuil);

    return seuillage(image, seuil);
}

int to_int(const char * word){
    char *str = "123";
    int num;
    sscanf(str, "%d", &num);
    return num;
}

int main(int argc, char **argv)
{
    const char *mon_image = "images/airport.pgm";
    const char *mon_image2 = "images/goldhill.pgm";
    int seuil;

    ImagePGM *image = lecture(argv[2]);
    if(strcmp(argv[1], "addition") == 0 || strcmp(argv[1], "soustraction") == 0)
    {
        ImagePGM *image2 = lecture(argv[2]);
        if (strcmp(argv[1], "addition"))
        {
            ImagePGM *somme = somme_images(image, image2);
            const char *somme_img = "somme_img.pgm";
            enregister_pgm(somme_img, somme);
        }
        else{
            ImagePGM *diff = difference_images(image, image2);
            const char *diff_img = "diff_img.pgm";
            enregister_pgm(diff_img, diff);
        }
        liberer_une_image(image2);
    }

    else if (strcmp(argv[1], "contrast") == 0)
    {
        ImagePGM *image_contrast = modification_basique_du_contraste(image);
        const char *contrast_img = "contrast_img.pgm";
        enregister_pgm(contrast_img, image_contrast);
    }

    else if (strcmp(argv[1], "eq_histogramme") == 0)
    {
        ImagePGM *image_eq_hist = egaliser_histogramme(image);
        const char *eq_hist_img = "eq_hist_img.pgm";
        enregister_pgm(eq_hist_img, image_eq_hist);
    }
    else if (strcmp(argv[1], "zoom_in") == 0)
    {
        ImagePGM *image_zoom_in = zomm_in(image);
        const char *zoom_in_img = "zoom_in_img.pgm";
        enregister_pgm(zoom_in_img, image_zoom_in);
    }
    else if (strcmp(argv[1], "zoom_out") == 0)
    {
        ImagePGM *image_zoom_out = zomm_out(image);
        const char *zoom_out_img = "zoom_out_img.pgm";
        enregister_pgm(zoom_out_img, image_zoom_out);
    }
    else if (strcmp(argv[1], "seuillage") == 0)
    {
        seuil = to_int(argv[3]);
        ImagePGM *image_seuillage = seuillage(image, seuil);
        const char *seuillage_img = "seuillage_img.pgm";
        enregister_pgm(seuillage_img, image_seuillage);
    }
    else if (strcmp(argv[1], "otsu") == 0)
    {
        ImagePGM *image_otsu = binaire_otsu(image);
        const char *otsu_img = "otsu_img.pgm";
        enregister_pgm(otsu_img, image_otsu);
    }
    else if (strcmp(argv[1], "moyenneur") == 0)
    {
        ImagePGM *image_moyenneur = filtre_moyenneur(image);
        const char *moyenneur_img = "moyenneur_img.pgm";
        enregister_pgm(moyenneur_img, image_moyenneur);
    }
    else if (strcmp(argv[1], "gaussien") == 0)
    {
        ImagePGM *image_gaussien = filtre_gaussien(image);
        const char *gaussien_img = "gaussien_img.pgm";
        enregister_pgm(gaussien_img, image_gaussien);
    }
    else if (strcmp(argv[1], "luminosite") == 0)
    {
        int delta_luminosite = to_int(argv[3]);
        ImagePGM *image_modif = modifier_luminosite(image, delta_luminosite);
        const char *modif_img = "lumin_img.pgm";
        enregister_pgm(modif_img, image_modif);
    }
    else if (strcmp(argv[1], "robert") == 0)
    {
        ImagePGM *image_robert = filtre_robert(image);
        const char *robert_img = "robert_img.pgm";
        enregister_pgm(robert_img, image_robert);
    }
    else if (strcmp(argv[1], "prewitt") == 0)
    {
        ImagePGM *image_prewitt = filtre_prewitt(image);
        const char *prewitt_img = "prewitt_img.pgm";
        enregister_pgm(prewitt_img, image_prewitt);
    }
    else if (strcmp(argv[1], "sobel") == 0)
    {
        ImagePGM *image_sobel = filtre_sobel(image);
        const char *sobel_img = "sobel_img.pgm";
        enregister_pgm(sobel_img, image_sobel);
    }
    else if (strcmp(argv[1], "laplace") == 0)
    {
        ImagePGM *image_laplace = filtre_laplacien(image);
        const char *laplace_img = "laplace_img.pgm";
        enregister_pgm(laplace_img, image_laplace);
    }
    else if (strcmp(argv[1], "robert_seuil") == 0)
    {
        seuil = to_int(argv[3]);
        ImagePGM *image_robert_seuil = filtre_robert_seuil(image, seuil);
        const char *robert_seuil_img = "robert_seuil_img.pgm";
        enregister_pgm(robert_seuil_img, image_robert_seuil);
    }
    else if (strcmp(argv[1], "prewitt_seuil") == 0)
    {
        seuil = to_int(argv[3]);
        ImagePGM *image_prewitt_seuil = filtre_prewitt_seuil(image, seuil);
        const char *prewitt_seuil_img = "prewitt_seuil_img.pgm";
        enregister_pgm(prewitt_seuil_img, image_prewitt_seuil);
    }
    else if (strcmp(argv[1], "sobel_seuil") == 0)
    {
        seuil = to_int(argv[3]);
        ImagePGM *image_sobel_seuil = filtre_sobel_seuil(image, seuil);
        const char *sobel_seuil_img = "sobel_seuil_img.pgm";
        enregister_pgm(sobel_seuil_img, image_sobel_seuil);
    }
    else if (strcmp(argv[1], "laplace_seuil") == 0)
    {
        seuil = to_int(argv[3]);
        ImagePGM *image_laplace_seuil = filtre_laplacien_seuil(image, seuil);
        const char *laplace_seuil_img = "laplace_seuil_img.pgm";
        enregister_pgm(laplace_seuil_img, image_laplace_seuil);
    }
    else if (strcmp(argv[1], "hough") == 0)
    {
        seuil = to_int(argv[3]);
        ImagePGM *image_hough = hough_transform(image, seuil);
        const char *hough_img = "hough_img.pgm";
        enregister_pgm(hough_img, image_hough);
    }
    else{
        printf("Commande inconnue.\n");
        return 1;
    }
    
    return 0;
}