/* 000440761 - HENEFFE Alexandre
 * 000443812 - GOMES RODRIGUES Ricardo
 * 
 */
#include <iostream>
#include "Solver.hpp"
#define FOR(k,lb,ub) for (int k = lb; k < ub; k++)

int BORD = 0, AMPOULE = 1, VIDE = 2, MUR = 3, ZERO = 4, UN = 5, DEUX = 6, TROIS = 7, QUATRE = 8;

/**
 * Pretty prints the given matrix
 * @param  matrix: matrix to print
 * @param  m: matrix height
 * @param  n: matrix width
 */
void pretty_print(int** matrix, int m, int n) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << matrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}
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

	// Fonction à compléter pour les questions 2 et 3 (et bonus 1)
	Solver s;
	
	vec<Lit> lits;

	int prop[m][n][9] = {{{0}}};

	FOR(i, 0, m) {
		FOR(j, 0, n) {
			FOR(k, 0, 9) {
				prop[i][j][k] = s.newVar();
			}
		}
	}

	// contrainte 1 : Chaque mur a 0 ou 1 ampoule autour de lui

	
	// // contrainte 2
	FOR(i, 0, m) {
		FOR(j, 0, n) {
			s.addBinary(~Lit(prop[i][j][AMPOULE]), ~Lit(prop[i][j][MUR]));
		}
	}
	
	// contrainte 3 : 2 ampoules ne s'éclairent pas mutuellement

	// contrainte 3.1 : Lignes
	FOR(i, 0, m) {
		FOR(j, 0, n) {
			FOR(jprime, j, n) {
				FOR(l, j, jprime) {
					s.addTernary(Lit(prop[i][l][MUR]), ~Lit(prop[i][j][AMPOULE]), Lit(prop[i][jprime][AMPOULE]));
				}
			}
		}
	}

	// // contrainte 3.2 : Colonnes
	FOR(i, 0, m) {
		FOR(j, 0, n) {
			FOR(iprime, i, m) {
				FOR(l, i, iprime) {
					s.addTernary(Lit(prop[l][j][MUR]), ~Lit(prop[i][j][AMPOULE]), Lit(prop[iprime][j][AMPOULE]));
				}
			}
		}
	}

	// contrainte 4

	FOR(i, 0, m) {
		FOR(j, 0, n) {
			jprime = j;
			iprime = i;
			FOR(l, j, jprime) {
				vec<Lit> lits;
				s.addTernary()
			}
			}
		}
	}
	







	s.solve(); // niquez vos mère je mets pas d'ampoule et en plus je suis seul ici :'(










	if (!s.okay()) {
		std::cout << "Il n'y a pas de solution sale pute" << std::endl;
	}
	else {
		std::cout << "La formule est satisfaisable." << std::endl;
		FOR(i, 0, m) {
			FOR(j, 0, n) {
				if (s.model[prop[i][j][AMPOULE]] == l_True) {
					std::cout << "La case (" << i << "," << j << ") contient une ampoule";
					std::cout << "\n";
				}
			}
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
