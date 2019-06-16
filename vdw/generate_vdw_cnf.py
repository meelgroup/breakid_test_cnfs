#!/usr/bin/env python

import sys

def getVar(x,y):
	return x*r+y+1

if len(sys.argv)<4 or len(sys.argv)>5:
  print "wrong number of arguments:",len(sys.argv)
  print "use arguments: r k N [\"sym\"]"
  exit(1)

r = int(sys.argv[1]) # number of colors/partitions
k = int(sys.argv[2]) # length of arithmetic progression sequence
N = int(sys.argv[3]) # max N

option = ""	
if len(sys.argv)==5:
  option = sys.argv[4]
  
if option == "sym":
  print "rows",r,"columns",N*2 # print row interchangeability sym
  for j in range(0,r): 
    for i in range(0,N):
      print getVar(i,j),-getVar(i,j),
    print ""
  for i in range(0,N/2): # print reflective symmetry
    for j in range(0,r):
      print "(",getVar(i,j),getVar(N-i-1,j),")","(",-getVar(i,j),-getVar(N-i-1,j),")",
  print ""
  sys.exit(0)

numclauses = N
for v in range(0,N):
	for d in range(1,N):
		if v+(k-1)*d < N: # count restricting clause
			numclauses+=r

print "c van der Waerden CNF for r =", r, "k =",k,"N =",N
print "p","cnf",N*r,numclauses

for i in range(0,N):
	for j in range(0,r):
		print getVar(i,j),
	print 0

for v in range(0,N):
	for d in range(1,N):
		if v+(k-1)*d < N: # print restricting clause
			for j in range(0,r):
				for i in range(0,k):
					print -getVar(v+d*i,j),
				print 0
