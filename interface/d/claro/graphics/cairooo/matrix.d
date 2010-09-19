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
module claro.graphics.cairooo.matrix;

private
{
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
}

struct Matrix
{
    double xx, yx, xy, yy, x0, y0;

    static Matrix
    init(double xx, double yx, double xy, double yy, double x0, double y0)
    {
        Matrix result;
        cairo_matrix_init(result.toPtr, xx, yx, xy, yy, x0, y0);
        return result;
    }

    static Matrix
    initIdentity()
    {
        Matrix result;
        cairo_matrix_init_identity(result.toPtr);
        return result;
    }

    static Matrix
    initTranslate(double tx, double ty)
    {
        Matrix result;
        cairo_matrix_init_translate(result.toPtr, tx, ty);
        return result;
    }

    static Matrix
    initScale(double sx, double sy)
    {
        Matrix result;
        cairo_matrix_init_scale(result.toPtr, sx, sy);
        return result;
    }

    static Matrix
    initRotate(double radians)
    {
        Matrix result;
        cairo_matrix_init_rotate(result.toPtr, radians);
        return result;
    }

    void
    invert()
    {
        checkStatus(cairo_matrix_invert(this.toPtr));
    }

    static Matrix
    multiply(Matrix a, Matrix b)
    {
        Matrix result;
        cairo_matrix_multiply(result.toPtr, a.toPtr, b.toPtr);
        return result;
    }

    void
    transformDistance(inout double dx, inout double dy)
    {
        cairo_matrix_transform_distance(this.toPtr, &dx, &dy);
    }

    void
    transformPoint(inout double x, inout double y)
    {
        cairo_matrix_transform_point(this.toPtr, &x, &y);
    }

    Matrix
    opMul(Matrix other)
    {
        return Matrix.multiply(*this, other);
    }

    cairo_matrix_t*
    toPtr()
    {
        return cast(cairo_matrix_t*) this;
    }

    static Matrix
    fromPtr(cairo_matrix_t* ptr)
    {
        //return cast(Matrix) (*ptr);
        return *(cast(Matrix*)ptr);
    }
}

