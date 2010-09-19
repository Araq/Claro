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
module claro.graphics.cairooo.fontoptions;

private
{
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
}

class FontOptions
{
private:
    cairo_font_options_t* _handle;

public:
    this()
    {
        scope(success) checkStatus();
        this._handle = cairo_font_options_create();
    }
    
    this(cairo_font_options_t* handle, bool takeref)
    {
        checkStatus(handle);
        if( takeref )
        {
            handle = cairo_font_options_copy(handle);
            checkStatus(handle);
        }
        this._handle = handle;
    }

    this(FontOptions fontOptions)
    {
        scope(success) checkStatus();
        this(fontOptions.handle, true);
    }

    ~this()
    {
        cairo_font_options_destroy(this.handle);
    }

    //
    // cairo api members
    //

    FontOptions
    copy()
    {
        return new FontOptions(this);
    }

    void
    merge(FontOptions other)
    {
        scope(success) checkStatus();
        cairo_font_options_merge(this.handle, other.handle);
    }

    bool
    equal(FontOptions other)
    {
        scope(success) checkStatus();
        return cast(bool) cairo_font_options_equal(this.handle, other.handle);
    }

    Antialias
    antialias()
    {
        scope(success) checkStatus();
        return cast(Antialias) cairo_font_options_get_antialias(this.handle);
    }

    Antialias
    antialias(Antialias value)
    {
        scope(success) checkStatus();
        cairo_font_options_set_antialias(this.handle,
                cast(cairo_antialias_t) value);
        return value;
    }

    SubpixelOrder
    subpixelOrder()
    {
        scope(success) checkStatus();
        return cast(SubpixelOrder)
            cairo_font_options_get_subpixel_order(this.handle);
    }

    SubpixelOrder
    subpixelOrder(SubpixelOrder value)
    {
        scope(success) checkStatus();
        cairo_font_options_set_subpixel_order(this.handle,
                cast(cairo_subpixel_order_t) value);
        return value;
    }

    HintStyle
    hintStyle()
    {
        scope(success) checkStatus();
        return cast(HintStyle)
            cairo_font_options_get_hint_style(this.handle);
    }

    HintStyle
    hintStyle(HintStyle value)
    {
        scope(success) checkStatus();
        cairo_font_options_set_hint_style(this.handle,
                cast(cairo_hint_style_t) value);
        return value;
    }

    HintMetrics
    hintMetrics()
    {
        scope(success) checkStatus();
        return cast(HintMetrics)
            cairo_font_options_get_hint_metrics(this.handle);
    }

    HintMetrics
    hintMetrics(HintMetrics value)
    {
        scope(success) checkStatus();
        cairo_font_options_set_hint_metrics(this.handle,
                cast(cairo_hint_metrics_t) value);
        return value;
    }

    //
    // miscellaneous members
    //

    FontOptions
    dup()
    {
        return this.copy();
    }

    cairo_font_options_t*
    handle()
    {
        return this._handle;
    }

    int
    opEquals(FontOptions other)
    {
        return cast(int) equal(other);
    }

    //
    // internal stuff
    //
protected:
    void
    checkStatus()
    {
        .checkStatus(cairo_font_options_status(this._handle));
    }

    void
    checkStatus(cairo_font_options_t* handle)
    {
        .checkStatus(cairo_font_options_status(handle));
    }
}

