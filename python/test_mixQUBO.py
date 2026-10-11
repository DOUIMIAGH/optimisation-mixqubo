#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
	Test of Mixted QUBO function V0

	Author: 
		Sébastien Verel

	Date: 
		2026-03-27

	Licence:
		CC-by
"""

import mixQUBO as mxq
from solution import Solution

def main():
	f = mxq.MixQUBO()
	f.read_json("../instances/mxqubo_test.json")

	solution = Solution(d = f.d, n = f.n)

	solution.z = [-0.5, 0.0, 0.2]
	solution.x = [0, 1, 1, 0]

	f.eval(solution)

	print(solution)


if __name__ == "__main__":
	main()