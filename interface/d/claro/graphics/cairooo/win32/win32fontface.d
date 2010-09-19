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
module cairooo.win32.win32fontface;

private
{
    import cairo.cairo;
    import cairo.win32.cairo_win32;
    import cairooo.enums;
    import cairooo.exceptions;
    import cairooo.fontface;
    import cairooo.fontoptions;
    import cairooo.matrix;
    import cairooo.win32.win32scaledfont;
}

class Win32FontFace : FontFace
{
    this(cairo_font_face_t* handle, bool takeref)
    {
        super(handle, takeref);
    }

    this(LOGFONTW* logfont)
    {
        this(cairo_win32_font_face_create_for_logfontw(logfont), false);
    }

    this(Win32FontFace fontFace)
    {
        super(fontFace);
    }

    override
    Win32ScaledFont
    createScaledFont(Matrix fontMatrix, Matrix ctm, FontOptions options)
    {
        return new Win32ScaledFont(cairo_scaled_font_create(this.handle,
                    fontMatrix.toPtr, ctm.toPtr, options.handle), false);
    }

    override
    Win32FontFace
    dup()
    {
        return new Win32FontFace(this);
    }
}

