/*
	Green Screen Tool created by Teedle
	This code was modified for the last time on: 05/26/2025

	This project was created for my 200,000 Subscribers celebration!

	Checking the source code to find the easter eggs is cheating you cheater. >:(

	Most of the citro2d draw commands were taken from the 2DShapes source code, but I understand it now.
*/

#include <3ds.h>
#include <stdio.h>
#include <citro2d.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

void brickScreen(C3D_RenderTarget *top, C3D_RenderTarget *bottom);
bool isHexColor(char *input);


int main(int argc, char **argv)
{
	//Initilize citro2d for top and bottom screen
	gfxInitDefault();
	romfsInit();
	
	//Loads the teedle egg in the romfs folder that I converted into a .t3x by entering a command in MSys2
	C2D_SpriteSheet spriteSheet;
	spriteSheet = C2D_SpriteSheetLoad("romfs:/teedleegg.t3x");
	C2D_Sprite egg;
	C2D_SpriteFromSheet(&egg, spriteSheet, 0);

	//Initilizes C2D AND C3D because C2D uses C3D to function
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	//Creates the top and bottom screens so I can use them when I need to later on in the code
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	//Create the screen colors
	u32 clrGreen = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
	u32 clrRed   = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
	u32 clrBlue  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
	u32 clrCustom  = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	u32 clrClear = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);

    //Determines if the home menu is being displayed. This is NOT how you create different pages on an app is it... 
	bool homeMenu = true;


	//Controls if you can change the color while a color is being displayed
	bool colorLock = false;

	//Determines if the brick screen is being shown
	bool brick = false;

	//PrintConsole topScreen, bottomScreen; Trying to use Citro2d for text and rectangles now
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_SceneBegin(top);
	C2D_SceneBegin(bottom);

	//Selected color text that shows on screen
	char selectedColor[32] = "Green";
	
	//Characters that are used for the custom color hex code input
	char firstTwo[3];
	char middleTwo[3];
	char lastTwo[3];
	char input[7];

    //initilizes keyboard (Most keyboard commands were taken from software-keyboard source code but I understand it now)
	SwkbdState swkbd;

	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();

		if(kDown & KEY_START) break;

		if(kDown & KEY_X && homeMenu){
			if(colorLock){
				colorLock = false;
			}
			else{
				colorLock = true;
			}
		}

		//Hotkey to change color to Green
		if((kDown & KEY_DDOWN))
		{
			
			if(!colorLock || homeMenu){
				//chatgpt taught me the strcpy and strcmp to modify and compare arrays, which I fully understand now
				strcpy(selectedColor, "Green");
				C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
			}
			else if(colorLock && homeMenu){
				strcpy(selectedColor, "Green");
				C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
			}
		}

		//Hotkey to change color to uh oh
		if((kDown & KEY_L))
		{
			if(homeMenu || !colorLock){
			 brick = true;
			brickScreen(top, bottom);
			homeMenu = false;
			colorLock = true;	
			}
		}

		//Hotkey to change color to Blue
		if((kDown & KEY_DLEFT))
		{
			if(!colorLock || homeMenu){
			strcpy(selectedColor, "Blue");
			C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
			}
			else if(colorLock && homeMenu){
              strcpy(selectedColor, "Blue");
			  C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
			}
		}

		//Hotkey to change color to Red
		if((kDown & KEY_DRIGHT))
		{
			if(!colorLock || homeMenu){
			strcpy(selectedColor, "Red");
			C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
			}
			else if(colorLock && homeMenu){
				strcpy(selectedColor, "Red");
			  C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
			}
		}

		//Opens keyboard to input a hex color code
		if((kDown & KEY_DUP))
		{
			if((!colorLock || homeMenu) || (colorLock && homeMenu)){
			swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, 6);
			swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
			swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Submit", true);
    		swkbdSetHintText(&swkbd, "Enter HEX Color value");

			SwkbdButton button = swkbdInputText(&swkbd, input, sizeof(input));
			

	


			//Sees if what was inputted is hex code or special text
			if(button == SWKBD_BUTTON_RIGHT && isHexColor(input)){
            
				if(strcmp(input, "teedle") == 0){
				strcpy(selectedColor, "Teedle");
			}

			else if(strcmp(input, "teddle") == 0){
				strcpy(selectedColor, "Teddle");
			}
			else if(strcmp(input, "200000") == 0){
				strcpy(selectedColor, "200,000");
			}
			else{
				strncpy(firstTwo, input, 2);
				strncpy(middleTwo, input + 2, 2);
				strncpy(lastTwo, input + 4, 2);
				u8 r = (u8)strtol(firstTwo, NULL, 16);
				u8 g = (u8)strtol(middleTwo, NULL, 16);
				u8 b = (u8)strtol(lastTwo, NULL, 16);
				strcpy(selectedColor, "Custom");
				clrCustom  = C2D_Color32(r, g, b, 0xFF);
			}
				

			}
			
			}
		
		}


		//Home menu screen text
		if(homeMenu)
		{
		  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		  C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
		  C2D_TargetClear(bottom, C2D_Color32(0, 0, 0, 255));
		  C2D_SceneBegin(top);

		  C2D_TextBuf titleBuf = C2D_TextBufNew(256);
		  C2D_Text titleText;
		  C2D_TextParse(&titleText, titleBuf, "3DS Screen Color Changer");
		  C2D_TextOptimize(&titleText);
		  C2D_DrawText(&titleText, C2D_AlignCenter | C2D_WithColor, 200, 10, 0.5, 1, 1, clrClear);
		  C2D_TextBufDelete(titleBuf);

		  char sbuf[64];
		  sprintf(sbuf, "Selected Color: %s", selectedColor);


		  C2D_TextBuf colorBuf = C2D_TextBufNew(256);
		  C2D_Text colorText;

		  C2D_TextParse(&colorText, colorBuf, sbuf);
		  C2D_TextOptimize(&colorText);
		  C2D_DrawText(&colorText, C2D_AlignCenter| C2D_WithColor, 200, 50, 0.5, 0.6, 0.6, clrClear);
		  C2D_TextBufDelete(colorBuf);

		  //Display if colorLock is enabled
		  if(colorLock){
			C2D_TextBuf lockBuf = C2D_TextBufNew(256);
			C2D_Text lockText;
			C2D_TextParse(&lockText, lockBuf, "Color Lock ON");
			C2D_TextOptimize(&lockText);
			C2D_DrawText(&lockText, C2D_AlignRight | C2D_WithColor, 200, 75, 0.5, 0.5, 0.5, clrRed);
			C2D_TextBufDelete(lockBuf);
			
		  }


		  //Shows press start to exit on the bottom screen
		  C2D_SceneBegin(bottom);

		  C2D_TextBuf exitBuf = C2D_TextBufNew(256);
		  C2D_Text exitText;
		  C2D_TextParse(&exitText, exitBuf, "Press START to exit");
		  C2D_TextOptimize(&exitText);
		  C2D_DrawText(&exitText, C2D_AlignCenter | C2D_WithColor, 160, 200, 0.5, 0.6, 0.6, clrClear);
		  C2D_TextBufDelete(exitBuf);


		  C3D_FrameEnd(0);
		  //printf("\x1b[1;1H3DS Screen Color Changer");
		  //printf("\x1b[3;1HSelected Color: %s", selectedColor);

		}
		
		//Code used to display the colored rectangles on screen
		else
		{
			if(!brick){
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
			C2D_TargetClear(bottom, C2D_Color32(0, 0, 0, 255));
		    C2D_SceneBegin(top);
			if (strcmp(selectedColor, "Green") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 400, 240, clrGreen);
			}

			if (strcmp(selectedColor, "200,000") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 400, 240, C2D_Color32(0x20, 0x00, 0x00, 0xFF));
				C2D_TextBuf tBuf = C2D_TextBufNew(256);
				C2D_Text tText;
				C2D_TextParse(&tText, tBuf, "Thank you for 200,000 Subscribers!");
				C2D_TextOptimize(&tText);
				C2D_DrawText(&tText, C2D_AlignCenter | C2D_WithColor, 200, 120, 0.5, 0.5, 0.5, clrClear);
				C2D_TextBufDelete(tBuf);
			}

			if (strcmp(selectedColor, "Teedle") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 400, 240, clrClear);
				C2D_DrawSprite(&egg);
			}

			if (strcmp(selectedColor, "Teddle") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 400, 240, clrGreen);
				C2D_DrawSprite(&egg);
			}

			if (strcmp(selectedColor, "Red") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 400, 240, clrRed);
			}

			if (strcmp(selectedColor, "Blue") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 400, 240, clrBlue);
			}

            if (strcmp(selectedColor, "Custom") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 400, 240, clrCustom);
			}

			
			C2D_SceneBegin(bottom);
			if (strcmp(selectedColor, "Green") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 320, 240, clrGreen);
			}

			if (strcmp(selectedColor, "200,000") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 400, 240, C2D_Color32(0x20, 0x00, 0x00, 0xFF));
			}

			if (strcmp(selectedColor, "Teedle") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 320, 240, clrClear);
				C2D_TextBuf tBuf = C2D_TextBufNew(256);
				C2D_Text tText;
				C2D_TextParse(&tText, tBuf, "Teedle says hi");
				C2D_TextOptimize(&tText);
				C2D_DrawText(&tText, C2D_AlignCenter | C2D_WithColor, 160, 100, 1, 1, 0.5, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
				C2D_TextBufDelete(tBuf);
				
			}

			if (strcmp(selectedColor, "Teddle") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 320, 240, clrGreen);
				C2D_TextBuf tBuf = C2D_TextBufNew(256);
				C2D_Text tText;
				C2D_TextParse(&tText, tBuf, "Teddle says hi");
				C2D_TextOptimize(&tText);
				C2D_DrawText(&tText, C2D_AlignCenter | C2D_WithColor, 160, 100, 1, 1, 0.5, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
				C2D_TextBufDelete(tBuf);
				
			}

			if (strcmp(selectedColor, "Red") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 320, 240, clrRed);
			}

			if (strcmp(selectedColor, "Blue") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 320, 240, clrBlue);
			}

            if (strcmp(selectedColor, "Custom") == 0){
				C2D_DrawRectSolid(0, 0, 0.0, 320, 240, clrCustom);
			}
			C3D_FrameEnd(0);
		}
			

		}

		
		//Toggles the home menu screen
		if (kDown & KEY_B)
		{
            if(homeMenu){
				homeMenu = false;
			}
			else{
				homeMenu = true;
				brick = false;
			}
		}
		
	
	}
	
	//My app stopped crashing after adding these
	romfsExit();
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}

//I learned how to create my own method! This is for showing the fake brick screen
void brickScreen(C3D_RenderTarget *top, C3D_RenderTarget *bottom){
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, C2D_Color32(0, 0, 255, 255));
	C2D_TargetClear(bottom, C2D_Color32(0, 0, 255, 255));
	C2D_SceneBegin(top);

	C2D_TextBuf brickBuf = C2D_TextBufNew(1024);
		C2D_Text brickText;
		C2D_TextParse(&brickText, brickBuf, "BOOTROM 8046\nERRCODE:  00F800EF\n                 FFFFFFFF  FFFFFFFF\n                 0000000  0000000");
        C2D_TextOptimize(&brickText);

		C2D_DrawText(&brickText, C2D_AlignLeft| C2D_WithColor, 7, 7, 0.4, 0.4, 0.5, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF));
		C2D_TextBufClear(brickBuf);




    C2D_SceneBegin(bottom);
	    C2D_Text bBrickText;
		C2D_TextParse(&bBrickText, brickBuf, "BOOTROM 8046\nERRCODE:  00F800EF\n                 FFFFFFFF  FFFFFFFF\n                 0000000  0000000");
        C2D_TextOptimize(&bBrickText);

		C2D_DrawText(&bBrickText, C2D_AlignLeft| C2D_WithColor, 5, 5, 0.35, 0.35, 0.5, C2D_Color32(0xFF, 0xFF, 0x00, 0xFF));
		C2D_TextBufDelete(brickBuf);

	C3D_FrameEnd(0);


}

//This determines if what was inputted on the keyboard was valid hex code OR a special word
bool isHexColor(char *input)
{
  if(strcmp(input, "teedle") == 0 || strcmp(input, "teddle") == 0){
	return true;
  }
	if(strlen(input) != 6){
	return false;
  }
  //This searches through every chracter in the input array and checks if it not a valid hex code. Returns false if it is not
  for(int i = 0; i < 6; i++){
	//isxdigit is a method used to check for valid hexcode. Thank GOODNESS this exisits, idk what I would've done
	if(!isxdigit(input[i])){
		return false;
	}
  }
  //Can return true at the end because if nothing at the top returned false, what was entered must be hex code or a special word.
  return true;
}
