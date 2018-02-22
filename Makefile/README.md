Makefile STM32
==============

Installation
------------
 - Exemple sur Ubuntu/Mint

        sudo apt-get install gcc-arm-none-eabi libnewlib-arm-none-eabi gdb-arm-none-eabi openocd
 
 - Exemple sur Arch/Manjaro

        sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib arm-none-eabi-gdb openocd

Configuration
-------------
 * BOARD
Le type de carte pour laquelle vous voulez compiler, par exemple f3 ou f4.

 * PROJECT_DIR
Le répertoire du projet que vous voulez compiler

 * PROJECT_NAME
Le nom de votre projet (en général le même que PROJECT_DIR).

 * DOC_DIR (optionnel)
Le répertoire dans lequel vous avez placé le fichier stm32$(BOARD)_doc.pdf, par exemple stm32f4_doc.pdf.

 * BINPATH
Le répertoire dans lequel se trouvent les programmes utiles à la compilation, en général /usr/bin sur GNU/Linux.

 * CUSTOM_C_SOURCES (optionnel)
La liste des fichiers .c à compiler que vous avez ajouté dans $(PROJECT_DIR)/Src/.
Cette ligne peut être laissée vide, ou contenir une liste de fichiers espacés par des virgules.

Configuration du projet STM32
-----------------------------
A la racine de votre projet, exécutez la commande 'make config'.
Cela va exécuter le script python 'configure.py', qui va configurer le Makefile de CubeMX en ajoutant le binpath, vos fichiers sources supplémentaires, tout en retirant les doublons.

Si cela ne fonctionne pas, suivez ces instructions :
Dans le Makefile de votre projet, modifiez la ligne BINPATH par :
BINPATH = /usr/bin
Dans C_SOURCES, ajoutez vos fichiers .c persos (à placer dans PROJECT_DIR/Src/) et supprimez les lignes en double s'il y en a.

Compilation
-----------
make c

Upload du programme sur STM32
-----------------------------
 * make f
pour flasher le programme compilé
 * make flash_file
pour flasher un local.bin contenu dans le même dossier que ce Makefile

Connexion de GDB
----------------
make debug

En cas d'erreur avec openocd
----------------------------
Si vous avez l'erreur LIBUSB_ERROR_ACCESS, lancez make f en root ou ajoutez une règle udev pour vous autoriser l'accès en lecture/écriture sur la carte STM32.
