#!/usr/bin/env python
# -*- coding: utf-8 -*-
import time
MAX_RANGE = 1000000

def test_dict():
	#[2012-01-31] set 1.05s get 0.75s
	#[2012-02-01] set 1.05s get 0.75s clear 0.11s
	start = time.time()
	print "test set"
	d = {}
	for i in xrange(MAX_RANGE):
		s = str(i)
		d[s] = s
	print "%.2fs"%(time.time()-start)
	start = time.time()
	print "test get"
	for i in xrange(MAX_RANGE):
		s = d[str(i)]
	print "%.2fs"%(time.time()-start)
	start = time.time()
	print "test clear"
	d.clear()
	print "%.2fs"%(time.time()-start)

if __name__ == "__main__":
	test_dict()