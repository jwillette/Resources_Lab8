#include "meteor.h"

Meteor::Meteor(SDL_Renderer *renderer, string filePath)
{
	string enemyPath = filePath + "meteor2.png";

	SDL_Surface *surface = IMG_Load(enemyPath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	Reset();

	direction = rand() % 3;
	if(direction == 1)
	{
		xDir = 0;
		yDir = 1;
	}
	else if(direction == 2)
	{
		xDir = 1;
		yDir = 0;
	}
	else if(direction == 3)
	{
		xDir = 0;
		yDir = -1;
	}
	else
	{
		xDir = -1;
		yDir = 0;
	}

	angle = rand() % (360);

	center.x = posRect.w / 2;
	center.y = posRect.h / 2;
}


void Meteor::Reset()
{
	//speed = rand() % (5) + 1;
	speed = 3;
	speed *= 100;

//	posRect.x = rand() % (1024-posRect.w) + 1;
	posRect.x = 100;
//	posRect.y = 0 - posRect.h;
	posRect.y = 100;

	pos_X = posRect.x;
	pos_Y = posRect.y;
}


void Meteor::Update(float deltaTime)
{

	if(direction == 1)
	{
		xDir = 0;
		yDir = 1;
	}
	else if(direction == 2)
	{
		xDir = 1;
		yDir = 0;
	}
	else if(direction == 3)
	{
		xDir = 0;
		yDir = -1;
	}
	else
	{
		xDir = -1;
		yDir = 0;
	}


	pos_X += (speed * xDir) * deltaTime;
	pos_Y += (speed * yDir) * deltaTime;

	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

	if(posRect.x > 1024 || posRect.x < 0)
	{
		Reset();
	}

	if(posRect.y > 768 || posRect.y < 0)
	{
		Reset();
	}

	angle += .1;
}


void Meteor::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, angle, &center, SDL_FLIP_NONE);
}


Meteor::~Meteor()
{

}
