#! /usr/bin/env python

##########################################################################
##                                                                      ##
##             Build script for the Claro library                       ##
##                      (c) 2010 Andreas Rumpf                          ##
##                                                                      ##
##########################################################################

# written because I had the code lying around and life is too short for
# Scons' over-engineered slow broken undocumented abstract crap...

CLARO_VERSION = "0.8.0"

# ---------------------- platform detection --------------------------------

import sys, os, os.path, re, shutil, time, getopt, glob, zlib, pickle

python3 = sys.version[0] >= "3"
python26 = sys.version[0] == "2" and sys.version[2] >= "6"

true, false = 0==0, 0==1

if python3:
  sys.exit("This script does not yet work with Python 3.0")

try:
  from cStringIO import StringIO
except ImportError:
  from io import StringIO

if python3:
  def replace(s, a, b): return s.replace(a, b)
  def lower(s): return s.lower()
  def join(a, s=""): return s.join(a)
  def find(s, a): return s.find(a)
  def split(s, a=None): return s.split(a)
  def strip(s): return s.strip()

  def has_key(dic, key): return key in dic
else:
  from string import replace, lower, join, find, split, strip

  def has_key(dic, key): return dic.has_key(key)

if not python3 and not python26:
  import md5
  def newMD5(): return md5.new()
  def MD5update(obj, x):
    return obj.update(x)
else:
  import hashlib
  def newMD5(): return hashlib.md5()
  def MD5update(obj, x):
    if python26:
      return obj.update(x)
    else:
      return obj.update(bytes(x, "utf-8"))

def getHost():
  # incomplete list that sys.platform may return:
  # win32 aix3 aix4 atheos beos5 darwin freebsd2 freebsd3 freebsd4 freebsd5
  # freebsd6 freebsd7 generic irix5 irix6 linux2 mac netbsd1 next3 os2emx
  # riscos sunos5 unixware7
  x = replace(lower(re.sub(r"[0-9]+$", r"", sys.platform)), "-", "")
  if x == "win": return "windows"
  elif x == "darwin": return "macosx"
  elif x == "sunos": return "solaris"
  else: return x

def ExecProcess(cmd):
  try:
    pipe = os.popen4(cmd)[1]
  except AttributeError:
    pipe = os.popen(cmd)
  result = ""
  for line in pipe.readlines():
    result = result + line[:-1]
  pipe.close()
  return result

# ---------------------------------------------------------------------------

IMPORT_LIBS = []
DEFINES = []
INCLUDE_DIRS = ["include"]
LINK_FLAGS = ""
CC_FLAGS = "-g -Wall -Wfatal-errors -Werror"
CC = "gcc"

if getHost() == "windows":
  DEFINES += ['IMAGES_USE_LIBPNG']
elif getHost() == "macosx":
  INCLUDE_DIRS += ['/usr/local/include/cairo', '/usr/include/cairo']
  CC_FLAGS += " -fno-common"
  LINK_FLAGS += " -framework Carbon -framework Cocoa -dynamiclib"
  DEFINES += ["_MAC", "NO_CAIRO"]
else:
  # only for gtk backend:
  deps = " gtk+-2.0 gdk-2.0 cairo"
  LINK_FLAGS += " " + ExecProcess("pkg-config --libs" + deps)
  CC_FLAGS += " " + ExecProcess("pkg-config --cflags" + deps)
  DEFINES += ["_NIX", "PIC"]

CLARO_FILES = [
  "src/block.c",
  "src/hashtable.c",
  "src/claro.c",
  "src/list.c",
  "src/log.c",
  "src/memory.c",
  "src/object.c",
  "src/oscompat.c", 
  "src/store.c", 
  "src/object_map.c", 
  
  "src/widgets/window.c",
  "src/widgets/toolbar.c",
  "src/widgets/textbox.c",
  "src/widgets/button.c",
  "src/widgets/canvas.c",
  "src/widgets/splitter.c",
  "src/widgets/container.c",
  "src/widgets/checkbox.c",
  "src/widgets/dialog.c",
  "src/widgets/label.c",
  "src/widgets/progress.c",
  "src/widgets/radio.c",
  "src/widgets/statusbar.c",
  "src/widgets/textarea.c",
  "src/widgets/frame.c",
  "src/widgets/image.c",
  "src/widgets/list.c",
  "src/widgets/listbox.c",
  "src/widgets/listview.c",
  "src/widgets/combo.c",
  "src/widgets/menubar.c",
  "src/widgets/menu.c",
  "src/widgets/scrollbar.c",
  "src/widgets/workspace.c",
  "src/widgets/opengl.c",
  "src/widgets/treeview.c",
  "src/widgets/stock.c",
  
  "src/layout_parser.c",
  "src/layout.c",
  "src/layout_heap.c",
  
  "src/widget.c",
  "src/graphics.c",
  "src/image.c",
  "src/font.c",
  "src/system.c",
]

if getHost() == "windows":
  CLARO_FILES += [
    "src/platform/win32.c",
    "src/platform/win32/window.c",
    "src/platform/win32/toolbar.c",
    "src/platform/win32/textbox.c",
    "src/platform/win32/button.c",
    "src/platform/win32/canvas.c",
    "src/platform/win32/splitter.c",
    "src/platform/win32/container.c",
    "src/platform/win32/label.c",
    "src/platform/win32/statusbar.c",
    "src/platform/win32/progressbar.c",
    "src/platform/win32/checkbox.c",
    "src/platform/win32/radio.c",
    "src/platform/win32/textarea.c",
    "src/platform/win32/frame.c",
    "src/platform/win32/image.c",
    "src/platform/win32/listbox.c",
    "src/platform/win32/listview.c",
    "src/platform/win32/combo.c",
    "src/platform/win32/menubar.c",
    "src/platform/win32/menu.c",
    "src/platform/win32/scrollbar.c",
    "src/platform/win32/workspace.c",
    "src/platform/win32/opengl.c",
    "src/platform/win32/treeview.c",
    
    "src/platform/win32/pngdib.c",
    "src/platform/win32/font.c",
    "src/platform/win32/images.c",
    "src/platform/win32/system.c",
  ]
  IMPORT_LIBS += [
    'gdi32', 'comctl32', 
    'ole32', 'msvcp60', 'Msimg32', 'png', 'z', 'opengl32', 'glu32',
    'cairo'
  ]
elif getHost() == "macosx":
  CLARO_FILES += [
    "src/platform/macosx_cocoa.m",
    "src/platform/cocoa/window.m",
    "src/platform/cocoa/toolbar.m",
    "src/platform/cocoa/textbox.m",
    "src/platform/cocoa/button.m",
    "src/platform/cocoa/canvas.m",
    "src/platform/cocoa/splitter.m",
    "src/platform/cocoa/container.m",
    "src/platform/cocoa/label.m",
    "src/platform/cocoa/statusbar.m",
    "src/platform/cocoa/progressbar.m",
    "src/platform/cocoa/checkbox.m",
    "src/platform/cocoa/radio.m",
    "src/platform/cocoa/textarea.m",
    "src/platform/cocoa/frame.m",
    "src/platform/cocoa/image.m",
    "src/platform/cocoa/listview.m",
    "src/platform/cocoa/listbox.m",
    "src/platform/cocoa/combo.m",
    "src/platform/cocoa/menubar.m",
    "src/platform/cocoa/menu.m",
    "src/platform/cocoa/scrollbar.m",
    "src/platform/cocoa/workspace.m",
    "src/platform/cocoa/opengl.m",
    "src/platform/cocoa/treeview.m",
    
    "src/platform/cocoa/font.m",
    "src/platform/cocoa/images.m",
    "src/platform/cocoa/system.m",
    
    "src/platform/cocoa/RBSplitView.m",
    "src/platform/cocoa/RBSplitSubview.m",
  ]
else:
  CLARO_FILES += [
    "src/platform/gtk.c", 
    "src/platform/gdkcairo.c", 
    "src/platform/gtkcairo.c",
    "src/platform/gtk/window.c",
    "src/platform/gtk/toolbar.c",
    "src/platform/gtk/textbox.c",
    "src/platform/gtk/button.c",
    "src/platform/gtk/canvas.c",
    "src/platform/gtk/splitter.c",
    "src/platform/gtk/container.c",
    "src/platform/gtk/label.c",
    "src/platform/gtk/statusbar.c",
    "src/platform/gtk/progressbar.c",
    "src/platform/gtk/checkbox.c",
    "src/platform/gtk/radio.c",
    "src/platform/gtk/textarea.c",
    "src/platform/gtk/frame.c",
    "src/platform/gtk/image.c",
    "src/platform/gtk/listview.c",
    "src/platform/gtk/listbox.c",
    "src/platform/gtk/combo.c",
    "src/platform/gtk/menubar.c",
    "src/platform/gtk/menu.c",
    "src/platform/gtk/scrollbar.c",
    "src/platform/gtk/workspace.c",
    "src/platform/gtk/opengl.c",
    "src/platform/gtk/treeview.c",
    
    "src/platform/gtk/font.c",
    "src/platform/gtk/images.c",
    "src/platform/gtk/system.c",
  ]
  
def mydigest(hasher):
  result = ""
  for c in hasher.digest():
    if python3:
      x = hex(c)[2:]
    else:
      x = hex(ord(c))[2:]
    if len(x) == 1: x = "0" + x
    result = result + x
  return result

def Subs(frmt, *args, **substitution):
  DIGITS = "0123456789"
  LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
  chars = DIGITS+LETTERS+"_"
  d = substitution
  a = args
  result = []
  i = 0
  num = 0
  L = len(frmt)
  while i < L:
    if frmt[i] == '$':
      i = i+1
      if frmt[i] == '#':
        result.append(a[num])
        num = num+1
        i = i+1
      elif frmt[i] == '$':
        result.append('$')
        i = i+1
      elif frmt[i] == '{':
        i = i+1
        j = i
        while frmt[i] != '}': i = i+1
        i = i+1 # skip }
        x = frmt[j:i-1]
        if x[0] in DIGITS:
          result.append(str(a[int(x)-1]))
        else:
          result.append(str(d[x]))
      elif frmt[i] in chars:
        j = i
        i = i+1
        while i < len(frmt) and frmt[i] in chars: i = i + 1
        x = frmt[j:i]
        if x[0] in DIGITS:
          num = int(x)
          result.append(str(a[num-1]))
        else:
          result.append(str(d[x]))
      else:
        assert(false)
    else:
      result.append(frmt[i])
      i = i+1
  return join(result, "")
# --------------------- constants  ----------------------------------------

EXPLAIN = true
force = false

# --------------------------------------------------------------------------

def Error(msg): sys.exit("[Koch] *** ERROR: " + msg)
def Warn(msg): print("[Koch] *** WARNING: " + msg)
def Echo(msg): print("[Koch] " + msg)
def _Info(msg): print("[Koch] " + msg)

_FINGERPRINTS_FILE = "koch.dat"
  # in this file all the fingerprints are kept to allow recognizing when a file
  # has changed. This works reliably, which cannot be said from just taking
  # filetime-stamps.

def SameFileContent(filenameA, filenameB):
  SIZE = 4096*2
  result = true
  a = open(filenameA, "rb")
  b = open(filenameB, "rb")
  while true:
    x = a.read(SIZE)
    y = b.read(SIZE)
    if x != y:
      result = false
      break
    elif len(x) < SIZE: # EOF?
      break
  a.close()
  b.close()
  return result

def SplitArg(s):
  if ':' in s: c = ':'
  elif '=' in s: c = '='
  else: return (s, '')
  i = find(s, c)
  return (s[:i], s[i+1:])

_baseDir = os.getcwd()
BaseDir = _baseDir

def Path(a):
  # Gets a UNIX like path and converts it to a path on this platform.
  # With UNIX like, I mean: slashes, not backslashes, only relative
  # paths ('../etc') can be used
  result = a
  if os.sep != "/": result = replace(result, "/", os.sep)
  if os.pardir != "..": result = replace(result, "..", os.pardir)
  return result

def Join(*args):
  result = []
  for a in args[:-1]:
    result.append(a)
    if result[-1] != "/": result.append("/")
  result.append(args[-1])
  return replace(join(result, ""), "//", "/")

def Exec(command):
  c = Path(command)
  Echo(c)
  result = os.system(c)
  if result != 0: Error("execution of an external program failed")
  return result

def TryExec(command):
  c = Path(command)
  Echo(c)
  result = os.system(c)
  return result

def RawExec(command):
  Echo(command)
  result = os.system(command)
  if result != 0: Error("execution of an external program failed")
  return result

def Remove(f):
  try:
    os.remove(Path(f))
  except OSError:
    Warn("could not remove: " + f)

def Move(src, dest):
  try:
    m = shutil.move
  except AttributeError:
    def f(src, dest):
      shutil.copy(src, dest)
      Remove(src)
    m = f
  s = Path(src)
  d = Path(dest)
  try:
    m(s, d)
  except IOError:
    Warn(Subs("could not move $1 to $2", s, d))
  except OSError:
    Warn(Subs("could not move $1 to $2", s, d))

def Copy(src, dest):
  s = Path(src)
  d = Path(dest)
  try:
    shutil.copyfile(s, d)
  except IOError:
    Warn(Subs("could not copy $1 to $2", s, d))
  except OSError:
    Warn(Subs("could not copy $1 to $2", s, d))

def RemoveDir(f):
  try:
    shutil.rmtree(Path(f))
  except OSError:
    Warn("could not remove: " + f)

def Exists(f): return os.path.exists(Path(f))

def Chdir(dest):
  d = Path(dest)
  try:
    os.chdir(d)
  except OSError:
    Warn("could not switch to directory: " + d)

def Mkdir(dest):
  d = Path(dest)
  try:
    os.mkdir(d)
  except OSError:
    Warn("could not create directory: " + d)

def Glob(pattern): # needed because glob.glob() is buggy on Windows 95:
  # things like tests/t*.nim won't work
  global _baseDir
  (head, tail) = os.path.split(Path(pattern))
  result = []
  if os.path.exists(head):
    try:
      os.chdir(os.path.join(_baseDir, head))
      try:
        for f in glob.glob(tail): result.append(os.path.join(head, f))
      except OSError:
        result = []
    finally:
      os.chdir(_baseDir)
  return result

def FilenameNoExt(f):
  return os.path.splitext(os.path.basename(f))[0]

def _Ext(trunc, posixFormat, winFormat):
  (head, tail) = os.path.split(Path(trunc))
  if os.name == "posix": frmt = posixFormat
  else:                  frmt = winFormat
  return os.path.join(head, Subs(frmt, trunc=tail))

def DllExt(trunc):
  h = getHost()
  if h == "windows": frmt = '${trunc}.dll'
  elif h == "macosx": frmt = 'lib${trunc}.dylib'
  else: frmt = 'lib${trunc}.so'
  (head, tail) = os.path.split(Path(trunc))
  return os.path.join(head, Subs(frmt, trunc=tail))

def LibExt(trunc):
  return _Ext(trunc, '${trunc}.a', '${trunc}.lib')

def ScriptExt(trunc):
  return _Ext(trunc, '${trunc}.sh', '${trunc}.bat')

def ExeExt(trunc):
  return _Ext(trunc, '${trunc}', '${trunc}.exe')

def ObjExt(trunc):
  if CC == "gcc": return trunc[:-2] + ".o"
  else: return trunc[:-2] + ".obj"

def MakeExe(file):
  os.chmod(file, 493)

class Changed:
  """ Returns a Changed object. check() returns true iff one of the
      given files has changed. You have to call the object's success() 
      method if the build has been a success.

      Example:

      c = Changed("unique_name", "file1.pas file2.pas file3.pas")
      if c.check():
        Exec("fpc file1.pas")
        # Exec raises an exception if it fails, thus if we reach the 
        # next statement, it was a success:
        c.success()
  """
  def __init__(self, id, files, explain=false,
               fingerprintsfile=_FINGERPRINTS_FILE):
    # load the fingerprints file:
    # fingerprints is a dict[target, files] where files is a dict[filename, hash]
    self.fingers = {} # default value
    if Exists(fingerprintsfile):
      try:
        self.fingers = pickle.load(open(fingerprintsfile, "rb"))
      except OSError:
        Error("Cannot read from " + fingerprintsfile)
    self.filename = fingerprintsfile
    self.id = id
    self.files = files
    self._hashStr = zlib.adler32 # our hash function
    self.explain = explain

  def _hashFile(self, f):
    x = open(f, "rb")
    result = self._hashStr(x.read())
    x.close() # for other Python implementations
    return result

  def check(self):
    if type(self.files) == type(""):
      self.files = split(self.files)
    result = false
    target = self.id
    if not has_key(self.fingers, target):
      self.fingers[target] = {}
      if self.explain: _Info(Subs("no entries for target '$1'", target))
      result = true
    for d in self.files:
      if Exists(d):
        n = self._hashFile(d)
        if not has_key(self.fingers[target], d) or n != self.fingers[target][d]:
          result = true
          if self.explain: _Info(Subs("'$1' modified since last build", d))
          self.fingers[target][d] = n
      else:
        Warn(Subs("'$1' does not exist!", d))
        result = true
    return result

  def update(self, filename):
    self.fingers[self.id][filename] = self._hashFile(filename)

  def success(self):
    pickle.dump(self.fingers, open(self.filename, "wb+"))

# ----------------------------- call compiler ---------------------------------

def buildCcFlags(cc): 
  # XXX don't ignore cc
  result = CC_FLAGS
  for d in DEFINES:
    result += " -D" + d
  for i in INCLUDE_DIRS:
    result += " -I" + i
  return result

def buildLinkFlags(cc):
  result = LINK_FLAGS
  for i in IMPORT_LIBS:
    result += " -l" + i
  return result
  
def Compile(outputfile, files, 
            ccflags="", linkflags="", target="exe", cc=CC): 
  if target == "dll":
    ccflags += " -fPIC"
    linkflags += " -shared -fPIC"
    ofile = DllExt(outputfile)
  elif target == "exe": 
    ofile = ExeExt(outputfile)
  else: 
    Error("unknown target " + type)
  linkCmd = ""
  for f in files:
    Exec(Subs("$# -c $# -o $# $#", cc, ccflags, ObjExt(f), f))
    linkCmd += " " + ObjExt(f)
  Exec(Subs("$# $# -o $# $#", cc, linkflags, ofile, linkCmd))

def cmd_claro(ccflags):
  Compile("build/claro", CLARO_FILES, 
          buildCcFlags(CC) + " " + ccflags, 
          buildLinkFlags(CC), 
          "dll", CC)

def cmd_examples(ccflags):
  ccflags += ccflags + " -Iclaro"
  Compile("build/hello", ["examples/helloworld/hello.c"], ccflags)
  Compile("build/radio", ["examples/radio/radio.c"], ccflags)
  Compile("build/combo", ["examples/combo/combo.c"], ccflags)
  Compile("build/layout_test", ["examples/layout/layout_test.c"], ccflags)
  Compile("build/layout_any", ["examples/layout/layout_any.c"], ccflags)
  Compile("build/splitter", ["examples/splitter/splitter-test.c"], ccflags)
  Compile("build/cldlg", ["examples/cldlg/cldlg.c"], ccflags)
  Compile("build/designer", ["examples/designer/designer.c"], ccflags)
  Compile("build/progress", ["examples/progress/progress-test.c"], ccflags)
  Compile("build/image", ["examples/image/image.c"], ccflags)
  Compile("build/list", ["examples/list/list.c"], ccflags)
  Compile("build/listview", ["examples/list/listview.c"], ccflags)
  Compile("build/workspace", ["examples/workspace/workspace.c"], ccflags)

# -----------------------------------------------------------------------------

HELP = Subs("""\
+-----------------------------------------------------------------+
|         Maintenance script for Claro                            |
|             Version $1|
|             (c) 2010 Andreas Rumpf                              |
+-----------------------------------------------------------------+
Your Python version: $2

Usage:
  koch.py [options] command [options for command]
Options:
  --force, -f, -B, -b  forces rebuild
  --help, -h           shows this help and quits
Possible Commands:
  claro [options]      builds the Claro library (with options)
  examples [options]   builds the examples (with options)
  clean                cleans Claro project; removes generated files
  install [prefix]     installs the Claro library; requires sudo
""", CLARO_VERSION + ' ' * (44-len(CLARO_VERSION)), sys.version)

def main(args):
  if len(args) == 0:
    print(HELP)
  else:
    i = 0
    while args[i][:1] == "-":
      a = args[i]
      if a in ("--force", "-f", "-B", "-b"):
        global force
        force = true
      elif a in ("-h", "--help", "-?"):
        print(HELP)
        return
      elif a == "--diff":
        global GENERATE_DIFF
        GENERATE_DIFF = true
      elif a == "--no_fpc":
        global USE_FPC
        USE_FPC = false
      else:
        Error("illegal option: " + a)
      i = i + 1
    cmd = args[i]
    cmdArgs = join(args[i+1:])
    if cmd == "claro": cmd_claro(cmdArgs)
    elif cmd == "examples": cmd_examples(cmdArgs)
    elif cmd == "clean": cmd_clean()
    elif cmd == "install": cmd_install(cmdArgs)
    else: Error("illegal command: " + cmd)
    
def cmd_install(prefix):
  # XXX to implement
  pass
  
# ------------------------------ clean ----------------------------------------

CLEAN_EXT = "o obj"

def cmd_clean(dir = "."):
  L = []
  for x in split(CLEAN_EXT):
    L.append(r".*\."+ x +"$")
  extRegEx = re.compile(join(L, "|"))
  if Exists("koch.dat"): Remove("koch.dat")
  for f in Glob("*.pdb"): Remove(f)
  for f in Glob("*.idb"): Remove(f)
  for f in Glob("web/*.html"): Remove(f)
  for f in Glob("doc/*.html"): Remove(f)
  def visit(extRegEx, dirname, names):
    if os.path.split(dirname)[1] == "nimcache":
      shutil.rmtree(path=dirname, ignore_errors=true)
      del names
    else:
      for name in names:
        x = os.path.join(dirname, name)
        if os.path.isdir(x): continue
        if extRegEx.match(name):
          Echo("removing: " + x)
          Remove(x)
  os.path.walk(dir, visit, extRegEx)

def mydirwalker(dir, L):
  for name in os.listdir(dir):
    path = os.path.join(dir, name)
    if os.path.isdir(path):
      mydirwalker(path, L)
    else:
      L.append(path)

if __name__ == "__main__":
  main(sys.argv[1:])
