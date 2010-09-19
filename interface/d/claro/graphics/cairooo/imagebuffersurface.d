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
module claro.graphics.cairooo.imagebuffersurface;

// TODO: Allow the this(cairo_surface_t*, bool) constructor to be used
// *INTERNALLY* iff the handle has keys on it for the data pointer, refcount
// and allocation flag.

private
{
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
    import claro.graphics.cairooo.imagesurface;
}

/+private
Surface
recastToImageBuffer(cairo_surface_t* handle)
{
    return new ImageBufferSurface(handle, true);
}+/

class ImageBufferSurface : ImageSurface
{
private:
    ubyte[] _data = null;
    uint* _refcount = null;
    bool selfAllocated = false;

    // Because we can't extract the data buffer from the surface handle, we
    // won't allow this constructor.
    // TODO: Allow this constructor :P
    /+this(cairo_surface_t* handle, bool takeref)
    {
        super(handle, takeref);
    }+/
    
public:
    this(ImageBufferSurface imageBufferSurface)
    out
    {
        assert( this._data != null );
        if( this.selfAllocated )
        {
            assert( this._refcount != null );
            assert( (*this._refcount) > 1 );
        }
    }
    body
    {
        super(imageBufferSurface);
        this._data = imageBufferSurface._data;
        this._refcount = imageBufferSurface._refcount;
        this.selfAllocated = imageBufferSurface.selfAllocated;

        // ONLY grab a reference if it is self-allocated
        if( this.selfAllocated )
            (*this._refcount) ++;
    }

    this(ubyte[] data, Format format, int width, int height, int stride)
    out
    {
        assert( this._data == data );
        assert( this.selfAllocated == false );
        assert( this._refcount == null );
    }
    body
    {
        // TODO: Check the size of the array to make sure it's large enough,
        // and check that stride is valid.
        this._data = data;
        this.selfAllocated = false;
        super(cairo_image_surface_create_for_data(data.ptr,
                    cast(cairo_format_t) format, width, height, stride),
                true);
    }

    this(Format format, int width, int height)
    out
    {
        assert( this._data != null );
    }
    body
    {
        int stride;
        switch(format)
        {
            case Format.ARGB32: stride = width * 4;     break;
            case Format.RGB24:  stride = width * 3;     break;
            case Format.A8:     stride = width;         break;
            case Format.A1:     stride = (width+31)/8;  break;
        }
        
        this(format, width, height, stride);
    }

    this(Format format, int width, int height, int stride)
    out
    {
        assert( this._data != null );
        assert( this.selfAllocated == true );
        assert( this._refcount != null );
        assert( (*this._refcount) == 1 );
    }
    body
    {
        // TODO: Check stride is valid
        this(new ubyte[stride*height], format, width, height, stride);
        this.selfAllocated = true;
        
        // We created this buffer, so we need to allocate the refcount
        this._refcount = new uint;
        (*this._refcount) = 1;
    }

    ~this()
    {
        // Deal with the case where we allocated the buffer ourselves.
        if( this.selfAllocated )
        {
            // Ok, drop a reference; if the refcount is then zero, destroy the
            // buffer.  If the user wants a copy, they'll just have to dup it.
            (*this._refcount) --;

            if( (*this._refcount) == 0 )
            {
                delete this._data;      this._data = null;
                delete this._refcount;  this._refcount = null;
            }
        }
    }

    //
    // miscellaneous members
    //
    ImageBufferSurface
    dup()
    {
        return new ImageBufferSurface(this);
    }

    ubyte[]
    data()
    {
        return this._data;
    }
}

