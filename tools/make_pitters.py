import string,os
from stat import ST_SIZE

i = 0
l = []
dirname = "."
stock_item_c = ""

def get_size(node):
	return os.stat(node)[ST_SIZE]

def read_file(fname):
	f = file(fname,"r")
	buf = f.read()
	f.close()
	return buf

filename = "%s/stock-items.c" % (dirname)
f = file(filename,"w")

for node in os.listdir("./"):
	if node[-4:] != ".png":	
		continue

	size = get_size(node)
	i += 1
	base = string.replace(node,".png","")
	l.append( ( string.replace(base,"-","_"), size ) )
	png = base+".c"
	os.system("./bin2c %s -o %s -n %s" % (node,png,base))
	f.write(read_file(png))
	f.write("\n")
print "wrote %d files..." % (i)

f.write("\n#define NUM_STOCK_ITEMS %d\n" % (i))
f.write( "\n\nstatic struct stock_img_ stock_raw_ [] = \n{\n")

for img in l:
	item = "\t{ %s, %s, %d }, \n" % (img[0]+"_source",img[0]+"_bytes",img[1])
	f.write(item)
	os.system("rm %s.c" % (string.replace(img[0],"_","-")) )
f.write("};\n")
f.close()

print "wrote %s.\ndone." % (filename)

