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
module cairooo.win32.win32scaledfont;

private
{
    import cairo.cairo;
    import cairo.win32.cairo_win32;
    import cairooo.enums;
    import cairooo.exceptions;
    import cairooo.scaledfont;
}

class Win32ScaledFont : ScaledFont
{
    this(cairo_scaled_font_t* handle, bool takeref)
    {
        super(handle, takeref);
    }

    this(Win32ScaledFont scaledFont)
    {
        this(scaledFont.handle, false);
    }

    //
    // cairo win32 api members
    //
    void
    selectFont(HDC hdc)
    {
        scope(success) checkStatus();
        .checkStatus(cairo_win32_scaled_font_select_font(this.handle, hdc));
    }

    void
    doneFont()
    {
        scope(success) checkStatus();
        cairo_win32_scaled_font_done_font(this.handle);
    }

    double
    getMetricsFactor()
    {
        scope(success) checkStatus();
        return cairo_win32_scaled_font_get_metrics_factor(this.handle);
    }

    override
    Win32ScaledFont
    dup()
    {
        return new Win32ScaledFont(this);
    }
}

