import sys, os
from distutils.core import setup, Extension

e = []
p = []

plat_defs = [('NO_CAIRO','1'), ]
cinc = []

if sys.platform == 'darwin':
	plat_defs += [('_MAC','1')]
elif sys.platform == 'win32':
	# win32 already defines it's _WIN32
	pass
else:
	# assume nix
	plat_defs += [('_NIX','1')]
	
	os.system( "pkg-config --cflags-only-I gtk+-2.0 > output.tmp" )
	
	f = open( 'output.tmp' )
	x = f.readlines( )
	f.close( )
	
	l = x[0]
	l = l.strip()
	pts = l.split()
	
	for dir in pts:
		cinc.append( dir[2:] )


# claro.base
e.append( Extension( "claro.wrapper.base", ["base.c"], include_dirs=['../../src'], define_macros=plat_defs, library_dirs=['../../src/claro/base/'], libraries=['claro-base'] ) )

# claro.graphics
e.append( Extension( "claro.wrapper.graphics", ["graphics.c"], include_dirs=['../../src'] + cinc, define_macros=plat_defs, library_dirs=['../../src/claro/graphics/','../../src/claro/base'], libraries=['claro-base', 'claro-graphics'] ) )

setup(	name="claro.base",
		version="0.1",
		packages=['claro', 'claro.wrapper'],
		ext_modules=e,
		py_modules=p )
