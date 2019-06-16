#!/usr/bin/env python

import sys

def getVar(n,k):
	return (n)*nbColors+k+1

filename = sys.argv[1]
option = sys.argv[2]

splitname = filename.split('/')[-1].split('.')
nbColors = int(splitname[-1])

if option == "sym":
	fyle = open(filename)
	for lyne in fyle:
		if lyne[:1] == 'p':
			tokens = lyne.split()
			nbNodes = int(tokens[2])
			print "rows",nbColors,"columns",nbNodes*2
			for j in range(0,nbColors):
				for i in range(0,nbNodes):
					print getVar(i,j),-getVar(i,j),
				print ""
	fyle.close()
	sys.exit(0)

# else:

print "c Hopefully unsat graph coloring CNF"
print "c Trying to color graph", 
for i in range(0,len(splitname)-2):
	print splitname[i], 
print "with", nbColors, "colors."
print "c Variable (n*colors)+k tries to color node n by color k"
print "c so variable 1 is the first node colored by the first color"
print "c signifying nodes start at 0 and colors at 1"

fyle = open(filename)
for lyne in fyle:
	if lyne[:1] == 'c':
		3==3
		# ignore
	elif lyne[:1] == 'p':
		tokens = lyne.split()
		nbNodes = int(tokens[2])
		nbEdges = int(tokens[3])
		boolvars = nbNodes*nbColors
		nbClauses = nbNodes+nbEdges*nbColors
		print "p","cnf",boolvars,nbClauses
		for i in range(0,nbNodes):
			for j in range(0,nbColors):
				print getVar(i,j),
			print 0
	elif lyne[:1] == 'e':
		tokens = lyne.split()
		n1 = int(tokens[1])-1
		n2 = int(tokens[2])-1
		for k in range(0,nbColors):
			print -getVar(n1,k),-getVar(n2,k),0
	else:
		print "ERROR: unexpected line!"
		print lyne
		sys.exit(1)
fyle.close()

# c FILE: anna.col
# c Translated from Stanford GraphBase File: anna.gb
# c Stanford GraphBase ID: book(?anna?,138,0,1,239,0,0,0)
# p edge 138 986
# e 1 36
