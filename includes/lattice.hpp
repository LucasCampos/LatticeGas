#ifndef LATTICE_HPP
#define LATTICE_HPP
#include "cell.hpp"
#include <cmath>
#include <GL/glfw.h>
#include <vector>
#include <fstream>
#include <array>

class Lattice {
	public:
		int L;
		int N;
		int scale;
		double flowSpeed;
		int nSquares;
		std::vector<Cell> cells1; 
		std::vector<Cell> cells2; 
		Lattice(int _L=100, int _s=4, double _fS=.5, int _nS=20): scale(_s), flowSpeed(_fS), nSquares(_nS) {
			N=_L*_L;
			L=_L;
			cells1.resize(N);
			cells2.resize(N);
			MakeTriangularLatticePos();
		}
		~Lattice() {
		}

		void MakeTriangularLatticePos() {
			int n =L;
			double ybox=1;

			double dx=0, dy=0, del = ybox/n;

			int counter =0;
			double dxbase = 0;

			while (counter < N) {

				for (int i=0; i<n; i++) {
					cells1[counter].x = dx;
					cells1[counter].y = dy;
					dx+=del;
					counter++;
				}
				dy += del*sqrt(3.0)/2;
				dxbase += del/2;
				dx = dxbase;
			}

			std::ofstream posiIniti("posicaoInicial.dat");

			for (int i=0; i<N; i++){
				if (cells1[i].x > 1) cells1[i].x -= 1;
				if (cells1[i].y > 1) cells1[i].y -= 1;
				cells2[i].x = cells1[i].x;
				cells2[i].y = cells1[i].y;
				posiIniti << cells2[i].x << "   " << cells2[i].y << std::endl;

			}
		};

		void UpdateLattice(const std::vector<Cell>& inCell, std::vector<Cell>& outCell) {
			for (int i=0; i<N; i++) {
				//std::cout << i << std::endl;
				outCell[i].current = inCell[i].Update(inCell,i, L, N);
				if (outCell[i].x < 0.01) outCell[i].current = RIGHT;
				//if ((outCell[i].y > 0.99) || (outCell[i].y < 0.01))  outCells[i].current =0;
			}
		}

		void FullUpdate() {
			//std::cout << "here" << std::endl;
			UpdateLattice(cells1, cells2);
			//Inject(cells2);
			UpdateLattice(cells2, cells1);
			//Inject(cells1);
			
		}

		void Inject(std::vector<Cell>& cells) { 
			const int numParticles = N;
			const int vxTotal = TotalMomentumX(cells);
			int injections = (flowSpeed*numParticles-vxTotal)/scale;
			//std::cout << injections << std::endl;

			for(int k = 0;k<abs(injections);k++) {
				int i = (rand()%N); // choose site at random
				// flip direction of horizontally moving particle if possible
				if ((cells[i].current & BARRIER) != 0) continue;
				if((cells[i].current&(RIGHT|LEFT))==((injections>0) ? LEFT : RIGHT)) {
					cells[i].current ^= RIGHT|LEFT;
				}   
			} 
		}


		static void Seta(double xInit, double yInit, double dx, double dy) {
			const double xFinal = xInit + dx;
			const double yFinal = yInit + dy;
			glBegin(GL_LINES);
			glVertex2d(xInit, yInit);
			glVertex2d(xFinal, yFinal);
			glEnd();
		}

		static void RetanguloCentrado(double xcentro, double ycentro, double b, double h) {

			glBegin(GL_QUADS);
			glVertex2d(xcentro-0.5*b, ycentro+0.5*h);
			glVertex2d(xcentro+0.5*b, ycentro+0.5*h);
			glVertex2d(xcentro+0.5*b, ycentro-0.5*h);
			glVertex2d(xcentro-0.5*b, ycentro-0.5*h);
			glEnd();
		}

		static void CentredHexagon(double xcentre, double ycentre, double radius) {
			glBegin(GL_TRIANGLE_FAN);
			glVertex2d(xcentre, ycentre);
			const double angleLa = 2.0*M_PI/6.0;
			for (int i=0; i<7; i++) {
				const double angle = i*angleLa;
				glVertex2d(xcentre+cos(angle)*radius, ycentre+sin(angle)*radius);
			}
			glEnd();
		}

		//First two doubles refer to the position, last two to momentum
		
		std::vector<std::array<double,4>> MomentumAvg(const std::vector<Cell>& cells) const {

			std::vector<std::array<double,4>> result;
			int idxMax = L/nSquares;
//#ifdef DEBUG
			if (L%nSquares != 0) std::cout << "Warning, dividing grids into unbalanced number os squares\n";
//#endif
			//Iterates over all grid
			for (int currI = 0; currI < L; currI+= idxMax){
				for ( int currJ = 0; currJ < L; currJ+=idxMax) {
					//Iterates over each square
					std::array<double, 4> pos4d;
					pos4d.fill(0);
					int counter =0;
					for (int i=currI; i<(currI+idxMax); i++){
						for (int j=currJ; j<(currJ+idxMax); j++) {
							const int currPos = j*L+i;
							//std::cout << i << "   " << j << "   " << currPos << std::endl;
							pos4d[0]+=cells[currPos].x; pos4d[1]+=cells[currPos].y;

							pos4d[2]+=cells[currPos].MomentumX(); pos4d[3]+=cells[currPos].MomentumY();
							counter ++;
						}
					}
					//std::cout << counter <<std::endl;
					//std::cout << currI << "   " << currJ << std::endl;
					pos4d[0]/=(idxMax*idxMax);
					pos4d[1]/=(idxMax*idxMax);
					pos4d[2]/=(idxMax*idxMax)*nSquares;
					pos4d[3]/=(idxMax*idxMax)*nSquares;
				//	for (int i=0; i<4; i++)
				//		std::cout << pos4d[i] << "\t";
					//std::cout << std::endl;
					result.push_back(pos4d);
				}
			}
	
			return result;
		}

		void DrawReducedArrows() const {
			glColor3f(1,1,1);
			std::vector<std::array<double,4>> setas =  MomentumAvg(cells1);
		//	exit(0);
			for (int i=0; i<setas.size(); i++)
				Seta(setas[i][0],setas[i][1],setas[i][2],setas[i][3]);
			DrawBarrier();
		}


		void DrawBarrier()  const {
			const double size = 1.0/L;
			glColor3f(1.0,0.0,0);
			for (int i=0; i<N; i++) {
				if ((cells1[i].current&BARRIER) == BARRIER){
					RetanguloCentrado(cells1[i].x, cells1[i].y , size, size);
				}
			}
		}


		void Draw() const {
			const double size = 1.0/L;
			for (int i=0; i<N; i++) {
				if ((cells1[i].current&BARRIER) == BARRIER){
					glColor3f(1.0,0.0,0);
				}else {
					const float color = cells1[i].TotalDensity()/8.0;
					glColor3f(0,color,color);
				}
				RetanguloCentrado(cells1[i].x, cells1[i].y , size, size);
			}
		}

		int CalculateVorticityUpTri(int leftIdx, int rightIdx, int upIdx) const {

			int vorticity = 0;

			//Checks the base vertex
			vorticity += cells1[leftIdx].isThere(RIGHT);
			vorticity -= cells1[rightIdx].isThere(LEFT);

			//Checks the left vertex
			vorticity += cells1[rightIdx].isThere(LEFT_UP);
			vorticity -= cells1[upIdx].isThere(RIGHT_DOWN);

			//Checks the right vertex
			vorticity += cells1[upIdx].isThere(LEFT_DOWN);
			vorticity -= cells1[leftIdx].isThere(RIGHT_UP);

			return vorticity;
		}

		double CalculateVorticityDownTri(int downIdx, int rightIdx, int leftIdx) const {

			double vorticity = 0;

			//Checks the right down vertex
			vorticity += cells1[downIdx].isThere(RIGHT_UP);
			vorticity -= cells1[rightIdx].isThere(LEFT_DOWN);

			//Checks the up vertex
			vorticity += cells1[rightIdx].isThere(LEFT);
			vorticity -= cells1[leftIdx].isThere(RIGHT);

			//Checks the left down vertex
			vorticity += cells1[leftIdx].isThere(RIGHT_DOWN);
			vorticity -= cells1[downIdx].isThere(LEFT_UP);

			return vorticity;
		}

		void DrawVorticity() const {

			const double size = 1.0/L;
			glBegin(GL_TRIANGLES);
			//Begins by drawing the upward triangles
			
			for (int leftIdx = 0; leftIdx < (N-L); leftIdx++) {
				const int rightIdx = RightNeigh(leftIdx, L, N);
				const int upIdx = LeftUpNeigh(rightIdx, L, N);

				float vorticity = (CalculateVorticityUpTri(leftIdx, rightIdx, upIdx))/3.0;
				glColor3f(0,0,fabs(vorticity));
				
				//std::cout << fabs(vorticity) << std::endl;
				glVertex2f(cells1[leftIdx].x,cells1[leftIdx].y);
				glVertex2f(cells1[leftIdx].x+size,cells1[leftIdx].y);
				glVertex2f(cells1[leftIdx].x+size/2,cells1[leftIdx].y+size*sqrt(3.0/4));


			}
			
			//Now draw the downward triangles
			for (int downIdx = L; downIdx < (N-L); downIdx++) {
				const int rightIdx = RightUpNeigh(downIdx, L, N);
				const int leftIdx = LeftNeigh(rightIdx, L, N);

				float vorticity = CalculateVorticityDownTri(downIdx, rightIdx, leftIdx)/3.0;
				//std::cout << fabs(vorticity) << std::endl;
				glColor3f(0,0,fabs(vorticity));

				glVertex2f(cells1[downIdx].x,cells1[downIdx].y);
				glVertex2f(cells1[downIdx].x+size/2,cells1[downIdx].y+size*sqrt(3.0/4));
				glVertex2f(cells1[downIdx].x-size/2,cells1[downIdx].y+size*sqrt(3.0/4));
			}

			glEnd();

			DrawBarrier();
		}


		void DrawArrows() const {
			const double size = 1.0/L;
			glColor3f(1,1,1);
			for (int i=0; i<N; i+=2) {
				const double dx = cells1[i].MomentumX()*size;
				const double dy = cells1[i].MomentumY()*size;
				Seta(cells1[i].x , cells1[i].y, dx, dy);
			}

			DrawBarrier();
		}



		int TotalMass() const {
			int tot=0;
			for (int i=0; i<N; i++)
				tot+=cells1[i].TotalDensity();
			return tot;
		}
	
		double TotalMomentumX(const std::vector<Cell>& cells) const {
			double tot=0;
			for (int i=0; i<N; i++){
				tot+=cells[i].MomentumX();
			}
			return tot;
		}

		double TotalMomentumY(const std::vector<Cell>& cells) const {
			double tot=0;
			for (int i=0; i<N; i++)
				tot+=cells[i].MomentumY();
			return tot;
		}

		friend std::ostream& operator<<(std::ostream& stream,const Lattice& c){


			for (int i=0; i<c.N; i++) {
				stream << c.cells1[i] << std::endl;
			}
			return stream;
		}
};

#endif
