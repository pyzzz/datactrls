#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
import os
import time
sys.stdout = os.fdopen(sys.stdout.fileno(), "wb", 0) #-u
MAX_RANGE = 1000000

def test_dict():
	#[2012-02-01] set 1.05s get 0.75s clear 0.11s
	print "---- test_dict -----"
	start = time.time()
	print "test dict set (%d)...\t"%MAX_RANGE,
	d = {}
	for i in xrange(MAX_RANGE):
		s = str(i)
		d[s] = s
	print "%.2fs"%(time.time()-start)
	start = time.time()
	print "test dict get (%d)...\t"%MAX_RANGE,
	for i in xrange(MAX_RANGE):
		s = d[str(i)]
	print "%.2fs"%(time.time()-start)
	start = time.time()
	print "test dict clear (%d)...\t"%MAX_RANGE,
	d.clear()
	print "%.2fs"%(time.time()-start)
	print

def test_list():
	#[2012-02-04] set 0.02s get 0.09s each 0.09s
	start = time.time()
	print "---- test_list -----"
	print "test dict set (%d)...\t"%MAX_RANGE,
	l = range(MAX_RANGE)
	print "%.2fs"%(time.time()-start)
	start = time.time()
	print "test dict get (%d)...\t"%MAX_RANGE,
	for i in xrange(MAX_RANGE):
		l[i]
	print "%.2fs"%(time.time()-start)
	start = time.time()
	print "test dict each (%d)...\t"%MAX_RANGE,
	for i in l: i+=1 #no effect
	print "%.2fs"%(time.time()-start)
	print

if __name__ == "__main__":
	test_dict()
	test_list()