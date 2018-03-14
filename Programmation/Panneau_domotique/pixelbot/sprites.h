/*
 * Fichier généré par pixelbot@Robotronik
 * Paramètres :
 * Taille de la matrice : $MATRIX_SIZE px
 * Images par secondes : $FPS FPS
 * Niveaux de luminosité : $BRIGHTNESS_LEVELS
 * Fréquence de rafraîchissement : $CLOCK_FREQUENCY Hz
 * Sens de lecture : $READING_DIERCTION
 * Rotation : $ROTATION
 * Taille d'un caractère : $CHARACTER_WIDTH*$CHARACTER_HEIGHT px
 * Police de caractère : $FONT
 * Ensemble de caractères : $CHARACTERS_SET
 * Date de création : $DATE
 */

#ifndef SPRITES_HEADER
#define SPRITES_HEADER

#define MATRIX_SIZE $MATRIX_SIZE
#define CLOCK_FREQUENCY $CLOCK_FREQUENCY
#define BRIGHTNESS_LEVELS $BRIGHTNESS_LEVELS

#define ROW_LENGTH $ROW_LENGTH
#define ROWS_PER_FRAME $ROWS_PER_FRAME
#define BYTES_PER_ROW $BYTES_PER_ROW

#define NUMBERS $NUMBERS
#define LOWERCASE $LOWERCASE
#define UPPERCASE $UPPERCASE

#if NUMBERS==1
#define NUMBERS_INDEX $INDEX_NUMBERS
#endif

#if LOWERCASE==1
#define LOWERCASE_INDEX $INDEX_LOWERCASE
#endif

#if UPPERCASE==1
#define UPPERCASE_INDEX $INDEX_UPPERCASE
#endif

/*
 * Syntaxe :
 * sprites[indice_caractère][n° de frame][rangée][octet]
 */
extern unsigned char sprites[$CHARACTERS_NUMBER][$BRIGHTNESS_LEVELS-1][$ROWS_PER_FRAME][$BYTES_PER_ROW];

#endif
