#include "screen.h"

typedef struct _font_char_info{
   uint8_t charWidth;
   uint8_t charHeight;
   uint16_t offset;
}FONT_CHAR_INFO;

typedef struct _font_info{
   char startCharacter;
   char endCharacter;
   const FONT_CHAR_INFO *descriptors;
   const uint8_t *bitmaps;
}FONT_INFO;

// Font data
extern const uint8_t font[];
extern const FONT_INFO fontInfo;
extern const FONT_CHAR_INFO fontDescriptors[];

