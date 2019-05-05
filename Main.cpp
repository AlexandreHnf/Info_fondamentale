/* 000440761 - HENEFFE Alexandre
 * 000443812 - GOMES RODRIGUES Ricardo
 * 
 */
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include "Solver.hpp"
#define FOR(k,lb,ub) for (int k = lb; k <= ub; k++)
#define HAUT prop[i-1][j]
#define BAS prop[i+1][j]
#define GAUCHE prop[i][j-1]
#define DROITE prop[i][j+1]

Solver s;
int M = 0;
int N = 0;

/**
 * Pretty prints the given matrix
 * @param  matrix: matrix to print
 * @param  m: matrix height
 * @param  n: matrix width
 */
void pretty_print(int** matrix, int m, int n) {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (matrix[i][j] == -1) {                              // ampoule
				std::cout << ("\033[1;37m■\033[0m") << "  ";
			} else if(matrix[i][j] == 0) {                         // mur 0
				std::cout << ("\033[1;37m0\033[0m") << "  ";
			} else if (matrix[i][j] == 1) {                        // mur 1
				std::cout << ("\033[1;37m1\033[0m") << "  ";
			} else if (matrix[i][j] == 2) {                        // mur 2
				std::cout << ("\033[1;37m2\033[0m") << "  ";
			} else if (matrix[i][j] == 3) {						   // mur 3
				std::cout << ("\033[1;37m3\033[0m") << "  ";
			} else if (matrix[i][j] == 4) {						   // mur 4
				std::cout << ("\033[1;37m4\033[0m") << "  ";
			} else if (matrix[i][j] == -2) {                       // case vide
				std::cout << ("\033[1;30m■\033[0m") << "  ";
			}
		}
		std::cout << std::endl;
	}
}

bool isWall(int** capacities, int i, int j){
	return capacities[i][j] == -1 or capacities[i][j] == 0 or capacities[i][j] == 1
	or capacities[i][j] == 2 or capacities[i][j] == 3 or capacities[i][j] == 4;
}

std::vector<int> getHorizontalInterval(int **capacities, int i, int j, int m, int n) {
    // get the horizontal interval in which a cell is
    int inf_j = j;
    int sup_j = j;
    while (inf_j >= 0) { // vers la gauche
        if (!isWall(capacities, i, inf_j)) { // tant que c'est pas un mur
            inf_j--;
        } else break;
    }
    while (sup_j <= n-1) { // vers la droite
        if (!isWall(capacities, i, sup_j)) { // tant que c'est pas un mur
            sup_j++;
        } else break;
    }
    std::vector<int> res;
    for (int z = inf_j+1; z <= sup_j-1; ++z){
        res.push_back(z);
    }
    return res;
}

std::vector<int> getVerticalInterval(int **capacities, int i, int j, int m, int n) {
    // get the vertical interval in which a cell is
    int inf_i = i;
    int sup_i = i;
    while (inf_i >= 0) { // vers le haut
        if (!isWall(capacities, inf_i, j)) { // tant que c'est pas un mur
            inf_i--;
        } else break;
    }
    while (sup_i <= m-1) { // vers le bas
        if (!isWall(capacities, sup_i, j)) { // tant que c'est pas un mur
            sup_i++;
        } else break;
    }
    std::vector<int> res;
    for (int z = inf_i+1; z <= sup_i-1; ++z){
        res.push_back(z);
    }
    return res;
}

std::vector<int> getIJFromLit(const Lit& p){
	// std::cout << var(p);
    if (sign(p))
        std::cout << "-";
    int j = (var(p)%N) ;
    int i = (var(p) - j ) / M;
    i++; j++;
    std::cout << var(p) << "(" << i << j<< ")";
    return std::vector<int> {i, j};
}

void addClause(const vec<Lit>& ps){
    s.addClause(ps);
    for (int i = 0; i < ps.size(); i++){
        if (i != ps.size() -1) {
        }
    }
}

void addUnit(Lit p){
    vec<Lit> vector;
    vector.push(p);
    addClause(vector);
}

void addBinary(Lit p, Lit q){
    vec<Lit> vector;
    vector.push(p);
    vector.push(q);
    addClause(vector);
}

void addTernary(Lit p, Lit q, Lit r){
    vec<Lit> vector;
    vector.push(p);
    vector.push(q);
    vector.push(r);
    addClause(vector);
}

// ================================================================================
// ============================== CONTRAINTE 1 ====================================
// =================== Chaque mur a 0 ou 1 ampoule autour =========================
// ================================================================================

void constraintOneZero(int** prop, int i, int j) {
	// [CAPACITE 0]
	addUnit(~Lit(BAS));
	addUnit(~Lit(HAUT));
	addUnit(~Lit(DROITE));
	addUnit(~Lit(GAUCHE));
}

void constraintOneOne(int** prop, int i, int j) {
    // (H && ~B && ~G && ~D) || (~H && B && ~G && ~D) || (~H && ~B && G && ~D) || (~H && ~B && ~G && D)
	//                                  est équivalent à
	// (¬B ∨ ¬D) ∧ (¬B ∨ ¬G) ∧ (¬B ∨ ¬H) ∧ (B ∨ D ∨ G ∨ H) ∧ (¬D ∨ ¬G) ∧ (¬D ∨ ¬H) ∧ (¬G ∨ ¬H)

	 addBinary(~Lit(BAS), ~Lit(DROITE)); // (-B v -D)
	 addBinary(~Lit(BAS), ~Lit(GAUCHE)); // (-B v -G)
	 addBinary(~Lit(BAS), ~Lit(HAUT)); // (-B v -H)
	 addBinary(~Lit(DROITE), ~Lit(GAUCHE)); // (-D v -G)
	 addBinary(~Lit(DROITE), ~Lit(HAUT)); // (-D v -H)
	 addBinary(~Lit(GAUCHE), ~Lit(HAUT)); // (-G v -H)

	 vec<Lit> lits;
	 lits.push(Lit(BAS));
	 lits.push(Lit(DROITE));
	 lits.push(Lit(GAUCHE));
	 lits.push(Lit(HAUT));
	 addClause(lits); // (B v D v G v H)
}

void constraintOneTwo(int** prop, int i, int j) {
	// [CAPACITE 2]
	// (H ∧ B ∧ ¬G ∧ ¬D) ∨ (¬H ∧ ¬B ∧ G ∧ D) ∨ (H ∧ ¬B ∧ ¬G ∧ D) ∨ (¬H ∧ B ∧ ¬G ∧ D) ∨
	//  (¬H ∧ B ∧ G ∧ ¬D) ∨ (H ∧ ¬B ∧ G ∧ ¬D)
	// equivalent à ci dessous en FNC
	// (¬B ∨ ¬D ∨ ¬G) ∧ (¬B ∨ ¬D ∨ ¬H) ∧ (¬B ∨ ¬G ∨ ¬H) ∧ (B ∨ D ∨ G) ∧ (B ∨ D ∨ H) ∧ 
	// (B ∨ G ∨ H) ∧ (¬D ∨ ¬G ∨ ¬H) ∧ (D ∨ G ∨ H)

	addTernary(~Lit(BAS), ~Lit(DROITE), ~Lit(GAUCHE)); // (¬B ∨ ¬D ∨ ¬G)
	addTernary(~Lit(BAS), ~Lit(DROITE), ~Lit(HAUT)); // (¬B ∨ ¬D ∨ ¬H)
	addTernary(~Lit(BAS), ~Lit(GAUCHE), ~Lit(HAUT)); // (¬B ∨ ¬G ∨ ¬H)
	addTernary(Lit(BAS), Lit(DROITE), Lit(GAUCHE)); // (B ∨ D ∨ G)
	addTernary(Lit(BAS), Lit(DROITE), Lit(HAUT)); // (B ∨ D ∨ H)
	addTernary(Lit(BAS), Lit(GAUCHE), Lit(HAUT)); // (B ∨ G ∨ H)
	addTernary(~Lit(DROITE), ~Lit(GAUCHE), ~Lit(HAUT)); // (¬D ∨ ¬G ∨ ¬H)
	addTernary(Lit(DROITE), Lit(GAUCHE), Lit(HAUT)); // (D ∨ G ∨ H)

}

void constraintOneThree(int** prop, int i, int j) {
	// [CAPACITE 3]
	// (H ∧ B ∧ ¬G ∧ D) ∨ (¬H ∧ B ∧ G ∧ D) ∨ (H ∧ B ∧ G ∧ ¬D) ∨ (H ∧ ¬B ∧ G ∧ D)
	// equivalent à ci dessous en FNC
	//(¬B ∨ ¬D ∨ ¬G ∨ ¬H) ∧ (B ∨ D) ∧ (B ∨ G) ∧ (B ∨ H) ∧ (D ∨ G) ∧ (D ∨ H) ∧ (G ∨ H)

	addBinary(Lit(BAS), Lit(DROITE)); // (B v D)
	addBinary(Lit(BAS), Lit(GAUCHE)); // (B v G)
	addBinary(Lit(BAS), Lit(HAUT)); // (B v H)
	addBinary(Lit(DROITE), Lit(GAUCHE)); // (D v G)
	addBinary(Lit(DROITE), Lit(HAUT)); // (D v H)
	addBinary(Lit(GAUCHE), Lit(HAUT)); // (G v H)

	vec<Lit> lits;
	lits.push(~Lit(BAS));
    lits.push(~Lit(HAUT));
	lits.push(~Lit(DROITE));
	lits.push(~Lit(GAUCHE));
	addClause(lits); // (-B v -D v -G v -H)
}

void constraintOneFour(int** prop, int i, int j) {
	// [CAPACITE 4]
	addUnit(Lit(BAS));
	addUnit(Lit(HAUT));
	addUnit(Lit(DROITE));
	addUnit(Lit(GAUCHE));
}

void constraintOne(int** capacities, int** prop, int m, int n) {
	FOR(i, 0, m-1) {
		FOR(j, 0, n-1) {
			switch(capacities[i][j]) {
				case 0: constraintOneZero(prop, i+1, j+1);  break;  // 0
				case 1: constraintOneOne(prop, i+1, j+1);   break;  // 1
				case 2: constraintOneTwo(prop, i+1, j+1);   break;  // 2
				case 3: constraintOneThree(prop, i+1, j+1); break;  // 3
				case 4: constraintOneFour(prop, i+1, j+1);  break;  // 4
			}
		}
	}
}

// ================================================================================
// ============================== CONTRAINTE 2 ====================================
// ======================== Pas d'ampoule sur un mur ==============================
// ================================================================================

void constraintTwo(int** capacities, int** prop, int m, int n) {
	FOR(i, 0, m-1) {
		FOR(j, 0, n-1) {
			if (isWall(capacities, i,j)) {
				addUnit(~Lit(prop[i+1][j+1]));
			}
		}
	}
}

// ================================================================================
// ============================== CONTRAINTE 3 ====================================
// =================== 2 ampoules ne s'éclairement pas ============================
// ================================================================================


void constraintThree(int **capacities, int **prop, int m, int n) {
    FOR(i, 0, m-1){
        FOR(j, 0, n-1) {
            std::vector<int> hor = getHorizontalInterval(capacities, i, j, m, n);
            if ((int) hor.size() >= 2) {
                FOR(k, 0, (int) hor.size() - 1) {
                    FOR(l, 0, (int) hor.size() - 1) {
                        if (k == l) continue;
                        addBinary(~Lit(prop[i + 1][hor[k] + 1]), ~Lit(prop[i + 1][hor[l] + 1]));
                    }
                }
            }
            std::vector<int> vert = getVerticalInterval(capacities, i, j, m, n);
            if ((int) vert.size() >= 2) {
                FOR(k, 0, (int) vert.size() - 1) {
                    FOR(l, 0, (int) vert.size() - 1) {
                        if (k == l) continue;
                        addBinary(~Lit(prop[vert[k] + 1][j + 1]), ~Lit(prop[vert[l] + 1][j + 1]));
                    }
                }
            }
        }
    }
}


// ================================================================================
// ============================== CONTRAINTE 4 ====================================
// =================== Toutes les cases sont éclairées ============================
// ================================================================================

void constraintFour(int** capacities, int** prop, int m, int n) {
    vec<Lit> lits;
	 FOR(i, 0, m-1) {
	 	FOR(j, 0, n-1) {
	 		if (isWall(capacities, i, j)) { // si c'est un mur, on skip
	 			continue;
	 		}

	 		std::vector<int> vert2 = getVerticalInterval(capacities, i, j, m, n);
	 		std::vector<int> hor2 = getHorizontalInterval(capacities, i, j, m, n);

	 		lits.clear();
	 		for (int z : hor2){
	 		    lits.push(Lit(prop[i+1][z+1]));
	 		}
	 		for (int z : vert2){
	 		    lits.push(Lit(prop[z+1][j+1]));
	 		}
	 		addClause(lits);
	 	}
	 }
}


// ================================================================================
// ============================== CONTRAINTE SUPP =================================
// ========== Ajoute des contraintes sur les cases non affichées.==================
// =================Elles ne peuvent pas contenir d'ampoules.======================
// =======================Pas utile dans le rapport.===============================
// ================================================================================


void constraintSupp(int** capacities, int** prop, int m, int n){
    FOR(i, 0, m+1){
        FOR(j, 0, n+1){
            if (i == 0 or j == 0 or i == m+1 or j == n+1){
                addUnit(~Lit(prop[i][j]));
            }
        }
    }
}

// ================================================================================
// =========================== Calcule les contraintes ============================
// ================================================================================

void setupConstraints(int** capacities, int** prop, int m, int n) {
	// ============================== CONTRAINTE 1 ====================================
	constraintOne(capacities, prop, m, n);
	// ============================== CONTRAINTE 2 ====================================
	constraintTwo(capacities, prop, m, n);
	// ============================== CONTRAINTE 3 ====================================
	constraintThree(capacities, prop, m, n);
	// ============================== CONTRAINTE 4 ====================================
	constraintFour(capacities, prop, m, n);
	// ============================== CONTRAINTE SUPP ====================================
	constraintSupp(capacities, prop, m, n);
}




// ================================================================================
// =============================== SHOW RESULT ====================================
// ================================================================================

void showResult(int** capacities, int** prop, int m, int n){

	if (!s.okay()) {
		std::cout << "Il n'y a pas de solution sale pute." << std::endl;
	}
	else {
		std::cout << "La formule est satisfaisable." << std::endl;
		FOR(i, 0, m-1) {
			FOR(j, 0, n-1) {
				if (s.model[prop[i+1][j+1]] == l_True) {
					std::cout << ("\033[1;33m▲\033[0m") << "  ";
				} else if (capacities[i][j] == -1){
					std::cout << ("\033[1;31m■\33[0m") << "  ";
				} else if (capacities[i][j] == 0) {
					std::cout << ("\033[1;31m0\033[0m") << "  ";
				} else if (capacities[i][j] == 1) {
					std::cout << ("\033[1;31m1\033[0m") << "  ";
				} else if (capacities[i][j] == 2) {
					std::cout << ("\033[1;31m2\033[0m") << "  ";
				} else if (capacities[i][j] == 3) {
					std::cout << ("\033[1;31m3\033[0m") << "  ";
				} else if (capacities[i][j] == 4) {
					std::cout << ("\033[1;31m4\033[0m") << "  ";
				} else {
					std::cout << ("\033[1;34m-\033[0m") << "  ";
				}
			}
			std::cout << "\n";
		}
	}
}

bool existSolution() {
	return s.okay(); // renvoie true si y'a une solution, false sinon
}

int** newPropositions(int m, int n) {
	// reset la solution (prop)
	int** prop = new int*[m+2];
	for (int i = 0; i < m+2; ++i) {
		prop[i] = new int[n+2];
		for (int j = 0; j < n+2; ++j) {
			prop[i][j] = 0;
			prop[i][j] = s.newVar();
			std::cout << prop[i][j] << ",";
		}
	}
	std::cout << std::endl;
	// FOR(i, 0, m) {
	// 	FOR(j, 0, n) {
	// 		prop[i][j] = s.newVar();
	// 	}
	// }
	return prop;
}


void forbidSolution(int** prop, int m, int n) {
	// contraint la solution pour ne plus l'avoir par après
	FOR(i, 0, m-1) {
		FOR(j, 0, n-1) {
			if (s.model[prop[i+1][j+1]] == l_True) { // ampoule
				s.addUnit(~Lit(prop[i+1][j+1]));
			}
		}
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
	// std::cout << std::endl;	

	int** prop = newPropositions(m, n);

	setupConstraints(capacities, prop, m, n); // Contraintes

	// ============================ SOLVE ==================================
	// s.solve();
	// showResult(capacities, prop, m, n);

	while (true) {
		s.solve();
		if (!existSolution()) { // si plus de solution
			std::cout << "y'a plus de solution\n";
			break;
		}

		showResult(capacities, prop, m, n);
		forbidSolution(prop, m, n); // contraint la solution pour ne plus l'avoir apres
		// prop = newPropositions(m, n);
		s.model.clear();
		if (not find_all) {break;} // Si on veut qu'une solution
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
		M = m;
		N = n;
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
