/**
    XXX
    
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
module claro.graphics.cairooo.scaledfont;

private
{
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
    import claro.graphics.cairooo.fontextents;
    import claro.graphics.cairooo.fontface;
    import claro.graphics.cairooo.fontoptions;
    import claro.graphics.cairooo.glyph;
    import claro.graphics.cairooo.matrix;
    import claro.graphics.cairooo.textextents;
}

class ScaledFont
{
private:
    cairo_scaled_font_t* _handle;

public:
    this(cairo_scaled_font_t* handle, bool takeref)
    {
        checkStatus(handle);
        this._handle = handle;
        if( takeref )
        {
            cairo_scaled_font_reference(this.handle);
            checkStatus(this.handle);
        }
    }
    
    this(FontFace fontFace, Matrix fontMatrix, Matrix ctm,
            FontOptions options)
    {
        // We need to call the method on the font face itself so that backends
        // can specialize what kind of ScaledFont they create.
        auto ScaledFont temp = fontFace.createScaledFont(fontMatrix, ctm,
                options);
        this(temp);
    }

    this(ScaledFont scaledFont)
    {
        this(scaledFont.handle, true);
    }

    ~this()
    {
        cairo_scaled_font_destroy(this.handle);
    }

    //
    // cairo api members
    //
    FontExtents
    extents()
    {
        scope(success) checkStatus();
        FontExtents result;
        cairo_scaled_font_extents(this.handle,
                cast(cairo_font_extents_t*) &result);
        return result;
    }

    TextExtents
    glyphExtents(Glyph[] glyphs)
    {
        scope(success) checkStatus();
        TextExtents result;
        cairo_scaled_font_glyph_extents(this.handle,
                cast(cairo_glyph_t*) glyphs, glyphs.length,
                cast(cairo_text_extents_t*) &result);
        return result;
    }

    //
    // miscellaneous members
    //
    ScaledFont
    dup()
    {
        return new ScaledFont(this);
    }

    cairo_scaled_font_t*
    handle()
    {
        return this._handle;
    }

    //
    // internal stuff
    //
    void
    checkStatus()
    {
        .checkStatus(cairo_scaled_font_status(this.handle));
    }

    void
    checkStatus(cairo_scaled_font_t* handle)
    {
        .checkStatus(cairo_scaled_font_status(handle));
    }
}

