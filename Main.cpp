/*
 * 000440761 - HENEFFE Alexandre
 * 000443812 - GOMES RODRIGUES Ricardo
 */
#include <iostream>
#include <vector>
#include "Solver.hpp"

//Macros pour simplifier la lecture du code
#define FOR(k,lb,ub) for (int k = lb; k <= ub; k++) //borne supérieure comprise
#define HAUT prop[i-1][j]    //case au-dessus de (i, j)
#define BAS prop[i+1][j]     //case en-dessous de (i, j)
#define GAUCHE prop[i][j-1]  //case à gauche de (i, j)
#define DROITE prop[i][j+1]  //case à droite de (i, j)

// Solver MiniSAT, en variable global pour pouvoir l'utiliser dans toutes
// les fonctions sans devoir le passer en paramètre à chaque fois.
Solver s;


// ================================================================================
// ========================== FONCTIONS UTILES ====================================
// =============== Ensemble de fonctions utiles pour plus tard ====================
// ================================================================================

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

/**
 * @param  capacities: matrix of capacities
 * @param  i: index line i
 * @param  j: index column j
 * @return true is the case (i, j) is a wall
 */
bool isWall(int** capacities, int i, int j){
	return capacities[i][j] == -1 or capacities[i][j] == 0 or capacities[i][j] == 1
	or capacities[i][j] == 2 or capacities[i][j] == 3 or capacities[i][j] == 4;
}


/**
 * Cherche l'intervalle dans lequel se trouve une case (horizontalement)
 * Exemple :  M V V C V M (M=Mur, V=Vide, C=Case actuelle) va retourner
 * un vecteur avec [1, 2, 3, 4] car nous avons un mur en 0 et en 5.
 * @param  capacities: matrix of capacities
 * @param  i: index line i
 * @param  j: index column j
 * @param  m: capacities height
 * @param  n: capacities width
 * @return vector of integers
 */
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

/**
 * Cherche l'intervalle dans lequel se trouve une case (verticalement)
 * Exemple :  M V V C V M (M=Mur, V=Vide, C=Case actuelle) va retourner
 * un vecteur avec [1, 2, 3, 4] car nous avons un mur en 0 et en 5.
 * @param  capacities: matrix of capacities
 * @param  i: index line i
 * @param  j: index column j
 * @param  m: capacities height
 * @param  n: capacities width
 * @return vector of integers
 */
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

std::vector<int> voisins(int i, int j) {
	// renvoie une liste avec les positions des 4 cases voisines de la case (i,j)
	std::vector<int> res = {i-1, i+1, i, i, j, j, j-1, j+1}; // i puis j correspondants
	return res;
}

// ================================================================================
// ============================== CONTRAINTE 1 ====================================
// ============== Chaque mur a 0, 1, 2, 3 our 4 ampoule(s) autour =================
// ================================================================================

/**
 * Contrainte 1.0 : 0 ampoule autour
 * @param  prop: matrix of prop
 * @param  i: index line i
 * @param  j: index column j
 */
void constraintOneZero(int** prop, int i, int j) {
    // (~H && ~B && ~G && ~D)
	// s.addUnit(~Lit(BAS));
	// s.addUnit(~Lit(HAUT));
	// s.addUnit(~Lit(DROITE));
	// s.addUnit(~Lit(GAUCHE));

	std::vector<int> v = voisins(i,j);

	FOR(l, 0, 3) {
		s.addUnit(~Lit(prop[ v[l] ][ v[l+4] ])); // Aucune des case autour n'a d'ampoule
	}
}

/**
 * Contrainte 1.1 : 1 ampoule autour
 * @param  prop: matrix of prop
 * @param  i: index line i
 * @param  j: index column j
 */
void constraintOneOne(int** prop, int i, int j) {

	std::vector<int> v = voisins(i,j); // voisins dans prop, de i et j
							// [0, 1, 2, 3,    4, 5, 6, 7]

	// Au plus une ampoule autour
	FOR(k, 0, 3) {
		FOR(l, 0, 3) { // v[k+4] = le j correspondant au k (i)
			if (l == k) {continue;}
			s.addBinary(~Lit(prop[ v[k] ][ v[k+4] ]), ~Lit(prop[ v[l] ][ v[l+4] ]));
		}
	}

	// Au moins une ampoule autour
	vec<Lit> lits;
	FOR(l, 0, 3) {
		lits.push(Lit(prop[ v[l] ][ v[l+4] ]));
	}
	s.addClause(lits); // H v B v G v D			

}

/**
 * Contrainte 1.2 : 2 ampoules autour
 * @param  prop: matrix of prop
 * @param  i: index line i
 * @param  j: index column j
 */
void constraintOneTwo(int** prop, int i, int j) {
	// (H ∧ B ∧ ¬G ∧ ¬D) ∨ (¬H ∧ ¬B ∧ G ∧ D) ∨ (H ∧ ¬B ∧ ¬G ∧ D) ∨ (¬H ∧ B ∧ ¬G ∧ D) ∨
	//  (¬H ∧ B ∧ G ∧ ¬D) ∨ (H ∧ ¬B ∧ G ∧ ¬D)
	// equivalent à ci dessous en FNC
	// (¬B ∨ ¬D ∨ ¬G) ∧ (¬B ∨ ¬D ∨ ¬H) ∧ (¬B ∨ ¬G ∨ ¬H) ∧ (B ∨ D ∨ G) ∧ (B ∨ D ∨ H) ∧ 
	// (B ∨ G ∨ H) ∧ (¬D ∨ ¬G ∨ ¬H) ∧ (D ∨ G ∨ H)

	s.addTernary(~Lit(BAS), ~Lit(DROITE), ~Lit(GAUCHE)); // (¬B ∨ ¬D ∨ ¬G)
	s.addTernary(~Lit(BAS), ~Lit(DROITE), ~Lit(HAUT)); // (¬B ∨ ¬D ∨ ¬H)
	s.addTernary(~Lit(BAS), ~Lit(GAUCHE), ~Lit(HAUT)); // (¬B ∨ ¬G ∨ ¬H)
	s.addTernary(Lit(BAS), Lit(DROITE), Lit(GAUCHE)); // (B ∨ D ∨ G)
	s.addTernary(Lit(BAS), Lit(DROITE), Lit(HAUT)); // (B ∨ D ∨ H)
	s.addTernary(Lit(BAS), Lit(GAUCHE), Lit(HAUT)); // (B ∨ G ∨ H)
	s.addTernary(~Lit(DROITE), ~Lit(GAUCHE), ~Lit(HAUT)); // (¬D ∨ ¬G ∨ ¬H)
	s.addTernary(Lit(DROITE), Lit(GAUCHE), Lit(HAUT)); // (D ∨ G ∨ H)

}

/**
 * Contrainte 1.3 : 3 ampoules autour
 * @param  prop: matrix of prop
 * @param  i: index line i
 * @param  j: index column j
 */
void constraintOneThree(int** prop, int i, int j) {
	// (H ∧ B ∧ ¬G ∧ D) ∨ (¬H ∧ B ∧ G ∧ D) ∨ (H ∧ B ∧ G ∧ ¬D) ∨ (H ∧ ¬B ∧ G ∧ D)
	// equivalent à ci dessous en FNC
	//(¬B ∨ ¬D ∨ ¬G ∨ ¬H) ∧ (B ∨ D) ∧ (B ∨ G) ∧ (B ∨ H) ∧ (D ∨ G) ∧ (D ∨ H) ∧ (G ∨ H)

	s.addBinary(Lit(BAS), Lit(DROITE)); // (B v D)
	s.addBinary(Lit(BAS), Lit(GAUCHE)); // (B v G)
	s.addBinary(Lit(BAS), Lit(HAUT)); // (B v H)
	s.addBinary(Lit(DROITE), Lit(GAUCHE)); // (D v G)
	s.addBinary(Lit(DROITE), Lit(HAUT)); // (D v H)
	s.addBinary(Lit(GAUCHE), Lit(HAUT)); // (G v H)

	vec<Lit> lits;
	lits.push(~Lit(BAS));
    lits.push(~Lit(HAUT));
	lits.push(~Lit(DROITE));
	lits.push(~Lit(GAUCHE));
	s.addClause(lits); // (-B v -D v -G v -H)
}

/**
 * Contrainte 1.4 : 4 ampoules autour
 * @param  prop: matrix of prop
 * @param  i: index line i
 * @param  j: index column j
 */
void constraintOneFour(int** prop, int i, int j) {
	// (H ∧ B ∧ ¬G ∧ D)
	s.addUnit(Lit(BAS));
	s.addUnit(Lit(HAUT));
	s.addUnit(Lit(DROITE));
	s.addUnit(Lit(GAUCHE));
}

/**
 * Contrainte 1 : avoir x ampoules autour
 * @param  capacities: matrix of capacities
 * @param  prop: matrix of prop
 * @param  m: capacities height
 * @param  n: capacities width
 */
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

/**
 * Contrainte 2 : pas d'ampoule sur un mur.
 * @param  capacities: matrix of capacities
 * @param  prop: matrix of prop
 * @param  m: capacities height
 * @param  n: capacities width
 */
void constraintTwo(int** capacities, int** prop, int m, int n) {
	FOR(i, 0, m-1) {
		FOR(j, 0, n-1) {
			if (isWall(capacities, i,j)) {
				s.addUnit(~Lit(prop[i+1][j+1]));
			}
		}
	}
}

// ================================================================================
// ============================== CONTRAINTE 3 ====================================
// =================== 2 ampoules ne s'éclairement pas ============================
// ================================================================================

/**
 * Contrainte 3 : 2 ampoules ne peuvent s'éclairer
 * Explication : pour chaque case, on va regarder dans quel intervalle horizontal
 * et vertical où elle se trouve. Dans cet intervalle, on va faire en sorte qu'on
 * ait pas deux ampoules. Pour cela, nous prenons chaque paire possible de deux
 * cases dans cet intervalle, et nous ajoutons une clause pour dire que les deux
 * ne cases ne peuvent pas être illuminées en même temps.
 * @param  capacities: matrix of capacities
 * @param  prop: matrix of prop
 * @param  m: capacities height
 * @param  n: capacities width
 */
void constraintThree(int **capacities, int **prop, int m, int n) {
    FOR(i, 0, m-1){
        FOR(j, 0, n-1) {
            std::vector<int> hor = getHorizontalInterval(capacities, i, j, m, n);
            if ((int) hor.size() > 1) { //si qu'une case, elle n'aura pas d'ampoule en face
                FOR(k, 0, (int) hor.size() - 1) {
                    FOR(l, 0, (int) hor.size() - 1) {
                        if (k == l) continue;
                        s.addBinary(~Lit(prop[i + 1][hor[k] + 1]), ~Lit(prop[i + 1][hor[l] + 1]));
                    }
                }
            }
            std::vector<int> vert = getVerticalInterval(capacities, i, j, m, n);
            if ((int) vert.size() > 1) { //si qu'une case, elle n'aura pas d'ampoule en face
                FOR(k, 0, (int) vert.size() - 1) {
                    FOR(l, 0, (int) vert.size() - 1) {
                        if (k == l) continue;
                        s.addBinary(~Lit(prop[vert[k] + 1][j + 1]), ~Lit(prop[vert[l] + 1][j + 1]));
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

/**
 * Contrainte 4 : toutes les cases doivent être éclairées
 * Explication : pour chaque case, on va regarder dans quel intervalle horizontal
 * et vertical où elle se trouve. On va ajouter toutes ces cases présentes dans
 * les intervalles dans une clause (donc 'OU') et l'ajouter au solveur. Ainsi, on
 * dit à la case qu'elle doit être illuminée par une ampoule présente dans le
 * même intervalle (horizontal OU vertical) qu'elle.
 * @param  capacities: matrix of capacities
 * @param  prop: matrix of prop
 * @param  m: capacities height
 * @param  n: capacities width
 */
void constraintFour(int** capacities, int** prop, int m, int n) {
    vec<Lit> lits;
	 FOR(i, 0, m-1) {
	 	FOR(j, 0, n-1) {
	 		if (isWall(capacities, i, j)) { // si c'est un mur, on skip
	 			continue;
	 		}

	 		std::vector<int> verticalInterval = getVerticalInterval(capacities, i, j, m, n);
	 		std::vector<int> horizontalInterval = getHorizontalInterval(capacities, i, j, m, n);

	 		lits.clear();
	 		for (int z : horizontalInterval){
	 		    lits.push(Lit(prop[i+1][z+1]));
	 		}
	 		for (int z : verticalInterval){
	 		    lits.push(Lit(prop[z+1][j+1]));
	 		}
	 		s.addClause(lits);
	 	}
	 }
}


// ================================================================================
// ============================== CONTRAINTE SUPP =================================
// ========== Ajoute des contraintes sur les cases non affichées.==================
// =================Elles ne peuvent pas contenir d'ampoules.======================
// =======================Pas utile dans le rapport.===============================
// ================================================================================

/**
 * Contrainte supplémentaire non présente dans le rapport, car elle est basée sur
 * notre implémentation du code.
 * Explication : nous avons fait en sorte que prop ait un "bord" non visible en plus
 * par rapport à capacities. Ainsi, nous devons pas vérifier que HAUT, BAS, GAUCHE,
 * DROITE soient des cases existantes (exemple si i=0, j=0, alors HAUT: i=-1, j=0,
 * causerait une erreur). Mais en faisant cela, il se peut que Minisat mette une
 * ampoule dans le bord (donc non-visible), ce que nous n'aurorisons pas grâce à
 * cette contrainte supplémentaire précisant que le bord ne peut contenir d'ampoules.
 * @param  capacities: matrix of capacities
 * @param  prop: matrix of prop
 * @param  m: capacities height
 * @param  n: capacities width
 */
void constraintSupp(int** capacities, int** prop, int m, int n){
    FOR(i, 0, m+1){
        FOR(j, 0, n+1){
            if (i == 0 or j == 0 or i == m+1 or j == n+1){
                s.addUnit(~Lit(prop[i][j]));
            }
        }
    }
}

// ================================================================================
// =========================== Calcule les contraintes ============================
// ================================================================================

/**
 * Fonction appellant toutes les contraintes.
 * @param  capacities: matrix of capacities
 * @param  prop: matrix of prop
 * @param  m: capacities height
 * @param  n: capacities width
 */
void setupConstraints(int** capacities, int** prop, int m, int n) {
	// ============================== CONTRAINTE 1 ====================================
	constraintOne(capacities, prop, m, n);
	// ============================== CONTRAINTE 2 ====================================
	constraintTwo(capacities, prop, m, n);
	// ============================== CONTRAINTE 3 ====================================
	constraintThree(capacities, prop, m, n);
	// ============================== CONTRAINTE 4 ====================================
	constraintFour(capacities, prop, m, n);
	// ============================== CONTRAINTE SUPP =================================
	constraintSupp(capacities, prop, m, n);
}

// ===============================================================================
// ================================== SOLVE ======================================
// ================== Fonctions pour solutionner le problème =====================
// ==================== et gérer l'affichage des solutions. ======================
// ===============================================================================

/**
 * Fonction affichant le résultat du solve de Minisat.
 * @param  capacities: matrix of capacities
 * @param  prop: matrix of prop
 * @param  m: capacities height
 * @param  n: capacities width
 */
void showResult(int** capacities, int** prop, int m, int n){
	if (!s.okay()) {
		std::cout << "Il n'y a pas de solution." << std::endl;
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

/**
 * Crée les propositions.
 * Explication : nous avons fait en sorte que prop ait un "bord" non visible en plus
 * par rapport à capacities. Ainsi, nous devons pas vérifier que HAUT, BAS, GAUCHE,
 * DROITE soient des cases existantes (exemple si i=0, j=0, alors HAUT: i=-1, j=0,
 * causerait une erreur).
 * @param  m: capacities height
 * @param  n: capacities width
 */
int** newPropositions(int m, int n) {
	int** prop = new int*[m+2];
	for (int i = 0; i < m+2; ++i) {
		prop[i] = new int[n+2];
		for (int j = 0; j < n+2; ++j) {
			prop[i][j] = s.newVar();
			// std::cout << prop[i][j] << ",";
		}
	}
	return prop;
}

/**
 * Fonction interdisant qu'une solution soit à nouveau proposée par Minisat
 * afin d'avoir plusieurs (si possible) solutions uniques.
 * Explication : pour avoir des solutions différentes, il suffit d'ajouter
 * une clause où on demande qu'au moins une des ampoules soit pas à la même
 * position que dans les autres solutions déjà trouvées.
 * @param  prop: matrix of prop
 * @param  m: capacities height
 * @param  n: capacities width
 */
void forbidSolution(int** prop, int m, int n) {
	vec<Lit> vector;
	FOR(i, 0, m-1) {
		FOR(j, 0, n-1) {
			if (s.model[prop[i+1][j+1]] == l_True) { // ampoule
				vector.push(~Lit(prop[i+1][j+1]));
			}
		}
	}
	s.addClause(vector);
}

/**
 * Solves the given light-up instance.
 * @param capacities: instance capacities to solve, an `m` by `n` matrix.
 * @param m: height of the instance
 * @param n: width of the instance
 * @param find_all: if true, find all valid solutions
 */  
void solve(int** capacities, int m, int n, bool find_all) {
    // ============================= SETUP ================================
	pretty_print(capacities, m, n);

	int** prop = newPropositions(m, n);
	setupConstraints(capacities, prop, m, n); // Contraintes

	// ============================ SOLVE ==================================
    int nbSolutions = 0;

	while (true) {
		s.solve();
		if (!s.okay()) break;

		nbSolutions++;

		showResult(capacities, prop, m, n);

		forbidSolution(prop, m, n); // contraint la solution pour ne plus l'avoir apres
		s.model.clear(); //clear pour le re-remplir de nouvelles solutions.

		if (not find_all) break; // Si on veut qu'une solution
	}

	std::cout << nbSolutions << " solution(s) trouvée(s) !" << std::endl;
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
