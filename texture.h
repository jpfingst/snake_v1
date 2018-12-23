#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

class Texture
{
public:
  Texture();
  ~Texture();

  bool loadFromFile(std::string path);
  bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
  void free();

  void render(int x, int y, SDL_Rect* clip=NULL, double angle=0.0, SDL_Point* centre=NULL);
  int getWidth();
  int getHeight();
  
private:
  SDL_Texture* mTexture;
  int mWidth;
  int mHeight;
};

#endif
