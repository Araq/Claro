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
module claro.graphics.cairooo.path;

// TODO: Add support for PathElement[]s.
// TODO: Should this be changed over to copy by value?  It looks like some
// people on the cairo ML think that's the way cairomm should go...

private
{
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
    import claro.graphics.cairooo.context;
    import claro.graphics.cairooo.glyph;
}

union PathData
{
    struct
    {
        PathDataType type;
        int length = 0;
    }
    struct
    {
        double x, y;
    }
}

/+struct PathElement
{
    PathDataType type;
    union
    {
        MoveToElement moveTo;
        LineToElement lineTo;
        CurveToElement curveTo;
        ClosePathElement closePath;
    }
}

struct MoveToElement
{
    double x, y;
}

struct LineToElement
{
    double x, y
}

struct CurveToElement
{
    double x1, y1, x2, y2, x3, y3;
}

struct ClosePathElement
{
}+/

class Path
{
private:
    cairo_path_t* _handle = null;
    uint* _refcount = null;
    bool _selfAllocated = false;

public:
    /**
     * Creates a new, empty path.
     */
    this()
    {
        this._handle = new cairo_path_t;
        this._refcount = new uint;
        this._selfAllocated = true;

        // Get initial reference
        (*this._refcount) = 1;
    }

    /**
        This constructor will wrap a raw cairo_path_t* handle in a Path
        instance.  However, user code should $(I not) use this method if you
        can help it.  Since this class maintains its own reference counting
        information, creating multiple wrappers around a single cairo_path_t*
        will cause the handle to be freed prematurely!
    **/
    this(cairo_path_t* handle)
    {
        // This constructor wraps an existing cairo_path_t* handle.  It simply
        // saves the handle, creates a new refcount (defaulting to 1), and
        // then sets selfAllocated to false.  This constructor will NOT accept
        // user-created handles that cairo does not own.  This is more for my
        // own sanity then any other reason at this point...
        this._handle = handle;
        this._refcount = new uint;
        this._selfAllocated = false;

        // Get initial reference
        (*this._refcount) = 1;
    }

    this(PathData[] data)
    {
        // This constructor runs the default constructor to give us an empty
        // cairo_path_t structure.  We then update its cairo_path_data_t
        // pointer, and element length.
        this();
        this.handle.data = cast(cairo_path_data_t*) data.dup;
        this.handle.num_data = data.length;
    }

    this(Path path)
    {
        // This constructor simply copies the pointer to the existing path
        // object's handle, copies the pointer to the reference counter, and
        // then adds a reference.
        this._handle = path._handle;
        this._refcount = path._refcount;
        this._selfAllocated = path._selfAllocated;

        // Increment references by one
        (*this._refcount) ++;
    }
    
    ~this()
    {
        // Drop a reference
        (*this._refcount) --;
        
        // If there are no more references, destroy the object
        if( (*this._refcount) == 0 )
        {
            if( selfAllocated )
            {
                // If we allocated this path ourselves, we need to explicitly
                // delete it.
                delete this._handle;
            }
            else
            {
                // Otherwise, let cairo deal with it.
                cairo_path_destroy(this._handle);
            }

            // Free the reference counter
            delete this._refcount;
        }
    }

    //
    // miscellaneous members
    //
    cairo_path_t*
    handle()
    {
        return this._handle;
    }
    
    Path
    dup()
    {
        return new Path(this);
    }
    
    bool
    selfAllocated()
    {
        return this._selfAllocated;
    }

    PathData[]
    data()
    {
        return (cast(PathData*) this.handle.data)[0..this.handle.num_data];
    }

    /+PathElement[]
    elements()
    {
        PathElement[] result;
        foreach( element ; this )
        {
            result.length = result.length + 1;
            result[$-1] = element;
        }
        return result;
    }+/

    /+int
    opApply(int delegate(inout PathElement) dg)
    {
        int result = 0;
        PathElement current;
        PathData[] data = this.data;

        for( int i=0; i<data.length; )
        {
            current.type = data[i].type;
            
            switch( current.type )
            {
                case PathDataType.MoveTo:
                    current.moveTo.x = data[i+1].x;
                    current.moveTo.y = data[i+1].y;
                    i += 2;
                    break;

                case PathDataType.LineTo:
                    current.lineTo.x = data[i+1].x;
                    current.lineTo.y = data[i+1].y;
                    i += 2;
                    break;

                case PathDataType.CurveTo:
                    current.curveTo.x1 = data[i+1].x;
                    current.curveTo.y1 = data[i+1].y;
                    current.curveTo.x2 = data[i+2].x;
                    current.curveTo.y2 = data[i+2].y;
                    current.curveTo.x3 = data[i+3].x;
                    current.curveTo.y3 = data[i+3].y;
                    i += 4;
                    break;

                case PathDataType.ClosePath:
                    i += 1;
                    break;
            }

            result = dg(current);
            if( result )
                break;
        }

        return result;
    }+/

protected:
    void
    checkStatus()
    {
        .checkStatus(this.handle.status);
    }

    void
    checkStatus(cairo_path_t* handle)
    {
        .checkStatus(handle.status);
    }
}

