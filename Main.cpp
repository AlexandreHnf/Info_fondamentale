/* 000440761 - HENEFFE Alexandre
 * 000443812 - GOMES RODRIGUES Ricardo
 * 
 */
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "Solver.hpp"
#define FOR(k,lb,ub) for (int k = lb; k <= ub; k++)

Solver s;

/**
 * Pretty prints the given matrix
 * @param  matrix: matrix to print
 * @param  m: matrix height
 * @param  n: matrix width
 */


void pretty_print(int** matrix, int m, int n) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			// std::cout << matrix[i][j] << "\t";
			if (matrix[i][j] == -1 or matrix[i][j] == 0 or matrix[i][j] == 1) {
				std::cout << ("\033[1;37m■\033[0m") << "  ";
			}
			else if (matrix[i][j] == -2) {
				std::cout << ("\033[1;30m■\033[0m") << "  ";
			}
		}
		std::cout << std::endl;
	}
}

void addUnit(int** prop, int i, int j, bool b){
	if (b){
		s.addUnit(Lit(prop[i][j]));
	} else{
		s.addUnit(~Lit(prop[i][j]));
	}
}

void adjacent(int** prop, int i, int j, bool up, bool down, bool left, bool right){
	addUnit(prop, i+1, j, down);
	addUnit(prop, i-1, j, up);
	addUnit(prop, i, j-1, left);
	addUnit(prop, i, j+1, right);
}

bool isWall(int** capacities, int i, int j){
	return capacities[i][j] == -1 or capacities[i][j] == 0 or capacities[i][j] == 1;
}

bool isBorder(int m, int n, int i, int j){
	return i == 0 or j == 0 or i == m or j == n;
}

bool isWallOrBorder(int** capacities, int m, int n, int i, int j){
	return isWall(capacities, i, j) or isBorder(m, n, i, j);
}

void betweenTwoObstaclesLign(int** prop, int inf, int sup) {
	
}

/**
 * Solves the given light-up instance.
 * @param capacities: instance capacities to solve, an `m` by `n` matrix.
 * @param m: height of the instance
 * @param n: width of the instance
 * @param find_all: if true, find all valid solutions
 */  
void solve(int** capacities, int m, int n, bool find_all) {
	pretty_print(capacities, m, n);
	std::cout << std::endl;

	// Fonction à compléter pour les questions 2 et 3 (et bonus 1)
	vec<Lit> lits;

	int** prop = new int*[m+2];
	for (int i = 0; i < m+2; ++i) {
		prop[i] = new int[n+2];
		for (int j = 0; j < n+2; ++j) {
			prop[i][j] = 0;
		}
	}

	FOR(i, 1, m) {
		FOR(j, 1, n) {
			prop[i][j] = s.newVar();
		}
	}

	// contrainte 1 : Chaque mur a 0 ou 1 ampoule autour de lui
	FOR(i, 0, m-1) {
		FOR(j, 0, n-1) {
			if (capacities[i][j] == 0){
				adjacent(prop, i, j, false, false, false, false);
			} 
		}
	}

	
	// // contrainte 2
	/* FOR(i, 0, m) {
		FOR(j, 0, n) {
			if (capacities[i][j] == -1) {
				s.addBinary(~Lit(prop[i][j][AMPOULE]), ~Lit(prop[i][j][MUR]));
				s.addUnit(~Lit(prop[i][j]));
			}
		}
	}
	 */
	// contrainte 3 : 2 ampoules ne s'éclairent pas mutuellement

	// contrainte 3.1 : Lignes
	/* FOR(i, 0, m) {
		FOR(j, 0, n) {
			FOR(jprime, j, n) {
				FOR(l, j, jprime) {
					s.addTernary(Lit(prop[i][l]), ~Lit(prop[i][j]), Lit(prop[i][jprime]);
				}
			}
		}
	}

	FOR(i, 0, m) {
		FOR(j, 0, n) {
			FOR(jprime, j, n) {
				if (capacities[i][j] == )
			}
		}
	} */
	/* int j = 0;
	FOR(i, 0, m-1) {
		int jprime = 2;
		while (jprime < n) {
			if (isWall(i, jprime) or jprime == n-1) { // soit j' est à un mur soit au bord
				int l = j;
				int lprime = 1;
				while (l < jprime and lprime < jprime) { // l et l' entre j et j'
					s.addBinary(~Lit(prop[i+1][l]), ~Lit(prop[i+1][lprime]));
					lprime++;
				}
				// on a atteint une borne supérieure, donc borne inf => borne suo
				j = jprime;
			}
			jprime++; // j' + 1
		}
	} */


	

	// // contrainte 3.2 : Colonnes
	/* FOR(i, 0, m) {
		FOR(j, 0, n) {
			FOR(iprime, i, m) {
				FOR(l, i, iprime) {
					s.addTernary(Lit(prop[l][j]), ~Lit(prop[i][j]), Lit(prop[iprime][j]));
				}
			}
		}
	} */

	// contrainte 4

	// FOR(i, 0, m) {
	// 	FOR(j, 0, n) {
	// 		int jprime = j;
	// 		int iprime = i;
	// 		FOR(l, j, jprime) {
	// 			vec<Lit> lits;
	// 			s.addTernary()
	// 		}
	// 		}
	// 	}
	// }
	

	FOR(i, 0, m-1){
		vec<Lit> lits;
		std::cout << "OH ";
		FOR(j, 0, n-1){
			lits.push(Lit(prop[i+1][j+1]));
			if (isWall(capacities, i, j)){
				s.addClause(lits);
				lits.clear();
			}
		}
		if (lits.size() > 0)
			s.addClause(lits);
	}
/* 
	FOR(j, 0, n-1){
		vec<Lit> lits;
		FOR(i, 0, m-1){
			lits.push(Lit(prop[i+1][j+1]));
		}
		s.addClause(lits);
	}
 */

	// LIGNES
	/* j = 0;
	FOR(i, 0, m-1) {
		int jprime = 2;
		while (jprime < n) {
			if (isWall(i, jprime) or jprime == n-1) { // soit j' est à un mur soit au bord
				int l = j;
				while (l < jprime) { // l et l' entre j et j'
					s.addUnit(Lit(prop[i+1][l]));
					l++;
				}
				// on a atteint une borne supérieure, donc borne inf => borne suo
				j = jprime;
			}
			jprime++; // j' + 1
		}
	} */


	/* //  COLONNES
	int i = 0;
	FOR(j, 0, n-1) {
		int iprime = 2;
		while (iprime < m) {
			if (isWall(iprime, j) or j == m-1) { // soit i' est à un mur soit au bord
				int l = i;
				while (l < iprime) { // l et l' entre i et i'
					s.addUnit(Lit(prop[l][j+1]));
					l++;
				}
				// on a atteint une borne supérieure, donc borne inf => borne suo
				i = iprime;
			}
			iprime++; // i' + 1
		}
	} */







	s.solve(); 










	if (!s.okay()) {
		std::cout << "Il n'y a pas de solution sale pute de chienasse de merde ! NIQUE TA MERE" << std::endl;
	}
	else {
		std::cout << "La formule est satisfaisable sale PUTASSE YEEEEEEEEEEEEEEEES." << std::endl;
		FOR(i, 0, m-1) {
			FOR(j, 0, n-1) {
				if (s.model[prop[i][j]] == l_True) {
					std::cout << ("\033[1;33mA\033[0m") << "  ";
				} else if (capacities[i][j] == -1){
					std::cout << ("\033[1;31m*\033[0m") << "  ";
				} else if (capacities[i][j] == 0) {
					std::cout << ("\033[1;31m0\033[0m") << "  ";
				} else if (capacities[i][j] == 0) {
					std::cout << ("\033[1;31m1\033[0m") << "  ";
				} else {
					std::cout << ("\033[1;33m.\033[0m") << "  ";
				}
			}
			std::cout << "\n";
		}
	}

}

/**
 * Generates `l` instances of the light-up problem, each with a unique solution,
 * and prints them on the standard output.
 * @param instance: instance to solve, an `m` by `n` matrix.
 * @param m: height of each instance
 * @param n: width of each instance
 */
void generate(int m, int n, int l) {
	// Fonction à compléter pour la question bonus 2
}

/**
 * Prints program help message
 * @param program: program name
 */
void print_help(char* program) {
	std::cerr << "Usage:" << std::endl;
	std::cerr << program << " [-g HAUTEUR LARGEUR NOMBRE]" << std::endl;
}

int main(int argc, char** argv) {
	int m, n, l;
	bool gen = false;
	bool find_all = false;

	// Parse command line arguments
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-g") == 0) {
			gen = true;
			try {
				m = std::stoi(argv[i + 1]);
				n = std::stoi(argv[i + 2]);
				l = std::stoi(argv[i + 3]);
			} catch (const std::logic_error& e) {
				print_help(argv[0]);
				return EXIT_FAILURE;
			}
		} else if (strcmp(argv[i], "-a") == 0) {
			find_all=true;
		 } else if ((strcmp(argv[i], "--help") == 0) ||
							 (strcmp(argv[i], "-h") == 0)) {
			print_help(argv[0]);
			return EXIT_SUCCESS;
		}
	}

	if (gen) {
		generate(m, n, l);
	} else {
		// read instance on standard input
		std::cin >> m >> n;
		int** capacities = new int*[m];
		for (int i = 0; i < m; ++i) {
			capacities[i] = new int[n];
			for (int j = 0; j < n; ++j) {
				std::cin >> capacities[i][j];
			}
		}
		solve(capacities, m, n, find_all);
	}

	return EXIT_SUCCESS;
}
