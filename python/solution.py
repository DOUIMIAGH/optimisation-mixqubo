
class Solution:
	"""
		Vector of continuous variables from [-1, 1]
		Vector of binary variables from {0, 1}.

		Input:
			d: dimension of the continuous space
			n: dimension of the binary space
	"""
	def __init__(self, d = 0, n = 0):
		self.d = d
		self.n = n

		# vector of continuous variables from [-1, 1]
		self.z = [ 0.0 ] * d

		# vector of binary variables from {0, 1}
		self.x = [ 0 ] * n

		# f-value
		self.f = 0.0

	def __str__(self):
		s = "%.6f \"" % (self.f)

		if self.d > 0:
			s += "%.6f" % self.z[0]
			for i in range(1, self.d):
				s += ",%.6f" % self.z[i]

		s += "\" "

		if self.n > 0:
			for i in range(self.n):
				s += "%d" % self.x[i]
		else:
			s += "e"

		return s

