#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/rtc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pictures.h"
#include "publicVariables.h"
#include "draw.h"
#include "getKeys.h"
#include "timeFunctions.h"

int playField[21][38];

short unsigned int *getVRAMColor(int x, int y) {
    unsigned short *p = GetVRAMAddress();

    return p+(LCD_WIDTH_PX*y+x);

}

unsigned int next = 0;

unsigned int nextRand(int ticks) {
    next=((next+ticks)*1576324345786455+59456863)%32645645620524;
    return next;
}

int checkCollision(int x,int y,int xF, int yF) {

    short unsigned int *collisionColor = getVRAMColor(x,y);

    if(*collisionColor == COLOR_WHITE){
        return 0;
    }else if (playField[yF][xF] != 0){ // game over id == -1
        return -1;
    }else if (*collisionColor == 0xb127){
        return 1;
    }

    return 0;
    
}

void gameOver(int score){
    char buff[50] = "---Your Score is: ";

    //Bdisp_AllClr_VRAM();
    itoa(score-5,(unsigned char *)buff+18);
    PrintXY(0, 1, buff, 0, 0);
    PrintXY(0,3,"---Press F1 to restart",0,0);
    Bdisp_PutDisp_DD();
}

void paintSnake(int width, int height, int len,int dir,int colorMode){
    color_t playFieldTemp;
    switch(colorMode){
        case 1:
            playFieldTemp = 0xffff;
            break;
        case 2:
            playFieldTemp = 0x5FE3;
            break;
        case 3:
            playFieldTemp = 0x0000;
            break;
    }
    for(int a = 0; a<21;a++){
        for(int b = 0; b<=37;b++){
            if(playField[a][b] == 1){
                switch (dir)
                {
                case 0:
                    switch(colorMode){
                        case 1:
                            copyPict(snakeHeadRightWhite,(b*10+3),(a*10+2),10,10);
                            break;
                        case 2:
                            copyPict(snakeHeadRightGreen,(b*10+3),(a*10+2),10,10);
                            break;
                        case 3:
                            copyPict(snakeHeadRightBlack,(b*10+3),(a*10+2),10,10);
                            break;
                    }
                    break;
                case 1:
                    switch(colorMode){
                        case 1:
                            copyPict(snakeHeadDownWhite,(b*10+3),(a*10+2),10,10);
                            break;
                        case 2:
                            copyPict(snakeHeadDownGreen,(b*10+3),(a*10+2),10,10);
                            break;
                        case 3:
                            copyPict(snakeHeadDownBlack,(b*10+3),(a*10+2),10,10);
                            break;
                    }
                    break;
                case 2:
                    switch(colorMode){
                        case 1:
                            copyPict(snakeHeadLeftWhite,(b*10+3),(a*10+2),10,10);
                            break;
                        case 2:
                            copyPict(snakeHeadLeftGreen,(b*10+3),(a*10+2),10,10);
                            break;
                        case 3:
                            copyPict(snakeHeadLeftBlack,(b*10+3),(a*10+2),10,10);
                            break;
                    }
                    break;
                case 3:
                    switch(colorMode){
                        case 1:
                            copyPict(snakeHeadUpWhite,(b*10+3),(a*10+2),10,10);
                            break;
                        case 2:
                            copyPict(snakeHeadUpGreen,(b*10+3),(a*10+2),10,10);
                            break;
                        case 3:
                            copyPict(snakeHeadUpBlack,(b*10+3),(a*10+2),10,10);
                            break;
                    }
                    break;
                }
                playField[a][b]++;
            }else if(playField[a][b] > 1 && playField[a][b] <= len){
                drawRect((b*10+3),(a*10+2),width,height,0x0b81);
                playField[a][b]++;
            }else if(playField[a][b] > len){
                drawRect((b*10+3),(a*10+2),width,height,playFieldTemp);
                playField[a][b] = 0;
            }
        }
    }
}

void credits(){
    int exit = 0;

    SaveVRAM_1();

    copyPict(creditScreen,0,0,384,216);
    Bdisp_PutDisp_DD();

    while(exit == 0){
        if(getKey() == 0x0302){
            exit = 1;
            LoadVRAM_1();
            Bdisp_PutDisp_DD();
        }
    }
}

void about(){

    int exit = 0;

    SaveVRAM_1();

    Bdisp_AllClr_VRAM();

    PrintXY(0,1,"---Version: 1.1",0,0);
    PrintXY(0,2,"---Date: 09.10.2022",0,0);
    PrintXY(0,8,"---To return press EXE",0,0);
    Bdisp_PutDisp_DD();

    while(exit == 0){
        if(getKey() == 0x0302){
            exit = 1;
            LoadVRAM_1();
            Bdisp_PutDisp_DD();
        }
    }

}

int selectScreen(){
    copyPict(startMenu,0,0,384,216);
    Bdisp_PutDisp_DD();

    while(1){
        switch(getKey()){
            case 0x0703:
                return 1;
            case 0x0603:
                return 2;
            case 0x0503:
                return 3;
            case 0x060A:
                credits();
                break;
            case 0x050A:
                about();
                break;
            case 0x0408:
                return 4;
        }
    }
}

int main() {

    Bdisp_EnableColor(1);

    for(int a = 0; a<21;a++){
        for(int b = 0; b<38;b++){
            playField[a][b] = 0;
        }
    }
    int x = 102;
    int y = 103;
    int possibleX = 37;
    int possibleY = 20;
    int width = 9;
    int height = 9;
    int colorMode = 1;
    int dir = 0;
    int len = 5;
    int lastTime = 0;
    int interval = 300;
    int time = 0;
    int stop = 0;
    int repeat = 0;
    int pDir = dir;
    int exitStart = 0;
    
    switch (selectScreen()){
        case 1:
            colorMode = 1;
            break;
        case 2:
            colorMode = 2;
            break;
        case 3:
            colorMode = 3;
            break;
        case 4:
            exitStart = 1;
    }

    Bdisp_AllClr_VRAM();

    switch (colorMode){
    case 1:
        break;
    case 2:
        drawRect(0,0,384,216,0x5FE3);
        break;
    case 3:
        drawRect(0,0,384,216,0x0000);
        break;
    }

    int appleX = nextRand(RTC_GetTicks())%possibleX;
    int appleY = nextRand(RTC_GetTicks())%possibleY;
    
    while(playField[appleY][appleX] != 0){
        appleX = nextRand(RTC_GetTicks())%possibleX;
        appleY = nextRand(RTC_GetTicks())%possibleY;
    }
    switch(colorMode){
        case 1:
            copyPict(appleWhite,appleX*10+3,appleY*10+2,width+1,height+1);
            break;
        case 2:
            copyPict(appleGreen,appleX*10+3,appleY*10+2,width+1,height+1);
            break;
        case 3:
            copyPict(appleBlack,appleX*10+3,appleY*10+2,width+1,height+1);
            break;
    }
    
    while (1 && exitStart == 0) {
        int key = getKey();
        if (key == 0x0408) { // 0x0408 is the EXIT key code
            break;
        }else if (key == 0x0208 && pDir != 2){ // 0x0208 is the RIGHT key code
            dir = 0;
        }else if (key == 0x0308 && pDir != 3){ // 0x0308 is the DOWN key code
            dir = 1;
        }else if (key == 0x0309 && pDir != 0){ // 0x0309 is the LEFT key code
            dir = 2;
        }else if (key == 0x0209 && pDir != 1){ // 0x0209 is the UP key code
            dir = 3;
        }else if(key == 0x070A){ // 0x070A is the F1 key code
            repeat = 1;
            break;
        }
        

        int now = millis();

        if (now - lastTime > interval && stop == 0) {
            lastTime = now;
            time++;
            nextRand(RTC_GetTicks());

            pDir = dir;
            
            switch (dir){
            case 0:
                if(x + 10 >= LCD_WIDTH_PX-7){
                    gameOver(len);
                    break;
                }
                x += 10;
                break;
            case 1:
                if(y + 10 >= LCD_HEIGHT_PX){
                    break;
                    gameOver(len);
                }
                y += 10;
                break;
            case 2:
                if(x - 10 <= 0){
                    break;
                    gameOver(len);
                }
                x -= 10;
                break;
            case 3:
                if(y - 10 <= 0){
                    break;
                    gameOver(len);
                }
                y -= 10;
                break;
            
            default:
                break;
            }

            switch (checkCollision(x+(width/2),y+(height/2),(x-2)/10,(y-3)/10)){
                case -1:
                    stop = 1;
                    break;
                case 0:

                    break;
                case 1:
                    len++;

                    appleX = nextRand(RTC_GetTicks())%possibleX;
                    appleY = nextRand(RTC_GetTicks())%possibleY;

                    while(playField[appleY][appleX] != 0){
                        appleX = nextRand(RTC_GetTicks())%possibleX;
                        appleY = nextRand(RTC_GetTicks())%possibleY;
                    }

                    switch(colorMode){
                        case 1:
                            copyPict(appleWhite,appleX*10+3,appleY*10+2,width+1,height+1);
                            break;
                        case 2:
                            copyPict(appleGreen,appleX*10+3,appleY*10+2,width+1,height+1);
                            break;
                        case 3:
                            copyPict(appleBlack,appleX*10+3,appleY*10+2,width+1,height+1);
                            break;
                    }

                    break;
                
                default:
                    break;
            }
            
            if (stop == 1){
                gameOver(len);
            }
            playField[(y-3)/10][(x-2)/10] = 1;

            if(stop == 0){
                paintSnake(width,height,len,dir,colorMode);
            }

            Bdisp_PutDisp_DD();

        }



    }

    if(repeat == 1){
        main();
    }
 
    return 0;
}
