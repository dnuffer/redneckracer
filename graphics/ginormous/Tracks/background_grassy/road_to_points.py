#!/usr/bin/env python
# 2011 FelenaSoft company.
#
# For use this script, you need to install Python Imaging Library (PIL)
# http://www.pythonware.com/products/pil/
#
# Output file format:
#
# scaleFactorX scaleFactorY										- first line
# yCoordinate borderLeftXcoordinate borderRightXcoordinate		- second and next lines
# ....
#
# @created 2011-04-19 Petrov Alexander

import os, sys, Image

def in_range(sample, value, rang):
	if (value > (sample -rang)) and (value < (sample + rang)):
		return True
	else:
		return False


def color_in_range(samplecolor, val, rang):
	if in_range(samplecolor[0], val[0], rang):
		if in_range(samplecolor[1], val[1], rang):
			if in_range(samplecolor[2], val[2], rang):
				return True
	return False


# !!! Change these parameters:
# !!! These variables set to use with "background_grassy" png images.
distance = 100 # interval between Y coordinates of road border points.
colorRange = 20 # "+-" differences between target and real color.
borderColor = [153, 153, 153] # target road border color (asphalt color).
trackPrefix = 'background_' # prefix for png images, representing track.
scaleFactorX = 1.0  # ScaleFactor must be equivalent to background race image scale factor in game.
scaleFactorY = 1.0

def createPointsForImage(unused, dirname, filenames):
	for fname in filenames:
		fullname=os.path.join(dirname, fname)
                print "fullname is ", fullname
		if fname.lower().startswith(trackPrefix.lower()):
			if fname.lower().endswith('.png'):
				print "Analize file: " + fullname + '\n'
				outfile = fullname[:-4] + ".road"
				im = Image.open(fullname)
				im = im.convert('RGBA')

				border = []
				for y in xrange(im.size[1] - 1, 0, -distance): # y changing from height to 0 , because in  background coordinates 0, 0 - left bottom corner
					#founding left border
					for x in xrange(im.size[0]):
						colorkey = im.getpixel((x,y))
						if color_in_range(borderColor, colorkey, colorRange):
							#print "Left x found!"
							border.append(im.size[1] - y)
							border.append(x)
							#founding right border
							for x2 in xrange(im.size[0]-1, x, -1):
								colorkey = im.getpixel((x2,y))
								if color_in_range(borderColor, colorkey, colorRange):
									#print "Right x found!"
									border.append(x2)
									break
							break

				print "Write output file: " + outfile +'\n'
				outputfile = open(outfile, 'w')
				sYcoord = 0
				sXLcoord = 0
				sXRcoord = 0
				outputfile.write(str(scaleFactorX))
				outputfile.write(" ")
				outputfile.write(str(scaleFactorY))
				outputfile.write("\n")
				for i in xrange(0, len(border), 3):
					sYcoord = str(border[i])
					sXLcoord = str(border[i+1])
					sXRcoord = str(border[i+2])
					outputfile.write(sYcoord)
					outputfile.write(" ")
					outputfile.write(sXLcoord)
					outputfile.write(" ")
					outputfile.write(sXRcoord)
					outputfile.write('\n')
				#write last line with Y = image height
				outputfile.write(str(im.size[1]))
				outputfile.write(" ")
				outputfile.write(sXLcoord)
				outputfile.write(" ")
				outputfile.write(sXRcoord)
				outputfile.write('\n')
				outputfile.close()


# This requires that the first argument is the base path to find images
os.path.walk(sys.argv[1], createPointsForImage, None)

print "Done"
#input("Press any key to exit.")
