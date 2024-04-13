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
#include<fstream>
using namespace std;


//creat a funtion to generate random number from seed time
int generateRandomNumber(int min, int max) {
    // Use the current time as the seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // Initialize a random number generator
    std::default_random_engine generator(seed);

    // Generate a random number between min and max
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}
//creat a funtion to generate n random numbers from seed time
vector<int> generateRandomNumbers(int min, int max, int n) {
	vector<int> result;
    for (int i = 0; i < n; i++) {
		result.push_back(generateRandomNumber(min, max));
	}
	return result;
}
const int positionnood[] = { 59,205,331,605,751,876 };//position of nood
const int numbernood[]={1,1,1,1,1,1,2,2,2,2,2,3,3,1,3,4,1,4,6,2};//rate of random number of nood
//funtion to random number of nood
int creatnumberofnood() {
	int numberofnood = generateRandomNumber(1,20);
	return numbernood[numberofnood];
}
//funtion to random position of nood with number of nood
vector<int> generateUniqueRandomNumbers(int min, int max, int n) {
    vector<int> numbers;
    for (int i = min; i <= max; i++) {
        numbers.push_back(i);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::shuffle(numbers.begin(), numbers.end(), generator);

    numbers.resize(n);
    return numbers;
}
vector<int> creatpositionofnood(int numbernood) {
    vector<int> result;
    if (numbernood % 2 == 0) {
        result = generateUniqueRandomNumbers(1, 3, numbernood / 2);
        for (int i = 0; i < numbernood / 2; i++) { result.push_back(6 - result[i] + 1); }
        return result;
    }
    else {
        int th = generateRandomNumber(1, 2);
        if (th == 1) {
            return generateUniqueRandomNumbers(1, 3, numbernood);
        }
        else {
            return generateUniqueRandomNumbers(4, 6, numbernood);
        }
    }
    return result;
}
//struct of nood
struct Noods {
    SDL_Texture* a;
    int x=0, y=0;
    bool check = false;
    int type = 0;
};
struct Longnoods:Noods {
    int length;
    bool isbeinghold;
    int type = 1;
};
void renderLongnood(SDL_Renderer* renderer, SDL_Texture* longnoodTexture, int customLength, int posX, int posY) {
    // Tạo SDL_Rect cho phần đầu của longnood
    SDL_Rect headRect = { 0, 0, 88, customLength/2*4.5 };
    SDL_Rect destHeadRect = { posX, posY, 44, customLength/2 };
    // Tạo SDL_Rect cho phần cuối của longnood
    SDL_Rect tailRect = { 0, 3532 - customLength/2*4.5, 88, customLength/2*4.5 };
    SDL_Rect destTailRect = { posX, posY + customLength/2, 44, customLength/2 };
    // Vẽ phần đầu, thân và cuối của longnood
    SDL_RenderCopyEx(renderer, longnoodTexture, &headRect, &destHeadRect, 0.0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, longnoodTexture, &tailRect, &destTailRect, 0.0, NULL, SDL_FLIP_NONE);
}
// hàm save game


void saveGameState(const vector<Noods*>& noods, float demSpeed, float demWave, int demNood, int speed, int demsp, int maxWave, int demkey, int demperfect, int perfect, int good, int miss, int tooearly, int waitlongnood, int demlight, time_t startTime, time_t elapsedTime) {
    ofstream file;
    file.open("gameState.txt");

    // Lưu các biến
    file << demSpeed << endl;
    file << demWave << endl;
    file << demNood << endl;
    file << speed << endl;
    file << demsp << endl;
    file << maxWave << endl;
    file << demkey << endl;
    file << demperfect << endl;
    file << perfect << endl;
    file << good << endl;
    file << miss << endl;
    file << tooearly << endl;
    file << waitlongnood << endl;
    file << demlight << endl;
    file << startTime << endl;
    file << elapsedTime << endl;
    

    // Lưu vector noods
    for ( Noods* nood : noods) {
        file << nood->x << " " << nood->y << " " << nood->check << " " << nood->type;
        if (nood->type == 1) {
             Longnoods*longnood = (Longnoods*)nood;
            file << longnood->length << " " << longnood->isbeinghold << endl;
        }
        else {
			file << endl;
		}
    }

    file.close();
}
void clearFile(const std::string& filename) {
    std::ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    file.close();
}

       

  
	


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
    SDL_Surface* noodSurface = IMG_Load("Image/nood.png");
    if (noodSurface == NULL) {
        printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface* longnoodSurface = IMG_Load("Image/longnood.png");
    if (longnoodSurface == NULL) {
		printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
		return 1;
	}
    SDL_Surface* longnood_holdSurface = IMG_Load("Image/longnood_hold.png");
    if (longnood_holdSurface == NULL) {
        printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface* bombSurface = IMG_Load("Image/bomb.png");
    if (bombSurface == NULL) {
		printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
		return 1;
	}

    SDL_Texture* noodTexture = SDL_CreateTextureFromSurface(renderer, noodSurface);
    SDL_FreeSurface(noodSurface);
    SDL_Rect noodRect = { 100,1.1, 100, 100 };
    SDL_Texture* longnoodTexture = SDL_CreateTextureFromSurface(renderer, longnoodSurface);
    SDL_FreeSurface(longnoodSurface);
    SDL_Texture* longnood_holdTexture = SDL_CreateTextureFromSurface(renderer, longnood_holdSurface);
    SDL_FreeSurface(longnood_holdSurface);
    SDL_Texture* bombTexture = SDL_CreateTextureFromSurface(renderer, bombSurface);
    SDL_FreeSurface(bombSurface);
    //create longnoodreact with full image of longnood
    SDL_Rect longnoodRect = { 205,50, 22, 200};

    SDL_Surface* openSurface = IMG_Load("Image/open.png");
    SDL_Texture* openTexture = SDL_CreateTextureFromSurface(renderer, openSurface);
    SDL_FreeSurface(openSurface);

   
    SDL_Texture* texture = openTexture;
    
    SDL_Rect destinationRect = { 0,0, 1080, 810 };
    SDL_RenderCopyEx(renderer, texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
    
    
    Noods nood;
    nood.a = noodTexture;
    nood.x = 59;
    nood.y = 0;
    nood.check = true;
    //creat vector nood for gameplay
    vector<Noods*> noods;
    
    Longnoods longnood;
    longnood.a = longnoodTexture;
    longnood.x = 205;
    longnood.y = 0;
    longnood.check = true;
    SDL_RenderPresent(renderer);

    //loop of opentexture go to game if mouse click
    while (true) {
        //if mouse click
        SDL_Event e;
        SDL_PollEvent(&e);
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            break;
        }
    }
    SDL_DestroyTexture(openTexture);
    bool quit = false;
    SDL_Event e;
    // variable to control speed and number of nood
    int demsleep = 0;
    float demSpeed = 0;
    float demWave = 0;
    int demNood = 0;
    int speed = 6;
    int demsp = 0;
    int maxWave = 125;
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }

    // load menu music
    Mix_Music* music = Mix_LoadMUS("music/menu.mp3");
    if (music == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }
    //main loop start menu 
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                gameplay = false;
            }
        }
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
        //thread to play menu music and play again if music menu end
        std::thread t1([&music]() {
            Mix_PlayMusic(music, -1);
            if (music == NULL) Mix_PlayMusic(music, -1);

            });
        t1.join();
        //menu loop
        while (!quit) {

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                    gameplay =false;
                    break;
                }
            }

            SDL_Rect destinationRect = { 0,0.0, 1080, 810 };
            // Get the current time
            Uint32 currentTime = SDL_GetTicks();

            // Calculate a value between 0 and 255 using a sin function for each color component
            Uint8 red = 128 * (sin(currentTime / 500.0) + 1)+75; // Change every 500 ms
            Uint8 green = 128 * (sin(currentTime / 750) + 1)+75; // Change every 1000 ms
            Uint8 blue = 128 * (sin(currentTime / 650) + 1)+75; // Change every 2000 ms

            // Set the color mod of the texture
            SDL_SetTextureColorMod(texture, red, green, blue);
            SDL_RenderCopyEx(renderer, texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
            SDL_RenderPresent(renderer);
   //get mouse position
            int x, y;
            SDL_GetMouseState(&x, &y);
            //action with each button of menu be clicked
            //play button
            if (x > 400 && x < 700) {
                if (y > 350 && y < 475) {
                    texture = menu1Texture;
                    //go to gameplay if mouse click in play button
                    SDL_PollEvent(&e);
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        gameplay = true;

                        break;
                    }
                }
                //how to play button
                else if (y > 490 && y < 600) {
                    texture = menu2Texture;
                    SDL_PollEvent(&e);
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
						//pause music and display how to play texture
                        Mix_PauseMusic();
						SDL_Surface* howtoplaySurface = IMG_Load("Image/how to play.png");
						SDL_Texture* howtoplayTexture = SDL_CreateTextureFromSurface(renderer, howtoplaySurface);
						SDL_FreeSurface(howtoplaySurface);
						destinationRect = { 0,0, 1080, 810 };
						SDL_RenderCopyEx(renderer, howtoplayTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
						SDL_RenderPresent(renderer);
                        SDL_Delay(500);
						//how to play loop
                        while (true) {
							SDL_PollEvent(&e);
                            //go back to menu if mouse click
                            if (e.type == SDL_MOUSEBUTTONDOWN) {
								SDL_DestroyTexture(howtoplayTexture);
								break;
                            }
						}
						SDL_DestroyTexture(howtoplayTexture);
                        //music resume
                        Mix_ResumeMusic();
					}
                    
                }
                //exit button
                else if (y > 600 && y < 700) {

                    texture = menu3Texture;
                    //program quif if mouse click

                    SDL_PollEvent(&e);
                    if (e.type == SDL_MOUSEBUTTONDOWN) {

                        return 0;

                    }

                }
				//if mouse not in button
                else {
                    texture = menuTexture;
                }
            }
			//if mouse not in button
            else {
                texture = menuTexture;
            }
        }
        // Load gameplay music
        Mix_Music* music = Mix_LoadMUS("music/AdhesiveWombat - 8 Bit Adventure.mp3");
        if (music == NULL) {
            printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
            return 1;
        }
        vector<Mix_Chunk*> tikmusiclist(3);
      tikmusiclist[0] = Mix_LoadWAV("music/tik1.mp3");
      if (tikmusiclist[0] == NULL) {
			printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
			return 1;
		}
      tikmusiclist[1] = Mix_LoadWAV("music/tik2.mp3");
      if (tikmusiclist[1] == NULL) {
          printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
          return 1;
      }
      tikmusiclist[2] = Mix_LoadWAV("music/tik3.mp3");
      if (tikmusiclist[2] == NULL) {
		  printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		  return 1;
      }
      Mix_Chunk* flashmusic = Mix_LoadWAV("music/flash.mp3");
      if (flashmusic== NULL) {
		  printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		  return 1;
	  }
     
     //chỉnh âm lượng max cho các tik music
     Mix_VolumeChunk(tikmusiclist[0], MIX_MAX_VOLUME );
     Mix_VolumeChunk(tikmusiclist[1], MIX_MAX_VOLUME );
     Mix_VolumeChunk(tikmusiclist[2], MIX_MAX_VOLUME );

        //setup for gameplay
        if (gameplay == true) {
            //delete menu texture
            
            SDL_Surface* backgroundSurface = IMG_Load("Image/gameplay.png");
            if (backgroundSurface == NULL) {
                printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
                return 1;
            }
            SDL_Surface*darkbackgroundSurface = IMG_Load("Image/gameplaydark.png");
            if (darkbackgroundSurface == NULL) {
				printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
				return 1;
			}
            SDL_Surface* redbackgroundSurface = IMG_Load("Image/gameplayred.png");
            if (redbackgroundSurface == NULL) {
				printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
				return 1;
			}
            SDL_Surface* darkredbackgroundSurface = IMG_Load("Image/gameplayreddark.png");
            if (darkredbackgroundSurface == NULL) {
                printf("SDL could not load image! SDL Error: %s\n", SDL_GetError());
                return 1;
            }
            SDL_Texture* whiteTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1080, 810);
            SDL_SetRenderTarget(renderer, whiteTexture);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderTarget(renderer, NULL);
            SDL_Texture* darkredbackgroundTexture = SDL_CreateTextureFromSurface(renderer, darkredbackgroundSurface);
            SDL_FreeSurface(darkredbackgroundSurface);
            SDL_Texture* redbackgroundTexture = SDL_CreateTextureFromSurface(renderer, redbackgroundSurface);
            SDL_FreeSurface(redbackgroundSurface);

            SDL_Texture* darkbackgroundTexture = SDL_CreateTextureFromSurface(renderer, darkbackgroundSurface);
            SDL_FreeSurface(darkbackgroundSurface);
            SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
            SDL_FreeSurface(backgroundSurface);
          
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
            
            //thread to play gameplay music
           
           
            //variable for gameplay
            time_t startTime = std::time(nullptr);
            time_t currentTime;
            time_t elapsedTime=0;
            int demkey = 0;
            int demperfect = 0;
            int choice;
            int perfect = 0;
            int good = 0;
            int miss = 0;
            int tooearly = 0;
            int waitlongnood = 0;
            speed = 6;
            int demlight = 0;
            maxWave = 125;
            int demflash = 0;
            int controlFPS = 12;
            //load game nếu file save game tồn tại và không bị trống
           
            ifstream file("gameState.txt");
            // Kiểm tra xem file có tồn tại và không rỗng không
            bool choiceloadgame = false;
            if (file.good() && file.peek() != std::ifstream::traits_type::eof()) {
                // File tồn tại và không rỗng, tải game
               
                SDL_Surface* loadgameSurface = IMG_Load("Image/continue_or_newgame.png");
                SDL_Texture* loadgameTexture = SDL_CreateTextureFromSurface(renderer, loadgameSurface);
                SDL_FreeSurface(loadgameSurface);
                SDL_SetTextureColorMod(texture,50,50,50);
                destinationRect = { 0,0, 1080, 810 };
                SDL_RenderCopyEx(renderer, texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
                while (true) {
                   SDL_RenderCopyEx(renderer,loadgameTexture,&destinationRect,&destinationRect,0.0,NULL,SDL_FLIP_NONE);
					SDL_RenderPresent(renderer);
					SDL_Event e;
					SDL_PollEvent(&e);
					int x, y;
					SDL_GetMouseState(&x, &y);
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        if (y > 275, y < 400) {
                            if (x > 150, x < 425) {
                                choiceloadgame = true;
                                break;
                            }
                            else if (x > 650, x < 975) {
                                choiceloadgame = false;
                                break;
                            }
                        }
                    }

                }
                

                // Load các biến
                if (choiceloadgame == true) {
                    file >> demSpeed;
                    file >> demWave;
                    file >> demNood;
                    file >> speed;
                    file >> demsp;
                    file >> maxWave;
                    file >> demkey;
                    file >> demperfect;
                    file >> perfect;
                    file >> good;
                    file >> miss;
                    file >> tooearly;
                    file >> waitlongnood;
                    file >> demlight;
                  
                    file >> startTime;
                    file >> elapsedTime;

                    // Set lại startTime


                    // Load vector noods
                    int x, y, type;
                    bool check;
                    while (file >> x >> y >> check >> type) {


                        if (type == 1) {
                            int length;
                            bool isbeinghold;
                            file >> length >> isbeinghold;
                            Longnoods* longnood = new Longnoods;
                            longnood->a = longnoodTexture;
                            longnood->x = x;
                            longnood->y = y;
                            longnood->check = check;
                            longnood->type = type;
                            longnood->length = length;
                            longnood->isbeinghold = isbeinghold;
                            file >> longnood->length >> longnood->isbeinghold;
                            noods.push_back(longnood);


                        }
                        else {
                            Noods* nood = new Noods;
                            nood->a = noodTexture;
                            nood->x = x;
                            nood->y = y;
                            nood->check = check;
                            nood->type = type;
                            noods.push_back(nood);

                        }
                    }

                    file.close();

                    // Xóa file sau khi đã tải xong
                    std::remove("gameState.txt");
                    if (elapsedTime >= 211) {
                        texture = redbackgroundTexture;
                    }
                    // nghỉ 1s để chờ nhạc chạy

                   
                    SDL_Delay(1000);
                }
            }
           
            SDL_DestroyTexture(menuTexture);
            SDL_DestroyTexture(menu1Texture);
            SDL_DestroyTexture(menu2Texture);
            SDL_DestroyTexture(menu3Texture);
            clearFile("gameState.txt");
            texture = backgroundTexture;
            if (choiceloadgame) {
                time_t currentTime = time(0);
                startTime = currentTime - elapsedTime;
            }

            t1 = std::thread([&music,elapsedTime,choiceloadgame]() {
                Mix_PlayMusic(music, -1);
                if (choiceloadgame) {
                    Mix_SetMusicPosition(elapsedTime);
                }
                });
            t1.join();
           
            while (gameplay) {
               
                
                // check time after each loop
                 currentTime = std::time(nullptr);
                elapsedTime = currentTime - startTime;
               
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                        gameplay = false;
                        //save game
                        saveGameState(noods, demSpeed, demWave, demNood, speed, demsp, maxWave, demkey, demperfect, perfect, good, miss, tooearly, waitlongnood, demlight, startTime, elapsedTime);
                    }
                }
                if (elapsedTime == 0) texture = backgroundTexture;
                else if (elapsedTime == 211) texture = redbackgroundTexture;
                if (demlight >= 50*(6/speed)) {
					demlight = 0;
					if (texture == backgroundTexture) texture = darkbackgroundTexture;
                    else if(texture==redbackgroundTexture) texture = darkredbackgroundTexture;
					else if (texture == darkredbackgroundTexture) texture = redbackgroundTexture;
					else texture = backgroundTexture;
				}
                demlight++;
                destinationRect = { 0,0, 1080, 810 };
                Uint8 brightness = 150 * (sin(SDL_GetTicks() / 500.0*speed/6) + 1) / 2+105; // Tạo hiệu ứng nhấp nháy
                SDL_SetTextureColorMod(texture, brightness, brightness, brightness);
                SDL_RenderCopyEx(renderer, texture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
                demSpeed += 1;
               
                if (elapsedTime == 210) {
                    //chỉnh âm lương cho voice
                    Mix_Chunk* voicemusic = Mix_LoadWAV("music/voice.mp3");
                    if (voicemusic == NULL) {
                        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
                        return 1;
                    }
                    Mix_VolumeChunk(voicemusic, MIX_MAX_VOLUME);
                   Mix_PlayChannel(-1, voicemusic, 0);
                   
                }
                //control speed, create wave nood
                if (demSpeed >= 1) {
                    demSpeed = 0;
                    for (Noods*& x : noods) {
                        if (x->check == true) {
                            if (x->y >= 810&&(x->type==0||x->type==2)) {
                                x->check = false;
                               
                                demperfect = 10;
                          
                                demsleep++;
                                if(x->type==0) {miss++,choice=0;}
                                else { perfect++; choice = 3; }
                            }
                            x->y += speed;//move nood and control speed
                            SDL_Rect noodrect = { x->x,x->y, 100, 100 };
                            //check type of nood to render
                            if (x->type == 1) {
                                //ép kiểu sang longnood để vẽ
                                Longnoods* y = (Longnoods*)x;
                                if (y->isbeinghold == true) {
                                    renderLongnood(renderer, longnood_holdTexture, y->length, y->x + 30, y->y);
								}
								else
                                renderLongnood(renderer, longnoodTexture, y->length, y->x + 30, y->y);
                            }
                            else {
                                Uint8 r = 127 * (sin(SDL_GetTicks() / 1000.0*speed/6) + 1); // Tạo hiệu ứng thay đổi màu sắc
                                Uint8 g = 127 * (sin(SDL_GetTicks() / 500.0*speed/6) + 1); // Tạo hiệu ứng thay đổi màu sắc
                                Uint8 b = 127 * (sin(SDL_GetTicks() / 2000.0*speed/6) + 1); // Tạo hiệu ứng thay đổi màu sắc
                                SDL_SetTextureColorMod(x->a, r, g, b);
                                SDL_RenderCopyEx(renderer, x->a, NULL, &noodrect, 0.0, NULL, SDL_FLIP_NONE);
                            }
                        }


                    }
                    demWave += 2;
                    // wave of nood
                    if (demWave >= maxWave && waitlongnood <= 0) {
                        demWave = 0;
                        //create new wave nood
                        vector<int> position = creatpositionofnood(creatnumberofnood());
                        if (elapsedTime <= 268 && !(elapsedTime >= 206 && elapsedTime <= 210)) {
                            int type = generateRandomNumber(1, 10);
                            if (elapsedTime < 10|| type>3) {
                                for (int i = 0; i < position.size(); i++) {
                                    Noods* nood = new Noods;
                                    int ratebombnood = generateRandomNumber(1, 10);
                                    if (ratebombnood == 1&&elapsedTime>=211) {
										nood->a = bombTexture;
										nood->type = 2;
									}
									else {
                                        nood->a = noodTexture;
                                        nood->type = 0;
                                    }
                                   
                                    nood->x = positionnood[position[i] - 1];
                                    nood->y = 0;
                                    nood->check = true;
                                    noods.push_back(nood);
                                }
                            }
                            else {
                                int length = generateRandomNumber(250, 500);
                                int ranDom = 0;
                                if (position.size() >= 4) {
                                    while (true) {
                                        ranDom = generateRandomNumber(0, size(position));
                                        if (ranDom == 0 || ranDom == size(position) / 2) break;
                                    }

                            }
                                for (int i = 0; i < position.size()-ranDom; i++) {
                                    Longnoods* longnood = new Longnoods;
                                    longnood->a = longnoodTexture;
                                    longnood->x = positionnood[position[i] - 1];
                                    longnood->y = 0 - length;
                                    longnood->check = true;
                                    longnood->length = length;
                                    longnood->isbeinghold = false;
                                    noods.push_back(longnood);
                                    noods.back()->type = 1;

                                }
                                for (int i = position.size() - ranDom; i < position.size(); i++) {
									Noods* nood = new Noods;
									nood->a = noodTexture;
									nood->x = positionnood[position[i] - 1];
									nood->y = 0-length/2;
									nood->check = true;
                                    nood->type = 0;
									noods.push_back(nood);
								}
                                waitlongnood = length / speed * 2 + maxWave;
                            }
                        }
                        // increase speecd of nood follow time
                        demsp++;
                        if (demsp >= 23) {
                            
                            if (maxWave > 60&&(demsp==23||demsp==40)) maxWave -= 5;
                            if (speed < 12&&demsp>=40) {
                                speed += 1;
                                demsp = 0;
                            }
                          
                        }
                    }
                    else if (waitlongnood > 0) waitlongnood -= 3;
                }
                if (noods.size() > 50) {
                    //giải phóng bộ nhớ và xóa 30 phần tử đầu
                    for (int i = 0; i < 30; i++) {
                        delete noods[i];
                    }
                    noods.erase(noods.begin(), noods.begin() + 30);
                }
                //demkey to reduce stick key
                    //main gameplay get point and check correct nood be pressed
                if (demkey <= 0) {
                    for (Noods*& x : noods) {
                        bool check = true;
                        if (x->type == 1) {

                            Longnoods* y = (Longnoods*)x;
                            if (y->isbeinghold == true || (y->isbeinghold == false && (y->y + y->length > 800))) check = false;

                        }
                        if (check) {
                            if (x->type == 1) {
                                Longnoods* y = (Longnoods*)x;
                                y->y += y->length;
                            }
                            if (x->check == true && x->y >= 525 && x->y <= 800) {
                                if (x->x == 59 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {


                                    if (x->type == 0||x->type==2) x->check = false;
                                    if (x->type != 2) {
                                        if (x->y >= 525 && x->y < 575) { choice = 1; demperfect = 10; tooearly++; }
                                        else if ((x->y >= 575 && x->y < 630) || (x->y >= 740 && x->y < 800)) { choice = 2; demperfect = 10; good++; }
                                        else if (x->y >= 630 && x->y < 740) { choice = 3; demperfect = 10; perfect++; }
                                    }
                                    else {
                                        miss++;
                                        choice = 0;
                                    }

                                    if (x->type == 1) {
                                        Longnoods* y = (Longnoods*)x;
                                        y->isbeinghold = true;

                                    }
                                    demsleep = 0;
                                    if (x->type != 2) {
                                        thread tikMusicThread([&tikmusiclist]() {

                                            Mix_PlayChannel(-1, tikmusiclist[generateRandomNumber(0, tikmusiclist.size() - 1)], 0);

                                            });


                                        tikMusicThread.detach();
                                    }
                                    else {
                                        demflash = 325;
                                        Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
                                        thread flashMusicThread([&flashmusic]() {
                                            //chỉnh âm lượng cho flash music
                                            Mix_VolumeChunk(flashmusic, MIX_MAX_VOLUME * 0.3);
                                            //chinh âm lượng cho music

                                            Mix_PlayChannel(-1, flashmusic, 0);
                                            });
                                        flashMusicThread.detach();
									}

                                }
                                else if (x->x == 205 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
                                    if (x->type == 0 || x->type == 2) x->check = false;
                                    if (x->type != 2) {
                                        if (x->y >= 525 && x->y < 575) { choice = 1; demperfect = 10; tooearly++; }
                                        else if ((x->y >= 575 && x->y < 630) || (x->y >= 740 && x->y < 800)) { choice = 2; demperfect = 10; good++; }
                                        else if (x->y >= 630 && x->y < 740) { choice = 3; demperfect = 10; perfect++; }
                                    }
                                    else {
                                        miss++;
                                        choice = 0;
                                    }

                                    if (x->type == 1) {
                                        Longnoods* y = (Longnoods*)x;
                                        y->isbeinghold = true;

                                    }
                                    demsleep = 0;
                                    if (x->type != 2) {
                                        thread tikMusicThread([&tikmusiclist]() {

                                            Mix_PlayChannel(-1, tikmusiclist[generateRandomNumber(0, tikmusiclist.size() - 1)], 0);

                                            });


                                        tikMusicThread.detach();
                                    }
                                    else {
                                        demflash = 325;
                                        Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
                                        thread flashMusicThread([&flashmusic]() {
                                            //chỉnh âm lượng cho flash music
                                            Mix_VolumeChunk(flashmusic, MIX_MAX_VOLUME * 0.3);
                                            //chinh âm lượng cho music

                                            Mix_PlayChannel(-1, flashmusic, 0);
                                            });
                                        flashMusicThread.detach();
                                    }
                                  
                                }
                                else if (x->x == 331 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
                                    if (x->type == 0 || x->type == 2) x->check = false;
                                    if (x->type != 2) {
                                        if (x->y >= 525 && x->y < 575) { choice = 1; demperfect = 10; tooearly++; }
                                        else if ((x->y >= 575 && x->y < 630) || (x->y >= 740 && x->y < 800)) { choice = 2; demperfect = 10; good++; }
                                        else if (x->y >= 630 && x->y < 740) { choice = 3; demperfect = 10; perfect++; }
                                    }
                                    else {
                                        miss++;
                                        choice = 0;
                                    }

                                    if (x->type == 1) {
                                        Longnoods* y = (Longnoods*)x;
                                        y->isbeinghold = true;

                                    }
                                    demsleep = 0;
                                    if (x->type != 2) {
                                        thread tikMusicThread([&tikmusiclist]() {

                                            Mix_PlayChannel(-1, tikmusiclist[generateRandomNumber(0, tikmusiclist.size() - 1)], 0);

                                            });


                                        tikMusicThread.detach();
                                    }
                                    else {
                                        demflash = 325;
                                        Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
                                        thread flashMusicThread([&flashmusic]() {
                                            //chỉnh âm lượng cho flash music
                                            Mix_VolumeChunk(flashmusic, MIX_MAX_VOLUME * 0.3);
                                            //chinh âm lượng cho music

                                            Mix_PlayChannel(-1, flashmusic, 0);
                                            });
                                        flashMusicThread.detach();
                                    }
                                }
                                else if (x->x == 605 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_J]) {
                                    if (x->type == 0 || x->type == 2) x->check = false;
                                    if (x->type != 2) {
                                        if (x->y >= 525 && x->y < 575) { choice = 1; demperfect = 10; tooearly++; }
                                        else if ((x->y >= 575 && x->y < 630) || (x->y >= 740 && x->y < 800)) { choice = 2; demperfect = 10; good++; }
                                        else if (x->y >= 630 && x->y < 740) { choice = 3; demperfect = 10; perfect++; }
                                    }
                                    else {
                                        miss++;
                                        choice = 0;
                                    }

                                    if (x->type == 1) {
                                        Longnoods* y = (Longnoods*)x;
                                        y->isbeinghold = true;

                                    }
                                    demsleep = 0;
                                    if (x->type != 2) {
                                        thread tikMusicThread([&tikmusiclist]() {

                                            Mix_PlayChannel(-1, tikmusiclist[generateRandomNumber(0, tikmusiclist.size() - 1)], 0);

                                            });


                                        tikMusicThread.detach();
                                    }
                                    else {
                                        demflash = 325;
                                        Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
                                        thread flashMusicThread([&flashmusic]() {
                                            //chỉnh âm lượng cho flash music
                                            Mix_VolumeChunk(flashmusic, MIX_MAX_VOLUME * 0.3);
                                            //chinh âm lượng cho music

                                            Mix_PlayChannel(-1, flashmusic, 0);
                                            });
                                        flashMusicThread.detach();
                                    }
                                }
                                else if (x->x == 751 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_K]) {
                                    if (x->type == 0 || x->type == 2) x->check = false;
                                    if (x->type != 2) {
                                        if (x->y >= 525 && x->y < 575) { choice = 1; demperfect = 10; tooearly++; }
                                        else if ((x->y >= 575 && x->y < 630) || (x->y >= 740 && x->y < 800)) { choice = 2; demperfect = 10; good++; }
                                        else if (x->y >= 630 && x->y < 740) { choice = 3; demperfect = 10; perfect++; }
                                    }
                                    else {
                                        miss++;
                                        choice = 0;
                                    }

                                    if (x->type == 1) {
                                        Longnoods* y = (Longnoods*)x;
                                        y->isbeinghold = true;

                                    }
                                    demsleep = 0;
                                    if (x->type != 2) {
                                        thread tikMusicThread([&tikmusiclist]() {

                                            Mix_PlayChannel(-1, tikmusiclist[generateRandomNumber(0, tikmusiclist.size() - 1)], 0);

                                            });


                                        tikMusicThread.detach();
                                    }
                                    else {
                                        demflash = 325;
                                        Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
                                        thread flashMusicThread([&flashmusic]() {
                                            //chỉnh âm lượng cho flash music
                                            Mix_VolumeChunk(flashmusic, MIX_MAX_VOLUME * 0.3);
                                            //chinh âm lượng cho music

                                            Mix_PlayChannel(-1, flashmusic, 0);
                                            });
                                        flashMusicThread.detach();
                                    }
                                }
                                else if (x->x == 876 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_L]) {
                                    if (x->type == 0 || x->type == 2) x->check = false;
                                    if (x->type != 2) {
                                        if (x->y >= 525 && x->y < 575) { choice = 1; demperfect = 10; tooearly++; }
                                        else if ((x->y >= 575 && x->y < 630) || (x->y >= 740 && x->y < 800)) { choice = 2; demperfect = 10; good++; }
                                        else if (x->y >= 630 && x->y < 740) { choice = 3; demperfect = 10; perfect++; }
                                    }
                                    else {
                                        miss++;
                                        choice = 0;
                                    }

                                    if (x->type == 1) {
                                        Longnoods* y = (Longnoods*)x;
                                        y->isbeinghold = true;

                                    }
                                    demsleep = 0;
                                    if (x->type != 2) {
                                        thread tikMusicThread([&tikmusiclist]() {

                                            Mix_PlayChannel(-1, tikmusiclist[generateRandomNumber(0, tikmusiclist.size() - 1)], 0);

                                            });


                                        tikMusicThread.detach();
                                    }
                                    else {
                                        demflash = 325;
                                        Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
                                        thread flashMusicThread([&flashmusic]() {
                                            //chỉnh âm lượng cho flash music
                                            Mix_VolumeChunk(flashmusic, MIX_MAX_VOLUME * 0.3);
                                            //chinh âm lượng cho music

                                            Mix_PlayChannel(-1, flashmusic, 0);
                                            });
                                        flashMusicThread.detach();
                                    }
                                }
                                
                                demkey=10;

                            }
                            else if (x->check == true && x->y < 575) {
                                if (x->type == 1) {
                                    Longnoods* y = (Longnoods*)x;
                                    y->y -= y->length;

                                }
                                break;

                            }
                            if (x->type == 1) {
                                Longnoods* y = (Longnoods*)x;
                                y->y -= y->length;

                            }
                        }
                        else if(!check&&x->check==true){
                            if (x->y > 800) { x->check = false; continue; }
                            
                            Longnoods* y = (Longnoods*)x;
                            if (y->isbeinghold == false && (y->y + y->length > 800)&&(y->y<750)) {
                                miss++; choice = 0; demperfect = 5;
                                demsleep++;
                            }
                            else if (y->isbeinghold == true) {
                                if (y->y <= 800) {
                                    if (y->x == 59 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
                                        
                                       
                                        perfect++;
                                        choice = 3;
                                        
                                        demsleep = 0;
                                      
                                    }
                                    else if (y->x == 205 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
                                        
                                        perfect++;
                                        choice = 3;
                                        
                                        demsleep = 0;
                                    }
                                    else if (y->x == 331 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
                                        
                                        perfect++;
                                        choice = 3;
                                        
                                        demsleep = 0;
                                    }
                                    else if (y->x == 605 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_J]) {
                                        
                                        perfect++;
                                        
                                        choice = 3;
                                        demsleep = 0;
                                    }
                                    else if (y->x == 751 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_K]) {
                                        
                                        perfect++;
                                        choice=3;
                                        
                                        demsleep = 0;
                                    }
                                    else if (y->x == 876 && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_L]) {
                                       
                                        perfect++;
                                        choice = 3;
                                        
                                        demsleep = 0;
                                    }
                                    else {
                                        if (y->y >= 680) { y->check = false; perfect++;
                                        choice = 3;

                                        demsleep = 0;
										}
                                        else { miss++; y->isbeinghold = false; choice = 0; demsleep++; } }

                                    demperfect = 5;
                                }
                                else {
                                    y->check = false;

                                }
                            }
                            demkey = 15;
                        }
                    }

                }
                else demkey--;
               
              
                    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE]) {

                        Mix_PauseMusic();
                        SDL_Surface* pauseSurface = IMG_Load("Image/pause.png");
                        SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
                        SDL_FreeSurface(pauseSurface);
                        destinationRect = { 230,200, 600, 450 };
                        SDL_RenderCopyEx(renderer, pauseTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
                        SDL_RenderPresent(renderer);
                        while (true) {
                            if (SDL_PollEvent(&e)) {
                                if (e.type == SDL_QUIT) {
									quit = true;
									gameplay = false;
									//save game
									saveGameState(noods, demSpeed, demWave, demNood, speed, demsp, maxWave, demkey, demperfect, perfect, good, miss, tooearly, waitlongnood, demlight, startTime, elapsedTime);
									break;
								}
							}
                            //if mouse click
                            SDL_PollEvent(&e);
                            if (e.type == SDL_MOUSEBUTTONDOWN) {
                                int x, y;
                                SDL_GetMouseState(&x, &y);
                                //check button be clicked
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
                                    startTime = std::time(nullptr);
                                    demkey = 0;
                                    demperfect = 0;
                                    demsleep = 0;
                                    demSpeed = 0;
                                    demWave = 0;
                                    demNood = 0;
                                    speed = 6;
                                    demsp = 0;
                                    maxWave = 125;
                                    
                                    break;

                                }
                                else if (y > 510 && y < 580 && x>610 && x < 730) {
                                    gameplay = false;
                                    noods.resize(0);
                                    break;
                                }
                            }
                        }
                        SDL_DestroyTexture(pauseTexture);
                     
                    }
                
                
              
                //code for display result of nood be pressed
                if (demperfect >= 0) {
                    demperfect--;
                    //thread to display result
                    thread t2([&renderer, perfectTexture,goodTexture,missTexture,tooearlyTexture,choice]() {
                        SDL_Rect destinationRect = { 430,550,200,250 };
                        if (choice == 0) SDL_RenderCopyEx(renderer, missTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
						else if (choice == 1) SDL_RenderCopyEx(renderer, tooearlyTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
						else if (choice == 2) SDL_RenderCopyEx(renderer, goodTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
						else if (choice == 3)
                        SDL_RenderCopyEx(renderer, perfectTexture, NULL, &destinationRect, 0.0, NULL, SDL_FLIP_NONE);
                        });

                    t2.join();
                }
                //renderer gameplay
                          //if q is pressed
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q]) {
                    demflash = 325;
                    Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
                    thread flashMusicThread([&flashmusic]() {
                        //chỉnh âm lượng cho flash music
                        Mix_VolumeChunk(flashmusic, MIX_MAX_VOLUME*0.3);
                       //chinh âm lượng cho music
                        					
						Mix_PlayChannel(-1,flashmusic, 0);
						});
                    flashMusicThread.detach();
                }
                if (demflash > 0) {
                    demflash--;
                    // Calculate the elapsed time
                    
                    
                    // Calculate the alpha value using a sin function
                    Uint8 alpha = 128 * (sin(SDL_GetTicks() / 2000.0) + 1);

                    // Set the alpha mod of the white texture
                    SDL_SetTextureAlphaMod(whiteTexture, alpha);

                    // Render the white texture over the entire screen
                    SDL_RenderCopy(renderer, whiteTexture, NULL, NULL);

                    if (demflash == 1) {
                        Mix_VolumeMusic(MIX_MAX_VOLUME);
                    }
				}
                SDL_RenderPresent(renderer);
             
                SDL_Delay(controlFPS);
               //check to end game
                if (((size(noods) == 0 || noods.back()->check == false) && elapsedTime >= 272)||demsleep>=50) {
                    Mix_HaltMusic();
                    //end music
                    Mix_Music* end = Mix_LoadMUS("music/end.mp3");
                    if (end == NULL) {
						printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
						return 1;
					}
                    Mix_PlayMusic(end, 1);
                    SDL_Delay(400);
                    gameplay = false;
                    noods.resize(0);
                    //complete game and caculate point and display
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
                   //use ttf to write completed art
                   thread t3([&renderer, point, perfect, good, miss, tooearly,demsleep]() {
                   TTF_Init();
                   TTF_Font* font = TTF_OpenFont("Front/font.ttf", 24);
                   SDL_Color color = { 255, 255, 0 };
                   SDL_Surface* surface;
                   SDL_Texture* texture;
                   SDL_Rect textRect;
                   if (demsleep >= 20) {
                       //write "Are you sleeping?"
                       
                       surface = TTF_RenderText_Solid(font, "Are you sleeping?", color);
                       texture = SDL_CreateTextureFromSurface(renderer, surface);
                       textRect = { 350,350, surface->w, 150};
                       SDL_RenderCopy(renderer, texture, NULL, &textRect);
                       
                   }
                   else {
                       if (miss + tooearly == 0 && good == 0) {
                           surface = TTF_RenderText_Solid(font, "So Amazing, You are perfect", color);
                           texture = SDL_CreateTextureFromSurface(renderer, surface);
                           textRect = { 210,350, surface->w, surface->h };
                           SDL_RenderCopy(renderer, texture, NULL, &textRect);
                       }
                       else if (miss + tooearly < 0.05 * (perfect + good + miss + tooearly) && good > 0 && good < 0.25 * perfect) {
                           surface = TTF_RenderText_Solid(font, "Great,you're almost perfect", color);
                           texture = SDL_CreateTextureFromSurface(renderer, surface);
                           textRect = { 220,350, surface->w, surface->h };
                           SDL_RenderCopy(renderer, texture, NULL, &textRect);
                       }
                       else if ((good + perfect * 2) > 0.8 * (miss + tooearly + good + perfect) * 2) {
                           surface = TTF_RenderText_Solid(font, "Good", color);
                           texture = SDL_CreateTextureFromSurface(renderer, surface);
                           textRect = { 475,350, surface->w, surface->h };
                           SDL_RenderCopy(renderer, texture, NULL, &textRect);
                       }
                       else if ((good + perfect * 2) > 0.6 * (miss + tooearly + good + perfect) * 2) {
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
                       surface = TTF_RenderText_Solid(font, ("Perfect: " + to_string(perfect)).c_str(), color);
                       texture = SDL_CreateTextureFromSurface(renderer, surface);
                       textRect = { 325,450, surface->w, surface->h };
                       SDL_RenderCopy(renderer, texture, NULL, &textRect);
                       surface = TTF_RenderText_Solid(font, ("Good: " + to_string(good)).c_str(), color);
                       texture = SDL_CreateTextureFromSurface(renderer, surface);
                       textRect = { 325,500, surface->w, surface->h };
                       SDL_RenderCopy(renderer, texture, NULL, &textRect);
                       surface = TTF_RenderText_Solid(font, ("Error: " + to_string(miss + tooearly)).c_str(), color);
                       texture = SDL_CreateTextureFromSurface(renderer, surface);
                       textRect = { 325,550, surface->w, surface->h };
                       SDL_RenderCopy(renderer, texture, NULL, &textRect);
                   }
					SDL_DestroyTexture(texture);
					SDL_FreeSurface(surface);
					TTF_CloseFont(font);
					TTF_Quit();
					});
					t3.join();
					SDL_RenderPresent(renderer);
                    //completed loop
                    while (true) {
						//check button be clicked
						SDL_PollEvent(&e);
                        if (e.type == SDL_QUIT) {
							quit = true;
							gameplay = false;
							break;
						}
                        if (e.type == SDL_MOUSEBUTTONDOWN) {
                            int x, y;
							SDL_GetMouseState(&x, &y);
							//replay button
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
                                demsleep = 0;
                                speed= 6;
                                maxWave = 125;
                                break;
							}
                            //quit button
                            else if (x > 680 && x < 860 && y > 610 && y < 780) {
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
                                demsleep = 0;
                                speed = 6;
                                maxWave = 125;

                                SDL_DestroyTexture(backgroundTexture);
                                SDL_DestroyTexture(goodTexture);
                                SDL_DestroyTexture(missTexture);
                                SDL_DestroyTexture(perfectTexture);
                                SDL_DestroyTexture(tooearlyTexture);
                                SDL_DestroyTexture(darkbackgroundTexture);
                                SDL_DestroyTexture(redbackgroundTexture);
                                SDL_DestroyTexture(darkredbackgroundTexture);
                                //giải phóng tất cả music

								break;
							}
                           
						}
					}
                    SDL_DestroyTexture(complete0Texture);
                    SDL_DestroyTexture(complete1Texture);
                    SDL_DestroyTexture(complete2Texture);
                    SDL_DestroyTexture(complete3Texture);
                    Mix_FreeMusic(end);
                    
                }
                //check time
                cout << elapsedTime<<" ";
                cout<<speed<<" "<<maxWave<<endl;

               
            }
            startTime = std::time(nullptr);
        }
        //close music
        Mix_FreeMusic(music);
      
        Mix_FreeChunk(tikmusiclist[0]);
        Mix_FreeChunk(tikmusiclist[1]);
        Mix_FreeChunk(tikmusiclist[2]);
        Mix_FreeChunk(flashmusic);
        //endgame
       
    }

   
    // endgame
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(noodTexture);
    SDL_DestroyTexture(longnoodTexture);
    SDL_DestroyTexture(longnood_holdTexture);
    SDL_DestroyTexture(bombTexture);
    SDL_Quit();
    return 0;
}
