#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <sstream>

using namespace std;



#if defined(_WIN32) || (_WIN64)

	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_mixer.h"
	#include "SDL_ttf.h"

	#include <direct.h>
	#define getcwd _getcwd

	string cwd(getcwd(NULL, 0));

	string images_dir = cwd + "\\Resources_Lab8\\images\\";
	string audio_dir = cwd + "\\Resources_Lab8\\audio\\";

#endif


#if defined(__APPLE__)

	#include "SDL2/SDL.h"
	#include "SDL2_image/SDL_image.h"
	#include "SDL2_mixer/SDL_mixer.h"
	#include "SDL2_ttf/SDL_ttf.h"

	string cwd(getcwd(NULL, 0));
	string images_dir = cwd + "/Resources_Lab8/images/";
	string audio_dir = cwd + "/Resources_Lab8/audio/";

#endif


#if defined(__linux__)

	#include "SDL2/SDL.h"
	#include "SDL2/SDL_image.h"
	#include "SDL2/SDL_mixer.h"
	#include "SDL2/SDL_ttf.h"

	#include <unistd.h>

	string cwd(getcwd(NULL, 0));
	string images_dir = cwd + "/Resources_Lab8/images/";
	string audio_dir = cwd + "/Resources_Lab8/audio/";

#endif



	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 768;

	float deltaTime = 0.0;
	int thisTime = 0;
	int lastTime = 0;

	TTF_Font *font;
	SDL_Color color = {255, 255, 255, 255};
	SDL_Surface *scoreSurface, *livesSurface;
	SDL_Texture *scoreTexture, *livesTexture;
	SDL_Rect scorePos, livesPos;

	int playerScore = 0;
	int playerLives = 5;
	string tempText = "";



	void PlayerText(SDL_Renderer *renderer)
	{
		string Result;
		ostringstream convert;
		convert << playerScore;
		Result = convert.str();

		tempText = "SCORE: " + Result;

		scoreSurface = TTF_RenderText_Solid(font, tempText.c_str(), color);
		scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
		SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);
		SDL_FreeSurface(scoreSurface);
	}





	int main()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_Window *window = nullptr;

		SDL_Renderer *renderer = nullptr;

		window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



		// main loop flag
		bool quit = false;

		// event handler
		SDL_Event e;



		// game controller
		SDL_GameControllerEventState(SDL_ENABLE);
		SDL_GameController* gGameController0 = NULL;
		gGameController0 = SDL_GameControllerOpen(0);


		// audio
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		Mix_Music *bgm = Mix_LoadMUS((audio_dir + "Blazing-Stars.mp3").c_str());
		if(!Mix_PlayingMusic())
			Mix_PlayMusic(bgm, -1);


		// bg
		SDL_Texture *bkgd = IMG_LoadTexture(renderer, (images_dir + "bg.png").c_str());
		SDL_Rect bkgdRect;
		bkgdRect.x = 0;
		bkgdRect.y = 0;
		bkgdRect.w = 1024;
		bkgdRect.h = 768;
		float X_pos = 0.0f, Y_pos = 0.0f;


		// font
		TTF_Init();
		font = TTF_OpenFont((images_dir + "One Starry Night.TTF").c_str(), 40);

		scorePos.x = 10;
		scorePos.y = 10;

		livesPos.x = 10;
		livesPos.y = 40;

		PlayerText(renderer);
		//TurretText(renderer, 0);



		// main game loop
		while(!quit)
		{
			thisTime = SDL_GetTicks();
			deltaTime = (float)(thisTime - lastTime)/1000;
			lastTime = thisTime;

			while(SDL_PollEvent(&e) != 0)
			{
				if(e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

			SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);
			//			SDL_RenderCopy(renderer, turretTexture, NULL, &turretPos);

			SDL_RenderPresent(renderer);
		}



		SDL_DestroyWindow(window);
		SDL_Quit();

		return 0;
	}
