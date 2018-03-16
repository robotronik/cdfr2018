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

/*
 * Syntaxe :
 * sprites[indice_caractère][n° de frame][rangée][octet]
 */
unsigned char sprites[$CHARACTERS_NUMBER][$BRIGHTNESS_LEVELS-1][$ROWS_PER_FRAME][$BYTES_PER_ROW] = {$DATA};

