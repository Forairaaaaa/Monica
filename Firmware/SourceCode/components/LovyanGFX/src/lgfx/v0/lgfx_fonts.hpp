#ifndef LGFX_FONTS_HPP_
#define LGFX_FONTS_HPP_

#include <stdint.h>

extern "C" {
  #pragma GCC diagnostic ignored "-Wshadow"
  #pragma GCC diagnostic ignored "-Wdouble-promotion"
}

namespace lgfx
{
 inline namespace v0
 {
  struct LGFXBase;
  struct TextStyle;

  struct FontMetrics
  {
    int16_t width;
    int16_t x_advance;
    int16_t x_offset;
    int16_t height;
    int16_t y_advance;
    int16_t y_offset;
    int16_t baseline;
  };

  struct IFont
  {
    enum font_type_t
    { ft_unknown
    , ft_glcd
    , ft_bmp
    , ft_rle
    , ft_gfx
    , ft_bdf
    , ft_vlw
    , ft_u8g2
    };

    virtual font_type_t getType(void) const { return font_type_t::ft_unknown; }
    virtual void getDefaultMetric(FontMetrics *metrics) const = 0;
    virtual bool updateFontMetric(FontMetrics *metrics, uint16_t uniCode) const = 0;
    virtual bool unloadFont(void) { return false; }
    virtual size_t drawChar(LGFXBase* gfx, int32_t x, int32_t y, uint16_t c, const TextStyle* style) const = 0;

  protected:
    size_t drawCharDummy(LGFXBase* gfx, int32_t x, int32_t y, int32_t w, int32_t h, const TextStyle* style) const;
  };

  struct BaseFont : public IFont {
    union
    {
      const void *void_chartbl;
      const uint8_t *chartbl;
    };
    const uint8_t *widthtbl;
    const uint8_t width;
    const uint8_t height;
    const uint8_t baseline;
    BaseFont() = default;
    constexpr BaseFont(const void *chartbl, const uint8_t *widthtbl, uint8_t width, uint8_t height, uint8_t baseline)
     : void_chartbl(chartbl  )
     , widthtbl (widthtbl )
     , width    (width    )
     , height   (height   )
     , baseline (baseline )
    {}
    void getDefaultMetric(FontMetrics *metrics) const override;
  };

  struct GLCDfont : public BaseFont {
    constexpr GLCDfont(const void *chartbl, const uint8_t *widthtbl, uint8_t width, uint8_t height, uint8_t baseline) : BaseFont(chartbl, widthtbl, width, height, baseline ) {}
    font_type_t getType(void) const override { return ft_glcd; }

    bool updateFontMetric(FontMetrics *metrics, uint16_t uniCode) const override;
    size_t drawChar(LGFXBase* gfx, int32_t x, int32_t y, uint16_t c, const TextStyle* style) const override;
  };

  struct FixedBMPfont : public BaseFont {
    constexpr FixedBMPfont(const void *chartbl, const uint8_t *widthtbl, uint8_t width, uint8_t height, uint8_t baseline) : BaseFont(chartbl, widthtbl, width, height, baseline ) {}
    font_type_t getType(void) const override { return ft_bmp;  }

    bool updateFontMetric(FontMetrics *metrics, uint16_t uniCode) const override;
    size_t drawChar(LGFXBase* gfx, int32_t x, int32_t y, uint16_t c, const TextStyle* style) const override;
  };

  struct BMPfont : public BaseFont {
    constexpr BMPfont(const void *chartbl, const uint8_t *widthtbl, uint8_t width, uint8_t height, uint8_t baseline) : BaseFont(chartbl, widthtbl, width, height, baseline ) {}
    font_type_t getType(void) const override { return ft_bmp;  }

    bool updateFontMetric(FontMetrics *metrics, uint16_t uniCode) const override;
    size_t drawChar(LGFXBase* gfx, int32_t x, int32_t y, uint16_t c, const TextStyle* style) const override;
  };

  struct RLEfont : public BMPfont {
    constexpr RLEfont(const void *chartbl, const uint8_t *widthtbl, uint8_t width, uint8_t height, uint8_t baseline) : BMPfont(chartbl, widthtbl, width, height, baseline ) {}
    font_type_t getType(void) const override { return ft_rle; }
    size_t drawChar(LGFXBase* gfx, int32_t x, int32_t y, uint16_t c, const TextStyle* style) const override;
  };

  struct BDFfont : public BaseFont {
    const uint16_t *indextbl;
    uint16_t indexsize;
    uint8_t halfwidth;
    uint8_t y_advance;
    BDFfont() = default;
    constexpr BDFfont(const void *chartbl, const uint16_t *indextbl, uint16_t indexsize, uint8_t width, uint8_t halfwidth, uint8_t height, uint8_t baseline, uint8_t y_advance)
     : BaseFont(chartbl, nullptr, width, height, baseline )
     , indextbl(indextbl)
     , indexsize(indexsize)
     , halfwidth(halfwidth)
     , y_advance(y_advance)
     {}
    font_type_t getType(void) const override { return ft_bdf;  }

    void getDefaultMetric(FontMetrics *metrics) const override;
    bool updateFontMetric(FontMetrics *metrics, uint16_t uniCode) const override;
    size_t drawChar(LGFXBase* gfx, int32_t x, int32_t y, uint16_t c, const TextStyle* style) const override;
  };

  // deprecated array.
  extern const IFont* fontdata [];

//----------------------------------------------------------------------------
// Adafruit GFX font

  struct EncodeRange {
    uint16_t start;
    uint16_t end;
    uint16_t base;
  };

  struct GFXglyph { // Data stored PER GLYPH
    uint32_t bitmapOffset;     // Pointer into GFXfont->bitmap
    uint8_t  width, height;    // Bitmap dimensions in pixels
    uint8_t  xAdvance;         // Distance to advance cursor (x axis)
    int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
  };

  struct GFXfont : public lgfx::IFont
  { // Data stored for FONT AS A WHOLE:
    uint8_t  *bitmap;      // Glyph bitmaps, concatenated
    GFXglyph *glyph;            // Glyph array
    uint16_t  first, last; // ASCII extents
    uint8_t   yAdvance;    // Newline distance (y axis)

    uint16_t range_num;    // Number of EncodeRange
    EncodeRange *range;         // Array ofEncodeRange

    constexpr GFXfont ( uint8_t *bitmap
                      , GFXglyph *glyph
                      , uint16_t first
                      , uint16_t last
                      , uint8_t yAdvance
                      , uint16_t range_num = 0
                      , EncodeRange *range = nullptr
                      )
    : bitmap   (bitmap   )
    , glyph    (glyph    )
    , first    (first    )
    , last     (last     )
    , yAdvance (yAdvance )
    , range_num(range_num)
    , range    (range    )
    {}

    font_type_t getType(void) const override { return font_type_t::ft_gfx; }
    void getDefaultMetric(FontMetrics *metrics) const override;
    bool updateFontMetric(FontMetrics *metrics, uint16_t uniCode) const override;
    size_t drawChar(LGFXBase* gfx, int32_t x, int32_t y, uint16_t c, const TextStyle* style) const override;

  private:
    GFXglyph* getGlyph(uint16_t uniCode) const;
  };

//----------------------------------------------------------------------------
// u8g2 font

  struct U8g2font : public lgfx::IFont
  {
    constexpr U8g2font(const uint8_t *u8g2_font) : _font(u8g2_font) {}
    font_type_t getType(void) const override { return ft_u8g2; }

    uint8_t glyph_cnt (void) const { return _font[0]; }
    uint8_t bbx_mode  (void) const { return _font[1]; }
    uint8_t bits_per_0(void) const { return _font[2]; }
    uint8_t bits_per_1(void) const { return _font[3]; }
    uint8_t bits_per_char_width (void) const { return _font[4]; }
    uint8_t bits_per_char_height(void) const { return _font[5]; }
    uint8_t bits_per_char_x     (void) const { return _font[6]; }
    uint8_t bits_per_char_y     (void) const { return _font[7]; }
    uint8_t bits_per_delta_x    (void) const { return _font[8]; }
    int8_t max_char_width (void) const { return _font[ 9]; }
    int8_t max_char_height(void) const { return _font[10]; } /* overall height, NOT ascent. Instead ascent = max_char_height + y_offset */
    int8_t x_offset       (void) const { return _font[11]; }
    int8_t y_offset       (void) const { return _font[12]; }
    int8_t ascent_A    (void) const { return _font[13]; }
    int8_t descent_g   (void) const { return _font[14]; }  /* usually a negative value */
    int8_t ascent_para (void) const { return _font[15]; }
    int8_t descent_para(void) const { return _font[16]; }

    uint16_t start_pos_upper_A(void) const { return _font[17] << 8 | _font[18]; }
    uint16_t start_pos_lower_a(void) const { return _font[19] << 8 | _font[20]; }
    uint16_t start_pos_unicode(void) const { return _font[21] << 8 | _font[22]; }

    void getDefaultMetric(FontMetrics *metrics) const override;
    bool updateFontMetric(FontMetrics *metrics, uint16_t uniCode) const override;
    size_t drawChar(LGFXBase* gfx, int32_t x, int32_t y, uint16_t c, const TextStyle* style) const override;

  private:
    const uint8_t* getGlyph(uint16_t encoding) const;
    const uint8_t* _font;
  };

//----------------------------------------------------------------------------
// VLW font
  struct DataWrapper;

  struct RunTimeFont : public IFont {
    virtual ~RunTimeFont() {}
  };

  struct VLWfont : public RunTimeFont
  {
    uint16_t gCount;     // Total number of characters
    uint16_t yAdvance;   // Line advance
    uint16_t spaceWidth; // Width of a space character
    int16_t  ascent;     // Height of top of 'd' above baseline, other characters may be taller
    int16_t  descent;    // Offset to bottom of 'p', other characters may have a larger descent
    uint16_t maxAscent;  // Maximum ascent found in font
    uint16_t maxDescent; // Maximum descent found in font

    // These are for the metrics for each individual glyph (so we don't need to seek this in file and waste time)
    uint16_t* gUnicode  = nullptr;  //UTF-16 code, the codes are searched so do not need to be sequential
    uint8_t*  gWidth    = nullptr;  //cwidth
    uint8_t*  gxAdvance = nullptr;  //setWidth
    int8_t*   gdX       = nullptr;  //leftExtent
    uint32_t* gBitmap   = nullptr;  //file pointer to greyscale bitmap

    DataWrapper* _fontData = nullptr;
    bool _fontLoaded = false; // Flags when a anti-aliased font is loaded

    font_type_t getType(void) const override { return ft_vlw; }

    size_t drawChar(LGFXBase* gfx, int32_t x, int32_t y, uint16_t c, const TextStyle* style) const override;

    void getDefaultMetric(FontMetrics *metrics) const override;

    virtual ~VLWfont();

    bool unloadFont(void) override;

    bool updateFontMetric(FontMetrics *metrics, uint16_t uniCode) const override;

    bool getUnicodeIndex(uint16_t unicode, uint16_t *index) const;

    bool loadFont(DataWrapper* data);
  };

//----------------------------------------------------------------------------
 }
}
//----------------------------------------------------------------------------

namespace fonts
{
 inline namespace v0
 {
#ifdef __EFONT_FONT_DATA_H__
  static constexpr lgfx::BDFfont efont = { (const uint8_t *)efontFontData, efontFontList, sizeof(efontFontList)>>1, 16, 8, 16, 14, 16 };
#endif

  extern const lgfx::GLCDfont Font0;
  extern const lgfx::BMPfont  Font2;
  extern const lgfx::RLEfont  Font4;
  extern const lgfx::RLEfont  Font6;
  extern const lgfx::RLEfont  Font7;
  extern const lgfx::RLEfont  Font8;
  extern const lgfx::GLCDfont Font8x8C64;
  extern const lgfx::FixedBMPfont AsciiFont8x16;
  extern const lgfx::FixedBMPfont AsciiFont24x48;

  extern const lgfx::GFXfont TomThumb                 ;
  extern const lgfx::GFXfont FreeMono9pt7b            ;
  extern const lgfx::GFXfont FreeMono12pt7b           ;
  extern const lgfx::GFXfont FreeMono18pt7b           ;
  extern const lgfx::GFXfont FreeMono24pt7b           ;
  extern const lgfx::GFXfont FreeMonoBold9pt7b        ;
  extern const lgfx::GFXfont FreeMonoBold12pt7b       ;
  extern const lgfx::GFXfont FreeMonoBold18pt7b       ;
  extern const lgfx::GFXfont FreeMonoBold24pt7b       ;
  extern const lgfx::GFXfont FreeMonoOblique9pt7b     ;
  extern const lgfx::GFXfont FreeMonoOblique12pt7b    ;
  extern const lgfx::GFXfont FreeMonoOblique18pt7b    ;
  extern const lgfx::GFXfont FreeMonoOblique24pt7b    ;
  extern const lgfx::GFXfont FreeMonoBoldOblique9pt7b ;
  extern const lgfx::GFXfont FreeMonoBoldOblique12pt7b;
  extern const lgfx::GFXfont FreeMonoBoldOblique18pt7b;
  extern const lgfx::GFXfont FreeMonoBoldOblique24pt7b;
  extern const lgfx::GFXfont FreeSans9pt7b            ;
  extern const lgfx::GFXfont FreeSans12pt7b           ;
  extern const lgfx::GFXfont FreeSans18pt7b           ;
  extern const lgfx::GFXfont FreeSans24pt7b           ;
  extern const lgfx::GFXfont FreeSansBold9pt7b        ;
  extern const lgfx::GFXfont FreeSansBold12pt7b       ;
  extern const lgfx::GFXfont FreeSansBold18pt7b       ;
  extern const lgfx::GFXfont FreeSansBold24pt7b       ;
  extern const lgfx::GFXfont FreeSansOblique9pt7b     ;
  extern const lgfx::GFXfont FreeSansOblique12pt7b    ;
  extern const lgfx::GFXfont FreeSansOblique18pt7b    ;
  extern const lgfx::GFXfont FreeSansOblique24pt7b    ;
  extern const lgfx::GFXfont FreeSansBoldOblique9pt7b ;
  extern const lgfx::GFXfont FreeSansBoldOblique12pt7b;
  extern const lgfx::GFXfont FreeSansBoldOblique18pt7b;
  extern const lgfx::GFXfont FreeSansBoldOblique24pt7b;
  extern const lgfx::GFXfont FreeSerif9pt7b           ;
  extern const lgfx::GFXfont FreeSerif12pt7b          ;
  extern const lgfx::GFXfont FreeSerif18pt7b          ;
  extern const lgfx::GFXfont FreeSerif24pt7b          ;
  extern const lgfx::GFXfont FreeSerifItalic9pt7b     ;
  extern const lgfx::GFXfont FreeSerifItalic12pt7b    ;
  extern const lgfx::GFXfont FreeSerifItalic18pt7b    ;
  extern const lgfx::GFXfont FreeSerifItalic24pt7b    ;
  extern const lgfx::GFXfont FreeSerifBold9pt7b       ;
  extern const lgfx::GFXfont FreeSerifBold12pt7b      ;
  extern const lgfx::GFXfont FreeSerifBold18pt7b      ;
  extern const lgfx::GFXfont FreeSerifBold24pt7b      ;
  extern const lgfx::GFXfont FreeSerifBoldItalic9pt7b ;
  extern const lgfx::GFXfont FreeSerifBoldItalic12pt7b;
  extern const lgfx::GFXfont FreeSerifBoldItalic18pt7b;
  extern const lgfx::GFXfont FreeSerifBoldItalic24pt7b;

  extern const lgfx::GFXfont Orbitron_Light_24;
  extern const lgfx::GFXfont Orbitron_Light_32;
  extern const lgfx::GFXfont Roboto_Thin_24   ;
  extern const lgfx::GFXfont Satisfy_24       ;
  extern const lgfx::GFXfont Yellowtail_32    ;

  extern const lgfx::U8g2font lgfxJapanMincho_8  ;
  extern const lgfx::U8g2font lgfxJapanMincho_12 ;
  extern const lgfx::U8g2font lgfxJapanMincho_16 ;
  extern const lgfx::U8g2font lgfxJapanMincho_20 ;
  extern const lgfx::U8g2font lgfxJapanMincho_24 ;
  extern const lgfx::U8g2font lgfxJapanMincho_28 ;
  extern const lgfx::U8g2font lgfxJapanMincho_32 ;
  extern const lgfx::U8g2font lgfxJapanMincho_36 ;
  extern const lgfx::U8g2font lgfxJapanMincho_40 ;
  extern const lgfx::U8g2font lgfxJapanMinchoP_8 ;
  extern const lgfx::U8g2font lgfxJapanMinchoP_12;
  extern const lgfx::U8g2font lgfxJapanMinchoP_16;
  extern const lgfx::U8g2font lgfxJapanMinchoP_20;
  extern const lgfx::U8g2font lgfxJapanMinchoP_24;
  extern const lgfx::U8g2font lgfxJapanMinchoP_28;
  extern const lgfx::U8g2font lgfxJapanMinchoP_32;
  extern const lgfx::U8g2font lgfxJapanMinchoP_36;
  extern const lgfx::U8g2font lgfxJapanMinchoP_40;
  extern const lgfx::U8g2font lgfxJapanGothic_8  ;
  extern const lgfx::U8g2font lgfxJapanGothic_12 ;
  extern const lgfx::U8g2font lgfxJapanGothic_16 ;
  extern const lgfx::U8g2font lgfxJapanGothic_20 ;
  extern const lgfx::U8g2font lgfxJapanGothic_24 ;
  extern const lgfx::U8g2font lgfxJapanGothic_28 ;
  extern const lgfx::U8g2font lgfxJapanGothic_32 ;
  extern const lgfx::U8g2font lgfxJapanGothic_36 ;
  extern const lgfx::U8g2font lgfxJapanGothic_40 ;
  extern const lgfx::U8g2font lgfxJapanGothicP_8 ;
  extern const lgfx::U8g2font lgfxJapanGothicP_12;
  extern const lgfx::U8g2font lgfxJapanGothicP_16;
  extern const lgfx::U8g2font lgfxJapanGothicP_20;
  extern const lgfx::U8g2font lgfxJapanGothicP_24;
  extern const lgfx::U8g2font lgfxJapanGothicP_28;
  extern const lgfx::U8g2font lgfxJapanGothicP_32;
  extern const lgfx::U8g2font lgfxJapanGothicP_36;
  extern const lgfx::U8g2font lgfxJapanGothicP_40;

  extern const lgfx::U8g2font efontCN_10   ;
  extern const lgfx::U8g2font efontCN_10_b ;
  extern const lgfx::U8g2font efontCN_10_bi;
  extern const lgfx::U8g2font efontCN_10_i ;
  extern const lgfx::U8g2font efontCN_12   ;
  extern const lgfx::U8g2font efontCN_12_b ;
  extern const lgfx::U8g2font efontCN_12_bi;
  extern const lgfx::U8g2font efontCN_12_i ;
  extern const lgfx::U8g2font efontCN_14   ;
  extern const lgfx::U8g2font efontCN_14_b ;
  extern const lgfx::U8g2font efontCN_14_bi;
  extern const lgfx::U8g2font efontCN_14_i ;
  extern const lgfx::U8g2font efontCN_16   ;
  extern const lgfx::U8g2font efontCN_16_b ;
  extern const lgfx::U8g2font efontCN_16_bi;
  extern const lgfx::U8g2font efontCN_16_i ;
  extern const lgfx::U8g2font efontCN_24   ;
  extern const lgfx::U8g2font efontCN_24_b ;
  extern const lgfx::U8g2font efontCN_24_bi;
  extern const lgfx::U8g2font efontCN_24_i ;

  extern const lgfx::U8g2font efontJA_10   ;
  extern const lgfx::U8g2font efontJA_10_b ;
  extern const lgfx::U8g2font efontJA_10_bi;
  extern const lgfx::U8g2font efontJA_10_i ;
  extern const lgfx::U8g2font efontJA_12   ;
  extern const lgfx::U8g2font efontJA_12_b ;
  extern const lgfx::U8g2font efontJA_12_bi;
  extern const lgfx::U8g2font efontJA_12_i ;
  extern const lgfx::U8g2font efontJA_14   ;
  extern const lgfx::U8g2font efontJA_14_b ;
  extern const lgfx::U8g2font efontJA_14_bi;
  extern const lgfx::U8g2font efontJA_14_i ;
  extern const lgfx::U8g2font efontJA_16   ;
  extern const lgfx::U8g2font efontJA_16_b ;
  extern const lgfx::U8g2font efontJA_16_bi;
  extern const lgfx::U8g2font efontJA_16_i ;
  extern const lgfx::U8g2font efontJA_24   ;
  extern const lgfx::U8g2font efontJA_24_b ;
  extern const lgfx::U8g2font efontJA_24_bi;
  extern const lgfx::U8g2font efontJA_24_i ;

  extern const lgfx::U8g2font efontKR_10   ;
  extern const lgfx::U8g2font efontKR_10_b ;
  extern const lgfx::U8g2font efontKR_10_bi;
  extern const lgfx::U8g2font efontKR_10_i ;
  extern const lgfx::U8g2font efontKR_12   ;
  extern const lgfx::U8g2font efontKR_12_b ;
  extern const lgfx::U8g2font efontKR_12_bi;
  extern const lgfx::U8g2font efontKR_12_i ;
  extern const lgfx::U8g2font efontKR_14   ;
  extern const lgfx::U8g2font efontKR_14_b ;
  extern const lgfx::U8g2font efontKR_14_bi;
  extern const lgfx::U8g2font efontKR_14_i ;
  extern const lgfx::U8g2font efontKR_16   ;
  extern const lgfx::U8g2font efontKR_16_b ;
  extern const lgfx::U8g2font efontKR_16_bi;
  extern const lgfx::U8g2font efontKR_16_i ;
  extern const lgfx::U8g2font efontKR_24   ;
  extern const lgfx::U8g2font efontKR_24_b ;
  extern const lgfx::U8g2font efontKR_24_bi;
  extern const lgfx::U8g2font efontKR_24_i ;

  extern const lgfx::U8g2font efontTW_10   ;
  extern const lgfx::U8g2font efontTW_10_b ;
  extern const lgfx::U8g2font efontTW_10_bi;
  extern const lgfx::U8g2font efontTW_10_i ;
  extern const lgfx::U8g2font efontTW_12   ;
  extern const lgfx::U8g2font efontTW_12_b ;
  extern const lgfx::U8g2font efontTW_12_bi;
  extern const lgfx::U8g2font efontTW_12_i ;
  extern const lgfx::U8g2font efontTW_14   ;
  extern const lgfx::U8g2font efontTW_14_b ;
  extern const lgfx::U8g2font efontTW_14_bi;
  extern const lgfx::U8g2font efontTW_14_i ;
  extern const lgfx::U8g2font efontTW_16   ;
  extern const lgfx::U8g2font efontTW_16_b ;
  extern const lgfx::U8g2font efontTW_16_bi;
  extern const lgfx::U8g2font efontTW_16_i ;
  extern const lgfx::U8g2font efontTW_24   ;
  extern const lgfx::U8g2font efontTW_24_b ;
  extern const lgfx::U8g2font efontTW_24_bi;
  extern const lgfx::U8g2font efontTW_24_i ;
 }
}

using namespace fonts;

#ifndef _GFXFONT_H_
#define _GFXFONT_H_
using GFXfont = lgfx::GFXfont;
using GFXglyph = lgfx::GFXglyph;
#endif

extern "C" {
  #pragma GCC diagnostic pop
}

#endif
