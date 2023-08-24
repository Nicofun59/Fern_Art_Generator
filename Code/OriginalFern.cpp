// Fougère Originale : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;

// Structure random
int seed = static_cast<int> (chrono::system_clock::now().time_since_epoch().count());
default_random_engine generator(seed);
uniform_real_distribution<double> distribution(0.0, 1);
double unrand01() { return distribution(generator); }

void initialisation_transformation(double T[] /*Utilisation extérieure*/, double a11, double a12, double a21, double a22, double b1, double b2)
{
	T[0] = a11; T[1] = a12; T[2] = a21; T[3] = a22; T[4] = b1; T[5] = b2;
}

void affichage_transformation(double T[])
{
	cout << "[" << T[0] << ", " << T[1] << ", " << T[2] << ", " << T[3] << ", " << T[4] << ", " << T[5] << "]" << endl;
}

void transformation_affine(double p[] /*Tableau de coordonnées du point p, retourné*/, double T[] /*Tableau de transformation*/)
{
	double x = p[0];
	double y = p[1];
	p[0] = (x * T[0] + y * T[1]) + T[4]; // Retourne la coordonnée x de p+1
	p[1] = (x * T[2] + y * T[3]) + T[5]; // Retourne la coordonnée y de p+1
}

void fougere_numerique(double p[] /*Tableau de coordonnées du point p0*/, int n /*Nombre de points pi*/, double* ptTabX, double* ptTabY /*Tableaux dynamiques des coordonnées des différents points*/)
{
	double T1[6], T2[6], T3[6], T4[6];
	initialisation_transformation(T1, 0.85, 0.04, (-0.04), 0.85, 0.0, 1.6);
	initialisation_transformation(T2, 0.20, (-0.26), 0.23, 0.22, 0.0, 1.6);
	initialisation_transformation(T3, (-0.15), 0.28, 0.26, 0.24, 0.0, 0.44);
	initialisation_transformation(T4, 0.0, 0.0, 0.0, 0.16, 0.0, 0.0);

	affichage_transformation(T1);
	affichage_transformation(T2);
	affichage_transformation(T3);
	affichage_transformation(T4);

	for (int i(0); i < n; i++)
	{
		double r = unrand01();

		if (r < 0.85)
		{
			transformation_affine(p, T1);
		}
		else if (r < 0.92) {
			transformation_affine(p, T2);
		}
		else if (r < 0.99) {
			transformation_affine(p, T3);
		}
		else {
			transformation_affine(p, T4);
		}

		ptTabX[i] = p[0];
		ptTabY[i] = p[1];
	}
}

bool sauvegarde_fougere(const char* nomFichier, int n, double p0[])
{
	ofstream out(nomFichier);
	if (out.good())
	{
		double* ptTabX = new double[n];

		if (ptTabX)
		{
			double* ptTabY = new double[n];

			if (ptTabY)
			{
				cout << endl << "Generation des coordonnees des " << n << " points en cours" << endl;

				fougere_numerique(p0, n, ptTabX, ptTabY);

				for (int i(0); i < n; i++)
				{
					out << setw(12) << ptTabX[i] << setw(12) << ptTabY[i] << endl;
				}

				delete[] ptTabX;
				delete[] ptTabY;

				out.close();
				return true;
			}
			else
			{
				delete[] ptTabX;
				return false;
			}
		}
	}
	return false;
}

void main()
{
	int n;
	double p0[2];
	p0[0] = 0.5;
	p0[1] = 0.5;

	cout << "Nombre de points desires :" << endl;
	cin >> n;

	const char nomFichier[128] = "Specimen.dat";
	if (sauvegarde_fougere(nomFichier, n, p0)) cout << endl << "Execution et enregistrement reussi" << endl;
	else cout << "Execution et enregistrement impossible" << endl;

	cout << "Fin de l'execution" << endl;
	cin.get(); cin.get();
}