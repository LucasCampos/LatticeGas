#include "lattice.hpp"
#include "graphicsBase.hpp"
#include <iostream>
#include <cstdlib>
#include <GL/glfw.h>
#include <fstream>

int UniformBetween(int nMax) {return rand()%nMax;}
using namespace std;

bool arrows = false;
bool arrowsSimple = true;
bool normal = false;
bool vorticity = false;

void setFalse() {
	arrows = false;
	arrowsSimple = false;
	normal = false;
	vorticity = false;
}

static void GLFWCALL KeyCallback(int key, int action){
	if( action != GLFW_PRESS )
	{
		return;
	}


	switch (key)
	{
		case GLFW_KEY_ESC:
			glfwTerminate();
			exit(0);
			break;
		case 'A':
			setFalse();
			arrows = true;
			break;
		case 'S':
			setFalse();
			arrowsSimple = true;
			break;
		case 'N':
			setFalse();
			normal = true;
			break;
		case 'V':
			setFalse();
			vorticity = true;
			break;
	}

}

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

int main() {
	double factorTriang = sqrt(3.0)/2;
	srand(time(NULL));
	SetRules();
	Lattice lat(400, .5, .5, 40);
	Graphics::GraphicsBase2D graphics(800*sqrt(3.0)/2, 800, 0, 1.0, 0, sqrt(3.0)/2, 0.01, "Gas Lattice", true);
	glfwSetKeyCallback(KeyCallback);

	InitAll(lat.cells1,RIGHT);
	//MakeRec(lat.cells1, .25, .25*factorTriang, .75, .75*factorTriang, FULL);
	
	//Faz barreira de cima e de baixo
	//MakeRec(lat.cells1, 0, 0.98*factorTriang, 1, 1.0*factorTriang, BARRIER);
	//MakeRec(lat.cells1, 0, 0.00*factorTriang, 1, 0.02*factorTriang, BARRIER);

	//MakeCircle(lat.cells1, 0.3, 0.5*factorTriang, 0.1, BARRIER);
	MakeRec(lat.cells1, .3, .3*factorTriang, .32, .7*factorTriang, BARRIER);

	while(true){
		graphics.Clear();
		if (arrowsSimple)
			lat.DrawReducedArrows();
		else if (normal)
			lat.Draw();
		else if (arrows)
			lat.DrawArrows();
		else if (vorticity)
			lat.DrawVorticity();

		for (int j=0; j<5; j++) {
			lat.FullUpdate();

		}
		graphics.EndFrame();
	}

}
