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
module claro.graphics.cairooo.callbacks;

private
{
    import claro.graphics.cairo.cairo;
}

alias void delegate(ubyte[]) ReadDelegate;
alias void delegate(ubyte[]) WriteDelegate;

package
{
    cairo_status_t
    readFunctionWrapper(void* closure, ubyte* data, uint length)
    {
        // Ok, the closure is actually a pointer to a ReadDelegate object (at
        // least, we *really* hope that it is :P).  What we need to do is wrap
        // the raw pointer into a dynamic array, and catch any thrown
        // exceptions.
        ubyte[] dataArray = data[0..length];
        ReadDelegate* readDelegate = cast(ReadDelegate*) closure;

        try
        {
            (*readDelegate)(dataArray);
            return cairo_status_t.CAIRO_STATUS_SUCCESS;
        }
        catch( Exception e )
        {
            // Ok, we caught an exception: just return the generic read
            // failure flag.
            return cairo_status_t.CAIRO_STATUS_READ_ERROR;
        }
    }

    cairo_status_t
    writeFunctionWrapper(void* closure, ubyte* data, uint length)
    {
        // Again, the closure is actually a pointer to a WriteDelegate.  We
        // just need to convert the raw pointer to a bounds-checked array, and
        // return the correct failure code when we cop an exception.
        ubyte[] dataArray = data[0..length];
        WriteDelegate* writeDelegate = cast(WriteDelegate*) closure;

        try
        {
            (*writeDelegate)(dataArray);
            return cairo_status_t.CAIRO_STATUS_SUCCESS;
        }
        catch( Exception e )
        {
            // Got an exception: return write failure
            return cairo_status_t.CAIRO_STATUS_WRITE_ERROR;
        }
    }
}

