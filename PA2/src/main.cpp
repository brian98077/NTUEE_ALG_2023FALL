//space saved version (use only half table, for all j in table[i][j], index of j = j-i)
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;


int countmax(int *chords, int len, int **record){
	
//	int m[len][len];
	int **m;
	m = new int*[len];
	int jjj = len;
	for(int i=0;i<len;i++){
		m[i] = new int[jjj]();
		jjj--;
	}
		
	

	int distance = 1, i = 0, j = 0;
	
	while(distance < len){
		
		i = 0, j = i + distance;
		
		while(j < len){

			int k = chords[j]; // find the other endpoint
			
			if(k == i){
				
				if(distance <= 2){
					m[i][j-i] = 1;
				}
				else{
					m[i][j-i] = m[i+1][j-1-(i+1)] + 1;
				}
				
				record[i][j-i] = -1;
			}
			
			else if(k>i && k<j){
				
				m[i][j-i] = max(m[i][j-1-i] , m[i][k-1-i] + m[k][j-k]);
				
				if(m[i][k-1-i] + m[k][j-k] > m[i][j-1-i])
					record[i][j-i] = -2;
			}
			
			else{
				m[i][j-i] = m[i][j-1-i];
			}
			i++; 
			j++;
		}
		distance++;
	}
	return m[0][len-1];
}

vector<int> findchords(int *chords, int i, int j, int **record){
	
	vector<int> bestchords = {}; 
	
	while(j-i > 1){
		
		// k = i
		if(record[i][j-i] == -1){ 
		//	cout << "tt: " << j << endl;
			bestchords.push_back(j);
//				for(int i=0;i<bestchords.size();i++)
//					cout << "now: "<<bestchords[i] << endl;
			vector <int> inside = findchords(chords,i+1,j-1,record);
			bestchords.insert(bestchords.end(), inside.begin(), inside.end());
			vector<int>().swap(inside); // clean memory
			j = i-1;
		}
		
		// k between i j 
		else if(record[i][j-i] == -2){
		//	cout << "gg: " << j << endl;
//			vector<int> left = findchords(chords,i,chords[j]-1,record);
//			bestchords.insert(bestchords.end(), left.begin(), left.end());
//			vector<int>().swap(left); // clean memory
			
			bestchords.push_back(j);
			
			vector<int> right = findchords(chords,chords[j]+1,j-1,record);
			bestchords.insert(bestchords.end(), right.begin(), right.end());
			vector<int>().swap(right); // clean memory
//							for(int i=0;i<bestchords.size();i++)
//					cout << "now: "<<bestchords[i] << endl;
			j = chords[j] - 1;
		}
		
		else			
			j--;
				//cout << "ehe: " << j << endl;	
	}
	return bestchords;
}


int main(int argc, char* argv[]){
	
	fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);
	int len; // len = 2n-1
	fin >> len ;
	//cout << "test:  "<<len;
	int *chords = new int [len];
	for(int i=0;i<len/2;i++){
		int end1, end2;
		fin >> end1 >> end2;
		chords[end1] = end2;
		chords[end2] = end1;
	}
	//cout << "check point 1" << endl;

	int **record;
	record = new int*[len];
	int jj = len;
	for(int i=0;i<len;i++){
		record[i] = new int[jj]();
		jj--;
	}
	//cout << "check point 2" << endl;
	int max = countmax(chords,len,record);
	//cout << "check point 2.5" << endl;
	vector<int> outputchords = findchords(chords,0,len-1,record);
//	cout << " A : "<<record[0][11] << endl;
	fout << max << endl;
	for(int i=outputchords.size()-1;i>=0;i--)
		outputchords[i] = min(outputchords[i],chords[outputchords[i]]);
	sort(outputchords.begin(), outputchords.end());
	for(int i=0;i<outputchords.size();i++)
		fout << outputchords[i] << " " << chords[outputchords[i]] << endl;
	
	fin.close();
	fout.close();
	return 0;
}