// Simple Snake game with tile level, sound and snake's length growth
//
// 7. Oct. 2018, Juergen Pfingstner

// Using SDL and standard IO
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"
#include "tile.h"
#include "snake.h"
#include "mouse.h"
#include "feeder.h"
#include "score.h"

////////////////////////////// Global variables /////////////////////////////

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

// THE window
SDL_Window* gWindow = NULL;

// Renderer
SDL_Renderer* gRenderer = NULL;

// Music data
Mix_Music* gBGMusic = NULL;

// Textures
//Texture snakeHead_texture;
Texture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILES_SPRITE];

// Globally used font
TTF_Font *gFont = NULL;

//////////////////////////////////// Prototypes //////////////////////////////

bool init_SDL();
bool close_SDL();

bool init_Level(int level_index, Tile *tiles[], double &x0_snake, double &y0_snake);
bool close_Level(Tile *tiles[]);

bool check_Collision(SDL_Rect a, SDL_Rect b);
bool touches_Wall(SDL_Rect box, Tile *tiles[]);
bool setTiles(Tile *tiles[], std::string level_file_name);

//////////////////////////////////// SDL ///////////////////////////////////

bool init_SDL()
{
  bool success = true;

  // Initialise SDL
  if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
      printf("Error: SDL could not be initialised: %s\n", SDL_GetError());
      success = false;
    }
  else
    {
      // Create window
      gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
      if(gWindow == NULL)
	{
	  printf("Error: Window could not be created: %s\n", SDL_GetError());
	  success = false;
	}
      else
	{
	  // Create renderer for window
	  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	  if(gRenderer == NULL)
	    {
	      printf("Error: Renderer could not be created: %s\n", SDL_GetError());
	      success = false;
	    }
	  else
	    {
	      // Initialize renderer color
	      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	      // Initialize SLD_Mixer
	      if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
		  printf("Error: SDL_Mixer could not be created: %s\n", Mix_GetError());
		  success = false;
		}

	      // Initialise SDL_ttf
	      if(TTF_Init() == -1)
		{
		  printf("Error: SDL_ttf could not be initialised: %s!\n", TTF_GetError());
		  success = false;
		}
	      
	    }
	}
    }
  
  return success;
}

bool close_SDL()
{
  // Free global font
  TTF_CloseFont(gFont);
  gFont = NULL;
  
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer = NULL;

  TTF_Quit();
  SDL_Quit();
  
  return true;
}

//////////////////////////////////// Level /////////////////////////////////

bool init_Level(int level_index, Tile *tiles[], double &x0_snake, double &y0_snake)
{
  bool success = true;
  std::string level_file_name = "";

  //Load tile texture
  if( !gTileTexture.loadFromFile( "tiles.png" ) )
    {
      printf( "Failed to load tile set texture!\n" );
      success = false;
    }

  // Load the font
  gFont = TTF_OpenFont("lazy.ttf", 36);
  if(gFont == NULL)
    {
      printf("Error: Failed to load lazy font: %s!\n", TTF_GetError());
      success = false;
    }
  
  // Load Level data
  switch(level_index)
    {
    case 1:
      gBGMusic = Mix_LoadMUS("Lounge_Beat.wav");
      level_file_name = "level1.map";
      x0_snake = SCREEN_WIDTH/2;
      y0_snake = SCREEN_HEIGHT/2;
      break;
    case 2:
      gBGMusic = Mix_LoadMUS("Lounge-synth-theme-loop.wav");
      level_file_name = "level2.map";
      x0_snake = SCREEN_WIDTH/2;
      y0_snake = SCREEN_HEIGHT/4;
      break;
    default:
      printf("Error: Level %i does not exist!\n", level_index);
      success = false;
      break;
    }

  //Load tile map
  if( !setTiles( tiles, level_file_name ))
    {
      printf( "Failed to load tile set!\n" );
      success = false;
    }  

  return success;
}

bool close_Level(Tile *tiles[])
{
  // Deallocate tiles
  for(int i=0; i < TOTAL_TILES; i++)
    {
      if(tiles[i] != NULL)
	{
	  delete tiles[i];
	  tiles[i] = NULL;
	}
    }

  gTileTexture.free();
  
  Mix_FreeMusic(gBGMusic);
  gBGMusic = NULL;

  Mix_Quit();

  return true;
}

////////////////////////////////Collision and Tiles //////////////////////////

bool check_Collision(SDL_Rect a, SDL_Rect b)
{
  //The sides of the rectangles
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  //Calculate the sides of rect A
  leftA = a.x;
  rightA = a.x + a.w;
  topA = a.y;
  bottomA = a.y + a.h;

  //Calculate the sides of rect B
  leftB = b.x;
  rightB = b.x + b.w;
  topB = b.y;
  bottomB = b.y + b.h;
    
  //If any of the sides from A are outside of B
  if( bottomA <= topB )
    {
      return false;
    }
  
  if( topA >= bottomB )
    {
      return false;
    }

  if( rightA <= leftB )
    {
      return false;
    }

  if( leftA >= rightB )
    {
      return false;
    }

  //If none of the sides from A are outside B
  return true;
}

bool touches_Wall(SDL_Rect box, Tile *tiles[])
{
  //Go through the tiles
  for( int i = 0; i < TOTAL_TILES; ++i )
    {
      //If the tile is a wall type tile
      if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) )
        {
	  //If the collision box touches the wall tile
	  if( check_Collision( box, tiles[ i ]->getBox() ) )
            {
	      return true;
            }
        }
    }

  //If no wall tiles were touched
  return false;
}

bool setTiles(Tile *tiles[], std::string level_file_name)
{
  bool tilesLoaded = true;
  int x = 0, y = 0;

  std::ifstream map(level_file_name);
  if(map == NULL)
    {
      printf("Unable to load map file!\n");
    }
  else
    {
      for(int i=0; i < TOTAL_TILES; ++i)
	{
	  int tileType = -1;
	  map >> tileType;
	  if(map.fail())
	    {
	      printf("Error loading the map: Unexpected EOF!\n");
	      tilesLoaded = false;
	      break;
	    }

	  // If the number is a valid tile number
	  if((tileType >= 0) && (tileType < TOTAL_TILES_SPRITE))
	    {
	      tiles[i] = new Tile(x, y, tileType);
	    }
	  else
	    {
	      printf("Error loading map: Individual tile type at %d!\n", i);
	      tilesLoaded = false;
	      break;
	    }

	  // Move to next tile spot
	  x += TILE_WIDTH;
	  if(x >= SCREEN_WIDTH)
	    {
	      x = 0;
	      y += TILE_HEIGHT;
	    }
	}
      
      // Clip the sprite sheet
      if(tilesLoaded)
	{
	  gTileClips[ TILE_RED ].x = 0;
	  gTileClips[ TILE_RED ].y = 0;
	  gTileClips[ TILE_RED ].w = TILE_WIDTH;
	  gTileClips[ TILE_RED ].h = TILE_HEIGHT;
	  
	  gTileClips[ TILE_GREEN ].x = 0;
	  gTileClips[ TILE_GREEN ].y = 80;
	  gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
	  gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

	  gTileClips[ TILE_BLUE ].x = 0;
	  gTileClips[ TILE_BLUE ].y = 160;
	  gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
	  gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

	  gTileClips[ TILE_TOPLEFT ].x = 80;
	  gTileClips[ TILE_TOPLEFT ].y = 0;
	  gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
	  gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

	  gTileClips[ TILE_LEFT ].x = 80;
	  gTileClips[ TILE_LEFT ].y = 80;
	  gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
	  gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;

	  gTileClips[ TILE_BOTTOMLEFT ].x = 80;
	  gTileClips[ TILE_BOTTOMLEFT ].y = 160;
	  gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
	  gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

	  gTileClips[ TILE_TOP ].x = 160;
	  gTileClips[ TILE_TOP ].y = 0;
	  gTileClips[ TILE_TOP ].w = TILE_WIDTH;
	  gTileClips[ TILE_TOP ].h = TILE_HEIGHT;
			
	  gTileClips[ TILE_CENTER ].x = 160;
	  gTileClips[ TILE_CENTER ].y = 80;
	  gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
	  gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;

	  gTileClips[ TILE_BOTTOM ].x = 160;
	  gTileClips[ TILE_BOTTOM ].y = 160;
	  gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
	  gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

	  gTileClips[ TILE_TOPRIGHT ].x = 240;
	  gTileClips[ TILE_TOPRIGHT ].y = 0;
	  gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
	  gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

	  gTileClips[ TILE_RIGHT ].x = 240;
	  gTileClips[ TILE_RIGHT ].y = 80;
	  gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
	  gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

	  gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
	  gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
	  gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
	  gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
	}
    }
  map.close();
  return tilesLoaded;
}

//////////////////////////////////// Main ///////////////////////////////////

int main( int argc, char* args[] )
{

  /////////////////////
  // Initialize game //
  /////////////////////

  Tile* tileSet[ TOTAL_TILES ];
  double x0_snake = 0, y0_snake = 0;
  int level_index = 2;
  double speed_snake = 2.0;
  double prolongment = 100;
  init_SDL();
  init_Level(level_index, tileSet, x0_snake, y0_snake);

  // Event handler
  SDL_Event e;

  // Our main actors
  Snake the_snake(x0_snake, y0_snake, speed_snake);
  Mouse* the_mouse = NULL;
  //Mouse* the_mouse = new(Mouse)(170, 170);
  Feeder the_feeder;
  Score the_score; 
  
  // Play the selected background music
  Mix_VolumeMusic(MIX_MAX_VOLUME/10);
  Mix_PlayMusic(gBGMusic, -1);

  ///////////////
  // Main loop //
  ///////////////

  // main loop flag
  bool run = true;
  bool game_over = false;
  
  while(run == true)
    { 
      
      ///////////////////////
      // Handle the events //
      ///////////////////////

      while(SDL_PollEvent(&e) != 0)
	{
	  // User request quit
	  if(e.type == SDL_QUIT)
	    {
	      run = false;
	    }

	  the_snake.handle_Event(e);
	}
      
      ////////////////
      // Game logic //
      ////////////////

      the_snake.advance();

      game_over = touches_Wall(the_snake.get_snake_head_box(), tileSet);
      if (game_over == true)
	{
	  run = false;
	}

      game_over = the_snake.check_Self_Collision();
      if (game_over == true)
     	{
      	  run = false;
      	}
      
      if(the_mouse == NULL)
	{
	  // Create a new mouse
	  the_mouse = the_feeder.dropMouse(tileSet, &the_snake);
	}
      else
	{
	  // Check for collision
	  if(check_Collision(the_mouse->get_box(), the_snake.get_snake_head_box()))
	    {
	      // Eat the mouse
	      the_snake.eatMouse(the_mouse);
	      the_mouse = NULL;
	      the_score.increment();
	      the_snake.prolong(prolongment);
	    }
	  else
	    {
	      // Go on
	    }
	}
      
      /////////////////////////
      // Render snakes world //
      /////////////////////////

      // Clear screen
      SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
      SDL_RenderClear(gRenderer);

      // Render the screen
      for(int i=0; i < TOTAL_TILES; i++)
	{
	  tileSet[i]->render();
	}

      the_score.render();
      
      // Render snake and the mouse
      if(the_mouse != NULL)
	{
	  the_mouse->render();
	}
      the_snake.render();
      
      // Update screen
      SDL_RenderPresent(gRenderer);

    }

  ////////////////
  // Close game //
  ////////////////

  if(the_mouse != NULL)
    {
      delete(the_mouse);
    }
  close_Level(tileSet);
  close_SDL();
  
  return 0;
}
