/*
 * examples.c
 *
 *  Created on: 11.02.2018
 *      Author: harebit
 *      same examples
 */

#include "ucg.h"
#include "main.h"
#include "stm32f1xx_hal.h"

void upper_pin(ucg_t *ucg, ucg_int_t x, ucg_int_t y)
{
  ucg_int_t w = 7;
  ucg_int_t h = 6;
  ucg_SetColor(ucg, 0, 212, 212, 212);
  ucg_SetColor(ucg, 1, 200, 200, 200);
  ucg_SetColor(ucg, 2, 200, 200, 200);
  ucg_SetColor(ucg, 3, 188, 188, 188);
  ucg_DrawGradientBox(ucg, x, y, w, h);

  ucg_SetColor(ucg, 0, 220, 220, 220);
  ucg_SetColor(ucg, 1, 232, 232, 232);
  ucg_DrawGradientLine(ucg, x+w, y, h, 1);
}

void lower_pin(ucg_t *ucg, ucg_int_t x, ucg_int_t y)
{
  ucg_int_t w = 7;
  ucg_int_t h = 5;
  ucg_SetColor(ucg, 0, 212, 212, 212);
  ucg_SetColor(ucg, 1, 200, 200, 200);
  ucg_SetColor(ucg, 2, 200, 200, 200);
  ucg_SetColor(ucg, 3, 188, 188, 188);
  ucg_DrawGradientBox(ucg, x, y, w, h);

  ucg_SetColor(ucg, 0, 220, 220, 220);
  ucg_SetColor(ucg, 1, 232, 232, 232);
  ucg_DrawGradientLine(ucg, x+w, y, h, 1);
  ucg_SetColor(ucg, 0, 220, 220, 220);
  ucg_SetColor(ucg, 1, 232, 232, 232);
  ucg_DrawGradientLine(ucg, x, y+h, w, 0);
  ucg_SetColor(ucg, 0, 240, 240, 240);
  ucg_DrawPixel(ucg, x+w, y+h);
}

void ic_body(ucg_t *ucg, ucg_int_t x, ucg_int_t y)
{
  ucg_int_t w = 4*14+4;
  ucg_int_t h = 31;
  ucg_SetColor(ucg, 0, 60, 60, 60);
  ucg_SetColor(ucg, 1, 40, 40, 40);
  ucg_SetColor(ucg, 2, 48, 48, 48);
  ucg_SetColor(ucg, 3, 30, 30, 30);
  ucg_DrawGradientBox(ucg, x, y, w, h);

  ucg_SetColor(ucg, 0, 255, 168, 0);
  //ucg_SetColor(ucg, 0, 225, 168, 30);
  ucg_DrawDisc(ucg, x+w-1, y+h/2-1, 7, UCG_DRAW_UPPER_LEFT|UCG_DRAW_LOWER_LEFT);

  ucg_SetColor(ucg, 0, 60, 30, 0);
  //ucg_DrawDisc(ucg, x+w-1, y+h/2+1, 7, UCG_DRAW_UPPER_LEFT|UCG_DRAW_LOWER_LEFT);

  ucg_SetColor(ucg, 0, 50, 50, 50);
  ucg_SetColor(ucg, 0, 25, 25, 25);
  ucg_DrawDisc(ucg, x+w-1, y+h/2+1, 7, UCG_DRAW_UPPER_LEFT|UCG_DRAW_LOWER_LEFT);

}


void draw_ucg_logo(ucg_t* ucg)
{
  ucg_int_t a,b;

  //ucg_Init(ucg, ucg_sdl_dev_cb, ucg_ext_none, (ucg_com_fnptr)0);
  ucg_SetFont(ucg, ucg_font_ncenB18_tr);

  //ucg_SetRotate270(ucg);
  //ucg_SetClipRange(ucg, 10,5,40,20);

  a = 2;
  b = 3;

  ucg_SetColor(ucg, 0, 135*a/b,206*a/b,250*a/b);
  ucg_SetColor(ucg, 1, 176*a/b,226*a/b,255*a/b);
  ucg_SetColor(ucg, 2, 25*a/b,25*a/b,112*a/b);
  ucg_SetColor(ucg, 3, 	85*a/b,26*a/b,139*a/b);
  ucg_DrawGradientBox(ucg, 0, 0, ucg_GetWidth(ucg)/4, ucg_GetHeight(ucg));

  ucg_SetColor(ucg, 1, 135*a/b,206*a/b,250*a/b);
  ucg_SetColor(ucg, 0, 176*a/b,226*a/b,255*a/b);
  ucg_SetColor(ucg, 3, 25*a/b,25*a/b,112*a/b);
  ucg_SetColor(ucg, 2, 	85*a/b,26*a/b,139*a/b);
  ucg_DrawGradientBox(ucg, ucg_GetWidth(ucg)/4, 0, ucg_GetWidth(ucg)/4, ucg_GetHeight(ucg));

  ucg_SetColor(ucg, 0, 135*a/b,206*a/b,250*a/b);
  ucg_SetColor(ucg, 1, 176*a/b,226*a/b,255*a/b);
  ucg_SetColor(ucg, 2, 25*a/b,25*a/b,112*a/b);
  ucg_SetColor(ucg, 3, 	85*a/b,26*a/b,139*a/b);
  ucg_DrawGradientBox(ucg, ucg_GetWidth(ucg)*2/4, 0, ucg_GetWidth(ucg)/4, ucg_GetHeight(ucg));

  ucg_SetColor(ucg, 1, 135*a/b,206*a/b,250*a/b);
  ucg_SetColor(ucg, 0, 176*a/b,226*a/b,255*a/b);
  ucg_SetColor(ucg, 3, 25*a/b,25*a/b,112*a/b);
  ucg_SetColor(ucg, 2, 	85*a/b,26*a/b,139*a/b);
  ucg_DrawGradientBox(ucg, ucg_GetWidth(ucg)*3/4, 0, ucg_GetWidth(ucg)/4, ucg_GetHeight(ucg));


  upper_pin(ucg, 7+0*14, 4);
  upper_pin(ucg, 7+1*14, 4);
  upper_pin(ucg, 7+2*14, 4);
  upper_pin(ucg, 7+3*14, 4);

  ic_body(ucg, 2, 10);

  lower_pin(ucg, 7+0*14, 41);
  lower_pin(ucg, 7+1*14, 41);
  lower_pin(ucg, 7+2*14, 41);
  lower_pin(ucg, 7+3*14, 41);

  ucg_SetColor(ucg, 0, 135*a/b, 206*a/b, 250*a/b);
  ucg_DrawString(ucg, 63+1, 33+1, 0, "glib");

  ucg_SetColor(ucg, 0, 255, 168, 0);
  ucg_DrawGlyph(ucg, 26, 38, 0, 'U');
  ucg_DrawString(ucg, 63, 33, 0, "glib");

  ucg_SetColor(ucg, 0, 135*a/b, 206*a/b, 250*a/b);
  ucg_SetColor(ucg, 1, 135*a/b, 206*a/b, 250*a/b);
  ucg_SetColor(ucg, 2, 135*a/b, 206*a/b, 250*a/b);
  ucg_SetColor(ucg, 3, 135*a/b, 206*a/b, 250*a/b);
  ucg_DrawGradientBox(ucg, 84+1, 42+1-6, 42, 4);

  ucg_SetColor(ucg, 0, 255, 180, 40);
  ucg_SetColor(ucg, 1, 235, 148, 0);
  //ucg_DrawGradientLine(ucg, 79, 42, 20, 0);
  ucg_SetColor(ucg, 2, 245, 158, 0);
  ucg_SetColor(ucg, 3, 220, 138, 0);
  ucg_DrawGradientBox(ucg, 84, 42-6, 42, 4);

  ucg_SetColor(ucg, 0, 255, 168, 0);
  //ucg_SetFont(ucg, ucg_font_5x8_tr);
  ucg_SetFont(ucg, ucg_font_7x13B_tr);
  //ucg_SetFont(ucg, ucg_font_courB08_tr);
  //ucg_SetFont(ucg, ucg_font_timR08_tr);
  ucg_DrawString(ucg, 2, 54+5, 0, "http://github.com");
  ucg_DrawString(ucg, 2, 61+10, 0, "/olikraus/ucglib");
  //ucg_DrawString(ucg, 1, 61, 0, "code.google.com/p/ucglib/");

}

/*
  Function which responds for drawing
  */

void ucg_box_frame(ucg_t* ucg)
{
  ucg_SetColor(ucg, 0,255,255,255);
  ucg_DrawString(ucg, 5,5,0, "DrawBox");
  ucg_SetColor(ucg, 0,255,255,255);
  ucg_DrawBox(ucg,3,25,20,10);
  ucg_SetColor(ucg, 0,0,255,0);
  ucg_DrawBox(ucg,30,25,25,7);
  ucg_SetColor(ucg, 0,0,0,255);
  ucg_DrawBox(ucg,60,25,25,7);
  ucg_SetColor(ucg, 0,255,0,0);
  ucg_DrawBox(ucg,90,25,25,7);

  ucg_SetColor(ucg, 0,255,255,255);
  ucg_DrawString(ucg, 5,62,0, "DrawFrame");
  ucg_SetColor(ucg, 0,255,255,255);
  ucg_DrawFrame(ucg,3,25+62,20,10);
  ucg_SetColor(ucg, 0,0,255,0);
  ucg_DrawFrame(ucg,30,25+62,25,7);
  ucg_SetColor(ucg, 0,0,0,255);
  ucg_DrawFrame(ucg,60,25+62,25,7);
  ucg_SetColor(ucg, 0,255,0,0);
  ucg_DrawFrame(ucg,90,25+62,25,7);
}

void ucg_disc_circle(ucg_t* ucg)
{

  ucg_DrawString(ucg, 5, 5,0, "DrawDisc");
  ucg_DrawDisc(ucg,10,32,7,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,255,0,0);
  ucg_DrawDisc(ucg,25,30,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,255,255,0);
  ucg_DrawDisc(ucg,40,30,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,0,255,0);
  ucg_DrawDisc(ucg,55,30,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,0,255,255);
  ucg_DrawDisc(ucg,70,30,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,0,0,255);
  ucg_DrawDisc(ucg,85,30,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,255,0,255);
  ucg_DrawDisc(ucg,100,30,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,255,0,0);
  ucg_DrawDisc(ucg,115,30,5,UCG_DRAW_ALL);


  ucg_SetColor(ucg, 0,255,255,255);
  ucg_DrawString(ucg, 5, 62,0, "DrawCircle");
  ucg_DrawCircle(ucg,10,92,7,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,255,0,0);
  ucg_DrawCircle(ucg,25,90,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,255,255,0);
  ucg_DrawCircle(ucg,40,90,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,0,255,0);
  ucg_DrawCircle(ucg,55,90,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,0,255,255);
  ucg_DrawCircle(ucg,70,90,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,0,0,255);
  ucg_DrawCircle(ucg,85,90,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,255,0,255);
  ucg_DrawCircle(ucg,100,90,5,UCG_DRAW_ALL);
  ucg_SetColor(ucg, 0,255,0,0);
  ucg_DrawCircle(ucg,115,90,5,UCG_DRAW_ALL);

}

void ucg_string(ucg_t* ucg)
{
  ucg_DrawString(ucg, 5, 5,0, "DrawString");
  ucg_SetColor(ucg, 0,255,0,0);
  ucg_DrawString(ucg,72,52,0, " 0");
  ucg_SetColor(ucg, 0,0,255,0);
  ucg_DrawString(ucg,67,62,1, " 90");
  ucg_SetColor(ucg, 0,0,0,255);
  ucg_DrawString(ucg,54,66,2, " 180");
  ucg_SetColor(ucg, 0,0,255,255);
  ucg_DrawString(ucg,55,82,3, " 270");
}

void ucg_line(ucg_t* ucg)
{
  ucg_DrawString(ucg, 5, 5,0, "DrawLine");
  ucg_SetColor(ucg, 0,255,0,0);
  ucg_DrawLine(ucg,10, 25, 60, 120);
  ucg_DrawLine(ucg,20, 25, 80, 120);
  ucg_DrawLine(ucg,30, 25, 100, 120);
  ucg_DrawLine(ucg,40, 25, 120, 120);
  ucg_SetColor(ucg, 0,0,255,0);
  ucg_DrawLine(ucg,10+5, 25, 60-5, 120);
  ucg_DrawLine(ucg,20+5*2, 25, 80-5*2, 120);
  ucg_DrawLine(ucg,30+5*3, 25, 100-5*3, 120);
  ucg_DrawLine(ucg,40+5*4, 25, 120-5*4, 120);
  ucg_SetColor(ucg, 0,0,0,255);
  ucg_DrawLine(ucg,10+10, 25, 60-10, 120);
  ucg_DrawLine(ucg,20+10*2, 25, 80-10*2, 120);
  ucg_DrawLine(ucg,30+10*3, 25, 100-10*3, 120);
  ucg_DrawLine(ucg,40+10*4, 25, 120-10*4, 120);
  ucg_SetColor(ucg, 0,0,255,255);
  ucg_DrawLine(ucg,10+15, 25, 60-15, 120);
  ucg_DrawLine(ucg,20+15*2, 25, 80-15*2, 120);
  ucg_DrawLine(ucg,30+15*3, 25, 100-15*3, 120);
  ucg_DrawLine(ucg,40+15*4, 25, 120-15*4, 120);
}


void draw(ucg_t* ucg)
{
	while(1)
	{
		ucg_SetFontPosBaseline(ucg);
	    draw_ucg_logo(ucg);
    	  HAL_Delay(1000);
    	  ucg_SetMaxClipRange(ucg);
    	  ucg_DrawBox(ucg, 0, 0, ucg_GetWidth(ucg), ucg_GetHeight(ucg));
    	  ucg_SetColor(ucg, 0, 55, 55, 55);
    	  ucg_SetFont(ucg, ucg_font_ncenB12_tr);
    	  ucg_SetFontRefHeightExtendedText(ucg);
    	  ucg_SetFontPosTop(ucg);
    	ucg_box_frame(ucg);
    	  HAL_Delay(1000);
    	  ucg_SetColor(ucg, 0, 100, 100, 100);
    	  ucg_DrawBox(ucg, 0, 0, ucg_GetWidth(ucg), ucg_GetHeight(ucg));
    	  ucg_SetColor(ucg, 0, 255, 255, 255);
    	ucg_disc_circle(ucg);
    	  HAL_Delay(1000);
    	  ucg_SetColor(ucg, 0, 100, 0, 0);
    	  ucg_DrawBox(ucg, 0, 0, ucg_GetWidth(ucg), ucg_GetHeight(ucg));
    	  ucg_SetColor(ucg, 0, 255, 255, 255);
    	ucg_string(ucg);
    	  HAL_Delay(1000);
    	  ucg_SetColor(ucg, 0, 0, 100, 0);
    	  ucg_DrawBox(ucg, 0, 0, ucg_GetWidth(ucg), ucg_GetHeight(ucg));
    	  ucg_SetColor(ucg, 0, 255, 255, 255);
    	ucg_line(ucg);
    	  HAL_Delay(1000);
    }
}
