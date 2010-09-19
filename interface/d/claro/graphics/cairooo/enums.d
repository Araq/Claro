/**
    This module contains various enumerations used throughout the cairooo
    library.
    
Authors: Daniel Keep
Copyright: 2006, Daniel Keep
License: BSD v2 (http://opensource.org/licenses/bsd-license.php).
**/
/**
    Copyright © 2006 Daniel Keep
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:
    
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
      
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    
    * Neither the name of this software, nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
module claro.graphics.cairooo.enums;

private
{
    import claro.graphics.cairo.cairo;
}

///
enum Antialias : int
{
    ///
    Default = cairo_antialias_t.CAIRO_ANTIALIAS_DEFAULT,
    ///
    None = cairo_antialias_t.CAIRO_ANTIALIAS_NONE,
    ///
    Gray = cairo_antialias_t.CAIRO_ANTIALIAS_GRAY,
    ///
    Subpixel = cairo_antialias_t.CAIRO_ANTIALIAS_SUBPIXEL
}

///
enum FillRule : int
{
    ///
    Winding = cairo_fill_rule_t.CAIRO_FILL_RULE_WINDING,
    ///
    EvenOdd = cairo_fill_rule_t.CAIRO_FILL_RULE_EVEN_ODD
}

///
enum LineCap : int
{
    ///
    Butt = cairo_line_cap_t.CAIRO_LINE_CAP_BUTT,
    ///
    Round = cairo_line_cap_t.CAIRO_LINE_CAP_ROUND,
    ///
    Square = cairo_line_cap_t.CAIRO_LINE_CAP_SQUARE
}

///
enum LineJoin : int
{
    ///
    Miter = cairo_line_join_t.CAIRO_LINE_JOIN_MITER,
    ///
    Round = cairo_line_join_t.CAIRO_LINE_JOIN_ROUND,
    ///
    Bevel = cairo_line_join_t.CAIRO_LINE_JOIN_BEVEL
}

///
enum Operator : int
{
    ///
    Clear = cairo_operator_t.CAIRO_OPERATOR_CLEAR,

    ///
    Source = cairo_operator_t.CAIRO_OPERATOR_SOURCE,
    ///
    Over = cairo_operator_t.CAIRO_OPERATOR_OVER,
    ///
    In = cairo_operator_t.CAIRO_OPERATOR_IN,
    ///
    Out = cairo_operator_t.CAIRO_OPERATOR_OUT,
    ///
    Atop = cairo_operator_t.CAIRO_OPERATOR_ATOP,

    ///
    Dest = cairo_operator_t.CAIRO_OPERATOR_DEST,
    ///
    DestOver = cairo_operator_t.CAIRO_OPERATOR_DEST_OVER,
    ///
    DestIn = cairo_operator_t.CAIRO_OPERATOR_DEST_IN,
    ///
    DestOut = cairo_operator_t.CAIRO_OPERATOR_DEST_OUT,
    ///
    DestAtop = cairo_operator_t.CAIRO_OPERATOR_DEST_ATOP,

    ///
    Xor = cairo_operator_t.CAIRO_OPERATOR_XOR,
    ///
    Add = cairo_operator_t.CAIRO_OPERATOR_ADD,
    ///
    Saturate = cairo_operator_t.CAIRO_OPERATOR_SATURATE
}

///
enum SubpixelOrder : int
{
    ///
    Default = cairo_subpixel_order_t.CAIRO_SUBPIXEL_ORDER_DEFAULT,
    ///
    RGB = cairo_subpixel_order_t.CAIRO_SUBPIXEL_ORDER_RGB,
    ///
    BGR = cairo_subpixel_order_t.CAIRO_SUBPIXEL_ORDER_BGR,
    ///
    VRGB = cairo_subpixel_order_t.CAIRO_SUBPIXEL_ORDER_VRGB,
    ///
    VBGR = cairo_subpixel_order_t.CAIRO_SUBPIXEL_ORDER_VBGR
}

///
enum HintStyle : int
{
    ///
    Default = cairo_hint_style_t.CAIRO_HINT_STYLE_DEFAULT,
    ///
    None = cairo_hint_style_t.CAIRO_HINT_STYLE_NONE,
    ///
    Slight = cairo_hint_style_t.CAIRO_HINT_STYLE_SLIGHT,
    ///
    Medium = cairo_hint_style_t.CAIRO_HINT_STYLE_MEDIUM,
    ///
    Full = cairo_hint_style_t.CAIRO_HINT_STYLE_FULL
}

///
enum HintMetrics : int
{
    ///
    Default = cairo_hint_metrics_t.CAIRO_HINT_METRICS_DEFAULT,
    ///
    Off = cairo_hint_metrics_t.CAIRO_HINT_METRICS_OFF,
    ///
    On = cairo_hint_metrics_t.CAIRO_HINT_METRICS_ON
}

///
enum Extend : int
{
    ///
    None = cairo_extend_t.CAIRO_EXTEND_NONE,
    ///
    Repeat = cairo_extend_t.CAIRO_EXTEND_REPEAT,
    ///
    Reflect = cairo_extend_t.CAIRO_EXTEND_REFLECT
}

///
enum Filter : int
{
    ///
    Fast = cairo_filter_t.CAIRO_FILTER_FAST,
    ///
    Good = cairo_filter_t.CAIRO_FILTER_GOOD,
    ///
    Best = cairo_filter_t.CAIRO_FILTER_BEST,
    ///
    Nearest = cairo_filter_t.CAIRO_FILTER_NEAREST,
    ///
    Bilinear = cairo_filter_t.CAIRO_FILTER_BILINEAR,
    ///
    Gaussian = cairo_filter_t.CAIRO_FILTER_GAUSSIAN
}

///
enum FontSlant : int
{
    ///
    Normal = cairo_font_slant_t.CAIRO_FONT_SLANT_NORMAL,
    ///
    Italic = cairo_font_slant_t.CAIRO_FONT_SLANT_ITALIC,
    ///
    Oblique = cairo_font_slant_t.CAIRO_FONT_SLANT_OBLIQUE
}

///
enum FontWeight : int
{
    ///
    Normal = cairo_font_weight_t.CAIRO_FONT_WEIGHT_NORMAL,
    ///
    Bold = cairo_font_weight_t.CAIRO_FONT_WEIGHT_BOLD
}

///
enum Format : int
{
    ///
    ARGB32 = cairo_format_t.CAIRO_FORMAT_ARGB32,
    ///
    RGB24 = cairo_format_t.CAIRO_FORMAT_RGB24,
    ///
    A8 = cairo_format_t.CAIRO_FORMAT_A8,
    ///
    A1 = cairo_format_t.CAIRO_FORMAT_A1
}

///
enum Content : int
{
    ///
    Color = cairo_content_t.CAIRO_CONTENT_COLOR,
    ///
    Alpha = cairo_content_t.CAIRO_CONTENT_ALPHA,
    ///
    ColorAlpha = cairo_content_t.CAIRO_CONTENT_COLOR_ALPHA
}

///
enum PathDataType : int
{
    ///
    MoveTo = cairo_path_data_type_t.CAIRO_PATH_MOVE_TO,
    ///
    LineTo = cairo_path_data_type_t.CAIRO_PATH_LINE_TO,
    ///
    CurveTo = cairo_path_data_type_t.CAIRO_PATH_CURVE_TO,
    ///
    ClosePath = cairo_path_data_type_t.CAIRO_PATH_CLOSE_PATH
}

