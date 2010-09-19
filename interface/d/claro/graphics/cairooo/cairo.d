/**
    This module provides functions for loading the cairo library at runtime,
    and for checking what version of the cairo library has been loaded.

    When using the cairooo binding, you need to call the appropriate load
    function for each package before using it.  You may also optionally pass
    the name of the library to load the package's functionality from, although
    this should be autodetected by the binding based on your operating system.

Examples:
------------------------------------------------------------------------------
import std.stdio;
import cairooo.all;

void main()
{
    // Loads the cairooo binding using the default library location
    Cairo.load();

    // Displays the version of the loaded cairo library.
    writefln("cairo version: %s", Cairo.cairoVersionString);
}
------------------------------------------------------------------------------
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
module claro.graphics.cairooo.cairo;

private
{
    import std.string;
    import claro.graphics.cairo.cairo;
    //import cairo.loader;
}

/**
    This exception is thrown when the binding cannot load the cairo library,
    or it cannot find a function in the library.
**/
//alias cairo.loader.CairoLoaderException CairoLoaderException;

/**
    This class provides a few functions for loading the binding, and accessing
    the underlying cairo library's version number.
**/
class Cairo
{
    /**
        Loads the default cairo library, and attempts to bind this package's
        functions.
    **/
    static
    void
    load()
    {
        //cairo_load();
    }

    /**
        Loads the cairo library from the given file path, and attempts to bind
        this package's functions.

    Params:
        libName = The path to load the cairo library from.  Depending on your
                  system, this need not be an absolute pathname.  Generally,
                  provided that the library is on your system PATH or LIBPATH,
                  then the name of the library should be sufficient.

                  For example, on Windows you might use "libcairo-2.dll",
                  whilst linux users might use "libcairo-2.so".
    **/
    static
    void
    load(char[] libName)
    {
        //cairo_load(libName);
    }
    
    /**
        Returns the version of the underlying cairo library.
    **/
    static
    int
    cairoVersion()
    {
        return cairo_version();
    }

    /// ditto
    static
    char[]
    cairoVersionString()
    {
        return .toString(cairo_version_string());
    }
}

