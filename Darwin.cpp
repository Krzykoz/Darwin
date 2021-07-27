/*****************************************************************//**
 * \file   Darwin.cpp
 * \brief  Program symulujący ewolucje populacji osobników
 *
 * \author Krzysztof Koźlik
 * \date   November 2020
 *********************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <windows.h>

using namespace std;

/** Dyrektywa zwracająca liczbę pseudolosową w przedziale od m do n.
 * \param m Minimalna wartość liczby losowej
 * \param m Maksymalna wartość liczby losowej
 */
#define QUICK_RAND(m,n) m + ( std::rand() % ( (n) - (m) + 1 ) )// Liczba Losowa z Przedziału

 /** Wzór na kod koloru osbonika wylicznany z jego chromosomu.
  * \param vect[osobnik][0] Pierwsza liczba naturalna z chromosomu
  * \param (vect[osobnik].size() - 1) Ostatnia liczba naturalna z chromosomu
  */
#define DNA_COLOR (((vect[osobnik][0]) % 11) + ((vect[osobnik][(unsigned(vect[osobnik].size()) - 1)])) % 11)// Wzór Kolor Dna

int argInfo(char** argv, int& argc, string& inFilePath, string& outFilePath, double& w, double& r, int& p, int& k);// Info
int argCheck(string& inFilePath, string& outFilePath, double& w, double& r, int& p, int& k);// Sprawdzanie argumentow
int fileRead(vector< vector<int> >& vect, string& filePath);// Wczytywanie pliku z populacją
void fileWrite(vector< vector<int> >& vect, string& filePath);// Zapisywanie populacji w pliku
void genomePrint(vector< vector<int> >& vect, int osobnik);// Wypisanie Genomu
void popPrint(vector< vector<int> >& vect);// Wypisanie Populacji
void childMix(vector< vector<int> >& vect, vector<int>& tmp, int parent1, int parent2, int parent1Break, int parent2Break, int popCount);//Łączanie genomu rodziców
void mixing(vector< vector<int> >& vect, int k);// Info o Krzyzowaniu
void fittnes(vector< vector<int> >& vect, vector<int>& clone, double w, double r);// Przystosowanie

vector< vector<int> > population;// Vector 2D z Populacją

/** Funcja główna wywołują inne funkcje.
 *
 * \param argc Ilość argumentów wprowadzonych do programu
 * \param argv Tablica z argumentami wprowadzonymi do programu
 * \return  Zwraca 0 jeśli jeśli wszystkie funkcje zostały poprawinie wykonane, w przeciwnym razie zwraca 1
 */
int main(int argc, char** argv) {
	setlocale(LC_CTYPE, "Polish");// Polskie znaki
	srand((unsigned)time(NULL));// Seed Losowści
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// Zmiana koloru terminalu

	string inFilePath;
	string outFilePath;
	double w; //współczynnik wymierania
	double r; //współczynnik rozmnażania
	int p;// Liczba pokoleń
	int k; //liczba k par osobników losowanych do krzyżowania

	if (argInfo(argv, argc, inFilePath, outFilePath, w, r, p, k)) {// info + menu
		return 1;
	}
	if (argCheck(inFilePath, outFilePath, w, r, p, k)) {// Sprawdzanie argumentow
		return 1;
	}
	if (fileRead(population, inFilePath)) {// Wczytywanie pliku z populacją
		return 1;
	}

	cout <<"                   //===================================================================================================\\\\" << endl;
	cout << "                   ||                                                                                                   ||" << endl;
	cout << "                   ||   `-:-.   ,-;\"`-:-.   ,-;\"`-:-.   ,-;\"`-:-.   ,-;\"               PROJEKT PKK DARWIN               ||" << endl;
	cout << "                   ||      `=`,'=/     `=`,'=/     `=`,'=/     `=`,'=/             Program Symulujący Ewolucje          ||" << endl;
	cout << "                   ||        y==/        y==/        y==/        y==/                                                   ||" << endl;
	cout << "                   ||      ,=,-<=`.    ,=,-<=`.    ,=,-<=`.    ,=,-<=`.                                                 ||" << endl;
	cout << "                   ||   ,-'-'   `-=_,-'-'   `-=_,-'-'   `-=_,-'-'   `-=_             Autor: Krzysztof Koźlik            ||" << endl;
	cout << "                   ||                                                                                                   ||" << endl;
	cout << "                   \\\\===================================================================================================//" << endl;

	for (int pCout = 0; pCout < p; pCout++) {
		vector<int> clone;

		cout << endl << "--------------------------------------------------------------POKOLENIE NR.";
		cout << pCout + 1;
		cout << "---------------------------------------------------------------" << endl << endl << endl;
		popPrint(population);// Wypisanie Populacji
		mixing(population, k);// Info o Krzyzowaniu

		cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		fittnes(population, clone, w, r);
		popPrint(population);// Wypisanie Populacji
		cout << endl << endl;
	}

	fileWrite(population, outFilePath);// Zapisywanie populacji w pliku

	//system("pause");
	return 0;
}

/** Funkcja wyświetla informacje i wczytuje argumenty wejściowe.
 *
 * \param argv Tablica z argumentami wprowadzonymi do programu
 * \param argc Ilość argumentów wprowadzonych do programu
 * \param inFilePath Ścierzka pliku wejściowego
 * \param outFilePath Ścierzka pliku wyjściowego
 * \param w Współczynnik Wymierania
 * \param r Współczynnik Rozmnażania
 * \param p Ilość Pokoleń
 * \param k Ilość par wylosowanych do krzyżowania
 * \return Zwraca 0 jeśli ilość argumentów jest równa 13 w przeciwnym razie zwraca 1
 */
int argInfo(char** argv, int& argc, string& inFilePath, string& outFilePath, double& w, double& r, int& p, int& k) {
	if (argc == 13) {
		inFilePath = argv[2];
		outFilePath = argv[4];
		w = atof(argv[6]);
		r = atof(argv[8]);
		p = atoi(argv[10]);
		k = atoi(argv[12]);
		return 0;
	}
	else {
		cout << "Nie podano przełączników wymaganych do prawidłowego działania programu!" << endl << endl;
		cout << "Przykładowe parametry przełączników: {\"Darwin.exe\" -i inFile.txt -o outFile.txt -w 0.4 -r 0.7 -p 3 -k 5}" << endl << endl;
		cout << "Parametry Przełączników: " << endl;
		cout << "-i plik wejściowy z populacja" << endl;
		cout << "-o plik wyjściowy z populacja" << endl;
		cout << "-w Współczynnik wymierania w E[0,1]" << endl;
		cout << "-r Współczynnik rozmnażania r E[0,1]" << endl;
		cout << "-p liczba pokoleń p" << endl;
		cout << "-k liczba k par osobników losowanych do krzyżowania" << endl << endl << endl;
		system("pause");
		return 1;
	}
}

/** Funkcja sprawdza poprawność argumentów wejśćiowych.
 *
 * \param inFilePath Ścierzka pliku wejściowego
 * \param outFilePath Ścierzka pliku wyjściowego
 * \param w Współczynnik Wymierania
 * \param r Współczynnik Rozmnażania
 * \param p Ilość Pokoleń
 * \param k Ilość par wylosowanych do krzyżowania
 * \return Zwraca 0 jeśli wszystkie argumenty są prawidłowe w przeciwnym razie zwraca 2
 */
int argCheck(string& inFilePath, string& outFilePath, double& w, double& r, int& p, int& k) {
	if (inFilePath.find(".txt") != string::npos) {
		//sprawdzanie pola!
	}
	else {
		cerr << "Plik wejściowy nie jest plikiem .txt" << endl;
		return 2;
	}

	if (outFilePath.find(".txt") != string::npos) {
		//sprawdzanie pola!
	}
	else {
		cerr << "Plik wyjściowy nie jest plikiem .txt" << endl;
		return 2;
	}

	if ((w > 0.0 && w < 1.0)) {
		//sprawdzanie pola!
	}
	else {
		cerr << "Współczynnik wymierania nie jest w przedziale od 0 do 1" << endl;
		return 2;
	}

	if ((r > 0.0 && r < 1.0)) {
		//sprawdzanie pola!
	}
	else {
		cerr << "Współczynnik rozmnażania nie jest w przedziale od 0 do 1" << endl;
		return 2;
	}

	if ((p > 0)) {
		//sprawdzanie pola!
	}
	else {
		cerr << "Liczba pokoleń nie jest większa od zera" << endl;
		return 2;
	}

	if ((k > 0)) {
		//sprawdzanie pola!
	}
	else {
		cerr << "Liczba par nie jest większa od zera" << endl;
		return 2;
	}

	if (w <= r) {
		//sprawdzanie współczynników
	}
	else {
		cerr << "Współczynnik wymierania jest większy niż współczynnik rozmnażania" << endl;
		return 2;
	}

	return 0;
}
/** Funcja wczytuje i weryfikuje dane z pliku wejśćiowego.
 *
 * \param vect Dwuwymiarowy vector z populacją
 * \param filePath ścieżka pliku wejściowego
 * \return Zwraca 0 jeśli plik zostanie prawdłowo wczytany ,jeśli nie udało otworzyć pliku lub są w nim błędne dane zwróci wartość 1
 */
int fileRead(vector< vector<int> >& vect, string& filePath)
{
	ifstream file(filePath);
	string line;
	string lineCheck;
	stringstream stream;
	int i = 0;
	vector<int> tmp;
	int number;

	if (file.good()) {
		while (getline(file, line)) {
			stream.clear();
			stream << line;
			tmp.clear();
			if (stream >> number) {
				while (stream >> number) {
					tmp.push_back(number);
				}
			}
			else {
				cerr << "Błędne dane w pliku!";
				file.close();
				return 1;
			}
			vect.push_back(tmp);
		}
	}
	else {
		cerr << "Błąd plik nie otwarty!";
		file.close();
		return 1;
	}
	file.close();
	return 0;
}
/** Funkcja zapisuje populacje w pliku wyjściowym.
 *
 * \param vect Dwuwymiarowy vector z populacją
 * \param filePath ścieżka pliku wyjściowego
 */
void fileWrite(vector< vector<int> >& vect, string& filePath)
{
	ofstream file;
	file.open(filePath);
	for (int i = 0; i < int(vect.size()); i++) {
		for (int j = 0; j < int(vect[i].size()); j++)
		{
			file << vect[i][j];
			if (!(j == unsigned(vect[i].size()))) {
				file << " ";
			}
		}
		if (!(i == vect.size())) {
			file << "\n";
		}
	}
	file.close();
}
/** Funkcja wyświetla chromosom danego osobnika z populacji.
 *
 * \param vect Dwuwymiarowy vector z populacją
 * \param osobnik id wyświetlanego osobnika z populacji
 */
void genomePrint(vector< vector<int> >& vect, int osobnik) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// Zmiana koloru terminalu
	cout << "{";
	SetConsoleTextAttribute(hConsole, DNA_COLOR | FOREGROUND_INTENSITY);// Kolor Osobnika
	cout << " ";
	for (int j = 0; j < int(vect[osobnik].size()); j++)
	{
		cout << vect[osobnik][j];
		cout << " ";
	}
	SetConsoleTextAttribute(hConsole, 15);// Reset koloru na biały
	cout << "}";
}

/** Funkcja wyświetla populacje do konsoli.
 *
 * \param vect Dwuwymiarowy vector z populacją
 * \see genomePrint(vector< vector<int> >& vect, int osobnik) Funkcja wyświetla chromosom danego osobnika z populacji
 */
void popPrint(vector< vector<int> >& vect) {// Wypisanie Populacji
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "-----------------------------------------------------------------POPULACJA-----------------------------------------------------------------" << endl;
	for (int i = 0; i < int(vect.size()); i++)
	{
		cout << i + 1 << ". ";
		genomePrint(vect, i);
		cout << endl;

		SetConsoleTextAttribute(hConsole, 15);// Reset koloru na biały
	}
	cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}

/** Funkcja tworząca chromosom nowego osobnika.
 *
 * \param vect Dwuwymiarowy vector z populacją
 * \param tmp Tymczasowy vector do przechowywania chromosomu nowego osobnika powstałego z krzyżowania
 * \param parent1 id 1 osobnika wylosowanego do krzyżowania
 * \param parent2 id 2 osobnika wylosowanego do krzyżowania
 * \param parent1Break Punkt pęknięcia chromosmu 1 osobnika wylosowanego do krzyżowania
 * \param parent2Break Punkt pęknięcia chromosmu 2 osobnika wylosowanego do krzyżowania
 * \param popCount Ilość osobników w populacji
 */
void childMix(vector< vector<int> >& vect, vector<int>& tmp, int parent1, int parent2, int parent1Break, int parent2Break, int popCount) {//Łączanie genomu rodziców
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// Zmiana koloru terminalu
	cout << "{";

	cout << " ";

	SetConsoleTextAttribute(hConsole, (((vect[parent1][0]) % 11) + ((vect[parent1][(vect[parent1].size() - 1)])) % 11) | FOREGROUND_INTENSITY);// Kolor rodzica 1
	for (int n = 0; n < parent1Break; n++) {
		tmp.push_back(vect[parent1][n]);
		cout << vect[parent1][n] << " ";
	}
	SetConsoleTextAttribute(hConsole, 15);

	cout << "|";

	SetConsoleTextAttribute(hConsole, (((vect[parent2][0]) % 11) + ((vect[parent2][(vect[parent2].size() - 1)])) % 11) | FOREGROUND_INTENSITY);// Kolor rodzica 2
	cout << " ";
	for (int n = parent2Break; n < int(vect[parent2].size()); n++) {
		tmp.push_back(vect[parent2][n]);
		cout << vect[parent2][n] << " ";
	}
	SetConsoleTextAttribute(hConsole, 15);

	cout << "}";
	cout << endl;

	vect.push_back(tmp);
}
/** Funckja wybierająca 2 osobników do krzyżowania i tworząca nowego osobnika z ich chromosomu.
 *
 * \param vect Dwuwymiarowy vector z populacją
 * \param k Ilość par wylosownaych do krzyżowania
 */
void mixing(vector< vector<int> >& vect, int k) {// Info o Krzyzowaniu
	cout << endl << "---------------------------------------------------------NASTĄPI ";
	cout << k;
	cout << " SKRZYŻOWAŃ GENÓW--------------------------------------------------------" << endl;
	for (int i = 0; i < k; i++) {
		vector<int> tmp;
		int popCount = vect.size();
		int parent1 = QUICK_RAND(0, popCount - 1);// wybor 1 rodzica
		int parent2 = QUICK_RAND(0, popCount - 1);
		if (parent1 == parent2) { // wybor 2 rodzica innego niz 1
			do {
				parent2 = QUICK_RAND(0, popCount - 1);
			} while (parent1 == parent2);
		}
		int parent1Break = QUICK_RAND(1, (vect[parent1].size() - 1)); // Miejsca pęknięcia genomu rodzica nr.1
		int parent2Break = QUICK_RAND(1, (vect[parent2].size() - 1)); // Miejsca pęknięcia genomu rodzica nr.2

		cout << endl << "--------------------------------------------------------------KRZYŻÓWKA NR.";
		cout << i + 1;
		cout << "---------------------------------------------------------------" << endl << endl;

		cout << "Pierwszy rodzic z " << i + 1 << " Pary to osobnik nr." << parent1 + 1 << " o genomie: "; // wypisanie 1 rodzica i jego genomu
		genomePrint(vect, parent1);
		cout << endl;

		cout << "Drugi rodzic z " << i + 1 << " Pary to osobnik nr." << parent2 + 1 << " o genomie:    "; // wypisanie 2 rodzica i jego genomu
		genomePrint(vect, parent2);
		cout << endl;
		cout << "Punkt pęknięcia genomu 1 Rodzica:" << parent1Break << endl;// Miejsca pęknięcia genomu rodzica nr.1
		cout << "Punkt pęknięcia genomu 2 Rodzica:" << parent2Break << endl;// Miejsca pęknięcia genomu rodzica nr.2

		cout << "Potomek z pary nr." << i + 1 << "  ";

		childMix(vect, tmp, parent1, parent2, parent1Break, parent2Break, popCount);
	}
}

/** Funkcja ustala wartość funkcji dopasowania dla każdego osobnika po czym usuwa lub klonuje go w zależnośći od współczynników Wymierania i Rozmnażania.
 *
 * \param vect Dwuwymiarowy vector z populacją
 * \param clone Vector z osobnikami do sklonowania
 * \param w Współczynnik Wymierania
 * \param r Współczynnik Rozmnażania
 */
void fittnes(vector< vector<int> >& vect, vector<int>& clone, double w, double r) {
	int number = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < int(vect.size()); i++) {
		number++;
		double fit = (float)rand() / RAND_MAX;
		cout << number<< ". ";
		genomePrint(vect, i);
		cout << " f:" << fit;
		if (fit < w) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << " f<w OSOBNIK UMIERA";
			SetConsoleTextAttribute(hConsole, 15);
			vect.erase(vect.begin() + i);
			--i;
		}
		else if (fit > r) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << " f>r OSOBNIK ZOSTAJE SKLONOWANY";
			SetConsoleTextAttribute(hConsole, 15);
			clone.push_back(i);
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < int(clone.size()); i++) {
		vect.push_back(vect[clone[i]]);
	}
	clone.clear();
}
