#include "player.h"

const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	active = true;

	speed = 200.0f;

	fire = Mix_LoadWAV((audioPath + "laser.wav").c_str());

	playerPath = filePath + "player.png";

	texture = IMG_LoadTexture(renderer, playerPath.c_str());

	posRect.x = x;
	posRect.y = y;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = 0;

	xDirOld = 1;
	yDirOld = 0;

	center.x = posRect.w/2;
	center.y = posRect.h/2;

	string bulletPath;

	bulletPath = filePath + "bullet.png";

	for(int i = 0; i < 10; i++)
	{
		Bullet tmpBullet(renderer, bulletPath, -1000, -1000, 0, 0);
		bulletList.push_back(tmpBullet);
	}
}



void Player::Update(float deltaTime)
{
	if(xValue != 0 || yValue != 0)
	{
		angle = atan2(yValue, xValue) * 180 / 3.14f;

		oldAngle = angle;

		// gives radians instead of degrees
		float radians = (angle * 3.14f) / 180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		pos_X += (move_x) * deltaTime;
		pos_Y += (move_y) * deltaTime;

		posRect.x = (int)(pos_X + 0.5f);
		posRect.y = (int)(pos_Y + 0.5f);



	} else {
		angle = oldAngle;
	}



	if(posRect.x < 0) {
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x > 1024 - posRect.w) {
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if(posRect.y < 0) {
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if(posRect.y > 768 - posRect.h) {
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}


	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active)
		{
			bulletList[i].Update(deltaTime);
		}
	}
}



void Player::Draw(SDL_Renderer *renderer)
{
	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active)
		{
			bulletList[i].Draw(renderer);
		}
	}

	SDL_RenderCopyEx(renderer, texture, nullptr, &posRect, angle, &center, SDL_FLIP_NONE);
}



void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if(event.which == 0)
	{
		if(event.button == 0)
		{
			CreateBullet();
		}
	}
}



void Player::OnControllerAxis(Sint16 X, Sint16 Y)
{
	xValue = X;
	yValue = Y;

	if(!(xValue < -JOYSTICK_DEAD_ZONE) && !(xValue > JOYSTICK_DEAD_ZONE))
	{
		xValue = 0.0f;
	}

	if(!(yValue < -JOYSTICK_DEAD_ZONE) && !(yValue > JOYSTICK_DEAD_ZONE))
	{
		yValue = 0.0f;
	}
}


void Player::CreateBullet()
{
	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active == false)
		{
			Mix_PlayChannel(-1, fire, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = (posRect.x + (posRect.w/2));
			bulletList[i].posRect.y = (posRect.y + (posRect.h/2));

			bulletList[i].posRect.x = bulletList[i].posRect.x - (bulletList[i].posRect.w/2);
			bulletList[i].posRect.y = bulletList[i].posRect.y - (bulletList[i].posRect.h/2);

			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			if(xValue != 0 || yValue != 0)
			{
				bulletList[i].tankAngle = angle;
			} else {
				bulletList[i].tankAngle = oldAngle;
			}

			break;
		}
	}
}
