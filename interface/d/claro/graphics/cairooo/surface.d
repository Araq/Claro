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
module claro.graphics.cairooo.surface;

private
{
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
    import claro.graphics.cairooo.fontoptions;
}

/**
 * A cairo surface.
 */
class Surface
{
private:
    cairo_surface_t* _handle;

public:
    /**
     * Wraps the low-level cairo surface in a new object.  This method is most
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
     *      handle      = Pointer to a cairo_surface_t context
     *      takeref     = Should the object take a reference to the handle?
     */
    this(cairo_surface_t* handle, bool takeref)
    {
        checkStatus(handle);
        this._handle = handle;
        if( takeref )
        {
            cairo_surface_reference(this._handle);
            checkStatus(this.handle);
        }
    }

    /**
     * Duplicates a surface, providing a new, independant reference to it.
     */
    this(Surface surface)
    {
        this(surface.handle, true);
    }

    ~this()
    {
        cairo_surface_destroy(this.handle);
    }

    //
    // cairo api members
    //

    Surface
    createSimilar(Content content, int width, int height)
    {
        scope(success) checkStatus();
        cairo_surface_t* new_handle =
            cairo_surface_create_similar(this.handle,
                    cast(cairo_content_t) content, width, height);
        return new Surface(new_handle, false);
    }

    void
    finish()
    {
        scope(success) checkStatus();
        cairo_surface_finish(this.handle);
    }

    void
    flush()
    {
        scope(success) checkStatus();
        cairo_surface_flush(this.handle);
    }

    FontOptions
    fontOptions()
    {
        scope(success) checkStatus();
        auto FontOptions temp = new FontOptions();
        cairo_surface_get_font_options(this.handle, temp.handle);
        return temp.dup;
    }

    void
    markDirty()
    {
        scope(success) checkStatus();
        cairo_surface_mark_dirty(this.handle);
    }

    void markDirtyRectangle(int x, int y, int width, int height)
    {
        scope(success) checkStatus();
        cairo_surface_mark_dirty_rectangle(this.handle, x, y, width, height);
    }

    void setDeviceOffset(double x, double y)
    {
        scope(success) checkStatus();
        cairo_surface_set_device_offset(this.handle, x, y);
    }

    //
    // miscellaneous members
    //

    Surface
    dup()
    {
        return new Surface(this);
    }

    cairo_surface_t*
    handle()
    {
        return this._handle;
    }

    int
    opEquals(Surface other)
    {
        return this.handle == other.handle;
    }

    /**
        This method will take a surface handle that has been returned from a
        function like cairo_get_target, and attempts to reconstruct the
        original wrapper type.

        This is done using cairo's internal user data functions.  If the
        handle was not wrapped by an object, then it will return a regular
        Surface object.

        Also note that this will always recast the handle to the type of the
        first object that it was wrapped in.  Therefore, if you wrap a handle
        first using a Win32Surface, and then a GlitzSurface, this method will
        always return a Win32Surface.
    **/
    package
    static
    Surface
    rewrapSurfaceHandle(cairo_surface_t* handle)
    {
        // First, try to get the function pointer from the surface itself.
        void* wrapfunc = cairo_surface_get_user_data(handle, WRAPFUNC_KEY);

        // If we got null, that means that key wasn't attached: just wrap in a
        // regular surface object.
        if( wrapfunc == null )
            return new Surface(handle, true);

        // Otherwise, cast to a SurfaceWrapFunc, and return the result of
        // calling that.
        else
            return (cast(SurfaceWrapFunc) wrapfunc)(handle);
    }

    //
    // Internal stuff
    //
protected:
    void
    checkStatus()
    {
        .checkStatus(cairo_surface_status(this._handle));
    }
    
    void
    checkStatus(cairo_surface_t* handle)
    {
        .checkStatus(cairo_surface_status(handle));
    }

    void
    setWrapFunction(SurfaceWrapFunc func, cairo_surface_t* handle)
    {
        // First, check to see if the user data has already been set.
        void* wrapfunc = cairo_surface_get_user_data(handle,
                WRAPFUNC_KEY);

        // Only continue if the cast function hasn't been set yet.
        if( wrapfunc == null )
        {
            cairo_surface_set_user_data(handle, WRAPFUNC_KEY,
                    cast(void*) func, dummy_surface_delete_func);
            checkStatus(handle);
        }
    }
}

// The following are used to implement correct surface-from-handle casting.
package
{
    // The following implements a dummy delete function.  This is handy
    // because we *really* don't want to try deleting stuff from the code
    // segment...
    private void _func(void* data) {}
    static dummy_surface_delete_func = cast(cairo_destroy_func_t) &_func;

    // This is the function pointer type that we'll store in our user data
    // field.  It should return a new instance of a Surface subclass wrapping
    // the given handle.
    alias Surface function(cairo_surface_t*) SurfaceWrapFunc;
    
    // This hideous little hack is to get a unique user data key on which to
    // store our casting function.
    private static cairo_user_data_key_t _WRAPFUNC_KEY;
    static WRAPFUNC_KEY = &_WRAPFUNC_KEY;
}

