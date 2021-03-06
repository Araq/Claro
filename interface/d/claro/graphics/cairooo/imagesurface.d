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
module claro.graphics.cairooo.imagesurface;

private
{
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
    import claro.graphics.cairooo.surface;
}

private
Surface
rewrapImageSurface(cairo_surface_t* handle)
{
    return new ImageSurface(handle, true);
}

class ImageSurface : Surface
{
    this(cairo_surface_t* handle, bool takeref)
    {
        setWrapFunction(&rewrapImageSurface, handle);
        super(handle, takeref);
    }

    this(Format format, int width, int height)
    {
        this(cairo_image_surface_create(cast(cairo_format_t) format, width,
                    height), false);
    }

    this(ImageSurface surface)
    {
        super(surface);
    }

    // NOTE: Supoprt for cairo_image_surface_create_for_data is implemented
    // via the ImageBufferSurface class.

    //
    // cairo api members
    //
    int
    width()
    {
        scope(success) checkStatus();
        return cairo_image_surface_get_width(this.handle);
    }

    int
    height()
    {
        scope(success) checkStatus();
        return cairo_image_surface_get_height(this.handle);
    }

    //
    // miscellaneous members
    //
    override
    ImageSurface
    dup()
    {
        return new ImageSurface(this);
    }
}

