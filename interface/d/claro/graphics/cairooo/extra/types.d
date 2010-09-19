/**
    This module contains several types for storing and working with geometric
    information such as 2D points, colours, etc.
    
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
module cairooo.extra.types;

private
{
    import std.math;
}

struct Point
{
    double x, y;

    Point
    opAdd(Point rhs)
    {
        Point result;
        result.x = this.x + rhs.x;
        result.y = this.y + rhs.y;
        return result;
    }

    Point
    opAddAssign(Point rhs)
    {
        this.x += rhs.x;
        this.y += rhs.y;
        return (*this);
    }

    Point
    opSub(Point rhs)
    {
        Point result;
        result.x = this.x - rhs.x;
        result.y = this.y - rhs.y;
        return result;
    }

    Point
    opSubAssign(Point rhs)
    {
        this.x -= rhs.x;
        this.y -= rhs.y;
        return (*this);
    }

    Rect
    opAdd(Size rhs)
    {
        Rect result;
        result.origin = *this;
        result.size = rhs;
        return result;
    }

    Rect
    opSub(Size rhs)
    {
        Rect result;
        result.origin.x = this.x - rhs.w;
        result.origin.y = this.y - rhs.h;
        result.size = rhs;
        return result;
    }

    Rect
    opSub_r(Size lhs)
    {
        Rect result;
        result.origin = -(*this);
        result.size = lhs;
        return result;
    }

    Point
    opMul(double rhs)
    {
        Point result;
        result.x = this.x * rhs;
        result.y = this.y * rhs;
        return result;
    }

    Point
    opMulAssign(double rhs)
    {
        this.x *= rhs;
        this.y *= rhs;
        return (*this);
    }

    Point
    opNeg()
    {
        Point result;
        result.x = -this.x;
        result.y = -this.y;
        return result;
    }

    Size
    toSize()
    {
        Size result;
        result.w = this.x;
        result.h = this.y;
        return result;
    }
}

struct Size
{
    double w, h;

    Size
    opAdd(Size rhs)
    {
        Size result;
        result.w = this.w + rhs.w;
        result.h = this.h + rhs.h;
        return result;
    }

    Size
    opAddAssign(Size rhs)
    {
        this.w += rhs.w;
        this.h += rhs.h;
        return (*this);
    }

    Size
    opSub(Size rhs)
    {
        Size result;
        result.w = this.w - rhs.w;
        result.h = this.h - rhs.h;
        return result;
    }

    Size
    opSubAssign(Size rhs)
    {
        this.w -= rhs.w;
        this.h -= rhs.h;
        return (*this);
    }

    Size
    opMul(double rhs)
    {
        Size result;
        result.w = this.w * rhs;
        result.h = this.h * rhs;
        return result;
    }

    Size
    opMulAssign(double rhs)
    {
        this.w *= rhs;
        this.h *= rhs;
        return (*this);
    }

    Point
    toPoint()
    {
        Point result;
        result.x = this.w;
        result.y = this.h;
        return result;
    }
}

struct Rect
{
    Point origin;
    Size size;

    Rect
    opAdd(Point rhs)
    {
        Rect result = (*this);
        result.origin += rhs;
        return result;
    }

    Rect
    opAssAssign(Point rhs)
    {
        this.origin += rhs;
        return (*this);
    }

    Rect
    opSub(Point rhs)
    {
        Rect result = (*this);
        result.origin -= rhs;
        return result;
    }

    /+Rect
    opSub_r(Point lhs)
    {
        Rect result = (*this);
        result.origin = lhs - result.origin;
        return result;
    }+/

    Rect
    opSubAssign(Point rhs)
    {
        this.origin -= rhs;
        return (*this);
    }

    Rect
    opAdd(Size rhs)
    {
        Rect result = (*this);
        result.size += rhs;
        return result;
    }

    Rect
    opAddAssign(Size rhs)
    {
        this.size += rhs;
        return (*this);
    }

    Rect
    opSub(Size rhs)
    {
        Rect result = (*this);
        result.size -= rhs;
        if( result.size.w < 0.0 )
        {
            result.origin.x += result.size.w;
            result.size.w = abs(result.size.w);
        }
        if( result.size.h < 0.0 )
        {
            result.origin.y += result.size.h;
            result.size.h = abs(result.size.h);
        }
        return result;
    }

    /+Rect
    opSub_r(Size lhs)
    {
        Rect result = (*this);
        result.size = lhs - result.size;
        return result;
    }+/

    Rect
    opSubAssign(Size rhs)
    {
        this.size -= rhs;
        if( this.size.w < 0.0 )
        {
            this.origin.x += this.size.w;
            this.size.w = abs(this.size.w);
        }
        if( this.size.h < 0.0 )
        {
            this.origin.y += this.size.h;
            this.size.h = abs(this.size.h);
        }
        return (*this);
    }

    Extent
    toExtent()
    {
        Extent result;
        result.pt0 = this.origin;
        result.pt1 = this.origin + this.size.toPoint;
        return result;
    }
}

struct Extent
{
    Point pt0, pt1;

    Extent
    opAdd(Point rhs)
    {
        Extent result = (*this);
        result.pt0 += rhs;
        result.pt1 += rhs;
        return result;
    }

    Extent
    opAddAssign(Point rhs)
    {
        this.pt0 += rhs;
        this.pt1 += rhs;
        return (*this);
    }

    Extent
    opSub(Point rhs)
    {
        Extent result = (*this);
        result.pt0 -= rhs;
        result.pt1 -= rhs;
        return result;
    }

    /+Extent
    opSub_r(Point lhs)
    {
        Extent result = (*this);
        result.pt0 = lhs - result.pt0;
        result.pt1 = lhs - result.pt1;
        return result;
    }+/

    Extent
    opSubAssign(Point rhs)
    {
        this.pt0 -= rhs;
        this.pt1 -= rhs;
        return (*this);
    }

    Extent
    opAdd(Size rhs)
    {
        this.pt1 += rhs.toPoint;
        return (*this);
    }

    Extent
    opAddAssign(Size rhs)
    {
        this.pt1 += rhs.toPoint;
        return (*this);
    }

    Extent
    opSub(Size rhs)
    {
        // Easier to do the subtraction using a rect, then convert back
        return (this.toRect - rhs).toExtent;
    }

    /+Extent
    opSub_r(Size lhs)
    {
        // Easier to do the subtraction using a rect, then convert back
        return (lhs - this.toRect).toExtent;
    }+/

    Extent
    opSubAssign(Size rhs)
    {
        // Easier to do the subtraction using a rect, then convert back
        (*this) = (this.toRect - rhs).toExtent;
        return (*this);
    }

    Rect
    toRect()
    {
        Rect result;
        result.origin = this.pt0;
        result.size = (this.pt1 - this.pt0).toSize;
        return result;
    }
}

struct RGB
{
    double r, g, b;

    static
    RGB
    fromHex(uint color)
    {
        RGB result;
        result.r = ((color >> 16) & 0xFF) / 256.;
        result.g = ((color >> 8) & 0xFF) / 256.;
        result.b = (color & 0xFF) / 256.;
        return result;
    }

    static
    RGB
    from8bit(ubyte r, ubyte g, ubyte b)
    {
        RGB result;
        result.r = r / 256.;
        result.g = g / 256.;
        result.b = b / 256.;
        return result;
    }
}

struct RGBA
{
    double r, g, b, a;

    static
    RGBA
    fromHex(uint color)
    {
        RGBA result;
        result.a = ((color >> 24) & 0xFF) / 256.;
        result.r = ((color >> 16) & 0xFF) / 256.;
        result.g = ((color >> 8) & 0xFF) / 256.;
        result.b = (color & 0xFF) / 256.;
        return result;
    }

    static
    RGBA
    from8bit(ubyte r, ubyte g, ubyte b, ubyte a)
    {
        RGBA result;
        result.r = r / 256.;
        result.g = g / 256.;
        result.b = b / 256.;
        result.a = a / 256.;
        return result;
    }
}

