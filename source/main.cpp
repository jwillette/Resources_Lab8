#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <sstream>

using namespace std;



// 1. new cross platform project
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



	#include "player.h"
	#include "meteor.h"

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

		tempText = "score: " + Result;

		scoreSurface = TTF_RenderText_Solid(font, tempText.c_str(), color);
		scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
		SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);
		SDL_FreeSurface(scoreSurface);
	}



	void PlayerLives(SDL_Renderer *renderer)
	{
		string Result;
		ostringstream convert;
		convert << playerLives;
		Result = convert.str();

		tempText = " lives: " + Result;

		livesSurface = TTF_RenderText_Solid(font, tempText.c_str(), color);
		livesTexture = SDL_CreateTextureFromSurface(renderer, livesSurface);
		SDL_QueryTexture(livesTexture, NULL, NULL, &livesPos.w, &livesPos.h);
		SDL_FreeSurface(livesSurface);
	}



	int main()
	{
		srand(time(NULL));

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


		// bg
		// 2. a background image 1024 x 768
		SDL_Texture *bkgd = IMG_LoadTexture(renderer, (images_dir + "bg.png").c_str());
		SDL_Rect bkgdRect;
		bkgdRect.x = 0;
		bkgdRect.y = 0;
		bkgdRect.w = 1024;
		bkgdRect.h = 768;


		// audio
		// 3. background music that loops
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		Mix_Music *bgm = Mix_LoadMUS((audio_dir + "Blazing-Stars.mp3").c_str());
		if(!Mix_PlayingMusic())
			Mix_PlayMusic(bgm, -1);

		Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "explosion.aif").c_str());


		// font
		TTF_Init();
		font = TTF_OpenFont((images_dir + "One Starry Night.TTF").c_str(), 80);

		scorePos.x = 10;
		scorePos.y = 0;

		livesPos.x = 10;
		livesPos.y = 60;

		PlayerText(renderer);
		PlayerLives(renderer);

		// player
		Player player = Player(renderer, images_dir.c_str(), audio_dir.c_str(), (SCREEN_WIDTH/2)-32, (SCREEN_HEIGHT/2)-32);

		// meteor
		Meteor meteor = Meteor(renderer, images_dir.c_str());



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

				switch(e.type)
				{
				case SDL_CONTROLLERBUTTONDOWN:

					if(e.cdevice.which == 0)
					{
						if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
						{
							// 5. player fires a bullet in the direction it's traveling
							player.OnControllerButton(e.cbutton);
							break;
						}
					} break;
				}
			}


			// 4. player character that moves on full rotation
			const Sint16 xValue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTX);
			const Sint16 yValue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTY);

			player.OnControllerAxis(xValue, yValue);


			player.Update(deltaTime);
			meteor.Update(deltaTime);



			// 9. Large rock can be shot, gives 50 points
			for (int i = 0; i < player.bulletList.size(); i++)
			{
				if (player.bulletList[i].active == true)
				{
					if (SDL_HasIntersection(&player.bulletList[i].posRect, &meteor.posRect))
					{
						Mix_PlayChannel(-1, explosionSound, 0);
						//MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);
						meteor.Reset();
						meteor.active = false;
						player.bulletList[i].Reset();
						playerScore += 50;
						PlayerText(renderer);
					}

				}
			}



			// 10. Large rock plays an explosion sound when shot.
			if (SDL_HasIntersection(&player.posRect, &meteor.posRect))
			{
				Mix_PlayChannel(-1, explosionSound, 0);
				//MakeExplosion(player1.posRect.x - 32, player1.posRect.y - 32);

				meteor.active = false;
				meteor.Reset();

				playerLives--;
				PlayerLives(renderer);
			}

			meteor.active = true;



			SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdRect);

				player.Draw(renderer);
				meteor.Draw(renderer);

				// 28. custom fonts to draw the score
				// 29. custom fonts to draw the lives
				SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);
				SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);

			SDL_RenderPresent(renderer);
		}



		SDL_DestroyWindow(window);
		SDL_Quit();

		return 0;
	}
