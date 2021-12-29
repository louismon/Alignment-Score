#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>

using namespace std;

int n,m;
vector<double> seq1, seq2;
map<vector<double>, double> sequenceMatrix;


void possibleSequences(int x, int y, vector<double> combo) {
	
	
	if (x != n) {
		//cout << x << " " << y << " " << seq1.at(x) << "x\n";
		vector<double> temp = combo;
		//for (auto& var : temp) {
		//	cout << var << " ";
		//}
		//cout << "\n";	
		temp.push_back(seq1[x]);
		sequenceMatrix.emplace(temp, -2);
		possibleSequences(x + 1, y, temp);
	}
	if (y != m) {
		vector<double> temp = combo;
		//cout << x << " " << y << " " << seq2.at(y) << "y\n";
		//for (auto& var : combo) {
		//	cout << var << " ";
		//}
		//cout << "\n";
		temp.push_back(seq2[y]);
		//if (temp.size() == n + m) {
		sequenceMatrix.emplace(temp, -2);
		//}
		possibleSequences(x, y + 1, temp);
	}

}

double MemoRec(int p, vector<double> target, vector<double> sequence)
{

	if (sequenceMatrix[sequence] != -2) {
		//cout << "exit at " << p << endl;
		return sequenceMatrix[sequence];
	}

	if (p == 1)
	{
		//cout << "before at " << p << ": " << sequence[0] << "*" << target[0] << endl;
		sequenceMatrix[sequence] = sequence[0] * target[0];
		//cout << "Sequence[0]: " << sequence[0] << " Target[0]: " << target[0] << endl;
		//cout << "exit at " << p << endl;
		return sequenceMatrix[sequence];
	}

	//cout << "Sequence[0]: " << sequence[0] << " Target[0]: " << target[0] << endl;
	//cout << "Sequence[" << p-1<< "]: " << sequence[p-1] << " Target[" << p-1 << "]: " << target[p-1] << endl;
	//cout << "before at " << p << ": " << sequence[p-1] << "*" << target[p-1] << endl;
	double temp = sequence[p - 1] * target[p - 1];
	vector<double> tempSeq = sequence;
	target.pop_back();
	tempSeq.pop_back();
	sequenceMatrix[sequence] = temp + MemoRec(--p, target, tempSeq);
	return sequenceMatrix[sequence];
}

map<vector<double>, double> memoAlignment(vector<double> seq1, vector<double> seq2, vector<double> target, int n, int m) {
	map<vector<double>, double> maxPair;
	vector<double> combo;
	double max = -2;
	double temp;

	possibleSequences(0, 0, combo);
	/*for (const auto& kv : sequenceMatrix) {
		for (auto& var : kv.first) {
			cout << var << " ";
		}
		cout << "\n";
	}
	cout << "\n";*/
	
	for(map<vector<double>, double>::iterator it = sequenceMatrix.begin(); it != sequenceMatrix.end(); it++)
		{
			temp = MemoRec(it->first.size(),target, it->first);
		}

	/*for (const auto& kv : sequenceMatrix) {
		for (auto& var : kv.first) {
			cout << var << " ";
		}
		cout << "Alignment: " << kv.second;
		cout << "\n";
	}
	cout << "\n";*/
	
	for (map<vector<double>, double>::iterator it = sequenceMatrix.begin(); it != sequenceMatrix.end(); it++) {
			double num = it->second;
			//cout << num << " " << max.begin()->second << "\n";
			if (num > max) {
				max = num;
				maxPair.clear();
				maxPair.emplace(it->first, num);
		}
	}
	return maxPair;
}

int main()
{
	vector<double> target;
	fstream newfile;
	newfile.open("input.txt", ios::in);
	string tp;
	int lines = 0;
	if (newfile.is_open()) {
		while (getline(newfile, tp)) {
			if (lines == 0) {
				n = stoi(tp.substr(0, tp.find(" ")));
				m = stoi(tp.substr(tp.find(" ") + 1));
			}
			else {
				int pos;
				vector<double> values;
				while ((pos = tp.find(" ")) != string::npos) {
					values.insert(values.end(), stod(tp.substr(0, pos)));
					tp = tp.substr(pos + 1);
				}
				values.insert(values.end(), stod(tp));
				switch (lines)
				{
				case 1:
					seq1 = values;
					break;
				case 2:
					seq2 = values;
					break;
				case 3:
					target = values;
					break;
				}
			}
			lines++;
		}
		newfile.close();
	}

	map<vector<double>, double> maxAlignment = memoAlignment(seq1, seq2, target, n, m);

	newfile.open("output.txt", ios::out);
	if (newfile.is_open())
	{
		newfile << maxAlignment.begin()->second << endl;
		for (auto& var : maxAlignment.begin()->first) {
			newfile << var << " ";
		}
		newfile.close();
	}

	cout << "DONE";

	return 0;
}