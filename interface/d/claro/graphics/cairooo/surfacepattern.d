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
module claro.graphics.cairooo.surfacepattern;

private
{
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
    import claro.graphics.cairooo.pattern;
    import claro.graphics.cairooo.surface;
}

class SurfacePattern : Pattern
{
    this(cairo_pattern_t* handle, bool takeref)
    {
        super(handle, takeref);
    }
    
    this(SurfacePattern surfacePattern)
    {
        super(surfacePattern);
    }

    //
    // cairo api members
    //
    static
    SurfacePattern
    create(Surface surface)
    {
        return new SurfacePattern(
                cairo_pattern_create_for_surface(surface.handle), false);
    }
    
    Extend
    extend()
    {
        scope(success) checkStatus();
        return cast(Extend) cairo_pattern_get_extend(this.handle);
    }

    Extend
    extend(Extend value)
    {
        scope(success) checkStatus();
        cairo_pattern_set_extend(this.handle, cast(cairo_extend_t) value);
        return value;
    }

    Filter
    filter()
    {
        scope(success) checkStatus();
        return cast(Filter) cairo_pattern_get_filter(this.handle);
    }

    Filter
    filter(Filter value)
    {
        scope(success) checkStatus();
        cairo_pattern_set_filter(this.handle, cast(cairo_filter_t) value);
        return value;
    }

    //
    // miscellaneous members
    //
    SurfacePattern
    dup()
    {
        return new SurfacePattern(this);
    }
}

