/**
    This module contains the ShapeTemplate; a class that wraps the standard
    cairo Context with additional abilities.
    
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
module cairooo.extra.shapetemplate;

private
{
    import std.math;
    import cairooo.context;
    import cairooo.extra.types;
}

class ShapeTemplate
{
private:
    Context _context;

public:
    this(Context cr)
    {
        this._context = cr.dup;
    }

    this(ShapeTemplate shapeTemplate)
    {
        this(shapeTemplate.context);
    }

    ~this()
    {
        delete this._context;
    }

    //
    // shapes
    //
    ///
    void
    circle(double cx, double cy, double radius)
    {
        cr.arc(cx, cy, radius, 0*PI, 2*PI);
    }

    /// ditto
    void
    circle(Point center, double radius)
    {
        this.circle(center.x, center.y, radius);
    }
    
    ///
    void
    roundedRectangle(double x, double y, double width, double height,
            double radius)
    {
       double x0 = x;
       double y0 = y;
       double x1 = x+width;
       double y1 = y+height;
       double r = radius;

       // Clamp radius
       if( radius < width/2 )
           radius = width/2;
       if( radius < height/2 )
           radius = height/2;

       // Draw the rectangle
       cr.moveTo    (x0+r, y0);
       
       cr.lineTo    (x1-r, y0);
       cr.arc       (x1-r, y0+r, r, 3*PI/2, 0*PI);
       cr.lineTo    (x1,   y1-r);
       cr.arc       (x1-r, y1-r, r, 0*PI,   PI/2);
       cr.lineTo    (x0+r, y1);
       cr.arc       (x0+r, y1-r, r, PI/2,   1*PI);
       cr.lineTo    (x0,   y0+r);
       cr.arc       (x0+r, y0+r, r, 1*PI,   3*PI/2);
       
       cr.closePath();
    }
    
    /// ditto
    void
    roundedRectangle(Rect rect, double radius)
    {
        this.roundedRectangle(rect.origin.x, rect.origin.y,
                rect.size.w, rect.size.h, radius);
    }

    //
    // miscellaneous members
    //
    ///
    ShapeTemplate
    dup()
    {
        return new ShapeTemplate(this);
    }

    ///
    Context
    context()
    {
        return this._context;
    }

    //
    // internal members
    //
protected:
    Context
    cr()
    {
        return this._context;
    }
}

