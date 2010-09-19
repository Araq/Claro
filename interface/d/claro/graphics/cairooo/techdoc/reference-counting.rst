
About manual reference counting in cairooo
==========================================

In cairo, there are two kinds of objects that are *not* reference counted like
everything else: cairo_path_t's and user-specified buffers for surfaces.

The problem with the first is that although it has a cairo_path_destroy
method, it *does not* have a corresponding cairo_path_reference method.  This
means that the binding gets exactly one reference.  Now, I *could* have
duplicated the path every time the user copied the Path object, but then Path
objects would have copy-by-value semantics instead of copy-by-reference like
everything else.

It gets worse because we can actually allocate our own cairo_path_t's, and we
then need to clean up their memory ourselves.

The problem with the second is that, again, users will expect the library to
behave uniformly, and this would create an ugly corner case.  When it comes to
user-supplied buffers, it's alright to leave them to the user to clean up.
But when we allocate them internally, then we should take care of that memory
in a deterministic fashion: after all, image buffers tend to be somewhat big,
and if we can free that memory when it's no longer needed, all the better.

For the above reasons, the Path and ImageBufferSurface classes implement a
simple reference counting scheme to track these resources lifetimes.  Because
I did not want to add the overhead of a complete class just to get reference
semantics on the refcount, I have instead used a pointer to a uint.  It's
simple, effective, and fast.  There shouldn't be any memory leaks since the
uint is allocated by the GC; and even then, the refcount gets destroyed along
with the underlying buffers when the count drops to zero.

You can grep the source code for its definition as "uint* _refcount".  Also,
because Path and ImageBufferSurface need to manage the memory slightly
differently, there isn't really much shared code between them.

So, if you see a uint* lying around, that's what it's for, and those are my
reasons.  If you have a better idea, then feel free to let me know.  Arguments
such as "... because pointers are evil!" will be summarily ignored :)

  -- Daniel Keep

