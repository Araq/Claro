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
module claro.graphics.cairooo.pattern;

private
{
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
    import claro.graphics.cairooo.matrix;
    import claro.graphics.cairooo.surface;
}

/**
 * A cairo pattern.
 */
class Pattern
{
private:
    cairo_pattern_t* _handle;

public:
    // NOTE: We are following the way cairomm does things, and allowing the
    // user to create the various kinds of patterns via subclasses.  So you
    // can't actually make anything interesting from this base class (although
    // it hangs around to wrap low-level handles).
    
    /**
     * Wraps the low-level cairo pattern in a new object.  This method is most
     * useful for wrapping pointers returned from cairo api functions that are
     * not yet supported by the OO api.  Please note that this function is
     * slightly dangerous and can lead to memory leaks if misused (see below).
     *
     * Please note that it is $(I vitally) important that you use the takeref
     * parameter correctly.  If takeref is set to true, then the
     * object will $(I not) add to the handle's reference count.  This means
     * that if you do not pass the correct value for takeref, then you could
     * end up with handles either not being destroyed, or being destroyed too
     * early!
     *
     * Params:
     *      handle      = Pointer to a cairo_pattern_t
     *      takeref     = Should the object take a reference to the handle?
     */
    this(cairo_pattern_t* handle, bool takeref)
    {
        checkStatus(handle);
        this._handle = handle;
        if( takeref )
        {
            cairo_pattern_reference(this.handle);
            checkStatus(this.handle);
        }
    }

    /**
     * Duplicates a pattern, providing a new, independant reference to it.
     */
    this(Pattern pattern)
    {
        this(pattern.handle, true);
    }

    ~this()
    {
        cairo_pattern_destroy(this.handle);
    }

    //
    // cairo api members
    //
    Matrix
    matrix()
    {
        scope(success) checkStatus();
        Matrix temp;
        cairo_pattern_get_matrix(this.handle, temp.toPtr);
        return temp;
    }

    Matrix
    matrix(Matrix value)
    {
        scope(success) checkStatus();
        cairo_pattern_set_matrix(this.handle, value.toPtr);
        return value;
    }

    //
    // miscellaneous members
    //

    Pattern
    dup()
    {
        return new Pattern(this);
    }

    cairo_pattern_t*
    handle()
    {
        return this._handle;
    }

    int
    opEquals(Pattern other)
    {
        return this.handle == other.handle;
    }

    //
    // internal stuff
    //
protected:
    void
    checkStatus()
    {
        .checkStatus(cairo_pattern_status(this.handle));
    }

    void
    checkStatus(cairo_pattern_t* handle)
    {
        .checkStatus(cairo_pattern_status(handle));
    }
}

