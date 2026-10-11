/*
  solution.hpp

 Author: 
  Sebastien Verel, 
  Univ. du Littoral Côte d'Opale, France.

 */

#ifndef _solution_h
#define _solution_h

#include <iostream>
#include <sstream>
#include <vector>

class Solution {
public:
    Solution(int _d = 0, int _n = 0) : d(_d), n(_n) {
      z.resize(d);
      x.resize(n);
    }

    Solution(const Solution & source) {
      z.resize(source.z.size());

      for(int u = 0; u < z.size(); u++)
        z[u] = source.z[u];

      x.resize(source.x.size());

      for(int i = 0; i < x.size(); i++)
        x[i] = source.x[i];

      f = source.f;
    }

    Solution& operator=(const Solution & source) {
      z.resize(source.z.size());

      for(int u = 0; u < z.size(); u++)
        z[u] = source.z[u];

      x.resize(source.x.size());

      for(int i = 0; i < x.size(); i++)
        x[i] = source.x[i];

      f = source.f;

      return *this;
    }

    // vector of continuous variables
    int d;
    std::vector<double> z;

    // vector of binary variables
    int n;
    std::vector<bool> x;

    // objective value
    double f;
};

// ouput stream
std::ostream & operator<<(std::ostream & os, const Solution & solution) {
  // number of digits
  os.precision(6);

  os << std::fixed << solution.f ;

  os << " \"" ;

  if (solution.z.size() > 0) {    
    os << std::fixed << solution.z[0] ;

    for(unsigned int i = 1; i < solution.z.size(); i++)
      os << ',' << std::fixed << solution.z[i] ;
  }

  os << "\" " ;
 
  if (solution.x.size() > 0)
    for(unsigned int i = 0; i < solution.x.size(); i++)
      if (solution.x[i])
        os << "1" ;
      else
        os << "0" ;
  else {
    os << 'e' ;
  }

  return os;
}

// input stream
std::istream & operator>>(std::istream & is, Solution & solution) {
  is >> solution.f ;

  // Continous part
  std::string s;
  is >> s;

  if (s.length() == 2) { // empty only ""
    solution.z.resize(0);    
  } else {
    std::vector<std::string> sequence;

    int pos = 0;
    int last;

    while (pos < s.length()) {
        pos++;
        last = pos;
        while (pos < s.length() and (s[pos] != ',' and s[pos] != '\"')) 
          pos++;
        
        if (pos < s.length())
          sequence.push_back(s.substr(last, pos - last));
    }

    solution.z.resize(sequence.size());

    for(int i = 0; i < sequence.size(); i++) {
        std::istringstream ss( sequence[i] );
        double zi;
        ss >> zi ;
        solution.z[i] = zi;
    }
  }

  // binary part
  std::string bit_string;
  is >> bit_string;

  if (bit_string.size() == 1 and bit_string[0] == 'e') { // empty only e
    solution.x.resize(0);
  } else {
    solution.x.resize(bit_string.size());

    for(unsigned int i = 0; i < solution.x.size(); i++)
      solution.x[i] = (char) bit_string[i] - (char) '0'; // robust way to do?
  }

  return is;
}

#endif