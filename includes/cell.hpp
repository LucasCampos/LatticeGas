#ifndef CELL_HPP
#define CELL_HPP
#include "rules.hpp"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>

inline int RightNeigh    (int i, int L, int N) { 
	if ((i+1)%L == 0) i-= L;
	return i+1;
}
	
inline int LeftNeigh     (int i, int L,int N) { 
	if (i%L == 0) i+= L;
	return i-1;
}

inline int RightUpNeigh  (int i, int L, int N) {
	i+=L;
	if (i>=N) i-=N;
	return i;
}


inline int RightDownNeigh(int i, int L, int N) { 
	i-= (L-1);
	if (i<0) i+= N;
	return i;
}

inline int LeftUpNeigh (int i, int L, int N) { 
	i+= (L-1);
	if (i>=N) i-=N;
	return i;
}

inline int LeftDownNeigh   (int i, int L, int N) {
	i-=L;
	if (i<0) i+=N;
	return i;
}

class Cell {
	private:
	public: 
		int current;
		double x,y;
		Cell(int _current=0): current( _current){
		}
		~Cell() {
		}
		int Update(const std::vector<Cell>& lat, int i, int L, int N) const {
			int newCurr = current & (BARRIER|STATIONARY);
			newCurr |= lat[RightNeigh(i,L,N)].current & LEFT;
			newCurr |= lat[RightDownNeigh(i,L,N)].current & LEFT_UP;
			newCurr |= lat[LeftDownNeigh(i,L,N)].current & RIGHT_UP;

			newCurr |= lat[LeftNeigh(i,L,N)].current & RIGHT;
			newCurr |= lat[LeftUpNeigh(i,L,N)].current & RIGHT_DOWN;
			newCurr |= lat[RightUpNeigh(i,L,N)].current & LEFT_DOWN;
			return rules[newCurr];
		}
		Cell& operator=(const Cell& c) {
			current = c.current;
			return (*this);
		}

		int TotalDensity() const {
			int den=0;
			int curr = current;
			while (curr > 0) {
				den += curr&1;
				curr = curr >> 1;
			}
			return den;
		}

		int isThere(int cond)const {
			return ((current&cond)/cond);
		}

		double MomentumX() const {
			double momentum=0;
			momentum +=  isThere(RIGHT);
			momentum += -isThere(LEFT);

			momentum +=  cos(M_PI/3.0)*isThere(RIGHT_UP);
			momentum += -cos(M_PI/3.0)*isThere(LEFT_UP);

			momentum +=  cos(M_PI/3.0)*isThere(RIGHT_DOWN);
			momentum += -cos(M_PI/3.0)*isThere(LEFT_DOWN);
			return momentum;
		}

		double MomentumY() const {
			double momentum =0;
			momentum += sin(M_PI/3.0)*isThere(RIGHT_UP);
			momentum += sin(M_PI/3.0)*isThere(LEFT_UP);

			momentum += -sin(M_PI/3.0)*isThere(RIGHT_DOWN);
			momentum += -sin(M_PI/3.0)*isThere(LEFT_DOWN);
			return momentum;
		}

		/*
		 * Indexes:
		 * RIGHT 0
		 * RIGHT_DOWN 1
		 * LEFT_DOWN 2
		 * LEFT 3
		 * LEFT_UP 4
		 * RIGHT_UP 5
		 */

		friend std::ostream& operator<<(std::ostream& stream,const Cell& c){

			stream << c.x << "   " << c.y << "   " << c.TotalDensity() << "\t";
			return stream;
		}
};



#endif
