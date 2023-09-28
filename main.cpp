//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

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
		gWindow = SDL_CreateWindow( "made by synnefo1337", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
	SDL_Quit();
}

/*double abs(double num){
    if(num<0){
        return -num;
    }else{
        return num;
    }
}*/

class plr {
    /****** values given in constructor ******/
    double movAcc;//acceleration
    double movFriction;
    double bounceStopping;//ratio of deacceleration for bounce impact.
public:
    int plrsize = 50;
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

    void drawface(int _c);

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
        double angle,xFriction,yFriction;
        if(acc.x){
            vel.x = vel.x + acc.x;
        }else if(acc.y){
            if(vel.x > 0){//if no acc in x, but acc in y then apply friction in x.
                vel.x = vel.x - movFriction;
            }else if(vel.x < 0){
                vel.x = vel.x + movFriction;
            }
        }

        if(acc.y){
            vel.y = vel.y + acc.y;
        }else if(acc.x){
            if(vel.y > 0){//if no acc in y but acc in x, then apply friction in y.
                vel.y = vel.y - movFriction;
            }else if(vel.y < 0){
                vel.y = vel.y + movFriction;
            }
        }

        if(!acc.x && !acc.y){//if no acc in both axes, then apply friction to both axes in a way that maintains direction of movement.
                //do something else if vel.x == 0... because cant divide by 0.
            if(vel.x != 0 && vel.y != 0){
                angle = atan(vel.y/vel.x);//angle of movement.
                xFriction = abs(movFriction*cos(angle));
                yFriction = abs(movFriction*sin(angle));
                if(vel.x < 0){
                    vel.x = vel.x + xFriction;
                }else if(vel.x > 0){
                    vel.x = vel.x - xFriction;
                }

                if(vel.y < 0){
                    vel.y = vel.y + yFriction;
                }else if(vel.y > 0){
                    vel.y = vel.y - yFriction;
                }
            }else{
                if (vel.x == 0 && vel.y != 0){
                    if(vel.y < 0){
                        vel.y = vel.y + movFriction;
                    }else if(vel.y > 0){
                        vel.y = vel.y - movFriction;
                    }
                }else if(vel.y == 0 && vel.x != 0){
                    if(vel.x < 0){
                        vel.x = vel.x + movFriction;
                    }else if(vel.x > 0){
                        vel.x = vel.x - movFriction;
                    }
                }
                if(abs(vel.x) < 0.002f){
                    vel.x = 0.0f;
                }
                if(abs(vel.y) < 0.002f){
                    vel.y = 0.0f;
                }
            }
        }

        if(abs(vel.x)<0.002f){//if floating point number is near 0, count it as 0.
            vel.x = 0.0f;
        }
        if(abs(vel.y)<0.002f){//if floating point number is near 0, count it as 0.
            vel.y = 0.0f;
        }
            pos.x = pos.x + vel.x;
            pos.y = pos.y + vel.y;

        if(pos.x >= SCREEN_WIDTH-plrsize/2){
            pos.x = SCREEN_WIDTH-plrsize/2;
            vel.x = -vel.x*bounceStopping;
        }else if(pos.x <= plrsize/2){
            pos.x = plrsize/2;
            vel.x = -vel.x*bounceStopping;
        }

        if(pos.y >= SCREEN_HEIGHT-plrsize/2){
            pos.y = SCREEN_HEIGHT-plrsize/2;
            vel.y = -vel.y*bounceStopping;
        }else if(pos.y <= plrsize/2){
            pos.y = plrsize/2;
            vel.y = -vel.y*bounceStopping;
        }

    }
};

void setcolor(int _c){
    if(_c/255==0){
        SDL_SetRenderDrawColor( gRenderer, 255-_c, _c, 0, 0xFF );
    }else if(_c/255==1){
        _c = _c%255;
       SDL_SetRenderDrawColor( gRenderer, 0, 255-_c, _c, 0xFF );
    }else if(_c/255==2){
        _c = _c%255;
       SDL_SetRenderDrawColor( gRenderer, _c, 0, 255-_c, 0xFF );
    }
}

void plr::drawface(int _c){

                SDL_Rect fillRect = { pos.x-plrsize*2/6-plrsize/12, SCREEN_HEIGHT-pos.y-plrsize*2/7-plrsize/12, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );

				fillRect = { pos.x+plrsize*2/6-plrsize/12, SCREEN_HEIGHT-pos.y-plrsize*2/7-plrsize/12, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );

				fillRect = { pos.x-plrsize/12, SCREEN_HEIGHT-pos.y-plrsize/12, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );

				fillRect = { pos.x-plrsize/12-plrsize*2/6, SCREEN_HEIGHT-pos.y-plrsize/12+plrsize*1/6, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );

				fillRect = { pos.x-plrsize/12+plrsize*2/6, SCREEN_HEIGHT-pos.y-plrsize/12+plrsize*1/6, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );

				fillRect = { pos.x-plrsize/12-plrsize*3/12, SCREEN_HEIGHT-pos.y-plrsize/12+plrsize*3/12, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );

				fillRect = { pos.x-plrsize/12+plrsize*3/12, SCREEN_HEIGHT-pos.y-plrsize/12+plrsize*3/12, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );

				fillRect = { pos.x-plrsize/12-plrsize*1/6, SCREEN_HEIGHT-pos.y-plrsize/12+plrsize*4/12, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );

				fillRect = { pos.x-plrsize/12+plrsize*1/6, SCREEN_HEIGHT-pos.y-plrsize/12+plrsize*4/12, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );

				fillRect = { pos.x-plrsize/12, SCREEN_HEIGHT-pos.y-plrsize/12+plrsize*4/12, plrsize/6, plrsize/6 };
				setcolor(_c);
				SDL_RenderFillRect( gRenderer, &fillRect );
}

plr::plr(double x, double y){
    pos.x=x;
    pos.y=y;
    vel.x=0.0f;
    vel.y=0.0f;
    acc.x=0.0f;
    acc.y=0.0f;
    movAcc = 0.01f;
    movFriction = 0.001f;//deacceleration once the ball isnt accelerating so it doesnt maintain its moment of inertia.
    bounceStopping = 0.5;
}

class plrtrail{
    int length = 1000;
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
    void render(int plrsize){
        int i;
        int realindex = 0;
        Uint8 trailSegmentSize;
        int thirdlength = length/3;
        if(stackTop == length-1){
            for(i = 0;i<length;i++){
                realindex++;
                trailSegmentSize = (Uint8)(plrsize*(double)realindex/length);
                SDL_Rect fillRect = { x[i]-trailSegmentSize/2, SCREEN_HEIGHT-y[i]-trailSegmentSize/2, trailSegmentSize, trailSegmentSize };

                int tone = 255-(Uint8)(255*(double)realindex/thirdlength);
                int tone2 = 255-(Uint8)(255*((double)realindex-thirdlength)/thirdlength);
                int tone3 = 255-(Uint8)(255*((double)realindex-thirdlength*2)/thirdlength);
                int tone4 = 255-(Uint8)(255*((double)realindex-thirdlength*3)/thirdlength);

				if (realindex < thirdlength){
                    SDL_SetRenderDrawColor( gRenderer, tone, 255-tone, 0, 255 );
                }else if(realindex < thirdlength*2){
                    SDL_SetRenderDrawColor( gRenderer, 0, tone2, 255-tone2, 255 );
                }else if(realindex < thirdlength*3){
                    SDL_SetRenderDrawColor( gRenderer, 255-tone3, 0, tone3, 255 );
                }else{
                   SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 255 );
                }
				SDL_RenderFillRect( gRenderer, &fillRect );
            }
        }else if(stackLoop == false){
            for(i = 0;i<stackTop+1;i++){
                realindex++;
                trailSegmentSize = (Uint8)(plrsize*(double)realindex/length);
                SDL_Rect fillRect = { x[i]-trailSegmentSize/2, SCREEN_HEIGHT-y[i]-trailSegmentSize/2, trailSegmentSize, trailSegmentSize };

                int tone = 255-(Uint8)(255*(double)realindex/thirdlength);
                int tone2 = 255-(Uint8)(255*((double)realindex-thirdlength)/thirdlength);
                int tone3 = 255-(Uint8)(255*((double)realindex-thirdlength*2)/thirdlength);
                int tone4 = 255-(Uint8)(255*((double)realindex-thirdlength*3)/thirdlength);

				if (realindex < thirdlength){
                    SDL_SetRenderDrawColor( gRenderer, tone, 255-tone, 0, 255 );
                }else if(realindex < thirdlength*2){
                    SDL_SetRenderDrawColor( gRenderer, 0, tone2, 255-tone2, 255 );
                }else if(realindex < thirdlength*3){
                    SDL_SetRenderDrawColor( gRenderer, 255-tone3, 0, tone3, 255 );
                }else{
                   SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 255 );
                }
				SDL_RenderFillRect( gRenderer, &fillRect );
            }
        }else{
            for(i = stackTop+1;i<length;i++){
                realindex++;
                trailSegmentSize = (Uint8)(plrsize*(double)realindex/length);
                SDL_Rect fillRect = { x[i]-trailSegmentSize/2, SCREEN_HEIGHT-y[i]-trailSegmentSize/2, trailSegmentSize, trailSegmentSize };

                int tone = 255-(Uint8)(255*(double)realindex/thirdlength);
                int tone2 = 255-(Uint8)(255*((double)realindex-thirdlength)/thirdlength);
                int tone3 = 255-(Uint8)(255*((double)realindex-thirdlength*2)/thirdlength);
                int tone4 = 255-(Uint8)(255*((double)realindex-thirdlength*3)/thirdlength);

				if (realindex < thirdlength){
                    SDL_SetRenderDrawColor( gRenderer, tone, 255-tone, 0, 255 );
                }else if(realindex < thirdlength*2){
                    SDL_SetRenderDrawColor( gRenderer, 0, tone2, 255-tone2, 255 );
                }else if(realindex < thirdlength*3){
                    SDL_SetRenderDrawColor( gRenderer, 255-tone3, 0, tone3, 255 );
                }else{
                   SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 255 );
                }
				SDL_RenderFillRect( gRenderer, &fillRect );
            }
            for(i = 0;i<stackTop+1;i++){
                realindex++;
                trailSegmentSize = (Uint8)(plrsize*(double)realindex/length);
                SDL_Rect fillRect = { x[i]-trailSegmentSize/2, SCREEN_HEIGHT-y[i]-trailSegmentSize/2, trailSegmentSize, trailSegmentSize };

                int tone = 255-(Uint8)(255*(double)realindex/thirdlength);
                int tone2 = 255-(Uint8)(255*((double)realindex-thirdlength)/thirdlength);
                int tone3 = 255-(Uint8)(255*((double)realindex-thirdlength*2)/thirdlength);
                int tone4 = 255-(Uint8)(255*((double)realindex-thirdlength*3)/thirdlength);

				if (realindex < thirdlength){
                    SDL_SetRenderDrawColor( gRenderer, tone, 255-tone, 0, 255 );
                }else if(realindex < thirdlength*2){
                    SDL_SetRenderDrawColor( gRenderer, 0, tone2, 255-tone2, 255 );
                }else if(realindex < thirdlength*3){
                    SDL_SetRenderDrawColor( gRenderer, 255-tone3, 0, tone3, 255 );
                }else{
                   SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 255 );
                }
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
    int c=0;
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

				trail.render(Player1.plrsize);
				//if(Player1.vel.x || Player1.vel.y){
                    trail.push(Player1.pos.x,Player1.pos.y);
				//}

				//Render red filled quad
                c++;
                if(c==255*3+1)c=0;
				SDL_Rect fillRect = { Player1.pos.x-Player1.plrsize/2, SCREEN_HEIGHT-Player1.pos.y-Player1.plrsize/2, Player1.plrsize, Player1.plrsize };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect );


                Player1.drawface(c);



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
