#include "lattice.hpp"
#include <iostream>
#include <cstdlib>

int UniformBetween(int nMax) {return rand()%nMax;}
using namespace std;


void MakeCircle(std::vector<Cell> &c, double xC, double yC, double radius, unsigned state) {

	for (int i=0; i<c.size(); i++) {
		double dx = c[i].x - xC;
		double dy = c[i].y - yC;
		double r = sqrt(dx*dx+dy*dy);

		if ( r < radius){
			c[i].current = state;
		}

	}

}

void InitAll(std::vector<Cell> &c, unsigned s) {
	for (int i=0; i<c.size(); i++) 
		c[i].current= s;
}

void MakeRec(std::vector<Cell> &c, double x0, double y0, double xF, double yF, unsigned s) {

	auto swp = [](double &a, double& b) {double c=a;a=b;b=c;};
	if (x0>xF) swp(x0,xF);
	if (y0>yF) swp(y0,yF);

	for (int i=0; i<c.size(); i++){
		if ((c[i].x > x0) && (c[i].x < xF))
			if ((c[i].y > y0) && (c[i].y < yF)){
					c[i].current = s;
			}
	}

}

int main(int argc, char** argv) {
	double factorTriang = sqrt(3.0)/2;
	const int mult = atoi(argv[1]);
	Lattice lat(192*mult, 5, .5, 10);

	InitAll(lat.cells1,RIGHT);
	MakeRec(lat.cells1, .3, .3*factorTriang, .32, .7*factorTriang, BARRIER);

	for (int i=0; i<1e5; i++){
		lat.FullUpdate();
	}
}
