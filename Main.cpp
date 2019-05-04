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
#define HAUT prop, i-1, j
#define BAS prop, i+1, j
#define GAUCHE prop, i, j-1
#define DROITE prop, i, j+1

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
    if (sign(p))
        std::cout << "-";
    int j = (var(p)%N) ;
    int i = (var(p) - j ) / M;
    std::cout << "(" << i << j<< ")";
    return std::vector<int> {i, j};
}

void addClause(const vec<Lit>& ps){
    vec<Lit> vector;
    for (int i = 0; i < ps.size(); ++i){
        if (ps[i] != Lit(-1)){
            vector.push(ps[i]);
        }
    }
    if (vector.size() < 1)
        return;
    s.addClause(vector);
}

void printClause(const vec<Lit>& vector){
    for (int i = 0; i < vector.size(); i++){
        getIJFromLit(vector[i]);
        if (i != vector.size() -1)
            std::cout << " v ";
    }
    std::cout << std::endl;
}

Lit Litt(int** prop, int i, int j, bool sign){
    if (i < 0 or j < 0 or i > M-1 or j > N-1)
        return Lit(-1);
    if (sign)
        return Lit(prop[i][j]);
    return ~Lit(prop[i][j]);
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
	addUnit(Litt(HAUT, false));
	addUnit(Litt(BAS, false));
	addUnit(Litt(GAUCHE, false));
	addUnit(Litt(DROITE, false));
}

void constraintOneOne(int** prop, int i, int j) {
    // (H && ~B && ~G && ~D) || (~H && B && ~G && ~D) || (~H && ~B && G && ~D) || (~H && ~B && ~G && D)
	//                                  est équivalent à
	// (¬B ∨ ¬D) ∧ (¬B ∨ ¬G) ∧ (¬B ∨ ¬H) ∧ (B ∨ D ∨ G ∨ H) ∧ (¬D ∨ ¬G) ∧ (¬D ∨ ¬H) ∧ (¬G ∨ ¬H)

	 addBinary(Litt(BAS, false), Litt(DROITE, false)); // (-B v -D)
	 addBinary(Litt(BAS, false), Litt(GAUCHE, false)); // (-B v -G)
	 addBinary(Litt(BAS, false), Litt(HAUT, false)); // (-B v -H)
	 addBinary(Litt(DROITE, false), Litt(GAUCHE, false)); // (-D v -G)
	 addBinary(Litt(DROITE, false), Litt(HAUT, false)); // (-D v -H)
	 addBinary(Litt(GAUCHE, false), Litt(HAUT, false)); // (-G v -H)

	 vec<Lit> lits;
	 lits.push(Litt(BAS, true));
	 lits.push(Litt(DROITE, true));
	 lits.push(Litt(GAUCHE, true));
	 lits.push(Litt(HAUT, true));
	 addClause(lits); // (B v D v G v H)
}

void constraintOneTwo(int** prop, int i, int j) {
	// [CAPACITE 2]
	// (H ∧ B ∧ ¬G ∧ ¬D) ∨ (¬H ∧ ¬B ∧ G ∧ D) ∨ (H ∧ ¬B ∧ ¬G ∧ D) ∨ (¬H ∧ B ∧ ¬G ∧ D) ∨
	//  (¬H ∧ B ∧ G ∧ ¬D) ∨ (H ∧ ¬B ∧ G ∧ ¬D)
	// equivalent à ci dessous en FNC
	// (¬B ∨ ¬D ∨ ¬G) ∧ (¬B ∨ ¬D ∨ ¬H) ∧ (¬B ∨ ¬G ∨ ¬H) ∧ (B ∨ D ∨ G) ∧ (B ∨ D ∨ H) ∧ 
	// (B ∨ G ∨ H) ∧ (¬D ∨ ¬G ∨ ¬H) ∧ (D ∨ G ∨ H)

	addTernary(Litt(BAS, false), Litt(DROITE, false), Litt(GAUCHE, false)); // (¬B ∨ ¬D ∨ ¬G)
	addTernary(Litt(BAS, false), Litt(DROITE, false), Litt(HAUT, false)); // (¬B ∨ ¬D ∨ ¬H)
	addTernary(Litt(BAS, false), Litt(GAUCHE, false), Litt(HAUT, false)); // (¬B ∨ ¬G ∨ ¬H)
	addTernary(Litt(BAS, true), Litt(DROITE, true), Litt(GAUCHE, true)); // (B ∨ D ∨ G)
	addTernary(Litt(BAS, true), Litt(DROITE, true), Litt(HAUT, true)); // (B ∨ D ∨ H)
	addTernary(Litt(BAS, true), Litt(GAUCHE, true), Litt(HAUT, true)); // (B ∨ G ∨ H)
	addTernary(Litt(DROITE, false), Litt(GAUCHE, false), Litt(HAUT, false)); // (¬D ∨ ¬G ∨ ¬H)
	addTernary(Litt(DROITE, true), Litt(GAUCHE, true), Litt(HAUT, true)); // (D ∨ G ∨ H)

}

void constraintOneThree(int** prop, int i, int j) {
	// [CAPACITE 3]
	// (H ∧ B ∧ ¬G ∧ D) ∨ (¬H ∧ B ∧ G ∧ D) ∨ (H ∧ B ∧ G ∧ ¬D) ∨ (H ∧ ¬B ∧ G ∧ D)
	// equivalent à ci dessous en FNC
	//(¬B ∨ ¬D ∨ ¬G ∨ ¬H) ∧ (B ∨ D) ∧ (B ∨ G) ∧ (B ∨ H) ∧ (D ∨ G) ∧ (D ∨ H) ∧ (G ∨ H)

	addBinary(Litt(BAS, true), Litt(DROITE, true)); // (B v D)
	addBinary(Litt(BAS, true), Litt(GAUCHE, true)); // (B v G)
	addBinary(Litt(BAS, true), Litt(HAUT, true)); // (B v H)
	addBinary(Litt(DROITE, true), Litt(GAUCHE, true)); // (D v G)
	addBinary(Litt(DROITE, true), Litt(HAUT, true)); // (D v H)
	addBinary(Litt(GAUCHE, true), Litt(HAUT, true)); // (G v H)

	vec<Lit> lits;
	lits.push(Litt(BAS, false));
    lits.push(Litt(HAUT, false));
	lits.push(Litt(DROITE, false));
	lits.push(Litt(GAUCHE, false));
	addClause(lits); // (-B v -D v -G v -H)
}

void constraintOneFour(int** prop, int i, int j) {
	// [CAPACITE 4]
	addUnit(Litt(BAS, true));
	addUnit(Litt(HAUT, true));
	addUnit(Litt(DROITE, true));
	addUnit(Litt(GAUCHE, true));
}

void constraintOne(int** capacities, int** prop, int m, int n) {
	FOR(i, 0, m-1) {
		FOR(j, 0, n-1) {
			switch(capacities[i][j]) {
				case 0: constraintOneZero(prop, i, j);  break;  // 0
				case 1: constraintOneOne(prop, i, j);   break;  // 1
				case 2: constraintOneTwo(prop, i, j);   break;  // 2
				case 3: constraintOneThree(prop, i, j); break;  // 3
				case 4: constraintOneFour(prop, i, j);  break;  // 4
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
				addUnit(Litt(prop, i, j, false));
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
                        addBinary(Litt(prop, i, hor[k], false), Litt(prop, i, hor[l], false));
                    }
                }
            }
            std::vector<int> vert = getVerticalInterval(capacities, i, j, m, n);
            if ((int) vert.size() >= 2) {
                FOR(k, 0, (int) vert.size() - 1) {
                    FOR(l, 0, (int) vert.size() - 1) {
                        if (k == l) continue;
                        addBinary(Litt(prop, vert[k], j, false), Litt(prop, vert[l], j, false));
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
	 		    lits.push(Litt(prop, i, z, true));
	 		}
	 		for (int z : vert2){
	 		    lits.push(Litt(prop, z, j, true));
	 		}
	 		addClause(lits);
	 	}
	 }
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
				if (s.model[prop[i][j]] == l_True) {
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


	int** prop = new int*[m];
	for (int i = 0; i < m; ++i) {
		prop[i] = new int[n];
		for (int j = 0; j < n; ++j) {
			prop[i][j] = s.newVar();
		}
	}

	// ============================== CONTRAINTE 1 ====================================
	constraintOne(capacities, prop, m, n);
	
	// ============================== CONTRAINTE 2 ====================================
	constraintTwo(capacities, prop, m, n);

	// ============================== CONTRAINTE 3 ====================================
    constraintThree(capacities, prop, m, n);

	// ============================== CONTRAINTE 4 ====================================
	constraintFour(capacities, prop, m, n);

	// ============================ SOLVE ==================================
	s.solve();
	showResult(capacities, prop, m, n);

	// One, two, three, one, two, three, drink
	// One, two, three, one, two, three, drink
	// One, two, three, one, two, three, drink
	// Throw em back, till I lose count
	// IIIIIIIIIIIIIIIIIIIIIIII'm gonna swing from the chandelier, from the chandeliiiiiiiier
	// (musique de SIA mdr)
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
