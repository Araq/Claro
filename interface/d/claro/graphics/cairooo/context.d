/**
    Context is the main class upon which you do your drawing operations.
    It acts as an interface to the backing surface-specific routines that
    actually do the drawing.

    The typical way to draw on to a surface in cairo is to first create a
    Surface object, wrap that in a Context, and then perform your drawing
    operations on your Context.

Examples
------------------------------------------------------------------------------
import cairooo.all;

void main()
{
    Cairo.load();

    auto Surface surface = new ImageSurface(Format.ARGB32, 256, 256);
    auto Context cr = new Context(surface);

    // Perform your drawing operations using cr...
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
module claro.graphics.cairooo.context;

private
{
    import std.string;
    import claro.graphics.cairo.cairo;
    import claro.graphics.cairooo.enums;
    import claro.graphics.cairooo.exceptions;
    import claro.graphics.cairooo.fontextents;
    import claro.graphics.cairooo.fontface;
    import claro.graphics.cairooo.fontoptions;
    import claro.graphics.cairooo.glyph;
    import claro.graphics.cairooo.matrix;
    import claro.graphics.cairooo.path;
    import claro.graphics.cairooo.pattern;
    import claro.graphics.cairooo.surface;
    import claro.graphics.cairooo.textextents;
}

/**
    The cairo drawing context.
**/
class Context
{
private:
    cairo_t* _handle;

public:
    /**
        Creates a new drawing context from the specified surface.
    **/
    this(Surface surface)
    {
        this._handle = cairo_create(surface.handle);
    }

    /**
        Wraps the low-level cairo context in a new object.  This method is most
        useful for wrapping pointers returned from cairo api functions that are
        not yet supported by the OO api.  Please note that this function is
        slightly dangerous and can lead to memory leaks if misused (see below).
        
        Please note that it is $(I vitally) important that you use the takeref
        parameter correctly.  If takeref is set to false, then the
        object will $(I not) add to the handle's reference count.  This means
        that if you do not pass the correct value for takeref, then you could
        end up with handles either not being destroyed, or being destroyed too
        early!
        
    Params:
        handle      = Pointer to a cairo_t context.
        takeref     = Should the object take a reference to this handle?
    **/
    this(cairo_t* handle, bool takeref)
    {
        checkStatus(handle);
        this._handle = handle;
        if( takeref )
        {
            cairo_reference(this.handle);
            checkStatus(this.handle);
        }
    }

    /**
        Duplicates a context, providing a new, independant reference
        to it.
    **/
    this(Context context)
    {
        this(context.handle, true);
    }

    ~this()
    {
        cairo_destroy(this.handle);
    }

    //
    // cairo api members
    //

    /**
        Makes a copy of the current state of the context, and places it on an
        internal stack.  When the restore method is called, it restores the
        context to the state stored at the top of this internal stack.

        It is not neccecary to explicitly restore all the saved states on this
        stack before the Context is destroyed.
    **/
    void
    save()
    {
        scope(success) checkStatus();
        cairo_save(this.handle);
    }

    /**
        Restores a state stored by the save member.

    See_Also: save
    **/
    void
    restore()
    {
        scope(success) checkStatus();
        cairo_restore(this.handle);
    }

    /**
        Returns the surface which this context is performing operations on.
    **/
    Surface
    target()
    {
        scope(success) checkStatus();
        return Surface.rewrapSurfaceHandle(cairo_get_target(this.handle));
    }

    /**
        Sets the current source pattern to the supplied opaque colour.
        This colour will be used until a different pattern is set.

    Params:
        red     = Colour's red component.
        green   = Colour's green component.
        blue    = Colour's blue component.
    **/
    void
    setSourceRGB(double red, double green, double blue)
    {
        scope(success) checkStatus();
        cairo_set_source_rgb(this.handle, red, green, blue);
    }

    /+/// ditto
    void
    setSourceRGB(RGB color)
    {
        this.setSourceRGB(color.r, color.g, color.b);
    }+/

    /**
        Sets the current source pattern to the supplied transparent colour.
        This colour will be used until a different pattern is set.

    Params:
        red     = Colour's red component.
        green   = Colour's green component.
        blue    = Colour's blue component.
        alpha   = Colour's alpha component.
    **/
    void
    setSourceRGBA(double red, double green, double blue, double alpha)
    {
        scope(success) checkStatus();
        cairo_set_source_rgba(this.handle, red, green, blue, alpha);
    }

    /+/// ditto
    void
    setSourceRGBA(RGBA color)
    {
        this.setSourceRGBA(color.r, color.g, color.b, color.a);
    }+/

    // NOTE: setSource/getSource are *not* implemented as getters/setters
    // since they are overloaded, and the surface version takes extra
    // arguments.

    /**
        Sets the source pattern for this context.  This pattern will be used
        until a different pattern is set.

    Params:
        source = The pattern to use.
    **/
    void
    setSource(Pattern source)
    {
        scope(success) checkStatus();
        cairo_set_source(this.handle, source.handle);
    }

    /**
        Sets the source pattern for this context to the given surface.  You
        can also specify the user-space coordinates at which the surface will
        appear at.  This pattern will be used until a different pattern is
        set.

    Params:
        source =    The surface to display.
        x =         The X user-space coordinate where the surface will appear.
        y =         The Y user-space coordinate where the surface will appear.
    **/
    void
    setSource(Surface source, double x, double y)
    {
        scope(success) checkStatus();
        cairo_set_source_surface(this.handle, source.handle, x, y);
    }

    /+/// ditto
    void
    setSource(Surface source, Point offset)
    {
        this.setSource(source, offset.x, offset.y);
    }+/

    /**
        Retrieves the current pattern being used by this context.

        Please note that this function will return the pattern as an instance
        of the base Pattern type, and not of a more specific type.

    Returns: The current Pattern used by this Context.
    **/
    Pattern
    getSource()
    {
        scope(success) checkStatus();
        return new Pattern(cairo_get_source(this.handle), true);
    }

    /**
        This property gives you access to the antialiasing scheme being used
        by this context.
    **/
    Antialias
    antialias()
    {
        scope(success) checkStatus();
        return cast(Antialias) cairo_get_antialias(this.handle);
    }

    /// ditto
    Antialias
    antialias(Antialias value)
    {
        scope(success) checkStatus();
        cairo_set_antialias(this.handle, cast(cairo_antialias_t) value);
        return value;
    }

    /**
        Sets the pattern of dashes which will be used by this context for
        stroking paths.

        The array describes the length of each alternating on/off segment of
        the dash.  For example, if you wanted a dash that had visible segments
        5 units in length and gaps 1 unit in length, you would pass an array
        containing the values 5.0 and 1.0 to the dashes argument.

    Params:
        dashes =    Describes the length of each alternating on/off segment.
        offset =    An offset into the dash pattern.
    **/
    void
    setDash(double[] dashes, double offset)
    {
        scope(success) checkStatus();
        cairo_set_dash(this.handle, dashes.ptr, dashes.length, offset);
    }

    /**
        This property lets you access this context's current fill rule.
    **/
    FillRule
    fillRule()
    {
        scope(success) checkStatus();
        return cast(FillRule) cairo_get_fill_rule(this.handle);
    }

    /// ditto
    FillRule
    fillRule(FillRule value)
    {
        scope(success) checkStatus();
        cairo_set_fill_rule(this.handle, cast(cairo_fill_rule_t) value);
        return value;
    }

    /**
        This property lets you access this context's current line cap style.
    **/
    LineCap
    lineCap()
    {
        scope(success) checkStatus();
        return cast(LineCap) cairo_get_line_cap(this.handle);
    }

    /// ditto
    LineCap
    lineCap(LineCap value)
    {
        scope(success) checkStatus();
        cairo_set_line_cap(this.handle, cast(cairo_line_cap_t) value);
        return value;
    }

    /**
        This property lets you access this context's current line join style.
    **/
    LineJoin
    lineJoin()
    {
        scope(success) checkStatus();
        return cast(LineJoin) cairo_get_line_join(this.handle);
    }

    /// ditto
    LineJoin
    lineJoin(LineJoin value)
    {
        scope(success) checkStatus();
        cairo_set_line_join(this.handle, cast(cairo_line_join_t) value);
        return value;
    }

    /// This property lets you access this context's current line width.
    double
    lineWidth()
    {
        scope(success) checkStatus();
        return cairo_get_line_width(this.handle);
    }

    /// ditto
    double
    lineWidth(double value)
    {
        scope(success) checkStatus();
        cairo_set_line_width(this.handle, value);
        return value;
    }
    
    /// This property lets you access this context's current miter limit.
    double
    miterLimit()
    {
        scope(success) checkStatus();
        return cairo_get_miter_limit(this.handle);
    }

    /// ditto
    double
    miterLimit(double value)
    {
        scope(success) checkStatus();
        cairo_set_miter_limit(this.handle, value);
        return value;
    }

    /// This property lets you access this context's current operator setting.
    Operator
    operator()
    {
        scope(success) checkStatus();
        return cast(Operator) cairo_get_operator(this.handle);
    }

    /// ditto
    Operator
    operator(Operator value)
    {
        scope(success) checkStatus();
        cairo_set_operator(this.handle, cast(cairo_operator_t) value);
        return value;
    }

    /**
        This property lets you access this context's current tolerance
        setting.

        This value is used when converting curved paths into line segments.
        The curved path will be subdivided until the maximum deviation between
        the original path and the straight line approximation is less than the
        value of tolerance.

        The default value is 0.1, and should be sufficient for most purposes.
        Larger values will give better performance at the cost of jagged
        curved.  Smaller values will give a smoother appearance to curved
        paths at the cost of slower performance.
        
        Except in extreme
        circumstances, reducing the value of tolerance from the default is
        unlikely to significantly improve appearance.
    **/
    double
    tolerance()
    {
        scope(success) checkStatus();
        return cairo_get_tolerance(this.handle);
    }

    /// ditto
    double
    tolerance(double value)
    {
        scope(success) checkStatus();
        cairo_set_tolerance(this.handle, value);
        return value;
    }

    /**
        Creates a new clip region by intersecting the current clip region with
        the current Path as if it was filled by calling the fill method under
        the current fill rule.

        Calling clip can only make the clip region smaller, never larger.  If
        you wish to reset the clip region, then you should call resetClip.
        The clip region is also part of the context's state, and as such can
        be managed using save/restore.

        Calling this method consumes the current path.
    **/
    void
    clip()
    {
        scope(success) checkStatus();
        cairo_clip(this.handle);
    }

    /**
        Creates a new clip region by intersecting the current clip region with
        the current Path as if it was filled by calling the fill method under
        the current fill rule.

        Calling clip can only make the clip region smaller, never larger.  If
        you wish to reset the clip region, then you should call resetClip.
        The clip region is also part of the context's state, and as such can
        be managed using save/restore.

        Calling this method will not consume the current path.
    **/
    void
    clipPreserve()
    {
        scope(success) checkStatus();
        cairo_clip_preserve(this.handle);
    }

    /**
        Resets the clip region to cover the entire surface.
    **/
    void
    resetClip()
    {
        scope(success) checkStatus();
        cairo_reset_clip(this.handle);
    }

    /**
        Fills the active path with the current source, using the current fill
        rule.

        Calling this method will consume the current path.
    **/
    void
    fill()
    {
        scope(success) checkStatus();
        cairo_fill(this.handle);
    }

    /**
        Fills the active path with the current source, using the current fill
        rule.

        Calling this method will not consume the current path.
    **/
    void
    fillPreserve()
    {
        scope(success) checkStatus();
        cairo_fill_preserve(this.handle);
    }

    /**
        Computes the rectangle that contains the complete filled area of the
        current path using the current fill rule.
    **/
    void
    fillExtents(out double x1, out double y1, out double x2, out double y2)
    {
        scope(success) checkStatus();
        cairo_fill_extents(this.handle, &x1, &y1, &x2, &y2);
    }

    /+/// ditto
    Extent
    fillExtents()
    {
        Extent result;
        this.fillExtents(result.pt0.x, result.pt0.y, result.pt1.x,
                result.pt1.y);
        return result;
    }+/

    /**
        Determines if the specified point is inside the current path's filled
        area.
    **/
    bool
    inFill(double x, double y)
    {
        scope(success) checkStatus();
        return cast(bool) cairo_in_fill(this.handle, x, y);
    }

    /+/// ditto
    bool
    inFill(Point pt)
    {
        return this.inFill(pt.x, pt.y);
    }+/

    /**
        This paints the current source using the specified pattern's alpha
        channel as a mask.
    **/
    void
    mask(Pattern pattern)
    {
        scope(success) checkStatus();
        cairo_mask(this.handle, pattern.handle);
    }

    /**
        This paints the current source using the specified surface's alpha
        channel as a mask.  The x and y coordinates have the same meaning as
        for setSource(surface, x, y).
    **/
    void
    maskSurface(Surface surface, double x, double y)
    {
        scope(success) checkStatus();
        cairo_mask_surface(this.handle, surface.handle, x, y);
    }

    /+/// ditto
    void
    maskSurface(Surface surface, Point pt)
    {
        this.maskSurface(surface, pt.x, pt.y);
    }+/

    /**
        This paints the current source everywhere on to the underlying
        surface.
    **/
    void
    paint()
    {
        scope(success) checkStatus();
        cairo_paint(this.handle);
    }

    /**
        This paints the current source everywhere on to the underlying
        surface, using the specified alpha blending value.
    **/
    void
    paintWithAlpha(double alpha)
    {
        scope(success) checkStatus();
        cairo_paint_with_alpha(this.handle, alpha);
    }

    ///
    void
    stroke()
    {
        scope(success) checkStatus();
        cairo_stroke(this.handle);
    }

    ///
    void
    strokePreserve()
    {
        scope(success) checkStatus();
        cairo_stroke_preserve(this.handle);
    }

    ///
    void
    strokeExtents(out double x1, out double y1, out double x2, out double y2)
    {
        scope(success) checkStatus();
        cairo_stroke_extents(this.handle, &x1, &y1, &x2, &y2);
    }

    /+/// ditto
    Extent
    strokeExtents()
    {
        Extent result;
        this.strokeExtents(result.pt0.x, result.pt0.y, result.pt1.x,
                result.pt1.y);
        return result;
    }+/

    ///
    bool
    inStroke(double x, double y)
    {
        scope(success) checkStatus();
        return cast(bool) cairo_in_stroke(this.handle, x, y);
    }

    /+/// ditto
    bool
    inStroke(Point pt)
    {
        return this.inStroke(pt.x, pt.y);
    }+/

    ///
    void
    copyPage()
    {
        scope(success) checkStatus();
        cairo_copy_page(this.handle);
    }

    ///
    void
    showPage()
    {
        scope(success) checkStatus();
        cairo_show_page(this.handle);
    }

    ///
    Path
    copyPath()
    {
        scope(success) checkStatus();
        return new Path(cairo_copy_path(this.handle));
    }

    ///
    Path
    copyPathFlat()
    {
        scope(success) checkStatus();
        return new Path(cairo_copy_path_flat(this.handle));
    }

    //
    // cairo path api members
    //
    
    ///
    void
    appendPath(Path path)
    {
        scope(success) checkStatus();
        cairo_append_path(this.handle, path.handle);
    }

    ///
    void
    getCurrentPoint(out double x, out double y)
    {
        scope(success) checkStatus();
        cairo_get_current_point(this.handle, &x, &y);
    }

    /+/// ditto
    Point
    getCurrentPoint()
    {
        Point result;
        this.getCurrentPoint(result.x, result.y);
        return result;
    }+/

    ///
    void
    newPath()
    {
        scope(success) checkStatus();
        cairo_new_path(this.handle);
    }

    ///
    void
    closePath()
    {
        scope(success) checkStatus();
        cairo_close_path(this.handle);
    }

    ///
    void
    arc(double xc, double yc, double radius, double angle1, double angle2)
    {
        scope(success) checkStatus();
        cairo_arc(this.handle, xc, yc, radius, angle1, angle2);
    }

    /+/// ditto
    void
    arc(Point center, double radius, double angle1, double angle2)
    {
        this.arc(center.x, center.y, radius, angle1, angle2);
    }+/

    ///
    void
    arcNegative(double xc, double yc, double radius, double angle1,
            double angle2)
    {
        scope(success) checkStatus();
        cairo_arc_negative(this.handle, xc, yc, radius, angle1, angle2);
    }

    /+/// ditto
    void
    arcNegative(Point center, double radius, double angle1, double angle2)
    {
        this.arcNegative(center.x, center.y, radius, angle1, angle2);
    }+/

    ///
    void
    curveTo(double x1, double y1, double x2, double y2,
            double x3, double y3)
    {
        scope(success) checkStatus();
        cairo_curve_to(this.handle, x1, y1, x2, y2, x3, y3);
    }

    /+/// ditto
    void
    curveTo(Point pt1, Point pt2, Point pt3)
    {
        this.curveTo(pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y);
    }+/

    ///
    void
    lineTo(double x, double y)
    {
        scope(success) checkStatus();
        cairo_line_to(this.handle, x, y);
    }

    /+/// ditto
    void
    lineTo(Point pt)
    {
        this.lineTo(pt.x, pt.y);
    }+/

    ///
    void
    moveTo(double x, double y)
    {
        scope(success) checkStatus();
        cairo_move_to(this.handle, x, y);
    }

    /+/// ditto
    void
    moveTo(Point pt)
    {
        this.moveTo(pt.x, pt.y);
    }+/

    ///
    void
    rectangle(double x, double y, double width, double height)
    {
        scope(success) checkStatus();
        cairo_rectangle(this.handle, x, y, width, height);
    }

    /+/// ditto
    void
    rectangle(Rect rect)
    {
        this.rectangle(rect.origin.x, rect.origin.y, rect.size.w,
                rect.size.h);
    }+/

    ///
    void
    glyphPath(Glyph[] glyphs)
    {
        scope(success) checkStatus();
        cairo_glyph_path(this.handle, cast(cairo_glyph_t*) glyphs.ptr,
                glyphs.length);
    }

    ///
    void
    textPath(char[] utf8)
    {
        scope(success) checkStatus();
        cairo_text_path(this.handle, toStringz(utf8));
    }

    ///
    void
    relCurveTo(double dx1, double dy1, double dx2, double dy2,
            double dx3, double dy3)
    {
        scope(success) checkStatus();
        cairo_rel_curve_to(this.handle, dx1, dy1, dx2, dy2, dx3, dy3);
    }

    /+/// ditto
    void
    relCurveTo(Point dpt1, Point dpt2, Point dpt3)
    {
        this.relCurveTo(dpt1.x, dpt1.y, dpt2.x, dpt2.y, dpt3.x, dpt3.y);
    }+/

    ///
    void
    relLineTo(double dx, double dy)
    {
        scope(success) checkStatus();
        cairo_rel_line_to(this.handle, dx, dy);
    }

    /+/// ditto
    void
    relLineTo(Point dpt)
    {
        this.relLineTo(dpt.x, dpt.y);
    }+/

    ///
    void
    relMoveTo(double dx, double dy)
    {
        scope(success) checkStatus();
        cairo_rel_move_to(this.handle, dx, dy);
    }

    /+/// ditto
    void
    relMoveTo(Point dpt)
    {
        this.relMoveTo(dpt.x, dpt.y);
    }+/

    //
    // cairo transformation api members
    //

    ///
    void
    translate(double tx, double ty)
    {
        scope(success) checkStatus();
        cairo_translate(this.handle, tx, ty);
    }

    /+/// ditto
    void
    translate(Point dpt)
    {
        this.translate(dpt.x, dpt.y);
    }+/

    ///
    void
    scale(double sx, double sy)
    {
        scope(success) checkStatus();
        cairo_scale(this.handle, sx, sy);
    }

    /+/// ditto
    void
    scale(Size sz)
    {
        this.scale(sz.w, sz.h);
    }+/

    ///
    void
    rotate(double angle)
    {
        scope(success) checkStatus();
        cairo_rotate(this.handle, angle);
    }

    ///
    void
    transform(Matrix matrix)
    {
        scope(success) checkStatus();
        cairo_transform(this.handle, matrix.toPtr);
    }

    ///
    Matrix
    matrix()
    {
        scope(success) checkStatus();
        Matrix result;
        cairo_get_matrix(this.handle, result.toPtr);
        return result;
    }

    ///
    Matrix
    matrix(Matrix value)
    {
        scope(success) checkStatus();
        cairo_set_matrix(this.handle, value.toPtr);
        return value;
    }

    ///
    void
    identityMatrix()
    {
        scope(success) checkStatus();
        cairo_identity_matrix(this.handle);
    }

    ///
    void
    userToDevice(inout double x, inout double y)
    {
        scope(success) checkStatus();
        cairo_user_to_device(this.handle, &x, &y);
    }

    /+/// ditto
    Point
    userToDevice(Point pt)
    {
        Point result = pt;
        this.userToDevice(pt.x, pt.y);
        return result;
    }+/

    ///
    void
    userToDeviceDistance(inout double dx, inout double dy)
    {
        scope(success) checkStatus();
        cairo_user_to_device_distance(this.handle, &dx, &dy);
    }

    /+/// ditto
    Point
    userToDeviceDistance(Point dpt)
    {
        Point result = dpt;
        this.userToDeviceDistance(dpt.x, dpt.y);
        return result;
    }+/

    ///
    void
    deviceToUser(inout double x, inout double y)
    {
        scope(success) checkStatus();
        cairo_device_to_user(this.handle, &x, &y);
    }

    /+/// ditto
    Point
    deviceToUser(Point pt)
    {
        Point result = pt;
        this.deviceToUser(result.x, result.y);
        return result;
    }+/

    ///
    void
    deviceToUserDistance(inout double dx, inout double dy)
    {
        scope(success) checkStatus();
        cairo_device_to_user_distance(this.handle, &dx, &dy);
    }

    /+/// ditto
    Point
    deviceToUserDistance(Point dpt)
    {
        Point result = dpt;
        this.deviceToUserDistance(dpt.x, dpt.y);
        return result;
    }+/

    //
    // cairo text api members
    //

    ///
    void
    selectFontFace(char[] family, FontSlant slant, FontWeight weight)
    {
        scope(success) checkStatus();
        cairo_select_font_face(this.handle, .toStringz(family),
                cast(cairo_font_slant_t) slant,
                cast(cairo_font_weight_t) weight);
    }

    ///
    void
    setFontSize(double size)
    {
        scope(success) checkStatus();
        cairo_set_font_size(this.handle, size);
    }

    ///
    Matrix
    fontMatrix()
    {
        scope(success) checkStatus();
        Matrix result;
        cairo_get_font_matrix(this.handle, result.toPtr);
        return result;
    }

    ///
    Matrix
    fontMatrix(Matrix value)
    {
        scope(success) checkStatus();
        cairo_set_font_matrix(this.handle, value.toPtr);
        return value;
    }
    
    ///
    FontOptions
    fontOptions()
    {
        scope(success) checkStatus();
        auto FontOptions temp = new FontOptions;
        cairo_get_font_options(this.handle, temp.handle);
        return temp.dup;
    }

    ///
    FontOptions
    fontOptions(FontOptions value)
    {
        scope(success) checkStatus();
        cairo_set_font_options(this.handle, value.handle);
    }

    ///
    void
    showText(char[] utf8)
    {
        scope(success) checkStatus();
        cairo_show_text(this.handle, toStringz(utf8));
    }

    ///
    void
    showGlyphs(Glyph[] glyphs)
    {
        scope(success) checkStatus();
        cairo_show_glyphs(this.handle, cast(cairo_glyph_t*) glyphs.ptr,
                glyphs.length);
    }

    ///
    FontFace
    fontFace()
    {
        scope(success) checkStatus();
        return new FontFace(cairo_get_font_face(this.handle), true);
    }

    ///
    FontFace
    fontFace(FontFace value)
    {
        scope(success) checkStatus();
        cairo_set_font_face(this.handle, value.handle);
    }

    ///
    FontExtents
    fontExtents()
    {
        scope(success) checkStatus();
        FontExtents result;
        cairo_font_extents(this.handle, cast(cairo_font_extents_t*) &result);
        return result;
    }

    ///
    TextExtents
    textExtents(char[] utf8)
    {
        scope(success) checkStatus();
        TextExtents result;
        cairo_text_extents(this.handle, .toStringz(utf8),
                cast(cairo_text_extents_t*) &result);
        return result;
    }

    ///
    TextExtents
    glyphExtents(Glyph[] glyphs)
    {
        scope(success) checkStatus();
        TextExtents result;
        cairo_glyph_extents(this.handle, cast(cairo_glyph_t*) glyphs.ptr,
                glyphs.length, cast(cairo_text_extents_t*) &result);
        return result;
    }
    
    //
    // Extra members
    //

    /**
     * Duplicates this context, providing a new, independant reference
     * to it.
     */
    Context
    dup()
    {
        return new Context(this);
    }

    /**
     * Returns the underlying cairo handle.
     */
    cairo_t*
    handle()
    {
        return this._handle;
    }

    ///
    int
    opEquals(Context other)
    {
        return this.handle == other.handle;
    }

    //
    // Internal stuff
    //
protected:
    void
    checkStatus()
    {
        .checkStatus(cairo_status(this._handle));
    }

    void
    checkStatus(cairo_t* handle)
    {
        .checkStatus(cairo_status(handle));
    }
}

