#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <iostream>
#include <ctime>
#include<vector>
#include<algorithm>
#include<random>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<thread>

using namespace std;


//creat a funtion to generate random number from seed time
int generateRandomNumber(int min, int max) {
	static bool first = true;
    if (first) {
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}
//creat a funtion to generate n random numbers from seed time
vector<int> generateRandomNumbers(int min, int max, int n) {
	vector<int> result;
    for (int i = 0; i < n; i++) {
		result.push_back(generateRandomNumber(min, max));
	}
	return result;
}

const int positionnood[] = { 59,205,331,605,751,876 };
const int numbernood[]={1,1,1,1,1,1,2,2,2,2,2,3,3,1,3,4,1,4,6,2};
int creatnumberofnood() {
	int numberofnood = generateRandomNumber(1,20);
	return numbernood[numberofnood];
}
vector<int> creatpositionofnood(int numbernood) {
    vector<int> result;
    if (numbernood % 2 == 0) {
        
        result= generateRandomNumbers(1, 3, numbernood/2);
        for (int i = 0; i < numbernood / 2; i++) { result.push_back(6 - result[i] + 1); }
        return result;
    }
    else {
        int th=generateRandomNumber(1,2);
        if (th == 1) {
           
            return generateRandomNumbers(1, 3, numbernood);
        }
        else {
            return generateRandomNumbers(4, 6, numbernood);
		}
    }
    return result;
}
struct Noods {
    SDL_Texture* a;
    int x=0, y=0;
    bool check = false;
};
  

int main(int argc, char* args[]) {
    bool gameplay = false;

    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL 2.0 Basic Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 810, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface* mouse = IMG_Load("Image/nood.png");
    if (mouse == NULL) {
        printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture* mouseTexture = SDL_CreateTextureFromSurface(renderer, mouse);
    SDL_FreeSurface(mouse);
    SDL_Rect mouseRect = { 100,1.1, 100, 100 };
    

    SDL_Surface* openSurface = IMG_Load("Image/open.png");
    SDL_Texture* openTexture = SDL_CreateTextureFromSurface(renderer, openSurface);
    SDL_FreeSurface(openSurface);

   
    SDL_Texture* texture = openTexture;
    
    SDL_Rect destinationRect = { 0,0, 1080, 810 };
    SDL_RenderCopyEx(renderer, texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
    
    
    Noods nood;
    nood.a = mouseTexture;
    nood.x = 59;
    nood.y = 0;
    nood.check = true;
    vector<Noods> noods;
    noods.push_back(nood);

    // Update renderer
    SDL_RenderPresent(renderer);


    while (true) {
        //if mouse click
        SDL_Event e;
        SDL_PollEvent(&e);
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            break;
        }
    }
    //delete opentexture
    SDL_DestroyTexture(openTexture);
 
    // Texture mặc định
  

    // Tạo cửa sổ
    // Main loop
    bool quit = false;
    SDL_Event e;
    float demSpeed = 0;
    float demWave = 0;
    int demNood = 0;
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }

    // Load file nhạc MP3
    Mix_Music* music = Mix_LoadMUS("music/menu.mp3");
    if (music == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }
    // creat a SDL_Thread to play music
    

    while (!quit) {
        SDL_Surface* menuSurface = IMG_Load("Image/Menu.png");
        SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
        SDL_FreeSurface(menuSurface);
        SDL_Surface* menu1Surface = IMG_Load("Image/Menu1.png");
        SDL_Texture* menu1Texture = SDL_CreateTextureFromSurface(renderer, menu1Surface);
        SDL_FreeSurface(menu1Surface);

        SDL_Surface* menu2Surface = IMG_Load("Image/Menu2.png");
        SDL_Texture* menu2Texture = SDL_CreateTextureFromSurface(renderer, menu2Surface);
        SDL_FreeSurface(menu2Surface);

        SDL_Surface* menu3Surface = IMG_Load("Image/Menu3.png");
        SDL_Texture* menu3Texture = SDL_CreateTextureFromSurface(renderer, menu3Surface);
        SDL_FreeSurface(menu3Surface);
        texture = menuTexture;
        int speed = 14;
        int demsp = 0;
        // Xử lý sự kiện
        std::thread t1([&music]() {
            Mix_PlayMusic(music, -1);
            // if music end reapeat
            if (music == NULL) Mix_PlayMusic(music, -1);

            });
        t1.join();
        while (true) {

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            SDL_Rect destinationRect = { 0,0.0, 1080, 810 };
            SDL_RenderCopyEx(renderer, texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);

            // Update renderer
            SDL_RenderPresent(renderer);
            //if mouse in position
            int x, y;
            SDL_GetMouseState(&x, &y);
            //if mouse don't
            if (x > 400 && x < 700) {
                if (y > 350 && y < 475) {
                    texture = menu1Texture;
                    //exit program if mouse click   
                    SDL_PollEvent(&e);
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        gameplay = true;

                        break;
                    }
                }
                else if (y > 490 && y < 600) {
                    texture = menu2Texture;
                    //if mouse click
                    
                    SDL_PollEvent(&e);
                    
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
						//pause music and display how to play
                        Mix_PauseMusic();
						SDL_Surface* howtoplaySurface = IMG_Load("Image/how to play.png");
						SDL_Texture* howtoplayTexture = SDL_CreateTextureFromSurface(renderer, howtoplaySurface);
						SDL_FreeSurface(howtoplaySurface);
						destinationRect = { 0,0, 1080, 810 };
						SDL_RenderCopyEx(renderer, howtoplayTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
						SDL_RenderPresent(renderer);
                        SDL_Delay(500);
						//if mouse click
                        while (true) {
							SDL_PollEvent(&e);
                            //if mouse click
                            if (e.type == SDL_MOUSEBUTTONDOWN) {
								//delete howtoplay texture
								SDL_DestroyTexture(howtoplayTexture);
								break;
                            }
						}
						//delete howtoplay texture
						SDL_DestroyTexture(howtoplayTexture);
                        //music resume
                        
					}
                    
                }
                else if (y > 600 && y < 700) {

                    texture = menu3Texture;
                    //program quif if mouse click

                    SDL_PollEvent(&e);
                    if (e.type == SDL_MOUSEBUTTONDOWN) {

                        return 0;

                    }

                }
                else {
                    texture = menuTexture;
                }
            }
            else {
                texture = menuTexture;
            }
           // SDL_Delay(5);
        }
        // Load file nhạc MP3
        Mix_Music* music = Mix_LoadMUS("music/AdhesiveWombat - 8 Bit Adventure.mp3");
        if (music == NULL) {
            printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
            return 1;
        }

        // Phát nhạc
       
        




        if (gameplay == true) {
            //delete menu texture
            SDL_DestroyTexture(menuTexture);
            SDL_DestroyTexture(menu1Texture);
            SDL_DestroyTexture(menu2Texture);
            SDL_DestroyTexture(menu3Texture);
            //gameplay
            SDL_Surface* backgroundSurface = IMG_Load("Image/gameplay.png");
            if (backgroundSurface == NULL) {
                printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
                return 1;
            }
            SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
            SDL_FreeSurface(backgroundSurface);
            texture = backgroundTexture;
            SDL_Surface* goodSurface = IMG_Load("Image/good.png");
            SDL_Texture* goodTexture = SDL_CreateTextureFromSurface(renderer, goodSurface);
            SDL_FreeSurface(goodSurface);
            SDL_Surface* missSurface = IMG_Load("Image/miss.png");
            SDL_Texture* missTexture = SDL_CreateTextureFromSurface(renderer, missSurface);
            SDL_FreeSurface(missSurface);
            SDL_Surface* perfectSurface = IMG_Load("Image/perfect.png");
            SDL_Texture* perfectTexture = SDL_CreateTextureFromSurface(renderer, perfectSurface);
            SDL_FreeSurface(perfectSurface);    
            SDL_Surface* tooearlySurface = IMG_Load("Image/too early.png");
            SDL_Texture* tooearlyTexture = SDL_CreateTextureFromSurface(renderer, tooearlySurface);
            SDL_FreeSurface(tooearlySurface);
            
            
            t1 = std::thread([&music]() {
                Mix_PlayMusic(music, -1);
                });
            t1.join();
            //stopwatch with ctime
            time_t startTime = std::time(nullptr);
            int demkey = 0;
            int demperfect = 0;
            int choice;
            int perfect = 0;
            int good = 0;
            int miss = 0;
            int tooearly = 0;
            speed = 14;
            while (gameplay) {
                time_t currentTime = std::time(nullptr);
                time_t elapsedTime = currentTime - startTime;
                //display background
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }
                destinationRect = { 0,0, 1080, 810 };
                SDL_RenderCopyEx(renderer, texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
                demSpeed += 1;
              //  if (size(noods) > 100) noods.erase(noods.begin(), noods.begin() + 80);
                if (demSpeed >= 1) {
                    demSpeed = 0;
                    for (Noods& x : noods) {

                        if (x.check == true) {
                            if (x.y >= 810) {
                                x.check = false;
                                choice = 0;
                                demperfect = 20;
                                miss++;
                                // gameplay = false;
                               //  noods.resize(0);
                            }
                            x.y += 5;

                            SDL_Rect mouseRect = { x.x,x.y, 100, 100 };
                            SDL_RenderCopyEx(renderer, x.a, NULL, &mouseRect, 0.0, NULL, SDL_FLIP_NONE);
                        }
                      
                      
                    }
                    demWave +=2;
                    if (demWave >= 125) {
                        demWave = 0;
                        vector<int> position = creatpositionofnood(creatnumberofnood());
                        
                        if (elapsedTime <= 268 && !(elapsedTime >= 206 && elapsedTime <= 210)) {
                            for (int y : position) {

                                noods.push_back(nood);
                                noods.back().x = positionnood[y - 1];

                            }
                        }
                        demsp++;
                        if (demsp >= 15) {
                            if (speed > 7) {
                                speed -= 1;
                            }
                            demsp = 0;
                        }
                    }
                }
                if (noods.size() > 50) noods.erase(noods.begin(), noods.begin() + 30);
                if (demkey == 0) {

                    for (Noods& x : noods) {
                        if (x.check == true && x.y >= 525 && x.y <= 800) {
                            if (x.x == 59 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
                                x.check = false;

                               
                                if (x.y >= 525 && x.y < 575) { choice = 1; demperfect = 20; tooearly++; }
                                else if ((x.y >= 575 && x.y < 630) || (x.y >= 740 && x.y < 800)) { choice = 2; demperfect = 20; good++; }
                                else if (x.y >= 630 && x.y < 740) { choice = 3; demperfect = 20; perfect++; }
                                demkey = 10;
                            }
                            else if (x.x == 205 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
                                x.check = false;
                                
                                if (x.y >= 525 && x.y < 575) { choice = 1; demperfect = 20; tooearly++; }
                                else if ((x.y >= 575 && x.y < 630) || (x.y >= 740 && x.y < 800)) { choice = 2; demperfect = 20; good++; }
                                else if (x.y >= 630 && x.y < 740) { choice = 3; demperfect = 20; perfect++; }
                               
                                demkey = 10;
                            }
                            else if (x.x == 331 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
                                x.check = false;
                               
                                if (x.y >= 525 && x.y < 575) { choice = 1; demperfect = 20; tooearly++; }
                                else if ((x.y >= 575 && x.y < 630) || (x.y >= 740 && x.y < 800)) { choice = 2; demperfect = 20;good++; }
                                else if (x.y >= 630 && x.y < 740) { choice = 3; demperfect = 20; perfect++; }
                                
                                    demkey = 10;
                            }
                            else if (x.x == 605 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_J]) {
                                x.check = false;
                                
                                if (x.y >= 525 && x.y < 575) { choice = 1; demperfect = 20; tooearly++; }
                                else if ((x.y >= 575 && x.y < 630) || (x.y >= 740 && x.y < 800)) { choice = 2; demperfect = 20; good++; }
                                else if (x.y >= 630 && x.y < 740) { choice = 3; demperfect = 20; perfect++; }
								
									
                                
                                demkey = 10;
                            }
                            else if (x.x == 751 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_K]) {
                                x.check = false;

                                if (x.y >= 525 && x.y < 575) { choice = 1; demperfect = 20; tooearly++; }
                                else if ((x.y >= 575 && x.y < 630) || (x.y >= 740 && x.y < 800)) { choice = 2; demperfect = 20; good++; }
                                else if (x.y >= 630 && x.y < 740) { choice = 3; demperfect = 20; perfect++; }
								
								
                             
                                demkey = 10;
                            }
                            else if (x.x == 876 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_L]) {
                                x.check = false;
                               
                                if (x.y >= 525 && x.y < 575) { choice = 1; demperfect = 20; tooearly++; }
                                else if ((x.y >= 575 && x.y < 630) || (x.y >= 740 && x.y < 800)) { choice = 2; demperfect = 20; good++; }
                                else if (x.y >= 630 && x.y < 740) { choice = 3; demperfect = 20; perfect++; }
                               
                                demkey = 10;
                            }
                            else continue;
                        }
                        else if (x.check == true && x.y < 575) { break; }
                    }
                }
                else demkey--;
                //pause game if esc press
                if (demkey == 0) {
                    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) {
                        Mix_PauseMusic();
                        SDL_Surface* pauseSurface = IMG_Load("Image/pause.png");
                        SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
                        SDL_FreeSurface(pauseSurface);
                        destinationRect = {230,200, 600, 450};
                        SDL_RenderCopyEx(renderer, pauseTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
                        SDL_RenderPresent(renderer);
                        //music pause
                        while (true) {
                            //if mouse click
                            SDL_PollEvent(&e);
                            if (e.type == SDL_MOUSEBUTTONDOWN) {
                                int x, y;
                                SDL_GetMouseState(&x, &y);
                                // Xác định xem người dùng đã nhấn vào nút nào
                                if (x > 450 && x < 600 && y > 350 && y < 500) {
                                    // Resume button clicked
                                    Mix_ResumeMusic();
                                    break;
                                }
                                else if (x > 320 && x < 440 && y > 510 && y < 580) {
                                    // Quit button clicked
                                    noods.resize(0);
                                    std::thread t1([&music]() {
                                        Mix_PlayMusic(music, -1);
                                        if (music == NULL) Mix_PlayMusic(music, -1);
                                        });
                                    t1.join();
                                    perfect = 0;
                                    good = 0;
                                    miss = 0;
                                    tooearly = 0;
                                    //time reset
                                    startTime = std::time(nullptr);

                                    break;
                                }
                                else if (y > 510 && y < 580&&x>610&&x<730) {
									gameplay = false;
                                    noods.resize(0);
                                    break;
								}
							}
                        }
                        
                        //delete pause texture
                        SDL_DestroyTexture(pauseTexture);
                        demkey = 10;
                    }

                }
                else demkey--;
                if (demperfect > 0) {
                    demperfect--;
                    thread t2([&renderer, perfectTexture,goodTexture,missTexture,tooearlyTexture,choice]() {
                        //display perfect
                        SDL_Rect destinationRect = { 430,550,200,250 };
                        if (choice == 0) SDL_RenderCopyEx(renderer, missTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
						else if (choice == 1) SDL_RenderCopyEx(renderer, tooearlyTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
						else if (choice == 2) SDL_RenderCopyEx(renderer, goodTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
						else if (choice == 3)
                        SDL_RenderCopyEx(renderer, perfectTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
                        });

                    t2.join();
                }

                SDL_RenderPresent(renderer);
               


                SDL_Delay(speed);
                //elapsedTime>=1
                //(size(noods)==0||noods.back().check == false) && elapsedTime >= 272
                if ((size(noods) == 0 || noods.back().check == false) && elapsedTime >= 271) {
                    Mix_HaltMusic();
                    //play end music 
                  //creat music end
                    Mix_Music* end = Mix_LoadMUS("music/end.mp3");
                    if (end == NULL) {
						printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
						return 1;
					}
                    //play music end
                    //increase volume
                    
                    Mix_PlayMusic(end, 1);
                    //while music end
                    SDL_Delay(400);
                    gameplay = false;
                    noods.resize(0);
                    //delete texture
                    //music stop
                    
                    SDL_Surface* complete0Surface = IMG_Load("Image/complete0.png");
                    SDL_Texture* complete0Texture = SDL_CreateTextureFromSurface(renderer, complete0Surface);
                    SDL_FreeSurface(complete0Surface);
                    SDL_Surface* complete1Surface = IMG_Load("Image/complete1.png");
                    SDL_Texture* complete1Texture = SDL_CreateTextureFromSurface(renderer, complete1Surface);
                    SDL_FreeSurface(complete1Surface);
                    SDL_Surface* complete2Surface = IMG_Load("Image/complete2.png");
                    SDL_Texture* complete2Texture = SDL_CreateTextureFromSurface(renderer, complete2Surface);
                    SDL_FreeSurface(complete2Surface);
                    SDL_Surface* complete3Surface = IMG_Load("Image/complete3.png");
                    SDL_Texture* complete3Texture = SDL_CreateTextureFromSurface(renderer, complete3Surface);
                    SDL_FreeSurface(complete3Surface);
                   int point=perfect*5+good*3;
                   float completionRate = (perfect*1.0 * 2 + good) / ((perfect + good + miss + tooearly) * 2);
                   destinationRect = {-10,0,1200,810 };
                   if (completionRate>0.8) {
						destinationRect = { -80,0,1200,810 };
						SDL_RenderCopyEx(renderer, complete3Texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
                       
					}
                   else if (completionRate>0.6) {
						
						SDL_RenderCopyEx(renderer, complete2Texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
					}
                   else if (completionRate>0.4) {
						
						SDL_RenderCopyEx(renderer, complete1Texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
					}
                   else {
						
						SDL_RenderCopyEx(renderer, complete0Texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
					}
                   //use tff to write string "Your point is: " + point in line 1
                   thread t3([&renderer, point, perfect, good, miss, tooearly]() {
                   TTF_Init();
                   TTF_Font* font = TTF_OpenFont("Front/font.ttf", 24);
                   SDL_Color color = { 255, 255, 0 };
                   SDL_Surface* surface;
                   SDL_Texture* texture;
                   SDL_Rect textRect;
                   if (miss + tooearly == 0 && good == 0) {
                       surface = TTF_RenderText_Solid(font, "So Amazing, You are perfect", color);
                       texture = SDL_CreateTextureFromSurface(renderer, surface);
                       textRect = { 210,350, surface->w, surface->h };
                       SDL_RenderCopy(renderer, texture, NULL, &textRect);
                   }
                   else if (miss + tooearly<0.05*(perfect+good+miss+tooearly) && good > 0 && good<0.2 * perfect) {
                       surface = TTF_RenderText_Solid(font, "Great,you're almost perfect", color);
                       texture = SDL_CreateTextureFromSurface(renderer, surface);
                       textRect = { 220,350, surface->w, surface->h };
                       SDL_RenderCopy(renderer, texture, NULL, &textRect);
                   }
                   else if ((good + perfect*2) > 0.8* (miss + tooearly + good + perfect)*2) {
                       surface = TTF_RenderText_Solid(font, "Good", color);
                       texture = SDL_CreateTextureFromSurface(renderer, surface);
                       textRect = { 450,350, surface->w, surface->h };
                       SDL_RenderCopy(renderer, texture, NULL, &textRect);
                   }
                   else if ( (good + perfect*2)> 0.6* (miss + tooearly + good + perfect)*2) {
                       surface = TTF_RenderText_Solid(font, "Not bad but you can better", color);
                       texture = SDL_CreateTextureFromSurface(renderer, surface);
                       textRect = { 240,350, surface->w, surface->h };
                       SDL_RenderCopy(renderer, texture, NULL, &textRect);
                   }
                   else {
                       surface = TTF_RenderText_Solid(font, "You need to practice more", color);
                       texture = SDL_CreateTextureFromSurface(renderer, surface);
                       textRect = { 230,350, surface->w, surface->h };
                       SDL_RenderCopy(renderer, texture, NULL, &textRect);
                   }
                    surface = TTF_RenderText_Solid(font, ("Your point is: " + to_string(point)).c_str(), color);
                    texture = SDL_CreateTextureFromSurface(renderer, surface);
                    textRect = { 325,400, surface->w, surface->h };
                   SDL_RenderCopy(renderer, texture, NULL, &textRect);
                   //wirite string Perfect: + perfect in line 2
                   surface = TTF_RenderText_Solid(font, ("Perfect: " + to_string(perfect)).c_str(), color);
                   texture = SDL_CreateTextureFromSurface(renderer, surface);
                   textRect = { 325,450, surface->w, surface->h };
                   SDL_RenderCopy(renderer, texture, NULL, &textRect);
                   //wirite string Good: + good in line 3
                   surface = TTF_RenderText_Solid(font, ("Good: " + to_string(good)).c_str(), color);
                   texture = SDL_CreateTextureFromSurface(renderer, surface);
                   textRect = { 325,500, surface->w, surface->h };
                   SDL_RenderCopy(renderer, texture, NULL, &textRect);  
                   //wirite string error:+(miss+tooearly) in line 4
                   surface = TTF_RenderText_Solid(font, ("Error: " + to_string(miss+tooearly)).c_str(), color);
                   texture = SDL_CreateTextureFromSurface(renderer, surface);
                    textRect = { 325,550, surface->w, surface->h };
                    SDL_RenderCopy(renderer, texture, NULL, &textRect);
                    //in line 5 if error=0 and good=0 wirite string "So Amazing, You are perfect" else if error=0 and good>0 wirite string "Great" else if error>0 and (good+perrfect)>70%total wirite string "Good" else if error>0 and (good+perrfect)>50%total wirite string "Not bad but you can better" else wirite string "You need to practice more"
                   
                    //delete texture
					SDL_DestroyTexture(texture);
					//delete surface
					SDL_FreeSurface(surface);
					//close font
					TTF_CloseFont(font);
					//quit ttf
					TTF_Quit();
					});
					t3.join();
					// Update renderer
					SDL_RenderPresent(renderer);
                    

                   //delete texture
                    
                   


                    //if mouse click break
                    while (true) {
						//if mouse click
						SDL_PollEvent(&e);
                        if (e.type == SDL_MOUSEBUTTONDOWN) {
							//if position of mouse
                            int x, y;
							SDL_GetMouseState(&x, &y);
							// Xác định xem người dùng đã nhấn vào nút nào
                            if (x > 230&& x <400 && y > 620 && y < 800) {
                                gameplay = true;
                                noods.resize(0);
                                std::thread t1([&music]() {
                                    Mix_PlayMusic(music, -1);
                                    if (music == NULL) Mix_PlayMusic(music, -1);
                                    });
                                t1.join();
                                //time reset
                                startTime = std::time(nullptr);
                                perfect = 0;
                                good = 0;
                                miss = 0;
                                tooearly = 0;
                                break;
							}
                            else if (x > 680 && x < 860 && y > 610 && y < 780) {
								// Quit button clicked
								noods.resize(0);
                                std::thread t1([&music]() {
									Mix_PlayMusic(music, -1);
									if (music == NULL) Mix_PlayMusic(music, -1);
									});
								t1.join();
								perfect = 0;
								good = 0;
								miss = 0;
								tooearly = 0;
                                //delete texture
                                SDL_DestroyTexture(backgroundTexture);
                                SDL_DestroyTexture(goodTexture);
                                SDL_DestroyTexture(missTexture);
                                SDL_DestroyTexture(perfectTexture);
                                SDL_DestroyTexture(tooearlyTexture);
                                //delete end
								break;
							}
                           
						}
					}
                    //delete texture
                    SDL_DestroyTexture(complete0Texture);
                    SDL_DestroyTexture(complete1Texture);
                    SDL_DestroyTexture(complete2Texture);
                    SDL_DestroyTexture(complete3Texture);
                    //delete end
                    Mix_FreeMusic(end);
                    
                }
                cout << elapsedTime << " " << noods.size() << endl;
               
            }
            //ìf music end reapeat
            //reset time
            startTime = std::time(nullptr);
         

        }
        //close music
        Mix_FreeMusic(music);
       
    }

   
    // Giải phóng bộ nhớ và đóng SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
