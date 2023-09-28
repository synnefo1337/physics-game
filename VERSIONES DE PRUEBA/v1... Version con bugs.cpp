/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

double abs(double num){
    if(num<0){
        return -num;
    }else{
        return num;
    }
}

class plr {
    /****** values given in constructor ******/
    double movAcc;//acceleration
    double movFriction;

public:
    struct _pos{
        double x;
        double y;
    }pos;
    struct _vel{
        double x;
        double y;
    }vel;
    struct _acc{
        double x;
        double y;
    }acc;
    plr(double x, double y);
    void ChangePos(double x, double y){
        pos.x=x;
        pos.y=y;
    }
    double GetX(void){
        return pos.x;
    }

    double GetY(void){
        return pos.y;
    }
    void MoveUp(void){
        acc.y = movAcc;
    }
    void MoveLeft(void){
       acc.x = -movAcc;
    }
    void MoveRight(void){
        acc.x = movAcc;
    }
    void MoveDown(void){
        acc.y = -movAcc;
    }

    void MoveNowhereYaxis(void){
        acc.y = 0.0f;
    }

    void MoveNowhereXaxis(void){
        acc.x = 0.0f;
    }

    void Update(void){
        /*unless there is some force of other kind than gravity in vertical direction, Y acceleration
        will always be equal to gravitational acceleration*/
        if(acc.x){
            vel.x = vel.x + acc.x;
        }else if(vel.x > 0){//if no acc, then apply friction.
            vel.x = vel.x - movFriction;
        }else if(vel.x < 0){
            vel.x = vel.x + movFriction;
        }

        if(acc.y){
            vel.y = vel.y + acc.y;
        }else if(vel.y > 0){//if no acc, then apply friction.
            vel.y = vel.y - movFriction;
        }else if(vel.y < 0){
            vel.y = vel.y + movFriction;
        }

        if(abs(vel.x)<0.0001f){//if floating point number is near 0, count it as 0.
            vel.x = 0.0f;
        }
        if(abs(vel.y)<0.0001f){//if floating point number is near 0, count it as 0.
            vel.y = 0.0f;
        }
            pos.x = pos.x + vel.x;
            pos.y = pos.y + vel.y;

        if(pos.x >= SCREEN_WIDTH){
            pos.x = SCREEN_WIDTH;
            vel.x = -10;
        }else if(pos.x <= 0){
            pos.x = 0;
            vel.x = 10;
        }

        if(pos.y >= SCREEN_HEIGHT){
            pos.y = SCREEN_HEIGHT;
            vel.y = -10;
        }else if(pos.y <= 0){
            pos.y = 0;
            vel.y = 10;
        }


    }
};

plr::plr(double x, double y){
    pos.x=x;
    pos.y=y;
    vel.x=0.0f;
    vel.y=0.0f;
    acc.x=0.0f;
    acc.y=0.0f;
    movAcc = 0.002f;
    movFriction = 0.001f;//deacceleration once the ball isnt accelerating so it doesnt maintain its moment of inertia.
}

class plrtrail{
    int length = 500;
    double *x = new double[length];
    double *y = new double[length];
    int stackTop = -1;
    bool stackLoop = false;
public:
    ~plrtrail();
    void push(double _x, double _y){
        if(stackLoop == false){
            stackTop++;
            x[stackTop]=_x;
            y[stackTop]=_y;
            if(stackTop == length-1){
                stackLoop = true;
            }
        }else if(stackTop == length-1){
            stackTop = 0;
            x[stackTop]=_x;
            y[stackTop]=_y;
        }else{
            stackTop++;
            x[stackTop]=_x;
            y[stackTop]=_y;
        }
    }
    void render(void){
        int i;
        int color = 0;
        if(stackTop == length-1){
            for(i = 0;i<length;i++){
                color++;
                SDL_Rect fillRect = { x[i]-5, SCREEN_HEIGHT-y[i]-5, 10, 10 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 255-(Uint8)(255*(double)color/length), (Uint8)(255*(double)color/length), 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect );
            }
        }else if(stackLoop == false){
            for(i = 0;i<stackTop+1;i++){
                color++;
                SDL_Rect fillRect = { x[i]-5, SCREEN_HEIGHT-y[i]-5, 10, 10 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 255-(Uint8)(255*(double)color/length), 255-(Uint8)(255*(double)color/length), 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect );
            }
        }else{
            for(i = stackTop+1;i<length;i++){
                color++;
                SDL_Rect fillRect = { x[i]-5, SCREEN_HEIGHT-y[i]-5, 10, 10 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 255-(Uint8)(255*(double)color/length), 255-(Uint8)(255*(double)color/length), 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect );
            }
            for(i = 0;i<stackTop+1;i++){
                color++;
                SDL_Rect fillRect = { x[i]-5, SCREEN_HEIGHT-y[i]-5, 10, 10 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 255-(Uint8)(255*(double)color/length), 255-(Uint8)(255*(double)color/length), 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect );
            }
        }
    }
};

plrtrail::~plrtrail(){
    delete[] x; delete[] y;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			plr Player1((double)(SCREEN_WIDTH/2),(double)(SCREEN_HEIGHT/2));
			plrtrail trail;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}else if(e.type==SDL_KEYDOWN){
					    switch(e.key.keysym.sym){
                        case SDLK_UP:
                            Player1.MoveUp();
                            break;
                        case SDLK_DOWN:
                            Player1.MoveDown();
                            break;
                        case SDLK_RIGHT:
                            Player1.MoveRight();
                            break;
                        case SDLK_LEFT:
                            Player1.MoveLeft();
                            break;
					    }
					}else if(e.type==SDL_KEYUP){
					    switch(e.key.keysym.sym){
                        case SDLK_UP:
                            Player1.MoveNowhereYaxis();
                            break;
                        case SDLK_DOWN:
                            Player1.MoveNowhereYaxis();
                            break;
                        case SDLK_RIGHT:
                            Player1.MoveNowhereXaxis();
                            break;
                        case SDLK_LEFT:
                            Player1.MoveNowhereXaxis();
                            break;
					    }
					}
				}
				Player1.Update();

                printf("pos.x:%lf, pos.y:%lf, vel.x:%lf, vel.y:%lf\n",Player1.pos.x,Player1.pos.y,Player1.vel.x,Player1.vel.y);

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				trail.render();
				trail.push(Player1.pos.x,Player1.pos.y);

				//Render red filled quad
				SDL_Rect fillRect = { Player1.pos.x-5, SCREEN_HEIGHT-Player1.pos.y-5, 10, 10 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect );

                /*
				//Render green outlined quad
				SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
				SDL_RenderDrawRect( gRenderer, &outlineRect );

				//Draw blue horizontal line
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
				SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

				//Draw vertical line of yellow dots
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
				for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
				{
					SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
				}*/

				//Update screen
				SDL_RenderPresent( gRenderer );
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
