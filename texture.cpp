#include "texture.h"

Texture::Texture()
{
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

Texture::~Texture()
{
  free();
}

bool Texture::loadFromFile(std::string path)
{
  // Get rid of preexisting texture
  free();

  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if(loadedSurface == NULL)
    {
      printf("Error: SDL_image unable to load image %s: %s\n", path.c_str(), IMG_GetError());
    }
  else
    {
      SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
      newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
      if(newTexture == NULL)
	{
	  printf("Error: SDL unable to create texture from %s: %s\n", path.c_str(), SDL_GetError());
	}
      else
	{
	  mWidth = loadedSurface->w;
	  mHeight = loadedSurface->h;
	}
      SDL_FreeSurface(loadedSurface);
    }

  mTexture = newTexture;
  return mTexture != NULL;
}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
  // Get rid of preexisting texture
  free();

  SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
  if(textSurface == NULL)
    {
      printf("Error: SDL_ttf unable to render text surface: %s\n", SDL_GetError());
    }
  else
    {
      // Create texture from surface pixel
      mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
      if(mTexture == NULL)
	{
	  printf("Error: SDL unable to create texture from rendered text: %s\n", SDL_GetError());
	}
      else
	{
	  mWidth = textSurface->w;
	  mHeight = textSurface->h;
	}
      SDL_FreeSurface(textSurface);
    }

  return mTexture != NULL;
}

void Texture::free()
{
  if (mTexture != NULL)
    {
      SDL_DestroyTexture(mTexture);
      mTexture = NULL;
      mWidth = 0;
      mHeight = 0;
    }
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* centre)
{
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};

  if(clip != NULL)
    {
      renderQuad.w = clip->w;
      renderQuad.h = clip->h;
    }

  SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, centre, SDL_FLIP_NONE);
}

int Texture::getWidth()
{
  return mWidth;
}

int Texture::getHeight()
{
  return mHeight;
}
