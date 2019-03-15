/**
  *@file mono_gfx.h
  *@brief module for monochromatic graphics
  *@author Jason Berger
  *@date 03/15/2019
  */
#pragma once

#include "Platforms/Common/mrt_platform.h"

struct mono_gfx_struct;
typedef mrt_status_t (*mono_gfx_write)(struct mono_gfx_struct* gfx, uint16_t x, uint16_t y, uint8_t* data, int len); //pointer to write function
typedef mrt_status_t (*mono_gfx_read)(struct mono_gfx_struct* gfx, uint16_t x, uint16_t y, uint8_t* data, int len); //pointer to write function

typedef struct{
	uint8_t* data;
	uint16_t width;
	uint16_t height;
}GFXBmp;

typedef struct { // Data stored PER GLYPH
	uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
	uint8_t  width, height;    // Bitmap dimensions in pixels
	uint8_t  xAdvance;         // Distance to advance cursor (x axis)
	int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
	uint8_t  *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	uint8_t   first, last; // ASCII extents
	uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;

typedef struct mono_gfx_struct{
  uint8_t* mBuffer;						 //buffer to store pixel data
  uint16_t mWidth;						 // width of buffer in pixels
  uint16_t mHeight;							//height of buffer in pixels
  uint32_t mBufferSize;					//size of buffer (in bytes)
	GFXfont* mFont;       				//font to use for printing
  mono_gfx_write fWritePixels; //pointer to write function
	void* mDevice;								//void pointer to device for unbuffered implementation
	bool mBuffered;
} mono_gfx_t;

/**
  *@brief initializes a mono_gfx_t that manages its own buffer
  *@param gfx ptr to mono_gfx_t to be initialized
	*@param width width (in pixels) of display buffer
  *@param height height (in pixels) of display buffer
  *@return status
  */
mrt_status_t mono_gfx_init_buffered(mono_gfx_t* gfx, uint16_t width, uint16_t height);

/**
  *@brief initializes a mono_gfx_t that does not manage its own buffer. This is used for large displays where storing the buffer locally doesnt make sense
  *@param gfx ptr to mono_gfx_t to be initialized
	*@param width width (in pixels) of display buffer
  *@param height height (in pixels) of display buffer
	*@param write_cb call back to function to write pixels
	*@param dev void ptr to device (used as )
  *@return status
  */
mrt_status_t mono_gfx_init_unbuffered(mono_gfx_t* gfx, uint16_t width, uint16_t height, mono_gfx_write write_cb, void* dev );

/**
  *@brief writes an array of bytes to the buffer
  *@param gfx ptr to mono_gfx_t descriptor
  *@param data ptr to black data being written
  *@param len number of bytes being written
  *@param wrap whether or not to wrap when we reach the end of current row
  *@return status of operation
  */
mrt_status_t mono_gfx_write_buffer(mono_gfx_t* gfx, uint16_t x, uint16_t y, uint8_t* data, int len, bool wrap);

/**
  *@brief Draws a bitmap to the buffer
  *@param gfx ptr to mono_gfx_t descriptor
  *@param x x coord to begin drawing at
  *@param y y coord to begin drawing at
  *@param bmp bitmap to draw
  *@return status of operation
  */
mrt_status_t mono_gfx_draw_bmp(mono_gfx_t* gfx, uint16_t x, uint16_t y, GFXBmp* bmp);

/**
  *@brief Draws rendered text to the buffer
  *@param gfx ptr to mono_gfx_t descriptor
  *@param x x coord to begin drawing at
  *@param y y coord to begin drawing at
  *@param text text to be written
  *@return status of operation
  */
mrt_status_t mono_gfx_print(mono_gfx_t* gfx, uint16_t x, uint16_t y, const char * text);

/**
  *@brief fill buffer with value
  *@param gfx ptr to gfxice
  *@param val value to write
  *@return status of operation
  */
mrt_status_t mono_gfx_fill(mono_gfx_t* gfx, uint8_t val);