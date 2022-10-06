#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <utility>

/*Data Structures:
1. Lc,r and Ln,s: List containing two elemnts which stores pairs {vi, vj} --> vector<vector<int,int>>
2. Lp and Lf: List containig one elements which stores pairs {vi, vj} --> vector<int,int>
3. L: List containing all Lc,r or Ln,s --> vector<vector<vector<pint>>>
4. Pc and Pn: matrices that contain a pointer to Lc,r and Ln,s --> vector <map<pint,pint>>

*/
using namespace std;
typedef pair<int, int> pint;

struct edge {
	int node1;
	int node2;
};
class Kernelization {
	int V;
	int k;
	vector<vector<pint>> commonNeighborsList; // Lc,r 
	vector<vector<pint>> nonCommonNeighborsList;// Ln,s
	vector <pint> permanentList; // Lp
	vector <pint> forbiddenList; // Lf
	vector <pint> listOfPermanentVertices;// T[v]:= permanent
	vector <pint> listOfForbiddenVertices;// T[v]:= forbidden
	vector <pint> vectorOfPairs;
	map <pint, pint> map_pint;
	vector <map<pint, pint> > Pc;
	vector <map<pint, pint> > Pn;
	vector <vector<vector<pint > > >  L;
	vector< vector< pint>> W;
	int** CounterNew;


public:
	Kernelization(int V, int k);
	/*--------------------------------------------Functions for kernelization basic ------------------------------------------------------------*/
	int** addCommonNeighbor(vector<vector<int> > adj, int V, int** commonCounter);// C[i,j]
	int** addNonCommonNeighbor(vector<vector<int> > adj, int V, int** NonCommonCounter); // N[i,j]
	vector< vector<pint>> updateCommonList(int** commonN, int** NonCommonN, vector <pint> commonNeighborsList, int V, int k);//Update Lc,r 
	vector< vector<pint>> updateNonCommonList(int** commonN, int** NonCommonN, vector <pint> nonCommonNeighborsList, int V, int k);//Update Ln,s
	vector< vector<pint>> updatePermanentList(int** commonN, int** NonCommonN, vector <pint> permanentList, int V, int k, vector<int> adjList[]);//Update Lp
	vector< vector<pint>> updateForbiddenList(int** commonN, int** NonCommonN, vector <pint> forbiddenList, int V, int k, vector<int> adjList[]);//Update Lf
	void investigateNewVertices(int** commonN, int** NonCommonN, vector<pint> permanentList, vector<pint> forbiddenList, int V, int k, vector<int> adjList[], vector<pint> commonNeighbor, vector<pint> nonCommonNeighbor);//Investigate new vertex v_l
	void ArrayPointerToListCommon(vector< vector< pint>> pair2dvector, int V);
	//void ArrayPointerToListNonCommon(int vi, int vj);

	/*--------------------------------------------Functions for adjacency list and matrix ------------------------------------------------------------*/
	void insert(int** adj, int u, int v);
	void insert(vector<int>* p, int u, int v);
	void printList(vector<int> adj[], int V);
	vector<vector<int>> convert(vector<int> adj[], int V);
	void printMatrix(vector<vector<int> > adj, int V);
	void edgeList(vector<vector<int> > adj, int V, vector <int> ListOfEdges[]);
	void remove(int** ccounter, int u, int v);
	void remove(vector<int>* adj, int u, int v);


};


/*--------------------------------------------------------------Kernelization basic-------------------------------------------------------------------------------*/
// Constructor
Kernelization::Kernelization(int V, int k) {
	this->V = V;
	this->k = k;
	CounterNew = new int* [V];
	for (int i = 0; i < V; i++) {
		CounterNew[i] = new int[V];
	}

	//initialization of vector of pairs: 1d array
	for (int i = 1; i < V; i++) {
		vectorOfPairs.push_back(make_pair(0, 0));
	}
	cout << "size of 1d array: " << vectorOfPairs.size() << endl;

	for (size_t i = 1; i < V; i++) {
		cout << "V[" << i - 1 << "] = " << vectorOfPairs[i - 1].first << vectorOfPairs[i - 1].second << endl;
	}




	//initialization of W: 2d array

	for (int i = 1; i < V; i++)
	{
		vectorOfPairs.push_back(make_pair(0, 0));

		for (int j = 1; j < V; j++)
		{
			W.push_back(vectorOfPairs);

		}
	}

	cout << "size of 2d array: " << W.size() << endl;
	for (size_t i = 1; i < V; i++)
		for (size_t j = 1; j < V; j++)
			cout << "W[" << i - 1 << "][" << j - 1 << "] = " << W[i - 1][j - 1].first << W[i - 1][j - 1].second << endl;



	//initialization of L: 3d arary
	for (int i = 1; i < V; i++)
	{
		vectorOfPairs.push_back(make_pair(0, 0));

		for (int j = 1; j < V; j++)
		{
			W.push_back(vectorOfPairs);
			for (int k = 1; k < V; k++)
			{
				L.push_back(W);
			}
		}
	}
	cout << "size of 3d array: " << L.size() << endl;
	for (size_t i = 1; i < V; i++)
		for (size_t j = 1; j < V; j++)
			for (size_t k = 1; k < V; k++)
				cout << "L[" << i - 1 << "][" << j - 1 << "][" << k - 1 << "] = " << L[i - 1][j - 1][k - 1].first << L[i - 1][j - 1][k - 1].second << endl;



	for (int i = 1; i < V; i++) {
		map_pint[make_pair(0, 0), make_pair(0, 0)];
		Pc.push_back(map_pint);
	}
	/*for (int i = 1; i <= V; i++) {
		for (auto x : map_pint[] {
			cout << x.first << "-" << x.second << endl;
		}
	}*/
	/*map <pint, pint> ::iterator itr;
	for (itr = map_pint.begin(); itr != map_pint.end(); ++itr) {
		cout <<  itr.first << itr.second << endl;
	}
	for (size_t i = 1; i < map_pint.size(); i++) {
		cout << "map of pairs is: " << map_pint[].first << map_pint[i].second << endl;
	}
	*/

	/*ArrayPointerToList = new int** [V];
	for (int i = 0; i < V; i++) {
		ArrayPointerToList[i] = new int* [V];
		for (int j = 0; j < V; j++) {
			ArrayPointerToList[i][j] = new int[V];
			for (int l = 0; l < V; k++) {
				ArrayPointerToList[i][j][l] = 0;
			}
		}
	}*/

	/*for (int i = 0; i < V; i++)
	{
		vector < vector < int > > w;
		ArrayPointer.push_back(w);
		for (int j = 0; j < V; j++)
		{
			vector <int> v;
			ArrayPointer[i].push_back(v);
			for (int k = 0; k < V; k++)
			{
				ArrayPointer[i][j].push_back(1);
			}
		}
	}

	for (size_t i = 0; i < ArrayPointer.size(); i++)
		for (size_t j = 0; j < ArrayPointer[i].size(); j++)
			for (size_t k = 0; k < ArrayPointer[i][j].size(); k++)
				cout << "ArrayPointer[" << i << "][" << j << "][" << k << "] = " << ArrayPointer[i][j][k] << endl;*/
}

int** Kernelization::addCommonNeighbor(vector<vector<int> > adj, int V, int** commonCounter) {

	for (int i = 1; i < V; i++) {
		for (int j = 1; j < V; j++) {
			for (int l = 1; l < V; l++) {
				if ((i < j) && i != l && j != l)
					if (adj[i - 1][l - 1] == 1 && adj[j - 1][l - 1] == 1)
						insert(commonCounter, i - 1, j - 1);


			}
		}
	}
	for (int i = 1; i < V; i++) {
		for (int j = 1; j < V; j++) {
			cout << commonCounter[i - 1][j - 1] << "  ";
		}
		cout << endl;
	}
	cout << "\n";

	return commonCounter;
}

int** Kernelization::addNonCommonNeighbor(vector<vector<int> > adj, int V, int** NonCommonCounter) {

	for (int i = 1; i < V; i++) {
		for (int j = 1; j < V; j++) {
			for (int l = 1; l < V; l++) {
				if ((i < j) && i != l && j != l)
					if (adj[i - 1][l - 1] != adj[j - 1][l - 1])
						insert(NonCommonCounter, i - 1, j - 1);
			}
		}
	}
	for (int i = 1; i < V; i++) {
		for (int j = 1; j < V; j++) {
			cout << NonCommonCounter[i - 1][j - 1] << "  ";
		}
		cout << endl;
	}
	cout << "\n";
	return NonCommonCounter;
}

vector< vector<pint>> Kernelization::updateCommonList(int** commonN, int** NonCommonN, vector <pint> commonNeighborsList, int V, int k) {


	vector< vector<pair<int, int> > > pair2dVectorCommon;
	pint apair;
	bool terminate = false;
	for (int i = 1; !terminate && i < V; i++) {
		for (int j = 1; !terminate && j < V; j++) {
			if (commonN[i - 1][j - 1] > k && NonCommonN[i - 1][j - 1] > k) {
				terminate = true;
				break;
			}
			if (commonN[i - 1][j - 1] <= k && commonN[i - 1][j - 1] != 0) {
				apair.first = i;
				apair.second = j;
				commonNeighborsList.push_back(apair);

			}
			else {
				commonNeighborsList.push_back(make_pair(0, 0));
			}
		}
		pair2dVectorCommon.push_back(commonNeighborsList);
		commonNeighborsList.clear();
	}

	for (vector< vector<pair<int, int> > >::iterator it = pair2dVectorCommon.begin(); it != pair2dVectorCommon.end(); ++it) {
		commonNeighborsList = *it;
		for (vector<pair<int, int> >::iterator it2 = commonNeighborsList.begin(); it2 != commonNeighborsList.end(); ++it2) {
			apair = *it2;
			cout << "(" << apair.first << "," << apair.second << ") ; ";
		}
		cout << '\n';
	}
	return pair2dVectorCommon;

}

vector< vector<pint>> Kernelization::updateNonCommonList(int** commonN, int** NonCommonN, vector <pint> nonCommonNeighborsList, int V, int k) {


	vector< vector<pair<int, int> > > pair2dvectorNonCommon;
	pint apair;
	bool terminate = false;
	for (int i = 1; !terminate && i < V; i++) {
		for (int j = 1; !terminate && j < V; j++) {
			if (commonN[i - 1][j - 1] > k && NonCommonN[i - 1][j - 1] > k) {
				terminate = true;
				break;
			}
			if (NonCommonN[i - 1][j - 1] <= k && NonCommonN[i - 1][j - 1] != 0) {
				apair.first = i;
				apair.second = j;
				nonCommonNeighborsList.push_back(apair);
			}
			else {
				nonCommonNeighborsList.push_back(make_pair(0, 0));
			}
		}
		pair2dvectorNonCommon.push_back(nonCommonNeighborsList);
		nonCommonNeighborsList.clear();
	}

	for (vector< vector<pair<int, int> > >::iterator it = pair2dvectorNonCommon.begin(); it != pair2dvectorNonCommon.end(); ++it) {
		nonCommonNeighborsList = *it;
		for (vector<pair<int, int> >::iterator it2 = nonCommonNeighborsList.begin(); it2 != nonCommonNeighborsList.end(); ++it2) {
			apair = *it2;
			cout << "(" << apair.first << "," << apair.second << ") ; ";
		}
		cout << '\n';
	}
	return pair2dvectorNonCommon;

}

vector< vector<pint>> Kernelization::updatePermanentList(int** commonN, int** NonCommonN, vector<pint> permanentList, int V, int k, vector<int> adjList[]) {

	vector< vector<pair<int, int> > > pair2dvectorPermanent;
	pint apair;
	bool terminate = false;
	for (int i = 1; !terminate && i < V; i++) {
		for (int j = 1; !terminate && j < V; j++) {
			if (commonN[i - 1][j - 1] > k && NonCommonN[i - 1][j - 1] > k) {
				terminate = true;
				break;
			}
			if (commonN[i - 1][j - 1] > k && NonCommonN[i - 1][j - 1] <= k) {
				apair.first = i;
				apair.second = j;
				permanentList.push_back(apair);
				if (!(find(adjList[i - 1].begin(), adjList[i - 1].end(), j - 1) != adjList[i - 1].end())) {
					insert(adjList, i - 1, j - 1);
					//k = k - 1;
				}
			}
			//k = k - 1;
		}
		pair2dvectorPermanent.push_back(permanentList);
		permanentList.clear();
	}
	//vector <pint> second;
	//for (vector< vector<pair<int, int> > >::iterator it = pair2dvector.begin(); !exit_kon && it != pair2dvector.end(); ++it) {

	for (vector< vector<pair<int, int> > >::iterator it = pair2dvectorPermanent.begin(); it != pair2dvectorPermanent.end(); ++it) {
		permanentList = *it;
		for (vector<pair<int, int> >::iterator it2 = permanentList.begin(); it2 != permanentList.end(); ++it2) {
			apair = *it2;
			cout << "(" << apair.first << "," << apair.second << ") ; ";
		}
		cout << '\n';
	}
	return pair2dvectorPermanent;
}

vector< vector<pint>> Kernelization::updateForbiddenList(int** commonN, int** NonCommonN, vector<pint> forbiddenList, int V, int k, vector<int> adjList[]) {
	vector< vector<pair<int, int> > > pair2dvectorForbidden;
	pint apair;
	bool terminate = false;
	for (int i = 1; !terminate && i < V; i++) {
		for (int j = 1; !terminate && j < V; j++) {
			if (commonN[i - 1][j - 1] > k && NonCommonN[i - 1][j - 1] > k) {
				terminate = true;
				break;
			}
			if (NonCommonN[i - 1][j - 1] > k && commonN[i - 1][j - 1] <= k) {
				apair.first = i;
				apair.second = j;
				forbiddenList.push_back(apair);
				if (find(adjList[i - 1].begin(), adjList[i - 1].end(), j - 1) != adjList[i - 1].end()) {
					remove(adjList, i - 1, j - 1);
					//k = k - 1;
				}

			}

		}
		pair2dvectorForbidden.push_back(forbiddenList);
		forbiddenList.clear();
	}

	for (vector< vector<pair<int, int> > >::iterator it = pair2dvectorForbidden.begin(); it != pair2dvectorForbidden.end(); ++it) {
		forbiddenList = *it;
		for (vector<pair<int, int> >::iterator it2 = forbiddenList.begin(); it2 != forbiddenList.end(); ++it2) {
			apair = *it2;
			cout << "(" << apair.first << "," << apair.second << ") ; ";
		}
		cout << '\n';
	}

	return pair2dvectorForbidden;

}

void Kernelization::ArrayPointerToListCommon(vector< vector< pint>> pair2dvector, int V) {
	cout << "Size of 3d array L is: " << L.size() << endl;
	//cout << "Size of 2d array pair2dvector is: " << pair2dvector.size() << endl;

	int vi, vj;
	for (int i = 1; i < V; i++) {
		pair2dvector = L[i - 1];
		cout << "Print i: " << i << endl;
		//cout << "Size of 2d array pair2dvector is: " << pair2dvector.size() <<endl;
		for (int j = 1; j < V; j++) {
			cout << "Print j: " << j << endl;
			vector<pint> T = pair2dvector[j - 1];
			//cout << "print size of 2d array" <<pair2dvector.size() << endl;
			bool found = false;
			for (int k = 1; k < V; k++) {
				cout << "print k: " << k << endl;
				if (T[k - 1] == make_pair(i - 1, j - 1)) {
					found = true;
					cout << "Size of 1d array Pc is: " << Pc.size() << endl;
					//cout << "value of i: " << i << endl;
					Pc[i - 1][make_pair(vi, vj)] = make_pair(i - 1, j - 1);
					//////
					cout << "here 3" << endl;
				}
				//cout << "value of i: " << i << endl;

				cout << "size of Pc: " << Pc.size() << endl;
				if (Pc.size() > i - 1) {
					cout << "******************************************************************************" << endl;
					///cout << "print Pc here: "<< endl;
					//for (vector<map<pint, pint>>::iterator ite = Pc.begin(); ite != Pc.end(); ite++) {
					//	cout << "*********************" << *ite << endl;
					//}
					map<pint, pint> T = Pc[i - 1];
					if (T.find(make_pair(vi, vj)) != T.end()) {
						//cout << "here 4 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
						// found it.
						cout << endl;
						//T[make_pair(vi, vj)];
					}
					//cout << "print T here: " << endl;
					//for (map<pint, pint>::iterator ite = T.begin(); ite != T.end(); ite++) {
						//	cout << "*********************" << (*ite).first << " " << (*ite).second << endl;
					//}
				}
				else {
					cout << "P_cij is null" << endl;
				}
				cout << "L[" << i - 1 << "][" << j - 1 << "][" << k - 1 << "] = " << L[i - 1][j - 1][k - 1].first << L[i - 1][j - 1][k - 1].second << endl;
			}

		}

	}
	/*for (int i = 1; i < V; i++) {
		for (int j = 1; j < V; j++) {
			cout << Pc[i][make_pair(vi, vj)] << "  ";
		}
		cout << endl;
	}
	cout << "\n";*/
}

/*------------------------------------------------------------------Kernelization part 1: Lp-------------------------------------------------------------------------------------------------*/

void Kernelization::investigateNewVertices(int** commonN, int** NonCommonN, vector<pint> permanentList, vector<pint> forbiddenList, int V, int k, vector<int> adjList[], vector<pint> commonNeighbor, vector<pint> nonCommonNeighbor) {

	bool terminate = false;
	for (int i = 1; !terminate && i < V; i++) {
		for (int j = 1; !terminate && j < V; j++) {
			if (forbiddenList.size() == permanentList.size() == 0) {
				terminate = true;
				break;
			}
			if (find(permanentList.begin(), permanentList.end(), make_pair(i - 1, j - 1)) != permanentList.end() && find(forbiddenList.begin(), forbiddenList.end(), make_pair(i - 1, j - 1)) != forbiddenList.end()) {
				terminate = true;
				break;
			}
			//Line 7-13 of Algorithm 2 to investigate a new node for List of permanent vertices 
			for (auto it1 = forbiddenList.begin(); it1 != forbiddenList.end(); it1++) {
				if (i < j && find(permanentList.begin(), permanentList.end(), make_pair(i - 1, j - 1)) != permanentList.end() && it1->first != i - 1 && it1->second != j - 1) {
					if (!(find(adjList[i - 1].begin(), adjList[i - 1].end(), j - 1) != adjList[i - 1].end())) {
						insert(adjList, i - 1, j - 1);
					}
				}
			}
			//Line 7-13 of Algorithm 3 to investigate a new node for List of forbidden vertices 
			for (auto it2 = permanentList.begin(); it2 != permanentList.end(); it2++) {
				if (i < j && find(forbiddenList.begin(), forbiddenList.end(), make_pair(i - 1, j - 1)) != forbiddenList.end() && it2->first != i - 1 && it2->second != j - 1) {
					if ((find(adjList[i - 1].begin(), adjList[i - 1].end(), j - 1) != adjList[i - 1].end())) {
						remove(adjList, i - 1, j - 1);
					}
				}
			}
		}
	}
	while (k > 0) {
		for (int i = 1; i < V; i++) {
			for (int j = 1; j < V; j++) {
				for (int l = 1; l < V; l++) {
					//Line 20-50 of Algorithm 2 for updating Lp
					cout << "print this line" << endl;
					if ((find(adjList[i - 1].begin(), adjList[i - 1].end(), j - 1) != adjList[i - 1].end())) {
						if (i < j < l && i != l && j != l && (find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) &&
							(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
							// Update all common and non-common counters
							NonCommonN[i - 1][l - 1] -= 1;
							commonN[i - 1][l - 1] += 1;
							NonCommonN[j - 1][l - 1] -= 1;
							commonN[j - 1][l - 1] += 1;
						}

						else if (i < j < l && i != l && j != l && !(find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) &&
							(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
							NonCommonN[i - 1][l - 1] -= 1;
							commonN[i - 1][l - 1] += 1;
							NonCommonN[j - 1][l - 1] += 1;
							commonN[j - 1][l - 1] -= 1;
							cout << "print the second line" << endl;
						}
						else if (i < j < l && i != l && j != l && (find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) &&
							!(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
							NonCommonN[i - 1][l - 1] += 1;
							commonN[i - 1][l - 1] -= 1;
							NonCommonN[j - 1][l - 1] -= 1;
							commonN[j - 1][l - 1] += 1;
							cout << "print the third line" << endl;
						}
						else if (i < j < l && i != l && j != l && !(find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) &&
							!(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
							NonCommonN[i - 1][l - 1] += 1;
							commonN[i - 1][l - 1] -= 1;
							NonCommonN[j - 1][l - 1] += 1;
							commonN[j - 1][l - 1] -= 1;
							cout << "print the 4th line" << endl;
						}
						else {
							cout << "None of conditions works for list of permanent vertices" << endl;
						}
						// After updating counters, we will update the Lc,r , Ln,s , Lf, and Lp
						// C[i , l] > k: remove from Lc,r, add to the Lp, add to the set of edges
						//C[i,l], c[j,l], N[i,l], N[j,l] >k and <=k --> in total we have to investigate 8 conditions for each list
						if (commonN[i - 1][l - 1] > k) {
							for (auto it1 = permanentList.begin(); it1 != permanentList.end(); it1++) {
								if (it1->first != i - 1 && it1->second != l - 1) {
									permanentList.push_back(make_pair(i - 1, l - 1));
									for (auto itr = commonNeighbor.begin(); itr != commonNeighbor.end(); itr++) {
										if (itr->first == i - 1 && itr->second == l - 1) {
											commonNeighbor.erase(itr);
										}
									}

									if (!(find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end())) {
										insert(adjList, i - 1, l - 1);
										k = k - 1;
									}
									else {
										cout << " this pair belongs to the edge set" << endl;
									}
								}
								else {
									cout << " this pair already belongs to Lp" << endl;
								}
							}

						}
						//N[i , l]> k
						if (NonCommonN[i - 1][l - 1] > k) {
							for (auto it1 = forbiddenList.begin(); it1 != forbiddenList.end(); it1++) {
								if (it1->first != i - 1 && it1->second != l - 1) {
									forbiddenList.push_back(make_pair(i - 1, l - 1));
									for (auto itr = nonCommonNeighbor.begin(); itr != nonCommonNeighbor.end(); itr++) {
										if (itr->first == i - 1 && itr->second == l - 1) {
											nonCommonNeighbor.erase(itr);
										}
									}

									if (find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) {
										remove(adjList, i - 1, l - 1);
										k = k - 1;
									}
									else {
										cout << " this pair does not belong to the edge set" << endl;
									}
								}
								else {
									cout << " this pair already belongs to Lf" << endl;
								}
							}

						}
						//N[j , l]> k
						if (NonCommonN[j - 1][l - 1] > k) {
							for (auto it1 = forbiddenList.begin(); it1 != forbiddenList.end(); it1++) {
								if (it1->first != j - 1 && it1->second != l - 1) {
									forbiddenList.push_back(make_pair(j - 1, l - 1));
									for (auto itr = nonCommonNeighbor.begin(); itr != nonCommonNeighbor.end(); itr++) {
										if (itr->first == j - 1 && itr->second == l - 1) {
											nonCommonNeighbor.erase(itr);
										}
									}

									if (find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end()) {
										remove(adjList, j - 1, l - 1);
										k = k - 1;
									}
									else {
										cout << " this pair does not belong to the edge set" << endl;
									}
								}
								else {
									cout << " this pair already belongs to Lf" << endl;
								}
							}

						}
						// C[i , l] > k

						if (commonN[i - 1][l - 1] > k) {
							for (auto it1 = permanentList.begin(); it1 != permanentList.end(); it1++) {
								if (it1->first != i - 1 && it1->second != l - 1) {
									permanentList.push_back(make_pair(i - 1, l - 1));
									for (auto itr = commonNeighbor.begin(); itr != commonNeighbor.end(); itr++) {
										if (itr->first == i - 1 && itr->second == l - 1) {
											commonNeighbor.erase(itr);
										}
									}

									if (!(find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end())) {
										insert(adjList, i - 1, l - 1);
										k = k - 1;
									}
									else {
										cout << " this pair belongs to the edge set" << endl;
									}
								}
								else {
									cout << " this pair already belongs to Lp" << endl;
								}
							}

						}

						// C[j , l] > k

						if (commonN[j - 1][l - 1] > k) {
							for (auto it1 = permanentList.begin(); it1 != permanentList.end(); it1++) {
								if (it1->first != j - 1 && it1->second != l - 1) {
									permanentList.push_back(make_pair(j - 1, l - 1));
									for (auto itr = commonNeighbor.begin(); itr != commonNeighbor.end(); itr++) {
										if (itr->first == j - 1 && itr->second == l - 1) {
											commonNeighbor.erase(itr);
										}
									}

									if (!(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
										insert(adjList, j - 1, l - 1);
										k = k - 1;
									}
									else {
										cout << " this pair belongs to the edge set" << endl;
									}
								}
								else {
									cout << " this pair already belongs to Lp" << endl;
								}
							}

						}

						if (NonCommonN[i - 1][l - 1] <= k) {
							forbiddenList.push_back(make_pair(i - 1, l - 1));
							if ((find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end())) {
								remove(adjList, i - 1, l - 1);
								k = k - 1;
							}
						}

						// N[j , l] <=k
						if (NonCommonN[j - 1][l - 1] <= k) {
							forbiddenList.push_back(make_pair(j - 1, l - 1));
							if ((find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
								remove(adjList, j - 1, l - 1);
								k = k - 1;
							}
						}
						// C[i , l] <=k
						if (commonN[i - 1][l - 1] <= k) {
							permanentList.push_back(make_pair(i - 1, l - 1));
							if (!(find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end())) {
								insert(adjList, i - 1, l - 1);
								k = k - 1;
							}
						}
						//C[j , l] <= k
						if (NonCommonN[j - 1][l - 1] <= k) {
							forbiddenList.push_back(make_pair(j - 1, l - 1));
							if ((find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
								remove(adjList, -1, l - 1);
								k = k - 1;
							}
						}
					}

					//Line 20-50 of Algorithm 3 for updating Lf 
					if (!(find(adjList[i - 1].begin(), adjList[i - 1].end(), j - 1) != adjList[i - 1].end())) {
						if (i < j < l && i != l && j != l && (find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) &&
							(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
							NonCommonN[i - 1][l - 1] += 1;
							commonN[i - 1][l - 1] -= 1;
							NonCommonN[j - 1][l - 1] += 1;
							commonN[j - 1][l - 1] -= 1;
						}
						else if (i < j < l && i != l && j != l && !(find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) &&
							(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
							NonCommonN[i - 1][l - 1] += 1;
							commonN[i - 1][l - 1] -= 1;


						}
						else if (i < j < l && i != l && j != l && (find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) &&
							!(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {

							NonCommonN[j - 1][l - 1] += 1;
							commonN[j - 1][l - 1] -= 1;

						}
						else if (i < j < l && i != l && j != l && !(find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) &&
							!(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
							cout << "Nothing has changed" << endl;
							cout << "\n";

						}
						else {
							cout << "None of conditions works for list of forbidden vertices" << endl;
						}

						//N[i , l]> k
						if (NonCommonN[i - 1][l - 1] > k) {
							for (auto it1 = forbiddenList.begin(); it1 != forbiddenList.end(); it1++) {
								if (it1->first != i - 1 && it1->second != l - 1) {
									forbiddenList.push_back(make_pair(i - 1, l - 1));
									for (auto itr = nonCommonNeighbor.begin(); itr != nonCommonNeighbor.end(); itr++) {
										if (itr->first == i - 1 && itr->second == l - 1) {
											nonCommonNeighbor.erase(itr);
										}
									}

									if (find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end()) {
										remove(adjList, i - 1, l - 1);
										k = k - 1;
									}
									else {
										cout << " this pair does not belong to the edge set" << endl;
									}
								}
								else {
									cout << " this pair already belongs to Lf" << endl;
								}
							}

						}
						//N[j , l]> k
						if (NonCommonN[j - 1][l - 1] > k) {
							for (auto it1 = forbiddenList.begin(); it1 != forbiddenList.end(); it1++) {
								if (it1->first != j - 1 && it1->second != l - 1) {
									forbiddenList.push_back(make_pair(j - 1, l - 1));
									for (auto itr = nonCommonNeighbor.begin(); itr != nonCommonNeighbor.end(); itr++) {
										if (itr->first == j - 1 && itr->second == l - 1) {
											nonCommonNeighbor.erase(itr);
										}
									}

									if (find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end()) {
										remove(adjList, j - 1, l - 1);
										k = k - 1;
									}
									else {
										cout << " this pair does not belong to the edge set" << endl;
									}
								}
								else {
									cout << " this pair already belongs to Lf" << endl;
								}
							}

						}
						// C[i , l] > k

						if (commonN[i - 1][l - 1] > k) {
							for (auto it1 = permanentList.begin(); it1 != permanentList.end(); it1++) {
								if (it1->first != i - 1 && it1->second != l - 1) {
									permanentList.push_back(make_pair(i - 1, l - 1));
									for (auto itr = commonNeighbor.begin(); itr != commonNeighbor.end(); itr++) {
										if (itr->first == i - 1 && itr->second == l - 1) {
											commonNeighbor.erase(itr);
										}
									}

									if (!(find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end())) {
										insert(adjList, i - 1, l - 1);
										k = k - 1;
									}
									else {
										cout << " this pair belongs to the edge set" << endl;
									}
								}
								else {
									cout << " this pair already belongs to Lp" << endl;
								}
							}

						}

						// C[j , l] > k

						if (commonN[j - 1][l - 1] > k) {
							for (auto it1 = permanentList.begin(); it1 != permanentList.end(); it1++) {
								if (it1->first != j - 1 && it1->second != l - 1) {
									permanentList.push_back(make_pair(j - 1, l - 1));
									for (auto itr = commonNeighbor.begin(); itr != commonNeighbor.end(); itr++) {
										if (itr->first == j - 1 && itr->second == l - 1) {
											commonNeighbor.erase(itr);
										}
									}

									if (!(find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
										insert(adjList, j - 1, l - 1);
										k = k - 1;
									}
									else {
										cout << " this pair belongs to the edge set" << endl;
									}
								}
								else {
									cout << " this pair already belongs to Lp" << endl;
								}
							}

						}

						// N[i , l] <=k
						if (NonCommonN[i - 1][l - 1] <= k) {
							forbiddenList.push_back(make_pair(i - 1, l - 1));
							if ((find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end())) {
								remove(adjList, i - 1, l - 1);
								k = k - 1;
							}
						}

						// N[j , l] <=k
						if (NonCommonN[j - 1][l - 1] <= k) {
							forbiddenList.push_back(make_pair(j - 1, l - 1));
							if ((find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
								remove(adjList, j - 1, l - 1);
								k = k - 1;
							}
						}
						// C[i , l] <=k
						if (commonN[i - 1][l - 1] <= k) {
							permanentList.push_back(make_pair(i - 1, l - 1));
							if (!(find(adjList[l - 1].begin(), adjList[l - 1].end(), i - 1) != adjList[l - 1].end())) {
								insert(adjList, i - 1, l - 1);
								k = k - 1;
							}
						}
						//C[j , l] <= k
						if (NonCommonN[j - 1][l - 1] <= k) {
							forbiddenList.push_back(make_pair(j - 1, l - 1));
							if ((find(adjList[l - 1].begin(), adjList[l - 1].end(), j - 1) != adjList[l - 1].end())) {
								remove(adjList, -1, l - 1);
								k = k - 1;
							}
						}
					}



				}
			}
		}

	}
}

/*for (auto it1  = forbiddenList.begin(), it2 = permanentList.begin(); it1 !=forbiddenList.end() && it2 != permanentList.end(); it1++ , it2++){
				if*/


				/*void Kernelization::terminate1(vector<int> forbiddenList[], vector<int> permanentList[], vector <int> ListOfEdges[], vector <int> listOfPermanentVertices[], vector <int> listOfForbiddenVertices[]) {
					bool isForbiddenListEmpty = true;
					bool isPermanentListEmpty = true;

					for (int i = 0; i < V; i++) {
						if (forbiddenList[i].size() != 0) {
							isForbiddenListEmpty = false;
						}
						if (permanentList[i].size() != 0) {
							isPermanentListEmpty = false;
						}
					}
					if (isPermanentListEmpty && isForbiddenListEmpty) {
						cout << "Algorithm terminates: Lp and Lf are both empty. " << endl;
						//exit(3);
					}
					else if (!isPermanentListEmpty && !isForbiddenListEmpty) {
						for (int i = 0; i < V; i++) {
							for (int j = 0; j < V; j++) {
								if (find(forbiddenList[i].begin(), forbiddenList[i].end(), j) != forbiddenList[i].end() &&
									find(permanentList[i].begin(), permanentList[i].end(), j) != permanentList[i].end()) {
									cout << "Algorithm terminates: Lp and Lf contains the same pair of vertices and it is impossible. " << endl;
									//exit(3);
								}
							}
						}
					}

					else if (!isPermanentListEmpty && isForbiddenListEmpty) {
						for (int i = 0; i < V; i++) {
							for (int j = 0; j < V; j++) {
								if (i < j && !(find(forbiddenList[i].begin(), forbiddenList[i].end(), j) != forbiddenList[i].end()) &&
									find(permanentList[i].begin(), permanentList[i].end(), j) != permanentList[i].end()) {
									if (!(find(ListOfEdges[i].begin(), ListOfEdges[i].end(), j) != ListOfEdges[i].end())) {
										insert(ListOfEdges, i, j);
										cout << "Add vi and vj to the set of edges" << endl;
										insert(listOfPermanentVertices, i, j);
										cout << "Add vi and vj to the list of permanent vertices" << endl;
									}
									else {
										insert(listOfPermanentVertices, i, j);
										cout << "Add vi and vj to the set of edges" << endl;
									}
								}
							}
						}
					}
					else if (isPermanentListEmpty && !isForbiddenListEmpty) {
						for (int i = 0; i < V; i++) {
							for (int j = 0; j < V; j++) {
								if (i < j && (find(forbiddenList[i].begin(), forbiddenList[i].end(), j) != forbiddenList[i].end()) &&
									!(find(permanentList[i].begin(), permanentList[i].end(), j) != permanentList[i].end())) {
									if (!(find(ListOfEdges[i].begin(), ListOfEdges[i].end(), j) != ListOfEdges[i].end())) {
										remove(ListOfEdges, i, j);
										cout << "Remove vi and vj from the set of edges" << endl;

									}
									else {
										remove(ListOfEdges, i, j);
										cout << "Remove vi and vj from the set of edges" << endl;
										insert(listOfForbiddenVertices, i, j);
										cout << "Add vi and vj to the list of forbidden vertices" << endl;
									}
								}
							}
						}
					}
				}

				void Kernelization::runKernelForLp(int k, int** commonN, int** NonCommonN, vector<int> forbiddenList[], vector<int> permanentList[], vector <int> ListOfEdges[], vector <int> listOfPermanentVertices[]) {
					while (k >= 0) {

						for (int i = 0; i < V; i++) {
							for (int j = 0; j < V; j++) {
								for (int l = 0; l < V; l++) {
									if ((i < j < l) && i != l && j != l)
										// find a list for edge not vertices
										if (find(ListOfEdges[l].begin(), ListOfEdges[l].end(), i) != ListOfEdges[l].end() &&
											find(ListOfEdges[l].begin(), ListOfEdges[l].end(), j) != ListOfEdges[l].end()) {
											NonCommonN[i][l] -= 1;
											commonN[i][l] += 1;
											NonCommonN[j][l] -= 1;
											commonN[j][l] += 1;
										}
									insert(listOfPermanentVertices, i, l);
									cout << "Add {vi, vl} to the list of permanent vertices" << endl;
									cout << "\n";
									insert(permanentList, i, l);
									cout << "Add {vi, vl} to the permanent list Lp" << endl;
									cout << "\n";
									if (find(listOfPermanentVertices[i].begin(), listOfPermanentVertices[i].end(), j) != listOfPermanentVertices[i].end() &&
										find(listOfPermanentVertices[i].begin(), listOfPermanentVertices[i].end(), l) != listOfPermanentVertices[i].end()) {
										insert(listOfPermanentVertices, j, l);
										cout << "Add {vj, vl} to the list of permanent vertices" << endl;
										cout << "\n";
										insert(permanentList, j, l);
										cout << "Add {vj, vl} to the permanent list Lp" << endl;
										cout << "\n";
									}
									else if (!(find(listOfPermanentVertices[l].begin(), listOfPermanentVertices[l].end(), i) != listOfPermanentVertices[l].end()) &&
										find(listOfPermanentVertices[l].begin(), listOfPermanentVertices[l].end(), j) != listOfPermanentVertices[l].end()) {
										NonCommonN[i][l] -= 1;
										commonN[i][l] += 1;
										NonCommonN[j][l] += 1;
									}
									else if (!(find(listOfPermanentVertices[l].begin(), listOfPermanentVertices[l].end(), i) != listOfPermanentVertices[l].end()) &&
										!(find(listOfPermanentVertices[l].begin(), listOfPermanentVertices[l].end(), j) != listOfPermanentVertices[l].end())) {
										NonCommonN[i][l] += 1;
										NonCommonN[j][l] += 1;
									}
								}
							}
						}

						k -= 1;
					}

				}

				/*------------------------------------------------------------------Kernelization part 2: Lf-------------------------------------------------------------------------------------------------*/

				/*void Kernelization::runKernelForLf(int k, int** commonN, int** NonCommonN, vector<int> forbiddenList[], vector<int> permanentList[], vector <int> ListOfEdges[], vector <int> listOfPermanentVertices[]) {
					while (k >= 0) {

						for (int i = 0; i < V; i++) {
							for (int j = 0; j < V; j++) {
								for (int l = 0; l < V; l++) {
									if ((i < j < l) && i != l && j != l)
										// find a list for edge not vertices
										if (find(ListOfEdges[l].begin(), ListOfEdges[l].end(), i) != ListOfEdges[l].end() &&
											find(ListOfEdges[l].begin(), ListOfEdges[l].end(), j) != ListOfEdges[l].end()) {
											NonCommonN[i][l] += 1;
											commonN[i][l] -= 1;
											NonCommonN[j][l] += 1;
											commonN[j][l] -= 1;
										}
									insert(listOfForbiddenVertices, i, l);
									cout << "Add {vi, vl} to the list of forbidden vertices" << endl;
									cout << "\n";
									insert(forbiddenList, i, l);
									cout << "Add {vi, vl} to the forbidden list Lf" << endl;
									cout << "\n";
									if (find(listOfPermanentVertices[i].begin(), listOfPermanentVertices[i].end(), j) != listOfPermanentVertices[i].end() &&
										find(listOfForbiddenVertices[i].begin(), listOfForbiddenVertices[i].end(), l) != listOfForbiddenVertices[i].end()) {
										insert(listOfForbiddenVertices, j, l);
										cout << "Add {vj, vl} to the list of forbidden vertices" << endl;
										cout << "\n";
										insert(permanentList, j, l);
										cout << "Add {vj, vl} to the forbidden list Lf" << endl;
										cout << "\n";
									}
									else if (!(find(ListOfEdges[l].begin(), ListOfEdges[l].end(), i) != ListOfEdges[l].end()) &&
										find(ListOfEdges[l].begin(), ListOfEdges[l].end(), j) != ListOfEdges[l].end()) {
										NonCommonN[i][l] += 1;
										commonN[i][l] -= 1;
									}
								}
							}
						}

						k -= 1;
					}

				}
				*/
				/*----------Create adjacency list from raw data and convert it to adjacency matrix and print both------*/

void Kernelization::insert(int** ccounter, int u, int v)
{
	ccounter[u][v] += 1;

}
void Kernelization::insert(vector<int>* adj, int u, int v)
{
	adj[u].push_back(v);

}
void Kernelization::remove(int** ccounter, int u, int v)
{
	ccounter[u][v] -= 1;

}
void Kernelization::remove(vector<int>* adj, int u, int v)
{
	// remove edge from u to v
	vector<int>::iterator it = find(adj[u].begin(), adj[u].end(), v);
	adj[u].erase(it);
	// remove edge from v to u
	it = find(adj[v].begin(), adj[v].end(), u);
	adj[v].erase(it);

}

// Function to display adjacency list
void Kernelization::printList(vector<int> adj[], int V)
{
	for (int i = 0; i < V; i++) {
		cout << i;
		for (auto j : adj[i])
			cout << " --> " << j;
		cout << endl;
	}
	cout << endl;
}


// Function to convert adjacency list to adjacency matrix
vector<vector<int> > Kernelization::convert(vector<int> adj[], int V)
{
	// Initialize a matrix
	vector<vector<int> > matrix(V, vector<int>(V, 0));

	for (int i = 0; i < V; i++) {
		for (auto j : adj[i])
			matrix[i][j] = 1;
	}
	return matrix;
}

// Function to display adjacency matrix
void Kernelization::printMatrix(vector<vector<int> > adj, int V)
{
	for (int i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			cout << adj[i][j] << "   ";
		}
		cout << endl;
	}
	cout << endl;
}

void Kernelization::edgeList(vector<vector<int> > adj, int V, vector <int> ListOfEdges[]) {
	for (int i = 1; i < V; i++) {
		for (int j = 1; j < V; j++) {
			if (adj[i - 1][j - 1] == 1)
			{
				insert(ListOfEdges, i - 1, j - 1);
			}
		}
		cout << endl;
	}
	cout << endl;
}
/*-----------------------------------------------------------------------------------------------------------------------*/
void insert(vector<int> adj[], int u, int v)
{
	adj[u].push_back(v);
	adj[v].push_back(u);
}

/*print element in array of vector*/
void printElements(vector<int> adj[], int V) {
	for (int i = 1; i < V; i++) {
		insert(adj, i, 0);
		cout << "element at index " << i << ": " << endl;
		for (auto it = adj[i].begin(); it != adj[i].end(); it++) {
			cout << *it << ' ';
		}
		cout << endl;
	}
}

void printList(vector<int> adj[], int V)
{
	for (int i = 1; i < V; i++) {
		cout << i;
		for (auto j : adj[i])
			cout << " --> " << j;
		cout << endl;
	}
	cout << endl;
}

vector<vector<int> > convert(vector<int> adj[], int V)
{
	// Initialize a matrix
	vector<vector<int> > matrix(V, vector<int>(V, 0));

	for (int i = 1; i < V; i++) {
		for (auto j : adj[i])
			matrix[i - 1][j - 1] = 1;
	}
	return matrix;
}

// Function to display adjacency matrix
void printMatrix(vector<vector<int> > adj, int V)
{
	for (int i = 1; i < V; i++) {
		for (int j = 1; j < V; j++) {
			cout << adj[i - 1][j - 1] << "   ";
		}
		cout << endl;
	}
	cout << endl;
}

bool getFileContent(string fileName, vector<string>& vecOfStrs)
{
	// Open the File
	ifstream in(fileName.c_str());
	// Check if object is valid
	if (!in)
	{
		cerr << "Cannot open the File : " << fileName << std::endl;
		return false;
	}
	string str;
	// Read the next line from File untill it reaches the end.
	while (getline(in, str))
	{
		if ((str.find('c') == str.npos) || (str.find('p') == str.npos))
			// Line contains string of length > 0 then save it in vector
			if (str.size() > 0)
				vecOfStrs.push_back(str);
	}
	//Close The File
	in.close();

	return true;
}

void conversion(std::vector<std::string>& vecOfStrs) {

	std::vector<int> intNumbers;

	for (int i = 0; i < vecOfStrs.size(); i++)
	{
		int num = stoi(vecOfStrs.at(i));
		intNumbers.push_back(num);
	}
	for (int& line1 : intNumbers)
		std::cout << line1 << std::endl;

}

vector<edge> extractEdgesFromRawData(vector<std::string> vecOfStrs)
{
	vector<edge> edges;
	for (int i = 0; i < vecOfStrs.size(); i++)
	{
		std::string s = vecOfStrs[i];
		std::string delimiter = " ";

		size_t pos = 0;
		std::string firstNode;
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			firstNode = s.substr(0, pos);
			// std::cout << "firstNode: "<< firstNode << std::endl;
			s.erase(0, pos + delimiter.length());
		}
		// std::cout << "secondNode: " << s << std::endl;
		// cout<<"---------------\n";

		edge edge;
		edge.node1 = stoi(firstNode);
		edge.node2 = stoi(s);

		edges.push_back(edge);
	}

	return edges;
}
void fill(int** p, int rowSize, int columnSize)
{
	for (int row = 1; row < rowSize; row++)
	{
		for (int col = 1; col < columnSize; col++) {
			p[row - 1][col - 1] = 0;
			cout << p[row - 1][col - 1] << "   ";
		}
		cout << endl;
	}
}

int main()
{


	const int V = 8;
	const int k = 3;
	Kernelization ker(V, k);
	vector<int> adjList[V];
	int** counter;
	counter = new int* [V];
	for (int row = 0; row < V; row++) {
		counter[row] = new int[V];
	}
	int** nonCommonCounter;
	nonCommonCounter = new int* [V];
	for (int row = 0; row < V; row++) {
		nonCommonCounter[row] = new int[V];
	}
	/*---fill out and print common and non_common counter----*/
	cout << "fill matrix for Common counter: " << endl;
	fill(counter, V, V);
	cout << "fill matrix for Non_common counter: " << endl;
	fill(nonCommonCounter, V, V);




	cout << "Vector of string result: " << "\n";
	std::vector<std::string> vecOfStrs;
	// Get the contents of file in a vector
	bool result = getFileContent("C:\\Users\\sanaz.rabinia\\Documents\\PACE\\exact\\exact000.gr", vecOfStrs);
	if (result)
	{
		// Print the vector contents
		for (std::string& line : vecOfStrs)
			std::cout << line << std::endl;

	}
	cout << "Vector of integer result: " << "\n";
	vector<edge> edges = extractEdgesFromRawData(vecOfStrs);
	// Now, we have list of edges conversion(vecOfStrs);

	for (int i = 0; i < edges.size(); i++)
	{
		cout << "(" << edges[i].node1 << ", " << edges[i].node2 << ")" << endl;
	}
	for (int i = 0; i < edges.size(); i++)
	{
		insert(adjList, edges[i].node1, edges[i].node2);
	}

	// Display adjacency list
	cout << "Adjacency List: \n";
	printList(adjList, V);

	// Function call which returns
	// adjacency matrix after conversion
	vector<vector<int> > adjMatrix
		= convert(adjList, V);

	// Display adjacency matrix
	cout << "Adjacency Matrix: \n";
	printMatrix(adjMatrix, V);
	cout << "print common neighbors counter C[i, j]: \n";
	int** equiCounter
		= ker.addCommonNeighbor(adjMatrix, V, counter);
	cout << "\n";
	cout << "print Non-common neighbors counter N[i, j]: \n";
	int** equiNonCounter
		= ker.addNonCommonNeighbor(adjMatrix, V, nonCommonCounter);
	cout << "\n";

	vector <pint> commonNeighborList;
	cout << "\n";
	cout << "Update Common Neighbor List Lc,r: \n";
	vector<vector<pint>> L = ker.updateCommonList(equiCounter, equiNonCounter, commonNeighborList, V, k);

	vector <pint> NonCommonNeighborList;
	cout << "Update Non_Common Neighbor List Ln,s: \n";
	ker.updateNonCommonList(equiCounter, equiNonCounter, NonCommonNeighborList, V, k);

	vector <pint> permanentList;
	cout << "Update Permanent Neighbor List Lp: \n";
	ker.updatePermanentList(equiCounter, equiNonCounter, permanentList, V, k, adjList);

	vector <pint> forbiddenList;
	cout << "Update Forbidden Neighbor List Lf: \n";
	ker.updateForbiddenList(equiCounter, equiNonCounter, forbiddenList, V, k, adjList);

	cout << "Adjacency List after removing forbidden pairs: \n";
	printList(adjList, V);

	cout << "Adjacency List after adding permanent pairs: \n";
	printList(adjList, V);


	cout << "Array pointer for pointing Pc to list of veritces {vi, vj}:" << endl;
	ker.ArrayPointerToListCommon(L, V);

	//investigateNewVertices(int** commonN, int** NonCommonN, vector<pint> permanentList, vector<pint> forbiddenList, int V, int k, vector<int> adjList[], vector<pint> commonNeighbor, vector<pint> nonCommonNeighbor)
	cout << "Update counters and lists : \n";
	ker.investigateNewVertices(equiCounter, equiNonCounter, permanentList, forbiddenList, V, k, adjList, commonNeighborList, NonCommonNeighborList);

	return 0;
}



