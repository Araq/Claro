import os

env = DefaultEnvironment(CCFLAGS=['-g', '-Wall'], CPPPATH=["#src"], LIBPATH=["#build"], STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME=1)

no_cairo = ARGUMENTS.get( 'nocairo', 0 )
env.no_cairo = no_cairo

opengl = ARGUMENTS.get( 'opengl', 0 )
env.opengl = opengl

def CheckProgram(context, program):
  context.Message( 'Checking for %s in PATH ...' % program )
  if WhereIs(program) == None:
    context.Result(0);
  else:
    context.Result(1)

conf = Configure(env, custom_tests = {'CheckProgram': CheckProgram})

if env['PLATFORM'] == "darwin":
  print "Configuring for Cocoa on Mac OS X"
  conf.env.Append(CCFLAGS=["-D_MAC","-fPIC","-fno-common"], 
                  LINKFLAGS=["-framework","Carbon","-framework","Cocoa"])
  excl = '-DNO_CAIRO'
  if no_cairo == 0:
    excl = ''
  conf.env.Replace(SHLINKFLAGS='$LINKFLAGS -Isrc/ -dynamiclib '+excl)
  conf.env.Replace(SHLIBSUFFIX='.dylib')
  conf.env.Append(LIBSUFFIXES=[".dylib"])
  from macosx.osxbundle import *
  TOOL_BUNDLE(env)
  env['VERSION_NAME'] = ''
  env['SHLIBPREFIX'] = 'lib'
  env['VERSION_NUM'] = ''

  # cairo
  if no_cairo == 0:
    conf.env.Append(CCFLAGS=['-I/usr/local/include/cairo'])
elif env['PLATFORM'] == 'win32':
  conf.env.Replace(SHLIBSUFFIX='.dll')
  conf.env.Append(LIBSUFFIXES=[".dll"])
  
  # cairo
  # FIXME: what about non-default or non-MSYS?
  if no_cairo == 0:
    conf.env.Append(CCFLAGS=['-IC:/msys/1.0/local/include/cairo'])
  
  conf.env.Append(CCFLAGS=['-DIMAGES_USE_LIBPNG'], 
                  LINKFLAGS=['-LC:/msys/1.0/local/lib'], 
                  LIBS=['gdi32','ole32'])
  
  if opengl:
    conf.env.Append(LIBS=['OpenGL32'])
else:
  env['SHLIBPREFIX'] = 'lib'
  conf.env.Append(CCFLAGS=["-D_NIX","-fPIC","-DPIC"])
  if env['PLATFORM'] == "posix":
    conf.CheckProgram("pkg-config")
    conf.CheckProgram("gtk-config")
    print "Configuring for GTK 2.0"
    env.ParseConfig('pkg-config --libs --cflags gtk+-2.0 gdk-2.0 cairo')
  
  # cairo
  if no_cairo == 0:
    conf.env.Append(CCFLAGS=['-I/usr/local/include/cairo',
                             '-I/usr/include/cairo'])

if conf.CheckFunc('mmap'):
  conf.env.Append(CCFLAGS="-DHAVE_MMAP")

if no_cairo == 0:
  conf.env.Append(LIBS=["cairo"])
else:
  conf.env.Append(CCFLAGS=['-DNO_CAIRO'])

if opengl:
  conf.env.Append(CCFLAGS=['-DCLARO_OPENGL'])

env = conf.Finish()

targets = [
  "src/claro/base/SConscript",
  "src/claro/graphics/SConscript",
  "examples/SConscript"
]

#if env['PLATFORM'] != 'win32':
#  targets.append("tests/SConscript")

SConscript(targets, 'env')

Alias("all", ["libraries", "examples"])
Alias("install", ["/usr/local/lib", "/usr/local/include"])

Default("all")
