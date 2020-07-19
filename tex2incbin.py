import sys
import os
from PIL import Image
import struct

def writeu8(val, outf):
    outf.write(struct.pack("B", val))

if __name__ == "__main__":
    #print("ARGV: {}".format(sys.argv))
    if len(sys.argv) < 2:
        print("{}: Expected input file".format(sys.argv[0]))
        exit
    inName = sys.argv[1]
    fSize = os.path.getsize(inName)
    baseName = os.path.splitext(inName)[0]
    outName = baseName + ".c"

    with open(outName, "w") as outf:
        im = Image.open(inName)
        pix = im.load()
        
        outf.write("#define INCBIN_PREFIX\n")
        outf.write('#include "incbin.h"\n')
        outf.write("\n")
        outf.write("unsigned int {}Width = {};\n".format(baseName, im.size[0]))
        outf.write("unsigned int {}Height = {};\n".format(baseName, im.size[1]))
        
        outf.write('INCBIN({}, "{}.bin");\n'.format(baseName, baseName))

    with open(baseName + ".h", "w") as outf:
        outf.write("#ifndef __{}__\n".format(baseName))
        outf.write("#define __{}__\n".format(baseName))

        outf.write("#define INCBIN_PREFIX\n")
        outf.write('#include "incbin.h"\n')
        outf.write("\n")
        outf.write("unsigned int {}Width;\n".format(baseName))
        outf.write("unsigned int {}Height;\n".format(baseName))

        outf.write("INCBIN_EXTERN({});\n".format(baseName))
        outf.write("#endif\n")

    with open(baseName + ".bin", "wb") as outf:
        for y in range(im.size[1]):
            for x in range(im.size[0]):
                r,g,b,a = pix[x,y]
                a = a >> 1
                writeu8(r, outf)
                writeu8(g, outf)
                writeu8(b, outf)
                writeu8(a, outf)

