#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <utility>
using namespace std;

class Polaczenie {
public:
	string nazwa;
	int odleglosc;

	Polaczenie(string _nazwa, int _odleglosc) {
		this->nazwa = _nazwa;
		this->odleglosc = _odleglosc;
	}
};

class PolaczeniePlus {
public:
	string nazwa;
	int odleglosc;
	int odlegloscOdZrodla;

	PolaczeniePlus(string _nazwa, int _odleglosc, int _odlegloscOdZrodla) {
		this->nazwa = _nazwa;
		this->odleglosc = _odleglosc;
		this->odlegloscOdZrodla = _odlegloscOdZrodla;
	}
};

class Graf {
private:
	vector<string> listaWierzcholkow;
	int iloscWierzcholkow;
	vector<vector<Polaczenie>> listaSasiedztwa;
	vector<pair<string, int>> listaTras;

public:
	Graf(ifstream* plik) {
		iloscWierzcholkow = 0;
		stringstream nazwa_stringstream;
		string nazwa, line;

		getline(*plik, line);
		nazwa_stringstream.str(line);
		
		while (nazwa_stringstream >> nazwa) {
			listaWierzcholkow.push_back(nazwa);
			iloscWierzcholkow++;
		} 

		for (int i = 0; i < iloscWierzcholkow; i++) {
			listaSasiedztwa.push_back(std::vector<Polaczenie>());
		}

		string str1, str2, odl;
		while (true) {
			*plik >> str1;
			*plik >> str2;
			*plik >> odl;
			
			if (getIndex(str1) != -1 && getIndex(str2) != -1) {
				listaSasiedztwa[getIndex(str1)].push_back(Polaczenie(str2, stoi(odl)));
			}
			else cout << "Do listy nie mozna dodac " + str1 + " i " + str2 
				+ "(nie zostaly uwzglednione w 1 wierszu pliku tekstowego)" << endl;
			if (plik->eof()) break;
		}
	}

	int getIndex(string str) {
		for (unsigned int i = 0; i < listaWierzcholkow.size(); i++) {
			if (listaWierzcholkow[i] == str) {
				return i;
			}
		}
		return -1;
	}

	string getName(int i) {
		return listaWierzcholkow[i];
	}

	void showList() {
		for (unsigned int i = 0; i < listaSasiedztwa.size(); i++) {
			cout << listaWierzcholkow[i] << " -> ";
			for (unsigned int j = 0; j < listaSasiedztwa[i].size(); j++) {
				cout << listaSasiedztwa[i][j].nazwa << "(" << listaSasiedztwa[i][j].odleglosc << ") ";
			}
			cout << endl;
		}
		cout << endl;
	}

	void showDirectly(string city) {
		for (unsigned int j = 0; j < listaSasiedztwa[getIndex(city)].size(); j++) {
			cout << listaSasiedztwa[getIndex(city)][j].nazwa << "(Czas przelotu: " 
				<< listaSasiedztwa[getIndex(city)][j].odleglosc << ")\n";
		}
		cout << endl;
	}

	void BFS(string v) {
		int index = getIndex(v);
		int* dystans = new int[iloscWierzcholkow];
		int* poprzednik = new int[iloscWierzcholkow];
		bool* visited = new bool[iloscWierzcholkow];
		for (int i = 0; i < iloscWierzcholkow; i++) {
			visited[i] = false;
			dystans[i] = INT_MAX;
			poprzednik[i] = -1;
		}
		list<Polaczenie> kolejka;
		visited[index] = true;
		dystans[index] = 0;
		kolejka.push_back(Polaczenie(getName(index), 0));
		
		while (!kolejka.empty()) {
			int aktualnyIndex = getIndex(kolejka.front().nazwa);
			if (kolejka.front().nazwa != v) {
				int iloscSegmentow = 0;
				int tmp = poprzednik[aktualnyIndex];
				while (tmp != -1) {
					tmp = poprzednik[tmp];
					iloscSegmentow++;
				}
				cout << getName(aktualnyIndex) << "(Czas przelotu: " << dystans[aktualnyIndex] 
					<< ", iloscSegmentow: " << iloscSegmentow << ")\n";
			}
			kolejka.pop_front();
			
			for (auto i : listaSasiedztwa[aktualnyIndex]) {
				if (!visited[getIndex(i.nazwa)]) {
					visited[getIndex(i.nazwa)] = true;
					dystans[getIndex(i.nazwa)] = dystans[aktualnyIndex] + i.odleglosc;
					poprzednik[getIndex(i.nazwa)] = aktualnyIndex;
					kolejka.push_back(Polaczenie(i.nazwa, i.odleglosc));
				}
			}
		}
	}

	void BFSdwaMiasta(string m1, string m2) {
		int index = getIndex(m1);
		bool* visited = new bool[iloscWierzcholkow];
		int* poprzednik = new int[iloscWierzcholkow];
		int* dystans = new int[iloscWierzcholkow];
		for (int i = 0; i < iloscWierzcholkow; i++) {
			visited[i] = false;
			poprzednik[i] = -1;
			dystans[i] = 0;
		}

		list<int> kolejka;
		visited[index] = true;
		kolejka.push_back(index);
		while (!kolejka.empty()) {
			int aktualnyIndex = kolejka.front();
			if (getName(aktualnyIndex) == m2) {
				int iloscSegmentow = 0;
				int tmp = poprzednik[getIndex(m2)];
				string tmpString = m2;
				int czas = dystans[getIndex(m2)];
				while (tmp != -1) {
					czas += dystans[tmp];
					tmpString = listaWierzcholkow[tmp] + " -> " + tmpString;
					tmp = poprzednik[tmp];
					iloscSegmentow++;
				}
				cout << tmpString << "\nCzas przelotu: " << dystans[getIndex(m2)] 
					<< ", iloscSegmentow: " << iloscSegmentow << endl;
				return;
			}
			kolejka.pop_front();
			for (auto i : listaSasiedztwa[aktualnyIndex]) {
				if (!visited[getIndex(i.nazwa)]) {
					visited[getIndex(i.nazwa)] = true;
					poprzednik[getIndex(i.nazwa)] = aktualnyIndex;
					dystans[getIndex(i.nazwa)] = dystans[aktualnyIndex] + i.odleglosc;
					kolejka.push_back(getIndex(i.nazwa));
				}
			}
		}
	}

	void dijkstraDwaMiasta(string m1, string m2) {
		int index = getIndex(m1);
		int* dystans = new int[iloscWierzcholkow];
		int* poprzednik = new int[iloscWierzcholkow];
		bool* visited = new bool[iloscWierzcholkow];
		for (int i = 0; i < iloscWierzcholkow; i++) {
			visited[i] = false;
			dystans[i] = INT_MAX;
			poprzednik[i] = -1;
		}
		list<PolaczeniePlus> kolejka;
		visited[index] = true;
		dystans[index] = 0;
		kolejka.push_back(PolaczeniePlus(getName(index), 0, 0));

		while (!kolejka.empty()) {
			kolejka.sort([](const PolaczeniePlus& a, const PolaczeniePlus& b) 
				{ return a.odlegloscOdZrodla < b.odlegloscOdZrodla; });
			int aktualnyIndex = getIndex(kolejka.front().nazwa);
			kolejka.pop_front();

			for (auto i : listaSasiedztwa[aktualnyIndex]) {
				if (!visited[getIndex(i.nazwa)]) {
					if (dystans[getIndex(i.nazwa)] > dystans[aktualnyIndex] + i.odleglosc) {
						dystans[getIndex(i.nazwa)] = dystans[aktualnyIndex] + i.odleglosc;
						poprzednik[getIndex(i.nazwa)] = aktualnyIndex;
					}
					kolejka.push_back(PolaczeniePlus(i.nazwa, i.odleglosc, dystans[getIndex(i.nazwa)]));
				}
			}
			visited[aktualnyIndex] = true;
		}
		int iloscSegmentow = 0;
		int indexMiasta = getIndex(m2);
		int tmp = poprzednik[indexMiasta];
		string tmpString = listaWierzcholkow[indexMiasta];
		while (tmp != -1) {
			tmpString = listaWierzcholkow[tmp] + " -> " + tmpString;
			tmp = poprzednik[tmp];
			iloscSegmentow++;
		}
		cout << tmpString << "\nCzas przelotu: " << dystans[indexMiasta] 
			<< ", iloscSegmentow: " << iloscSegmentow << endl;
	}

	void dijkstraDwaMiastaNajdluzej(string m1, string m2) {
		vector<vector<Polaczenie>> odwroconaListaSasiedztwa;
		for (int i = 0; i < iloscWierzcholkow; i++) {
			odwroconaListaSasiedztwa.push_back(std::vector<Polaczenie>());
		}
		for (unsigned int i = 0; i < listaSasiedztwa.size(); i++) {
			for (unsigned int j = 0; j < listaSasiedztwa[i].size(); j++) {
				odwroconaListaSasiedztwa[i].push_back(
					Polaczenie(listaSasiedztwa[i][j].nazwa, -(listaSasiedztwa[i][j].odleglosc)));
			}
		}

		int index = getIndex(m1);
		int* dystans = new int[iloscWierzcholkow];
		int* poprzednik = new int[iloscWierzcholkow];
		bool* visited = new bool[iloscWierzcholkow];
		for (int i = 0; i < iloscWierzcholkow; i++) {
			visited[i] = false;
			dystans[i] = INT_MAX;
			poprzednik[i] = -1;
		}
		list<PolaczeniePlus> kolejka;
		visited[index] = true;
		dystans[index] = 0;
		kolejka.push_back(PolaczeniePlus(getName(index), 0, 0));

		while (!kolejka.empty()) {
			kolejka.sort([](const PolaczeniePlus& a, const PolaczeniePlus& b) 
				{ return a.odlegloscOdZrodla < b.odlegloscOdZrodla; });
			int aktualnyIndex = getIndex(kolejka.front().nazwa);
			kolejka.pop_front();

			for (auto i : odwroconaListaSasiedztwa[aktualnyIndex]) {
				if (!visited[getIndex(i.nazwa)]) {
					if (dystans[getIndex(i.nazwa)] > dystans[aktualnyIndex] + i.odleglosc) {
						dystans[getIndex(i.nazwa)] = dystans[aktualnyIndex] + i.odleglosc;
						poprzednik[getIndex(i.nazwa)] = aktualnyIndex;
					}
					kolejka.push_back(PolaczeniePlus(i.nazwa, i.odleglosc, dystans[getIndex(i.nazwa)]));
				}
			}
			visited[aktualnyIndex] = true;
		}
		int iloscSegmentow = 0;
		int indexMiasta = getIndex(m2);
		int tmp = poprzednik[indexMiasta];
		string tmpString = listaWierzcholkow[indexMiasta];
		while (tmp != -1) {
			tmpString = listaWierzcholkow[tmp] + " -> " + tmpString;
			tmp = poprzednik[tmp];
			iloscSegmentow++;
		}
		cout << tmpString << "\nCzas przelotu: " << -dystans[indexMiasta] 
			<< ", iloscSegmentow: " << iloscSegmentow << endl;
	}

	void findAllPaths(string m1, string m2)
	{
		bool* visited = new bool[iloscWierzcholkow];
		int* path = new int[iloscWierzcholkow];
		int path_index = 0; 
		for (int i = 0; i < iloscWierzcholkow; i++) {
			visited[i] = false;
		}

		findAllPathsUtil(getIndex(m1), getIndex(m2), visited, path, path_index);
	}

	void findAllPathsUtil(int m1, int m2, bool visited[], int path[], int& path_index)
	{
		visited[m1] = true;
		path[path_index] = m1;
		path_index++;

		if (m1 == m2) {
			int odl = 0;
			string tmpStr = "";
			int tmpDystans = 0;
			for (int i = 0; i < path_index; i++) {
				tmpStr += getName(path[i]);
				tmpStr += " ";
				if (i < path_index - 1) {
					for (auto s : listaSasiedztwa[path[i]]) {
						if (s.nazwa == getName(path[i + 1])) {
							tmpDystans += s.odleglosc;
							break;
						}
					}
				}
			}
			listaTras.push_back(make_pair(tmpStr, tmpDystans));
		}
		else {
			for (auto i : listaSasiedztwa[m1]) {
				if (!visited[getIndex(i.nazwa)])
					findAllPathsUtil(getIndex(i.nazwa), m2, visited, path, path_index);
			}
		}

		path_index--;
		visited[m1] = false;
	}

	void getLongestPath() {
		int max = 0;
		string tmpStr = "";
		int czas;
		for (auto n : this->listaTras) {
			if (countSpaces(n.first) > max) {
				max = countSpaces(n.first);
				tmpStr = n.first;
				czas = n.second;
			}
		}
		cout << tmpStr << "\nCzas przelotu: " << czas << ", iloscSegmentow: " << max - 1;
	}

	int countSpaces(string s) {
		int count = 0;
		for (int i = 0; i < s.size(); i++)
			if (s[i] == ' ') count++;
		return count;
	}
};


int main() {
	ifstream input;
	input.open("miasta.txt");
	Graf g(&input);
	input.close();

	cout << "LISTA SASIEDZTWA:\n";
	g.showList();

	string city = "Wroclaw";
	cout << "DOSTEPNE BEZPOSREDNIO (" << city << "):\n";
	g.showDirectly(city);

	cout << "DOSTEPNE Z PRZESIADKAMI (" << city << "):\n";
	g.BFS(city);

	string miasto1 = "Wroclaw", miasto2 = "Poznan";
	cout << "\nNAJKROTSZA TRASA " << miasto1 << " - " << miasto2 << " (pod wzgledem czasu):\n";
	g.dijkstraDwaMiasta(miasto1, miasto2);

	cout << "\nNAJKROTSZA TRASA " << miasto1 << " - " << miasto2 << " (pod wzgledem ilosci segmentow):\n";
	g.BFSdwaMiasta(miasto1, miasto2);

	cout << "\nNAJDLUZSZA TRASA " << miasto1 << " - " << miasto2 << " (pod wzgledem czasu):\n";
	g.dijkstraDwaMiastaNajdluzej(miasto1, miasto2);

	cout << "\nNAJDLUZSZA TRASA " << miasto1 << " - " << miasto2 << " (pod wzgledem ilosci segmentow):\n";
	g.findAllPaths(miasto1, miasto2);
	g.getLongestPath();
}