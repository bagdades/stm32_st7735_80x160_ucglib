/*

  ucg.h

  ucglib = universal color graphics library
  ucglib = micro controller graphics library
  
  Universal uC Color Graphics Library
  
  Copyright (c) 2013, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  

  SW layers
  
  High Level Procedures
    [ hline, init message interface ]
  display callback procedure
    [Calls to]
  device dev cb 		
    [calls to COM API]
  com callback


  font data:
  offset	bytes	description
  0		1		glyph_cnt		number of glyphs
  1		1		bbx_mode	0: proportional, 1: common height, 2: monospace, 3: multiple of 8
  2		1		bits_per_0	glyph rle parameter
  3		1		bits_per_1	glyph rle parameter

  4		1		bits_per_char_width		glyph rle parameter
  5		1		bits_per_char_height	glyph rle parameter
  6		1		bits_per_char_x		glyph rle parameter
  7		1		bits_per_char_y		glyph rle parameter
  8		1		bits_per_delta_x		glyph rle parameter

  9		1		max_char_width
  10		1		max_char_height
  11		1		x offset
  12		1		y offset (descent)
  
  13		1		ascent (capital A)
  14		1		descent (lower g)
  15		1		ascent '('
  16		1		descent ')'
  
  17		1		start pos 'A' high byte
  18		1		start pos 'A' low byte

  19		1		start pos 'a' high byte
  20		1		start pos 'a' low byte


*/

#ifndef _UCG_H
#define _UCG_H

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define width_of_display 80	//max. 132
#define height_of_display 160	//max. 162


#if defined(ARDUINO)
#ifndef USE_PIN_LIST
#define USE_PIN_LIST
#endif
#endif

#ifdef __GNUC__
#  define UCG_NOINLINE __attribute__((noinline))
#  define UCG_SECTION(name) __attribute__ ((section (name)))
#  if defined(__MSPGCC__)
/* mspgcc does not have .progmem sections. Use -fdata-sections. */
#    define UCG_FONT_SECTION(name)
#  elif defined(__AVR__)
#    define UCG_FONT_SECTION(name) UCG_SECTION(".progmem." name)
#  else
#    define UCG_FONT_SECTION(name)
#  endif
#else
#  define UCG_NOINLINE
#  define UCG_SECTION(name)
#  define UCG_FONT_SECTION(name)
#endif

#if defined(__AVR__)
#include <avr/pgmspace.h>
/* UCG_PROGMEM is used by the XBM example */
#define UCG_PROGMEM UCG_SECTION(".progmem.data")
typedef uint8_t PROGMEM ucg_pgm_uint8_t;
typedef uint8_t ucg_fntpgm_uint8_t;
#define ucg_pgm_read(adr) pgm_read_byte_near(adr)
#define UCG_PSTR(s) ((ucg_pgm_uint8_t *)PSTR(s))
#else
#define UCG_PROGMEM
#define PROGMEM
typedef uint8_t ucg_pgm_uint8_t;
typedef uint8_t ucg_fntpgm_uint8_t;
#define ucg_pgm_read(adr) (*(const ucg_pgm_uint8_t *)(adr)) 
#define UCG_PSTR(s) ((ucg_pgm_uint8_t *)(s))
#endif

/*================================================*/
/* several type and forward definitions */

typedef int16_t ucg_int_t;
typedef struct _ucg_t ucg_t;
typedef struct _ucg_xy_t ucg_xy_t;
typedef struct _ucg_wh_t ucg_wh_t;
typedef struct _ucg_box_t ucg_box_t;
typedef struct _ucg_color_t ucg_color_t;
typedef struct _ucg_ccs_t ucg_ccs_t;
typedef struct _ucg_pixel_t ucg_pixel_t;
typedef struct _ucg_arg_t ucg_arg_t;
typedef struct _ucg_com_info_t ucg_com_info_t;

typedef ucg_int_t (*ucg_dev_fnptr)(ucg_t *ucg, ucg_int_t msg, void *data); 
typedef int16_t (*ucg_com_fnptr)(ucg_t *ucg, int16_t msg, uint16_t arg, uint8_t *data); 
typedef ucg_int_t (*ucg_font_calc_vref_fnptr)(ucg_t *ucg);
//typedef ucg_int_t (*ucg_font_mode_fnptr)(ucg_t *ucg, ucg_int_t x, ucg_int_t y, uint8_t dir, uint8_t encoding);


/*================================================*/
/* list of supported display modules */

ucg_int_t ucg_dev_ssd1351_18x128x128_ilsoft(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ssd1351_18x128x128_ft(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ili9325_18x240x320_itdb02(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ili9325_spi_18x240x320(ucg_t *ucg, ucg_int_t msg, void *data); /*  1 May 2014: Currently, this is not working */
ucg_int_t ucg_dev_ili9341_18x240x320(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_hx8352c_18x240x400(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ili9486_18x320x480(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ili9163_18x128x128(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_st7735_18x128x160(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_st7735_18x80x160(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_st7735_18xwidthxheight(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_st7735_18xwidthxheight(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_pcf8833_16x132x132(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ld50t6160_18x160x128_samsung(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ssd1331_18x96x64_univision(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_seps225_16x128x128_univision(ucg_t *ucg, ucg_int_t msg, void *data);



/*================================================*/
/* 
  list of extensions for the controllers 
  
  each module can have the "none" extension (ucg_ext_none) or the specific
  extensions, that matches the controller name and color depth.
  
  example: for the module ucg_dev_ssd1351_18x128x128_ilsoft
  valid extensions are:
    ucg_ext_none
    ucg_ext_ssd1351_18
*/

ucg_int_t ucg_ext_none(ucg_t *ucg, ucg_int_t msg, void *data);

ucg_int_t ucg_ext_ssd1351_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_ili9325_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_ili9325_spi_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_ili9341_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_hx8352c_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_ili9486_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_ili9163_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_st7735_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_st7735L_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_pcf8833_16(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_ld50t6160_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_ssd1331_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_ext_seps225_16(ucg_t *ucg, ucg_int_t msg, void *data);


/*================================================*/
/* list of supported display controllers */

ucg_int_t ucg_dev_ic_ssd1351_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_ili9325_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_ili9325_spi_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_ili9341_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_hx8352c_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_ili9486_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_ili9163_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_st7735_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_st7735L_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_pcf8833_16(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_ld50t6160_18(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_dev_ic_ssd1331_18(ucg_t *ucg, ucg_int_t msg, void *data);   /* actually this display only has 65k colors */
ucg_int_t ucg_dev_ic_seps225_16(ucg_t *ucg, ucg_int_t msg, void *data);   /* this display could do 262k colors, but only 65k are supported via SPI */


/*================================================*/
/* struct declarations */

struct _ucg_xy_t
{
  ucg_int_t x;
  ucg_int_t y;
};

struct _ucg_wh_t
{
  ucg_int_t w;  
  ucg_int_t h;
};

struct _ucg_box_t
{
  ucg_xy_t ul;
  ucg_wh_t size;
};

struct _ucg_color_t
{
  uint8_t color[3];		/* 0: Red, 1: Green, 2: Blue */
};

struct _ucg_ccs_t
{
  uint8_t current;	/* contains the current color component */
  uint8_t start;
  ucg_int_t dir;		/* 1 if start < end or -1 if start > end */
  ucg_int_t num;
  ucg_int_t quot;
  
  ucg_int_t den;
  ucg_int_t rem;  
  ucg_int_t frac;
};

struct _ucg_pixel_t
{
  ucg_xy_t pos;
  ucg_color_t rgb;  
};

struct _ucg_arg_t
{
  ucg_pixel_t pixel;
  ucg_int_t len;
  ucg_int_t dir;
  ucg_int_t offset;			/* calculated offset from the inital point to the start of the clip window (ucg_clip_l90fx) */
  ucg_int_t scale;			/* upscale factor, used by UCG_MSG_DRAW_L90BF */
  const unsigned char *bitmap;
  ucg_int_t pixel_skip;		/* within the "bitmap" skip the specified number of pixel with the bit. pixel_skip is always <= 7 */
  ucg_color_t rgb[4];			/* start and end color for L90SE , two more colors for the gradient box */
  ucg_ccs_t ccs_line[3];		/* color component sliders used by L90SE */
};

#define UCG_FONT_HEIGHT_MODE_TEXT 0
#define UCG_FONT_HEIGHT_MODE_XTEXT 1
#define UCG_FONT_HEIGHT_MODE_ALL 2

struct _ucg_com_info_t
{
  uint16_t serial_clk_speed;	/* nano seconds cycle time */
  uint16_t parallel_clk_speed;	/* nano seconds cycle time */
};


struct _ucg_font_info_t
{
  /* offset 0 */
  uint8_t glyph_cnt;
  uint8_t bbx_mode;
  uint8_t bits_per_0;
  uint8_t bits_per_1;
  
  /* offset 4 */
  uint8_t bits_per_char_width;
  uint8_t bits_per_char_height;		
  uint8_t bits_per_char_x;
  uint8_t bits_per_char_y;
  uint8_t bits_per_delta_x;
  
  /* offset 9 */
  int8_t max_char_width;
  int8_t max_char_height; /* overall height, NOT ascent. Instead ascent = max_char_height + y_offset */
  int8_t x_offset;
  int8_t y_offset;
  
  /* offset 13 */
  int8_t  ascent_A;
  int8_t  descent_g;
  int8_t  ascent_para;
  int8_t  descent_para;
  
  /* offset 17 */
  uint16_t start_pos_upper_A;
  uint16_t start_pos_lower_a;  
};
typedef struct _ucg_font_info_t ucg_font_info_t;

struct _ucg_font_decode_t
{
  const uint8_t *decode_ptr;			/* pointer to the compressed data */
  
  ucg_int_t target_x;
  ucg_int_t target_y;
  
  int8_t x;						/* local coordinates, (0,0) is upper left */
  int8_t y;
  int8_t glyph_width;	
  int8_t glyph_height;

  uint8_t decode_bit_pos;			/* bitpos inside a byte of the compressed data */
  uint8_t is_transparent;
  uint8_t dir;				/* direction */
};
typedef struct _ucg_font_decode_t ucg_font_decode_t;



#ifdef USE_PIN_LIST
#define UCG_PIN_RST 0
#define UCG_PIN_CD 1
#define UCG_PIN_CS 2
#define UCG_PIN_SCL 3
#define UCG_PIN_WR 3
#define UCG_PIN_SDA 4

#define UCG_PIN_D0 5
#define UCG_PIN_D1 6
#define UCG_PIN_D2 7
#define UCG_PIN_D3 8
#define UCG_PIN_D4 9
#define UCG_PIN_D5 10
#define UCG_PIN_D6 11
#define UCG_PIN_D7 12

#define UCG_PIN_COUNT 13

#define UCG_PIN_VAL_NONE 255
#endif

struct _ucg_t
{
  unsigned is_power_up:1;
  /* the dimension of the display */
  ucg_wh_t dimension;
  /* display callback procedure to handle display specific code */
  //ucg_dev_fnptr display_cb;
  /* controller and device specific code, high level procedure will call this */
  ucg_dev_fnptr device_cb;
  /* name of the extension cb. will be called by device_cb if required */
  ucg_dev_fnptr ext_cb;
  /* if rotation is applied, than this cb is called after rotation */
  ucg_dev_fnptr rotate_chain_device_cb;
  ucg_wh_t rotate_dimension;

  /* if rotation is applied, than this cb is called by the scale device */
  ucg_dev_fnptr scale_chain_device_cb;
  
  /* communication interface */
  ucg_com_fnptr com_cb;
  
  /* offset, that is additionally added to UCG_VARX/UCG_VARY */
  /* seems to be required for the Nokia display */
  ucg_xy_t display_offset;
  
  /* data which is passed to the cb procedures */
  /* can be modified by the cb procedures (rotation, clipping, etc) */
  ucg_arg_t arg;
  /* current window to which all drawing is clipped */
  /* should be modified via UCG_MSG_SET_CLIP_BOX by a device callback. */
  /* by default this is done by ucg_dev_default_cb */
  ucg_box_t clip_box;
  

  /* information about the current font */
  const unsigned char *font;             /* current font for all text procedures */
  ucg_font_calc_vref_fnptr font_calc_vref;
  //ucg_font_mode_fnptr font_mode;		/* OBSOLETE?? UCG_FONT_MODE_TRANSPARENT, UCG_FONT_MODE_SOLID, UCG_FONT_MODE_NONE */

  ucg_font_decode_t font_decode;		/* new font decode structure */
  ucg_font_info_t font_info;			/* new font info structure */

  int8_t glyph_dx;			/* OBSOLETE */
  int8_t glyph_x;			/* OBSOLETE */
  int8_t glyph_y;			/* OBSOLETE */
  uint8_t glyph_width;		/* OBSOLETE */
  uint8_t glyph_height;		/* OBSOLETE */
  
  uint8_t font_height_mode;
  int8_t font_ref_ascent;
  int8_t font_ref_descent;

  /* only for Arduino/C++ Interface */
#ifdef USE_PIN_LIST
  uint8_t pin_list[UCG_PIN_COUNT];

#ifdef __AVR__
  volatile uint8_t *data_port[UCG_PIN_COUNT];
  uint8_t data_mask[UCG_PIN_COUNT];
#endif

#endif

  /* 
    Small amount of RAM for the com interface (com_cb).
    Might be unused on unix systems, where the com sub system is 
    not required, but should be usefull for all uC projects.
  */
  uint8_t com_initial_change_sent;	/* Bit 0: CD/A0 Line Status, Bit 1: CS Line Status, Bit 2: Reset Line Status */
  uint8_t com_status;		/* Bit 0: CD/A0 Line Status, Bit 1: CS Line Status, Bit 2: Reset Line Status,  Bit 3: 1 for power up */
  uint8_t com_cfg_cd;		/* Bit 0: Argument Level, Bit 1: Command Level */
  
  
};

#define ucg_GetWidth(ucg) ((ucg)->dimension.w)
#define ucg_GetHeight(ucg) ((ucg)->dimension.h)

#define UCG_MSG_DEV_POWER_UP	10
#define UCG_MSG_DEV_POWER_DOWN 11
#define UCG_MSG_SET_CLIP_BOX 12
#define UCG_MSG_GET_DIMENSION 15

/* draw pixel with color from idx 0 */
#define UCG_MSG_DRAW_PIXEL 20
#define UCG_MSG_DRAW_L90FX 21
/* draw  bit pattern, transparent and draw color (idx 0) color */
//#define UCG_MSG_DRAW_L90TC 22		/* can be commented, used by ucg_DrawTransparentBitmapLine */
#define UCG_MSG_DRAW_L90SE 23		/* this part of the extension */
//#define UCG_MSG_DRAW_L90RL 24	/* not yet implemented */
/* draw  bit pattern with foreground (idx 1) and background (idx 0) color */
//#define UCG_MSG_DRAW_L90BF 25	 /* can be commented, used by ucg_DrawBitmapLine */


#define UCG_COM_STATUS_MASK_POWER 8
#define UCG_COM_STATUS_MASK_RESET 4
#define UCG_COM_STATUS_MASK_CS 2
#define UCG_COM_STATUS_MASK_CD 1

/*
  arg:	0
  data:	ucg_com_info_t *
  return:	0 for error
  note: 
    - power up is the first command, which is sent
*/
#define UCG_COM_MSG_POWER_UP 10

/*
  note: power down my be followed only by power up command
*/
#define UCG_COM_MSG_POWER_DOWN 11

/*
  arg:	delay in microseconds  (0..4095) 
*/
#define UCG_COM_MSG_DELAY 12

/*
  ucg->com_status	contains previous status of reset line
  arg:			new logic level for reset line
*/
#define UCG_COM_MSG_CHANGE_RESET_LINE 13
/*
  ucg->com_status	contains previous status of cs line
  arg:	new logic level for cs line
*/
#define UCG_COM_MSG_CHANGE_CS_LINE 14

/*
  ucg->com_status	contains previous status of cd line
  arg:	new logic level for cd line
*/
#define UCG_COM_MSG_CHANGE_CD_LINE 15

/*
  ucg->com_status	current status of Reset, CS and CD line (ucg->com_status)
  arg:			byte for display
*/
#define UCG_COM_MSG_SEND_BYTE 16

/*
  ucg->com_status	current status of Reset, CS and CD line (ucg->com_status)
  arg:			how often to repeat the 2/3 byte sequence 	
  data:			pointer to two or three bytes
*/
#define UCG_COM_MSG_REPEAT_1_BYTE 17
#define UCG_COM_MSG_REPEAT_2_BYTES 18
#define UCG_COM_MSG_REPEAT_3_BYTES 19

/*
  ucg->com_status	current status of Reset, CS and CD line (ucg->com_status)
  arg:			length of string 	
  data:			string
*/
#define UCG_COM_MSG_SEND_STR 20

/*
  ucg->com_status	current status of Reset, CS and CD line (ucg->com_status)
  arg:			number of cd_info and data pairs (half value of total byte cnt) 	
  data:			uint8_t with CD and data information
	cd_info data cd_info data cd_info data cd_info data ... cd_info data cd_info data
	cd_info is the level, which is directly applied to the CD line. This means,
	information applied to UCG_CFG_CD is not relevant.
*/
#define UCG_COM_MSG_SEND_CD_DATA_SEQUENCE 21



/*================================================*/
/* interrupt safe code */
#define UCG_INTERRUPT_SAFE
#if defined(UCG_INTERRUPT_SAFE)
#  if defined(__AVR__)
extern uint8_t global_SREG_backup;	/* ucg_init.c */
#    define UCG_ATOMIC_START()		do { global_SREG_backup = SREG; cli(); } while(0)
#    define UCG_ATOMIC_END()			SREG = global_SREG_backup
#    define UCG_ATOMIC_OR(ptr, val) 	do { uint8_t tmpSREG = SREG; cli(); (*(ptr) |= (val)); SREG = tmpSREG; } while(0)
#    define UCG_ATOMIC_AND(ptr, val) 	do { uint8_t tmpSREG = SREG; cli(); (*(ptr) &= (val)); SREG = tmpSREG; } while(0)
#  else
#    define UCG_ATOMIC_OR(ptr, val) (*(ptr) |= (val))
#    define UCG_ATOMIC_AND(ptr, val) (*(ptr) &= (val))
#    define UCG_ATOMIC_START()
#    define UCG_ATOMIC_END()
#  endif /* __AVR__ */
#else
#  define UCG_ATOMIC_OR(ptr, val) (*(ptr) |= (val))
#  define UCG_ATOMIC_AND(ptr, val) (*(ptr) &= (val))
#  define UCG_ATOMIC_START()
#  define UCG_ATOMIC_END()
#endif /* UCG_INTERRUPT_SAFE */

/*================================================*/
/* ucg_dev_msg_api.c */
void ucg_PowerDown(ucg_t *ucg);
ucg_int_t ucg_PowerUp(ucg_t *ucg);
void ucg_SetClipBox(ucg_t *ucg, ucg_box_t *clip_box);
void ucg_SetClipRange(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t w, ucg_int_t h);
void ucg_SetMaxClipRange(ucg_t *ucg);
void ucg_GetDimension(ucg_t *ucg);
void ucg_DrawPixelWithArg(ucg_t *ucg);
void ucg_DrawL90FXWithArg(ucg_t *ucg);
void ucg_DrawL90TCWithArg(ucg_t *ucg);
void ucg_DrawL90BFWithArg(ucg_t *ucg);
void ucg_DrawL90SEWithArg(ucg_t *ucg);
/* void ucg_DrawL90RLWithArg(ucg_t *ucg); */

/*================================================*/
/* ucg_init.c */
ucg_int_t ucg_Init(ucg_t *ucg, ucg_dev_fnptr device_cb, ucg_dev_fnptr ext_cb, ucg_com_fnptr com_cb);


/*================================================*/
/* ucg_dev_sdl.c */
ucg_int_t ucg_sdl_dev_cb(ucg_t *ucg, ucg_int_t msg, void *data);

/*================================================*/
/* ucg_pixel.c */
void ucg_SetColor(ucg_t *ucg, uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void ucg_DrawPixel(ucg_t *ucg, ucg_int_t x, ucg_int_t y);

/*================================================*/
/* ucg_line.c */
void ucg_Draw90Line(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t len, ucg_int_t dir, ucg_int_t col_idx);
void ucg_DrawHLine(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t len);
void ucg_DrawVLine(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t len);
void ucg_DrawHRLine(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t len);
void ucg_DrawLine(ucg_t *ucg, ucg_int_t x1, ucg_int_t y1, ucg_int_t x2, ucg_int_t y2);
/* the following procedure is only available with the extended callback */
void ucg_DrawGradientLine(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t len, ucg_int_t dir);


/*================================================*/
/* ucg_box.c */
void ucg_DrawBox(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t w, ucg_int_t h);
void ucg_ClearScreen(ucg_t *ucg);
void ucg_DrawRBox(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t w, ucg_int_t h, ucg_int_t r);
void ucg_DrawGradientBox(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t w, ucg_int_t h);
void ucg_DrawFrame(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t w, ucg_int_t h);
void ucg_DrawRFrame(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t w, ucg_int_t h, ucg_int_t r);


/*================================================*/
/* ucg_circle.c */
#define UCG_DRAW_UPPER_RIGHT 0x01
#define UCG_DRAW_UPPER_LEFT  0x02
#define UCG_DRAW_LOWER_LEFT 0x04
#define UCG_DRAW_LOWER_RIGHT  0x08
#define UCG_DRAW_ALL (UCG_DRAW_UPPER_RIGHT|UCG_DRAW_UPPER_LEFT|UCG_DRAW_LOWER_RIGHT|UCG_DRAW_LOWER_LEFT)
void ucg_DrawDisc(ucg_t *ucg, ucg_int_t x0, ucg_int_t y0, ucg_int_t rad, uint8_t option);
void ucg_DrawCircle(ucg_t *ucg, ucg_int_t x0, ucg_int_t y0, ucg_int_t rad, uint8_t option);

/*================================================*/
/* ucg_bitmap.c */
void ucg_DrawTransparentBitmapLine(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t dir, ucg_int_t len, const unsigned char *bitmap);
void ucg_DrawBitmapLine(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t dir, ucg_int_t len, const unsigned char *bitmap);
/* void ucg_DrawRLBitmap(ucg_t *ucg, ucg_int_t x, ucg_int_t y, ucg_int_t dir, const unsigned char *rl_bitmap); */


/*================================================*/
/* ucg_rotate.c */
void ucg_UndoRotate(ucg_t *ucg);
void ucg_SetRotate90(ucg_t *ucg);
void ucg_SetRotate180(ucg_t *ucg);
void ucg_SetRotate270(ucg_t *ucg);

/*================================================*/
/* ucg_scale.c */
void ucg_UndoScale(ucg_t *ucg);
void ucg_SetScale2x2(ucg_t *ucg);


/*================================================*/
/* ucg_polygon.c */

typedef int16_t pg_word_t;

#define PG_NOINLINE UCG_NOINLINE

struct pg_point_struct
{
  pg_word_t x;
  pg_word_t y;
};

typedef struct _pg_struct pg_struct;	/* forward declaration */

struct pg_edge_struct
{
  pg_word_t x_direction;	/* 1, if x2 is greater than x1, -1 otherwise */
  pg_word_t height;
  pg_word_t current_x_offset;
  pg_word_t error_offset;
  
  /* --- line loop --- */
  pg_word_t current_y;
  pg_word_t max_y;
  pg_word_t current_x;
  pg_word_t error;

  /* --- outer loop --- */
  uint8_t (*next_idx_fn)(pg_struct *pg, uint8_t i);
  uint8_t curr_idx;
};

/* maximum number of points in the polygon */
/* can be redefined, but highest possible value is 254 */
#define PG_MAX_POINTS 4

/* index numbers for the pge structures below */
#define PG_LEFT 0
#define PG_RIGHT 1


struct _pg_struct
{
  struct pg_point_struct list[PG_MAX_POINTS];
  uint8_t cnt;
  uint8_t is_min_y_not_flat;
  pg_word_t total_scan_line_cnt;
  struct pg_edge_struct pge[2];	/* left and right line draw structures */
};

void pg_ClearPolygonXY(pg_struct *pg);
void pg_AddPolygonXY(pg_struct *pg, ucg_t *ucg, int16_t x, int16_t y);
void pg_DrawPolygon(pg_struct *pg, ucg_t *ucg);
void ucg_ClearPolygonXY(void);
void ucg_AddPolygonXY(ucg_t *ucg, int16_t x, int16_t y);
void ucg_DrawPolygon(ucg_t *ucg);
void ucg_DrawTriangle(ucg_t *ucg, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
/* the polygon procedure only works for convex tetragons (http://en.wikipedia.org/wiki/Convex_polygon) */
void ucg_DrawTetragon(ucg_t *ucg, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);


/*================================================*/
/* ucg_font.c */

//ucg_int_t ucg_draw_transparent_glyph(ucg_t *ucg, ucg_int_t x, ucg_int_t y, uint8_t dir, uint8_t encoding);
//ucg_int_t ucg_draw_solid_glyph(ucg_t *ucg, ucg_int_t x, ucg_int_t y, uint8_t dir, uint8_t encoding);

// old font procedures
//#define UCG_FONT_MODE_TRANSPARENT ucg_draw_transparent_glyph
//#define UCG_FONT_MODE_SOLID ucg_draw_solid_glyph
//#define UCG_FONT_MODE_NONE ((ucg_font_mode_fnptr)0)

// new font procedures
#define UCG_FONT_MODE_TRANSPARENT 1
#define UCG_FONT_MODE_SOLID 0
#define UCG_FONT_MODE_NONE 1


/* Information on a specific given font */
uint8_t ucg_font_GetFontStartEncoding(const void *font);
uint8_t ucg_font_GetFontEndEncoding(const void *font);

uint8_t ucg_font_GetCapitalAHeight(const void *font);

int8_t ucg_font_GetFontAscent(const void *font);
int8_t ucg_font_GetFontDescent(const void *font);

int8_t ucg_font_GetFontXAscent(const void *font);
int8_t ucg_font_GetFontXDescent(const void *font);

size_t ucg_font_GetSize(const void *font);

/* Information on the current font */

uint8_t ucg_GetFontBBXWidth(ucg_t *ucg);
uint8_t ucg_GetFontBBXHeight(ucg_t *ucg);
uint8_t ucg_GetFontCapitalAHeight(ucg_t *ucg) UCG_NOINLINE; 
uint8_t ucg_IsGlyph(ucg_t *ucg, uint8_t requested_encoding);
int8_t ucg_GetGlyphWidth(ucg_t *ucg, uint8_t requested_encoding);

#define ucg_GetFontAscent(ucg)	((ucg)->font_ref_ascent)
#define ucg_GetFontDescent(ucg)	((ucg)->font_ref_descent)

/* Drawing procedures */

ucg_int_t ucg_DrawGlyph(ucg_t *ucg, ucg_int_t x, ucg_int_t y, uint8_t dir, uint8_t encoding);
ucg_int_t ucg_DrawString(ucg_t *ucg, ucg_int_t x, ucg_int_t y, uint8_t dir, const char *str);

/* Mode selection/Font assignment */

void ucg_SetFontRefHeightText(ucg_t *ucg);
void ucg_SetFontRefHeightExtendedText(ucg_t *ucg);
void ucg_SetFontRefHeightAll(ucg_t *ucg);

void ucg_SetFontPosBaseline(ucg_t *ucg) UCG_NOINLINE;
void ucg_SetFontPosBottom(ucg_t *ucg);
void ucg_SetFontPosTop(ucg_t *ucg);
void ucg_SetFontPosCenter(ucg_t *ucg);

void ucg_SetFont(ucg_t *ucg, const ucg_fntpgm_uint8_t  *font);
//void ucg_SetFontMode(ucg_t *ucg, ucg_font_mode_fnptr font_mode);
void ucg_SetFontMode(ucg_t *ucg, uint8_t is_transparent);

ucg_int_t ucg_GetStrWidth(ucg_t *ucg, const char *s);


/*================================================*/
/* LOW LEVEL PROCEDRUES, ONLY CALLED BY DEV CB */

/*================================================*/
/* ucg_clip.c */
ucg_int_t ucg_clip_is_pixel_visible(ucg_t *ucg);
ucg_int_t ucg_clip_l90fx(ucg_t *ucg);
ucg_int_t ucg_clip_l90tc(ucg_t *ucg);
ucg_int_t ucg_clip_l90se(ucg_t *ucg);


/*================================================*/
/* ucg_ccs.c */
void ucg_ccs_init(ucg_ccs_t *ccs, uint8_t start, uint8_t end, ucg_int_t steps);
void ucg_ccs_step(ucg_ccs_t *ccs);
void ucg_ccs_seek(ucg_ccs_t *ccs, ucg_int_t pos);


/*================================================*/
/* ucg_dev_default_cb.c */
ucg_int_t ucg_dev_default_cb(ucg_t *ucg, ucg_int_t msg, void *data);
ucg_int_t ucg_handle_l90fx(ucg_t *ucg, ucg_dev_fnptr dev_cb);
ucg_int_t ucg_handle_l90tc(ucg_t *ucg, ucg_dev_fnptr dev_cb);
ucg_int_t ucg_handle_l90se(ucg_t *ucg, ucg_dev_fnptr dev_cb);
ucg_int_t ucg_handle_l90bf(ucg_t *ucg, ucg_dev_fnptr dev_cb);
void ucg_handle_l90rl(ucg_t *ucg, ucg_dev_fnptr dev_cb);


/*================================================*/
/* ucg_com_msg_api.c */

/* send command bytes and optional arguments */
#define UCG_C10(c0)				0x010, (c0)
#define UCG_C20(c0,c1)				0x020, (c0),(c1)
#define UCG_C11(c0,a0)				0x011, (c0),(a0)
#define UCG_C21(c0,c1,a0)			0x021, (c0),(c1),(a0)
#define UCG_C12(c0,a0,a1)			0x012, (c0),(a0),(a1)
#define UCG_C22(c0,c1,a0,a1)		0x022, (c0),(c1),(a0),(a1)
#define UCG_C13(c0,a0,a1,a2)		0x013, (c0),(a0),(a1),(a2)
#define UCG_C23(c0,c1,a0,a1,a2)		0x023, (c0),(c1),(a0),(a1),(a2)
#define UCG_C14(c0,a0,a1,a2,a3)		0x014, (c0),(a0),(a1),(a2),(a3)
#define UCG_C24(c0,c1,a0,a1,a2,a3)	0x024, (c0),(c1),(a0),(a1),(a2),(a3)
#define UCG_C15(c0,a0,a1,a2,a3,a4)	0x015, (c0),(a0),(a1),(a2),(a3),(a4)


#define UCG_C25(c0,c1,a0,a1,a2,a3,a4)	0x025, (c0),(c1),(a0),(a1),(a2),(a3),(a4)
#define UCG_C16(c0,a0,a1,a2,a3,a4,a5)	0x016, (c0),(a0),(a1),(a2),(a3),(a4),(a5)
#define UCG_C26(c0,c1,a0,a1,a2,a3,a4,a5)	0x026, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5)
#define UCG_C17(c0,a0,a1,a2,a3,a4,a5,a6)	0x017, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6)
#define UCG_C27(c0,c1,a0,a1,a2,a3,a4,a5,a6)	0x027, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6)
#define UCG_C18(c0,a0,a1,a2,a3,a4,a5,a6,a7)	0x018, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7)
#define UCG_C28(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7)	0x028, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7)
#define UCG_C19(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8)	0x019, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)
#define UCG_C29(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8)	0x029, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)
#define UCG_C1A(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9)
#define UCG_C2A(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9)
#define UCG_C1B(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa)
#define UCG_C2B(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa)
#define UCG_C1C(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab)
#define UCG_C2C(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab)
#define UCG_C1D(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac)
#define UCG_C2D(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac)
#define UCG_C1E(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac),(ad)
#define UCG_C2E(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac),(ad)
#define UCG_C1F(c0,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae)	0x01f, (c0),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac),(ad),(ae)
#define UCG_C2F(c0,c1,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae)	0x02f, (c0),(c1),(a0),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(aa),(ab),(ac),(ad),(ae)
 


/* send one or more argument bytes */
#define UCG_A1(d0)					0x061, (d0)
#define UCG_A2(d0,d1)					0x062, (d0),(d1)
#define UCG_A3(d0,d1,d2)				0x063, (d0),(d1),(d2)
#define UCG_A4(d0,d1,d2,d3)				0x064, (d0),(d1),(d2),(d3)
#define UCG_A5(d0,d1,d2,d3,d4)			0x065, (d0),(d1),(d2),(d3),(d4)
#define UCG_A6(d0,d1,d2,d3,d4,d5)		0x066, (d0),(d1),(d2),(d3),(d4),(d5)
#define UCG_A7(d0,d1,d2,d3,d4,d5,d6)		0x067, (d0),(d1),(d2),(d3),(d4),(d5),(d6)
#define UCG_A8(d0,d1,d2,d3,d4,d5,d6,d7)	0x068, (d0),(d1),(d2),(d3),(d4),(d5),(d6),(d7)

/* force data mode on CD line */
#define UCG_DATA()					0x070
/* send one or more data bytes */
#define UCG_D1(d0)				0x071, (d0)
#define UCG_D2(d0,d1)				0x072, (d0),(d1)
#define UCG_D3(d0,d1,d2)			0x073, (d0),(d1),(d2)
#define UCG_D4(d0,d1,d2,d3)			0x074, (d0),(d1),(d2),(d3)
#define UCG_D5(d0,d1,d2,d3,d4)		0x075, (d0),(d1),(d2),(d3),(d4)
#define UCG_D6(d0,d1,d2,d3,d4,d5)	0x076, (d0),(d1),(d2),(d3),(d4),(d5)

/* delay by specified value. t = [0..4095] */
#define UCG_DLY_MS(t)				0x080 | (((t)>>8)&15), (t)&255
#define UCG_DLY_US(t)				0x090 | (((t)>>8)&15), (t)&255

/* access procedures to ucg->arg.pixel.pos.x und ucg->arg.pixel.pos.y */
#define UCG_VARX(s,a,o)				0x0a0 | ((s)&15), (a), (o)
#define UCG_VARY(s,a,o)				0x0b0 | ((s)&15), (a), (o)

/* force specific level on RST und CS */
#define UCG_RST(level)				0x0f0 | ((level)&1)
#define UCG_CS(level)				0x0f4 | ((level)&1)

/* Configure CD line for command, arguments and data */
/* Configure CMD/DATA line: "c" logic level CMD, "a" logic level CMD Args */
#define UCG_CFG_CD(c,a)			0x0fc | (((c)&1)<<1) | ((a)&1)

/* Termination byte */
#define UCG_END()					0x00

/*
#define ucg_com_SendByte(ucg, byte) \
  (ucg)->com_cb((ucg), UCG_COM_MSG_SEND_BYTE, (byte), NULL)
*/

#define ucg_com_SendRepeat3Bytes(ucg, cnt, byte_ptr) \
  (ucg)->com_cb((ucg), UCG_COM_MSG_REPEAT_3_BYTES, (cnt), (byte_ptr))

void ucg_com_PowerDown(ucg_t *ucg);
int16_t ucg_com_PowerUp(ucg_t *ucg, uint16_t serial_clk_speed, uint16_t parallel_clk_speed);  /* values are nano seconds */
void ucg_com_SetLineStatus(ucg_t *ucg, uint8_t level, uint8_t mask, uint8_t msg) UCG_NOINLINE;
void ucg_com_SetResetLineStatus(ucg_t *ucg, uint8_t level);
void ucg_com_SetCSLineStatus(ucg_t *ucg, uint8_t level);
void ucg_com_SetCDLineStatus(ucg_t *ucg, uint8_t level);
void ucg_com_DelayMicroseconds(ucg_t *ucg, uint16_t delay) UCG_NOINLINE;
void ucg_com_DelayMilliseconds(ucg_t *ucg, uint16_t delay) UCG_NOINLINE;
#ifndef ucg_com_SendByte
void ucg_com_SendByte(ucg_t *ucg, uint8_t byte);
#endif
void ucg_com_SendRepeatByte(ucg_t *ucg, uint16_t cnt, uint8_t byte);
void ucg_com_SendRepeat2Bytes(ucg_t *ucg, uint16_t cnt, uint8_t *byte_ptr);
#ifndef ucg_com_SendRepeat3Bytes
void ucg_com_SendRepeat3Bytes(ucg_t *ucg, uint16_t cnt, uint8_t *byte_ptr);
#endif
void ucg_com_SendString(ucg_t *ucg, uint16_t cnt, const uint8_t *byte_ptr);
void ucg_com_SendCmdDataSequence(ucg_t *ucg, uint16_t cnt, const uint8_t *byte_ptr, uint8_t cd_line_status_at_end);
void ucg_com_SendCmdSeq(ucg_t *ucg, const ucg_pgm_uint8_t *data);


/*================================================*/
/* ucg_dev_tga.c */
int tga_init(uint16_t w, uint16_t h);
void tga_save(const char *name);

ucg_int_t ucg_dev_tga(ucg_t *ucg, ucg_int_t msg, void *data);




/*================================================*/

extern const ucg_fntpgm_uint8_t ucg_font_ncenB12_tr[] UCG_FONT_SECTION("ucg_font_ncenB12_tr");
extern const ucg_fntpgm_uint8_t ucg_font_ncenB18_hf[] UCG_FONT_SECTION("ucg_font_ncenB18_hf");
extern const ucg_fntpgm_uint8_t ucg_font_ncenB18_hr[] UCG_FONT_SECTION("ucg_font_ncenB18_hr");
extern const ucg_fntpgm_uint8_t ucg_font_ncenB18_tf[] UCG_FONT_SECTION("ucg_font_ncenB18_tf");
extern const ucg_fntpgm_uint8_t ucg_font_ncenB18_tr[] UCG_FONT_SECTION("ucg_font_ncenB18_tr");
extern const ucg_fntpgm_uint8_t ucg_font_7x13B_tr[] UCG_FONT_SECTION("ucg_font_7x13B_tr");



#ifdef __cplusplus
}
#endif


#endif /* _UCG_H */
