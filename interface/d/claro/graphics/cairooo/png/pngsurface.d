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
module cairooo.png.pngsurface;

private
{
    import std.stream;
    import std.string;
    import cairo.cairo;
    import cairo.png.cairo_png;
    import cairooo.callbacks;
    import cairooo.enums;
    import cairooo.exceptions;
    import cairooo.imagesurface;
    import cairooo.surface;
}

private
Surface
rewrapPNGSurface(cairo_surface_t* handle)
{
    return new PNGSurface(handle, true);
}

class PNGSurface : ImageSurface
{
    alias void delegate(ubyte[] data) ReadDelegate;
    
    this(cairo_surface_t* handle, bool takeref)
    {
        setWrapFunction(&rewrapPNGSurface, handle);
        super(handle, takeref);
    }

    this(PNGSurface surface)
    {
        super(surface);
    }

    this(InputStream inputStream)
    {
        void readDelegate(ubyte[] data)
        {
            inputStream.readExact(cast(void*) cast(ubyte*) data, data.length);
        }
        this(&readDelegate);
    }

    this(ReadDelegate readDelegate)
    {
        cairo_surface_t* handle = null;
        handle = cairo_image_surface_create_from_png_stream(
                cast(cairo_read_func_t) &readFunctionWrapper,
                cast(void*) &readDelegate);

        // Annoyingly, we get only one return value in the case of any error
        // whatsoever.  Grr.
        if( handle == null )
            // TODO: Replace with custom exception, or is it just not worth
            // it?
            throw new Exception("Failed to read PNG image.");

        // Ok, construct the actual object
        this(handle, false);
    }

    this(char[] filename)
    {
        super(cairo_image_surface_create_from_png(
                    toStringz(filename)), false);
    }

    //
    // cairo api members
    //
    void
    writeToPNG(char[] filename)
    {
        //.checkStatus(cairo_surface_write_to_png(this.handle, filename));
        PNGSurface.writeToPNG(this, filename);
    }

    void
    writeToPNG(OutputStream outputStream)
    {
        PNGSurface.writeToPNG(outputStream);
    }

    void
    writeToPNG(WriteDelegate writeDelegate)
    {
        PNGSurface.writeToPNG(writeDelegate);
    }

    static
    void
    writeToPNG(Surface surface, char[] filename)
    {
        .checkStatus(cairo_surface_write_to_png(surface.handle,
                    toStringz(filename)));
    }

    static
    void
    writeToPNG(Surface surface, OutputStream outputStream)
    {
        void writeDelegate(ubyte[] data)
        {
            outputStream.writeExact(cast(void*) cast(ubyte*) data,
                    data.length);
        }
        
        PNGSurface.writeToPNG(surface, &writeDelegate);
    }

    static
    void
    writeToPNG(Surface surface, WriteDelegate writeDelegate)
    {
        .checkStatus(cairo_surface_write_to_png_stream(
                surface.handle,
                cast(cairo_write_func_t) &writeFunctionWrapper,
                cast(void*) &writeDelegate));
    }

    //
    // miscellaneous members
    //
    override
    PNGSurface
    dup()
    {
        return new PNGSurface(this);
    }
}

