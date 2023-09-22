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

typedef struct {
	float x;
	float y;
	float z;
} vec3;

void game_init(void) {
	screenSize = 300;
	CP_System_SetWindowSize(screenSize, screenSize);
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_AntiAlias(0);

	mainImage = CP_Image_Screenshot(0, 0, screenSize, screenSize);
	arraySize = screenSize * screenSize;
	imgPixelData = malloc(arraySize * sizeof(CP_Color));
	CP_Image_GetPixelData(mainImage, imgPixelData);
}

float mapToRGB(float x) {
	return (x < 0) ? 0 : (x > 255) ? 255 : x * 255;
}

float step(float a, float x) {
	return (x >= a) ? 1 : 0;
}

float stepSmoothly(float a, float b, float x) {
	if (x > b) return 1;
	if (x < a) return 0;
	float mult = 1 / (b - a);
	return x * mult;
}

vec3 v3Scale(vec3 v, float s) {
	vec3 temp = { 0,0,0 };
	temp.x = v.x * s; 
	temp.y = v.y * s;
	temp.z = v.z * s;
	return temp;
};

vec3 v3Multiply(vec3 v, vec3 u) {
	vec3 temp = { 0,0,0 };
	temp.x = v.x * u.x; 
	temp.y = v.y * u.y;
	temp.z = v.z * u.z;
	return temp;
}

vec3 v3Add(vec3 v, vec3 u) {
	vec3 temp = { 0,0,0 };
	temp.x = v.x + u.x;
	temp.y = v.y + u.y;
	temp.z = v.z + u.z;
	return temp;
}

vec3 v3Cos(vec3 v) {
	vec3 temp = { 0,0,0 };
	temp.x = cos(v.x);
	temp.y = cos(v.y);
	temp.z = cos(v.z);
	return temp;
}

CP_Color v3toColor(vec3 v) {
	return CP_Color_Create(v.x, v.y, v.z, 255);
}

vec3 palette(float t) {
	vec3 a = { 0.5, 0.5, 0.5 };
	vec3 b = { 1.0, 1.0, 1.0 };
	vec3 c = { 1.0, 1.0, 1.0 };
	vec3 d = { 0.0, 0.33f, 0.66f };

	//return a + b * cos(6.28318 * (c * t + d));

	vec3 temp = v3Scale(c, t);
	temp = v3Add(temp, d);
	temp = v3Scale(temp, 6.2831f);
	temp = v3Cos(temp);
	temp = v3Multiply(temp, b);
	temp = v3Add(temp, a); 
	return temp;
}

CP_Vector fract(CP_Vector v) {
	CP_Vector temp = { v.x , v.y };
	int convertVectorX = (int)v.x;
	int convertVectorY = (int)v.y;
	temp.x -= convertVectorX;
	temp.y -= convertVectorY;
	return temp;
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

		uv = fract(uv);

		float d = CP_Vector_Length(uv);

		vec3 col = palette(d + CP_System_GetSeconds());

		d = sin(d * 8 + CP_System_GetSeconds()) / 8;
		d = fabs(d);

		d = 0.02 / d;

		//MAP the normalized vectors to RGB.
		d = mapToRGB(d);

		col = v3Scale(col, d);
		
		imgPixelData[i] = v3toColor(col);
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
