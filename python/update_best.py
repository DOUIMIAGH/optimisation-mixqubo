#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
	Update the best known solution

	Author: 
		Sébastien Verel

	Date: 
		2026-03-31

	Licence:
		CC-by
"""

import argparse
import pandas as pd

import mixQUBO as mxq
from solution import Solution

def decode_x(s, solution):
	solution.n = len(s)
	solution.x = [ 0 ] * solution.n

	for i in range(solution.n):
		solution.x[i] = ord(s[i]) - ord('0')

def decode_z(s, solution):
	t = s.split(',')

	solution.d = len(t)
	solution.z = [ 0.0 ] * solution.d

	for i in range(solution.d):
		solution.z[i] = float(t[i])

def read_bk(filename_bk):
	bk = pd.read_csv(filename_bk, sep = ' ', header = 0)

	best_known = [ None ] * 16

	for index, row in bk.iterrows():
		#print(row['id'], row['f'], row['z'], row['x'])

		solution = Solution(d = 0, n = 0)

		decode_z(row['z'], solution)
		decode_x(row['x'], solution)
		solution.f = row['f']

		best_known[ row['id'] ] = solution

	return best_known

def read_candidate(filename_results):
	new = pd.read_csv(filename_results, sep = ' ', header = 0)

	f = mxq.MixQUBO()
	previous_id = -1

	candidate = [ None ] * 16

	for index, row in new.iterrows():
		#print(row['id'], row['f'], row['z'], row['x'])

		id = row['id']

		if id != previous_id:
			f.read_json("../instances/mxqubo_v0_%d.json" % row['id'])
			previous_id = row['id']

		solution = Solution(d = f.d, n = f.n)

		decode_z(row['z'], solution)
		decode_x(row['x'], solution)

		f.eval(solution)

		if candidate[id] == None:
			candidate[id] = solution
		else:
			if solution.f < candidate[id].f:
				candidate[id] = solution

	return candidate

def main(filename_bk, filename_results, filename_newbk):
	best_known = read_bk(filename_bk)
	
	"""
	for s in best_known:
		print(s)
	"""

	candidate = read_candidate(filename_results)

	"""
	for s in candidate:
		print(s)
	"""

	# Compare
	with open(filename_newbk, "w") as f:
		f.write('id f z x\n')
		for i in range(len(candidate)):
			f.write(str(i) + ' ')
			if candidate[i] != None:
				if candidate[i].f < best_known[i].f:
					print("New best instance", i, ":", candidate[i].f, "(previously", best_known[i].f, ")")
					best_known[i] = candidate[i]
			f.write(str(best_known[i]) + '\n')

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="Update best know solutions. Each file with 4 columns: id f z x.")
	parser.add_argument("--bk", "-b", type=str, required=True, help="File name of the best known solutions")
	parser.add_argument("--results", "-i", type=str, required=True, help="File name of new candidate solutions")
	parser.add_argument("--newbk", "-o", type=str, required=True, help="File name of updated best known solutions")
	args = parser.parse_args()

	main(args.bk, args.results, args.newbk)