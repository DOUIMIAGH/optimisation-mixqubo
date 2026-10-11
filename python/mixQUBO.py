#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
	Mixed QUBO function V0

	Author: 
		Sébastien Verel

	Date: 
		2026-03-27

	Licence:
		CC-by
"""

import math
import json

class MixQUBO:
	"""
		Input:
			d: dimension of the continuous space
			n: dimension of the binary space
		
	"""
	def __init__(self, d = 0, n = 0):
		self.d = d
		self.n = n

		# list of terms
		self.terms = []

	"""
		Define a term

		Input:
			u,v : id of the continuous variables
			i,j : id of the binary variables
			c   : vector of dim 2 of the position of the maximum
			m   : minimum value before scaling
			J   : value of the intensity h(z,x) in [-J, J]
	"""
	def addTerm(self, u, v, i, j, c, m, J):
		d2_max = 0
		if c[0] >= 0:
			d2_max = (-1 - c[0])**2
		else:
			d2_max = (1 - c[0])**2
		if c[1] >= 0:
			d2_max += (-1 - c[1])**2
		else:
			d2_max += (1 - c[1])**2

		# exponent
		alpha = - math.log(m) / d2_max

		# shift
		K = (1 + m) / 2

		# scaling
		A = 2 / (1 - m)

		self.terms.append( {
			'u':u, 'v':v, 'i':i, 'j':j, 
			'c':c, 'm':m, 'J':J,
			'alpha': alpha, 'K':K, 'A':A
			} )

	"""
		Elementary function

		Input:
			id : id the elementary function to define parameters
			z  : vector of continuous variable from [-1, 1]
			x  : vector of binary variable from {0, 1}
		Output:
			value of the function
	"""
	def h(self, id, z, x):
		param = self.terms[id]

		kernel = param['J'] * param['A'] * \
			(math.exp( -param['alpha'] * \
				( (z[ param['u'] ] - param['c'][0])**2 + (z[ param['v'] ] - param['c'][1])**2) ) \
			 - param['K'])

		if x[ param['i'] ] == x[ param['j'] ]:
			return kernel
		else:
			return -kernel

	"""
		Evaluation function

		Input:
			z  : vector of continuous variable from [-1, 1]
			x  : vector of binary variable from {0, 1}
		Output:
			value of the function
	"""
	def eval(self, solution):
		f = 0

		for i in range(len(self.terms)):
			f += self.h(i, solution.z, solution.x)

		solution.f = f

	def to_json(self, info = None):
		s = "{\"problem\":{"

		if info != None:
			s += "\"info\":" + info + ", "
		else:
			s += "\"info\":{}, "

		s += "\"d\":%d, \"n\":%d, \"terms\":" % (self.d, self.n)

		s += json.dumps(self.terms, separators=(',', ':'))

		s += "}}"

		return s	

	"""
		Read the instance in json format string
	"""
	def from_json(self, json_string):
		data = json.load(json_string)

		self.d = data['problem']['d']
		self.n = data['problem']['n']

		# list of terms
		self.terms = data['problem']['terms']

	def write_json(self, file_name, info = None):
		json_file = open(file_name, "w")
		json_file.write(self.to_json(info))
		json_file.close()

	def read_json(self, file_name):
		with open(file_name, 'r+') as f:
		    self.from_json(f)

	def __str__(self):
		s = "{\"d\":%d, \"n\":%d, \"terms\":" % (self.d, self.n)

		s += json.dumps(self.terms, separators=(',', ':')) + '}'

		return s

