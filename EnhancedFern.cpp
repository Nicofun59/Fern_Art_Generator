// Foug�re Am�lior�e : Ce fichier contient la fonction 'main'. L'ex�cution du programme commence et se termine � cet endroit.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;

// Structure random
int seed = static_cast<int>(chrono::system_clock::now().time_since_epoch().count());
default_random_engine generator(seed);
uniform_real_distribution<double> distribution(0.0, 1);
double unrand01() { return distribution(generator); }

void initialisation_transformation(double T[] /*Utilisation ext�rieure*/, double a11, double a12, double a21, double a22, double b1, double b2)
{
	T[0] = a11;
	T[1] = a12;
	T[2] = a21;
	T[3] = a22;
	T[4] = b1;
	T[5] = b2;
}

/*void affichage_transformation(double T[])
{
	cout << "[" << T[0] << ", " << T[1] << ", " << T[2] << ", " << T[3] << ", " << T[4] << ", " << T[5] << "]" << endl;
}*/

void transformation_affine(double p[] /*Tableau de coordonn�es du point p, retourn�*/, double T[] /*Tableau de transformation*/)
{
	double x = p[0];
	double y = p[1];
	p[0] = (x * T[0] + y * T[1]) + T[4]; // Retourne la coordonn�e x de p+1
	p[1] = (x * T[2] + y * T[3]) + T[5]; // Retourne la coordonn�e y de p+1
}

void fougere_numerique(double p[] /*Tableau de coordonn�es du point p0*/, int n /*Nombre de points pi*/, double *ptTabX, double *ptTabY /*Tableaux dynamiques des coordonn�es des diff�rents points*/)
{
	double T1[6], T2[6], T3[6], T4[6];
	initialisation_transformation(T1, 0.85, 0.04, (-0.04), 0.85, 0.0, 1.6);
	initialisation_transformation(T2, 0.20, (-0.26), 0.23, 0.22, 0.0, 1.6);
	initialisation_transformation(T3, (-0.15), 0.28, 0.26, 0.24, 0.0, 0.44);
	initialisation_transformation(T4, 0.0, 0.0, 0.0, 0.16, 0.0, 0.0);

	/*
	affichage_transformation(T1);
	affichage_transformation(T2);
	affichage_transformation(T3);
	affichage_transformation(T4);
	*/

	ptTabX[0] = p[0];
	ptTabY[0] = p[1];

	for (int i(1); i < n + 1; i++)
	{
		double r = unrand01();

		if (r < 0.85)
		{
			transformation_affine(p, T1);
		}
		else if (r < 0.92)
		{
			transformation_affine(p, T2);
		}
		else if (r < 0.99)
		{
			transformation_affine(p, T3);
		}
		else
		{
			transformation_affine(p, T4);
		}

		ptTabX[i] = p[0];
		ptTabY[i] = p[1];
	}
}

bool fichier_svg(const char *nomFichier, const char *nomSVG)
{
	ifstream in(nomFichier);
	ofstream out(nomSVG);
	if (in.good() && out.good())
	{
		out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
		out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"" << 1500 << "\" height=\"" << 1050 << "\">" << endl;

		double xC, yC;
		char couleur[15];

		cout << endl
			 << "Quelle couleur de fougere souhaitez-vous ?" << endl
			 << endl
			 << "1: Blue" << endl
			 << "2: Green" << endl
			 << "3: Black" << endl
			 << "4: Red"
			 << endl
			 << "5: Pink" << endl
			 << "6: Yellow" << endl
			 << "7: Purple" << endl
			 << "8: Orange" << endl
			 << endl;
		cin >> couleur;

		while (in >> xC && in >> yC)
		{
			out << "	<circle cx=\"" << (xC + 5) * 100 << "\" cy=\"" << (-yC) * 100 + 1050 << "\" r=\"" << 0.5 << "\" fill=\"" << couleur << "\"/>" << endl;
		}

		out << "</svg>" << endl;
		return true;
	}
	return false;
}

bool sauvegarde_fougere(const char *nomFichier, const char *nomSVG, int n, double p0[])
{
	ofstream out(nomFichier);
	if (out.good())
	{
		double *ptTabX = new double[n + 1];

		if (ptTabX)
		{
			double *ptTabY = new double[n + 1];

			if (ptTabY)
			{
				cout << endl
					 << "Generation des coordonnees des " << n << " points en cours" << endl;

				fougere_numerique(p0, n, ptTabX, ptTabY);

				for (int i(0); i < n; i++)
				{
					out << setw(12) << ptTabX[i] << setw(12) << ptTabY[i] << endl;
				}

				cout << endl
					 << "Generation du fichier d'affichage en cours" << endl;

				fichier_svg(nomFichier, nomSVG);

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

	cout << "Combien de points desirez-vous ?" << endl;
	cin >> n;

	const char nomFichier[128] = "Specimen.dat";
	char nomSVG[128];
	cout << endl
		 << "Quel nom souhaitez-vous donner au fichier d'affichage de la fougere (suivi d'un .svg) ?" << endl;
	cin >> nomSVG;

	if (sauvegarde_fougere(nomFichier, nomSVG, n, p0))
		cout << endl
			 << "Execution et enregistrement reussi" << endl;
	else
		cout << "Execution et enregistrement impossible" << endl;

	cout << "Fin de l'execution" << endl;
	cin.get();
	cin.get();
	// return 0;
}

// Ex�cuter le programme�: Ctrl+F5 ou menu D�boguer�> Ex�cuter sans d�bogage
// D�boguer le programme�: F5 ou menu D�boguer�> D�marrer le d�bogage

// Astuces pour bien d�marrer�:
//   1. Utilisez la fen�tre Explorateur de solutions pour ajouter des fichiers et les g�rer.
//   2. Utilisez la fen�tre Team Explorer pour vous connecter au contr�le de code source.
//   3. Utilisez la fen�tre Sortie pour voir la sortie de la g�n�ration et d'autres messages.
//   4. Utilisez la fen�tre Liste d'erreurs pour voir les erreurs.
//   5. Acc�dez � Projet�> Ajouter un nouvel �l�ment pour cr�er des fichiers de code, ou � Projet�> Ajouter un �l�ment existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, acc�dez � Fichier�> Ouvrir�> Projet et s�lectionnez le fichier .sln.