/**
    Defines various exceptions that can be thrown by the binding.
    
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
module claro.graphics.cairooo.exceptions;

private
{
    import std.outofmemory;
    import claro.graphics.cairo.cairo;
}

void
checkStatus(cairo_status_t status)
{
    switch(status)
    {
        case cairo_status_t.CAIRO_STATUS_SUCCESS:
            break;

        case cairo_status_t.CAIRO_STATUS_NO_MEMORY:
            throw new CairoNoMemoryException;

        case cairo_status_t.CAIRO_STATUS_READ_ERROR:
            throw new CairoReadException;
            
        case cairo_status_t.CAIRO_STATUS_WRITE_ERROR:
            throw new CairoWriteException;
            
        case cairo_status_t.CAIRO_STATUS_INVALID_RESTORE:
        case cairo_status_t.CAIRO_STATUS_INVALID_POP_GROUP: // Damn you N'sync!
        case cairo_status_t.CAIRO_STATUS_NO_CURRENT_POINT:
        case cairo_status_t.CAIRO_STATUS_INVALID_MATRIX:
        case cairo_status_t.CAIRO_STATUS_INVALID_STATUS:
        case cairo_status_t.CAIRO_STATUS_INVALID_STRING:
        case cairo_status_t.CAIRO_STATUS_SURFACE_FINISHED:
        case cairo_status_t.CAIRO_STATUS_INVALID_CONTENT:
        case cairo_status_t.CAIRO_STATUS_INVALID_FORMAT:
        case cairo_status_t.CAIRO_STATUS_INVALID_VISUAL:
        case cairo_status_t.CAIRO_STATUS_FILE_NOT_FOUND:
        case cairo_status_t.CAIRO_STATUS_INVALID_DASH:
            throw new CairoFatalError;

        case cairo_status_t.CAIRO_STATUS_NULL_POINTER:
        case cairo_status_t.CAIRO_STATUS_INVALID_PATH_DATA:
        case cairo_status_t.CAIRO_STATUS_SURFACE_TYPE_MISMATCH:
        case cairo_status_t.CAIRO_STATUS_PATTERN_TYPE_MISMATCH:
            throw new CairoBindingError;
            
        default:
            throw new CairoUnknownException;
    }
}

package
{
    template Ex(alias base)
    {
        class Ex : base
        {
            this(char[] msg) { super(msg); }
        }
    }
    
    template Ex(alias base, char[] message)
    {
        class Ex : base
        {
            this() { super(message); }
            this(char[] msg) { super(message ~ ": " ~ msg); }
        }
    }
}

alias Ex!(Exception, "cairo read error")    CairoReadException;
alias Ex!(Exception, "cairo write error")   CairoWriteException;
alias Ex!(Exception, "Unknown cairo error") CairoUnknownException;
alias Ex!(Error, "Fatal cairo error")       CairoFatalError;
alias Ex!(Error, "cairo binding error")     CairoBindingError;

class CairoNoMemoryException : OutOfMemoryException {}

