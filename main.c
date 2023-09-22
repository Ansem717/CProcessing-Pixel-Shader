//---------------------------------------------------------
// file:	main.c
// author:	Andy Malik
// email:	andy.malik@digipen.edu
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma warning(disable: 4244)

CP_Image mainImage;
CP_Color* imgPixelData;

int screenSize;
int arraySize;

void game_init(void) {
	screenSize = 600;
	CP_System_SetWindowSize(screenSize, screenSize);
	CP_Settings_ImageMode(CP_POSITION_CORNER);

	mainImage = CP_Image_Screenshot(0, 0, screenSize, screenSize);
	arraySize = screenSize * screenSize;
	imgPixelData = malloc(arraySize * sizeof(CP_Color));
	CP_Image_GetPixelData(mainImage, imgPixelData);
}

float mapToRGB(float x) {
	return (x < 0) ? 0 : x * 255;
}

void game_update(void) {

	//This is the shader. Do some math for every pixel.
	for (int i = 0; i < arraySize; i++) {
		float x = i % screenSize;
		float y = (float)i / screenSize;
		CP_Vector uv = CP_Vector_Set(
			(x/screenSize - 0.5) * 2, 
			(y/screenSize - 0.5) * 2
		);

		float d = CP_Vector_Length(uv);

		d -= 0.5;
		d = fabs(d);

		//MAP the normalized vectors to RGB.
		d = mapToRGB(d);
		imgPixelData[i] = CP_Color_Create(d, 0, 0, 255);
	}
	CP_Image_UpdatePixelData(mainImage, imgPixelData);
	
	CP_Image_Draw(mainImage, 0, 0, screenSize, screenSize, 255);
}

void game_exit(void) {
	free(imgPixelData);
}

int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}
