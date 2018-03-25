#include "main.h"
#include "song.h"
#include <SDL2/SDL.h>
#include <fftw3.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

int is_file(const char *path){
  struct stat path_stat;
  if(stat(path, &path_stat) == -1){
    perror("stat");
  }
  return S_ISREG(path_stat.st_mode);
}

int filter(const struct dirent* dir){
  const char *path = dir->d_name;
  
  if(!is_file(path)){
    return 0;
  }
  
  char magic1[5], magic2[5];

  FILE *file = fopen(path, "r");
  fread(magic1, 1, 4, file);
  fseek(file, 4, SEEK_CUR);
  fread(magic2, 1, 4, file);
  fclose(file);
  
  magic1[4] = magic2[4] = '\0';
  if(strcmp(magic1, "RIFF") || strcmp(magic2, "WAVE")){
    return 0;
  }
  
  return 1;
}

int compar(const struct dirent** dirent1, const struct dirent** dirent2){
  (void)dirent1;
  (void)dirent2;
  return rand() > (RAND_MAX / 2);
}

int main(int argc, char *argv[]){
  //Checking arguments
  if(argc < 2){
    printf("Usage : ./sound (file.wav | directory)\n");
    exit(EXIT_FAILURE);
  }
  
  //SDL Init
  if(SDL_Init(SDL_INIT_AUDIO) != 0){
    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  
  //WiringPi
#if PWM_PIN != -1
  if(wiringPiSetup() == -1){
    return EXIT_FAILURE;
  }
  softPwmCreate(PWM_PIN, 0, 255);//SPIO_MOSI = GPIO10 = pin 19
#endif

  Player *player;
  
  char* path = argv[1];
  char **list_songs;
  int nbFiles;
  int i;
  if(!is_file(path)){
    if(chdir(path) != 0){
      perror("chdir");
    }
    srand(time(NULL));
    
    struct dirent **nameList;
    nbFiles = scandir("./", &nameList, filter, compar);

    list_songs = calloc(nbFiles, sizeof(*list_songs));
    
    for(i = 0; i < nbFiles; i++){
      char *file_name = nameList[i]->d_name;
      list_songs[i] = file_name;
    }
  }else{
    nbFiles = 1;
    list_songs = calloc(nbFiles, sizeof(*list_songs));
    *list_songs = path;
  }

  do{
    for(i = 0; i < nbFiles; i++){
      player = Init_Player(list_songs[i], BUFFER_SIZE_MS);
      if(player == NULL){
	return EXIT_FAILURE;
      }

      printf("Now playing %s\n", list_songs[i]);
      Play(player, PWM_PIN);
      Destroy_Player(&player);
    }
  }while(1);

  SDL_Quit();
  return EXIT_SUCCESS;
}
