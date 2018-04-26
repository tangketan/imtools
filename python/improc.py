# 1. Put this file in Python/Lib so python could find it
# 2. Create a file named .pythonstart and an environment variable PYTHONSTARTUP pointing to it
# 3. The content in .pythonstart will be executed every time python is called from command line

import cv2
import glob


def im2bin(imname, binname):
    im = cv2.imread(imname, 0)
    im.tofile(binname) # could also use im.tostring()


def im2pgm(imname, pgmname):
    im = cv2.imread(imname, 0)
    cv2.imwrite(pgmname, im)


def ims2bin(folder, ext):
    onlyfiles = glob.glob(folder + "/*." + ext)
    for imname in onlyfiles:
        im2bin(imname, imname[0:imname.rfind(ext)] + 'bin')

