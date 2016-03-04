#include "meteor.h"

Meteor::Meteor(SDL_Renderer *renderer, string filePath)
{
	active = true;

	// 6. one large rock enemy that rotates
	string enemyPath = filePath + "meteor2.png";

	SDL_Surface *surface = IMG_Load(enemyPath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;


	// 7. rock travels in one of four randomly chosen directions
	direction = rand() % 4;

	Reset();


	angle = rand() % (360);

	center.x = posRect.w / 2;
	center.y = posRect.h / 2;
}


void Meteor::Reset()
{
	if(!active)
	{
		direction = rand() % 4;
	}

	speed = 3;
	speed *= 100;

	if(direction == 1)
	{
		posRect.x = -200;
		posRect.y = -200;
	}

	else if(direction == 2)
	{
		posRect.x = -200;
		posRect.y = 768;
	}

	else if(direction == 3)
	{
		posRect.x = 1024;
		posRect.y = -200;
	}

	else
	{
		posRect.x = 1024;
		posRect.y = 768;
	}



	pos_X = posRect.x;
	pos_Y = posRect.y;
}


void Meteor::Update(float deltaTime)
{
	if(active)
	{
		// down, right
		if(direction == 1)
		{
			xDir = 1;
			yDir = 1;
		}
		// up, right
		else if(direction == 2)
		{
			xDir = 1;
			yDir = -1;
		}
		// down, left
		else if(direction == 3)
		{
			xDir = -1;
			yDir = 1;
		}
		// up, left
		else
		{
			xDir = -1;
			yDir = -1;
		}


		pos_X += (speed * xDir) * deltaTime;
		pos_Y += (speed * yDir) * deltaTime;

		posRect.x = (int)(pos_X + 0.5f);
		posRect.y = (int)(pos_Y + 0.5f);

		// 8. Large rock wraps around the edges of the screen
		if(posRect.x > 1024 + 101 || posRect.x < 0 - 201)
		{
			Reset();
		}

		if(posRect.y > 768 + 101 || posRect.y < 0 - 201)
		{
			Reset();
		}

		angle += .1;
	}

	else
	{
		posRect.x = -2000;
		posRect.y = -2000;
	}
}


void Meteor::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, angle, &center, SDL_FLIP_NONE);
}


Meteor::~Meteor()
{

}
