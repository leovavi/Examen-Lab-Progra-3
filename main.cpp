#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

bool resetorder = true;
stringstream sst;
int mouseX, mouseY, clicks = 1, state = 1;
int numeros[3][3];
SDL_Window * window;
SDL_Renderer * renderer;
SDL_Event event;
SDL_Texture *num1, *num2, *num3, *num4, *num5, *num6, *num7, *num8, *num0, *menu, *play, *again, *reset, *GO, *backg;
SDL_Rect rn1, rn2, rn3, rn4, rn5, rn6, rn7, rn8, rn0, rmenu, rplay, ragain, rreset, rGO, rbackg;
Mix_Music * music;
vector<SDL_Texture*> textures;
vector<SDL_Rect> rect;
map<string,SDL_Texture*> texts;

void initComponents(){
    textures.push_back(num0); rect.push_back(rn0);
    textures.push_back(num1); rect.push_back(rn1);
    textures.push_back(num2); rect.push_back(rn2);
    textures.push_back(num3); rect.push_back(rn3);
    textures.push_back(num4); rect.push_back(rn4);
    textures.push_back(num5); rect.push_back(rn5);
    textures.push_back(num6); rect.push_back(rn6);
    textures.push_back(num7); rect.push_back(rn7);
    textures.push_back(num8); rect.push_back(rn8);
}

void randomOrder(){
    state = 2;
    for(int x = 0; x<3; x++)
        for(int y = 0; y<3; y++)
            numeros[y][x] = -1;

    srand(time(NULL));
    for(int x = 0; x<9; x++){
        int randX = rand()%3;
        int randY = rand()%3;

        while(numeros[randY][randX]!=-1){
            randX = rand()%3;
            randY = rand()%3;
        }
        numeros[randY][randX] = x;
    }
    clicks = 1;
    cout << endl << endl;
}

void verifyWin(){
    int cont = 1;
    for(int x = 0; x<3; x++){
        for(int y = 0; y<3; y++){
            if(numeros[y][x]==cont)
                cont++;
            else if(x == 2 && y == 2 && numeros[y][x] == 0)
                cont++;
            else
                cont = 0;
        }
    }
    if(cont==10)
        state = 3;
}

void setTexture(string img, SDL_Texture*(&text), SDL_Rect& rect, int w, int h, int x, int y){
    text = texts[img];
    SDL_QueryTexture(text, NULL, NULL, &w, &h);
    rect.x = x; rect.y = y; rect.w = w; rect.h = h;
}

int main(int argc, char* args[]){
    if(SDL_Init(SDL_INIT_EVERYTHING)<0)
        return 1;

    window = SDL_CreateWindow("Puzzle", 10, 30, 600, 450, SDL_RENDERER_PRESENTVSYNC);
    if(window == NULL)
        return 2;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(renderer == NULL){
        cout << SDL_GetError() << endl;
        return 3;
    }

//    Mix_OpenAudio(44100, MIX_INIT_MP3, 1, 1024);
//    music = Mix_LoadMUS("Great Fairy.mp3");
//    Mix_PlayMusic(music, -1);
    int w=0, h=0;
    initComponents();
    texts["menu.png"] = IMG_LoadTexture(renderer, "menu.png");
    texts["play.png"] = IMG_LoadTexture(renderer, "play.png");
    texts["gameover.png"] = IMG_LoadTexture(renderer, "gameover.png");
    texts["playagain.png"] = IMG_LoadTexture(renderer, "playagain.png");
    texts["background.png"] = IMG_LoadTexture(renderer, "background.png");
    texts["reset.png"] = IMG_LoadTexture(renderer, "reset.png");

    for(int x = 0; x<9; x++){
        sst << x << ".png";
        texts[sst.str()] = IMG_LoadTexture(renderer, sst.str().c_str());
        sst.str(string());
    }

    while(true){
        switch(state){
            case 1:
                setTexture("menu.png", menu, rmenu, w, h, 0, 0);
                setTexture("play.png", play, rplay, w, h, 75, 250);
                SDL_RenderCopy(renderer, menu, NULL, &rmenu);
                SDL_RenderCopy(renderer, play, NULL, &rplay);
                break;

            case 2:
                if(resetorder)
                    randomOrder();

                resetorder = false;
                setTexture("background.png", backg, rbackg, w, h, 0, 0);
                setTexture("reset.png", reset, rreset, w, h, 460, 175);
                SDL_RenderCopy(renderer, backg, NULL, &rbackg);
                SDL_RenderCopy(renderer, reset, NULL, &rreset);
                for(int x = 0; x<3; x++){
                    for(int y = 0; y<3; y++){
                        int pos = numeros[x][y];
                        sst << pos << ".png";
                        setTexture(sst.str(), textures[numeros[x][y]], rect[numeros[x][y]], w, h, x*150, y*150);
                        SDL_RenderCopy(renderer, textures[numeros[x][y]], NULL, &rect[numeros[x][y]]);
                        sst.str(string());
                    }
                }
                break;

            case 3:
                setTexture("gameover.png", GO, rGO, w, h, 0, 0);
                setTexture("playagain.png", again, ragain, w, h, 100, 335);
                SDL_RenderCopy(renderer, GO, NULL, &rGO);
                SDL_RenderCopy(renderer, again, NULL, &ragain);
                break;
        }

        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                return 0;

            else if(event.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&mouseX, &mouseY);

                switch(state){
                    case 1:
                        if(mouseX>75 && mouseX<525 && mouseY>250 && mouseY<400)
                            state = 2;
                        break;

                    case 2:
                        if(!resetorder)
                            if(mouseX>460 && mouseX<590 && mouseY>175 && mouseY<275)
                                resetorder = true;

                        cout << "Clicks: " << clicks << endl;
                        for(int x = 0; x<3; x++){
                            for(int y = 0; y<3; y++){
                                if(mouseX>y*150 && mouseX<(y*150)+150 && mouseY>x*150 && mouseY<(x*150)+150){
                                    if(y+1<3 && numeros[y+1][x]==0){
                                        numeros[y+1][x] = numeros[y][x];
                                        numeros[y][x] = 0;
                                        clicks++;
                                    }

                                    else if(y-1>=0 && numeros[y-1][x]==0){
                                        numeros[y-1][x] = numeros[y][x];
                                        numeros[y][x] = 0;
                                        clicks++;
                                    }

                                    else if(x+1<3 && numeros[y][x+1]==0){
                                        numeros[y][x+1] = numeros[y][x];
                                        numeros[y][x] = 0;
                                        clicks++;
                                    }

                                    else if(x-1>=0 && numeros[y][x-1]==0){
                                        numeros[y][x-1] = numeros[y][x];
                                        numeros[y][x] = 0;
                                        clicks++;
                                    }
                                }
                            }
                        }
                        break;

                    case 3:
                        if(mouseX>100 && mouseX<500 && mouseY>335 && mouseY<445)
                            randomOrder();
                }
            }
            verifyWin();
        }
    }
    return 0;
}
