#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <fstream>
#include <climits>

using namespace std;

class edge{   
    public:
    	int endpoint1, endpoint2, weight;
    	 
    	// compare edge's weight
    	bool operator< (const edge& another) const{
			if(weight < another.weight)
				return true;
			else
				return false; 
		}
		
		bool operator> (const edge& another) const{
			if(weight > another.weight)
				return true;
			else
				return false;
		}
		
//		bool operator= (const edge& another) const{
//			if(weight == another.weight)
//				return true;
//			else
//				return false;
//		}
};

class disjointset{
	int n;
	vector<int> parent,rank;

	public:
		// makeset
		disjointset(int a) : n(a){
			parent = vector<int> (n,0);
			rank = vector<int> (n,0);
			for(int i=0;i<n;i++)
				parent[i] = i;
		}
		
		int findset(int i){
			if(i != parent[i])
				parent[i] = findset(parent[i]); // path compression
				return parent[i];
		}
		
        void unionset(int i,int j){ 
			i = findset(i);
			j = findset(j);
			if (i == j)
				return;
	        else if(rank[i] < rank[j]) // union by rank
				parent[i] = j;
	        else{
				parent[j] = i;
				if(rank[i] == rank[j])
					rank[i]++;
			}
		}

};

// binary max heap
class maxheap{
	public :
		int n;
		vector<int> key;
		vector<int> data;
		vector<int> index; // record the index in maxheap
		maxheap(int n) : n(n){
			key = vector<int>(n, INT_MIN);
			for(int i = 0; i < n; i++){
				data.push_back(i);
				index.push_back(i);
			}
		}
		
		void MaxHeapify(int root){
			int leftchild = root*2+1;
			int rightchild = root*2+2;
			int max;
			if(leftchild < data.size() && key[data[leftchild]] > key[data[root]])
		        max = leftchild;
		    else
		        max = root;

		    if(rightchild < data.size() && key[data[rightchild]] > key[data[max]]){
		        max = rightchild;
		    }
		    swap(data[max], data[root]);
		    swap(index[data[max]], index[data[root]]);
		    if(max != root && max*2+1 < data.size()){
		        MaxHeapify(max);
		    }
		}
//		int maxheap::Max(){ // returns node id
//			if (data.size() > 0){
//				return data[0];
//			}
//			else{
//				cout << "Heap is empty!" << endl;
//				return -1;
//			}
//		}
		void IncreaseKey(int x, int k){
			if(k <= key[x])
				return;
			key[x] = k;
			int parent = (index[x]-1)/2;
			int temp;
			
			while(1){
				temp = data[parent];
				MaxHeapify(parent);
				if((key[temp] == key[data[parent]]) || parent == 0)
					break;
				else
					parent = (parent-1)/2;
			}
		}
		
		int ExtractMax(){
			if(data.size() <= 0)
				return INT_MIN;
			int max = data[0];
			swap(data[0], data[data.size()-1]);
			swap(index[data[0]], index[data[data.size()-1]]);
			index[data[data.size()-1]] = -1;
			data.pop_back();
			MaxHeapify(0);
			return max;
		}
		
};


// directed disjoint set
// members in the set point to the lastest node
//class d_disjointset{
//	int n;
//	vector<int> son;
//
//	public:
//		// makeset
//		d_disjointset(int a) : n(a){
//			son = vector<int> (n,0);
//			for(int i=0;i<n;i++)
//				son[i] = i;
//		}
//		
//		int dfindset(int i){
//			if(i != son[i])
//				son[i] = dfindset(son[i]); // path compression
//				return son[i];
//		}
//		
//		// j always be i's son
//        void dunionset(int i,int j){ 
//			i = dfindset(i);
//			j = dfindset(j);
//			if (i == j)
//				return;
//	        else
//				son[i] = j;
//		}
//
//};

class Graph{
    int N,M;
	public:
		vector<edge> inputedges;
	    int outputweight = 0;
		vector<edge> outputedges;
		list<int>* adj;
		vector<vector<int>> adjmatrix; // for directed graph
		vector<vector<int>> dirweight; // directed wegiht
		
	    Graph(int verticesnum, int edgesnum){
	        N = verticesnum;
	        M = edgesnum;
	        adjmatrix.resize(N);
			vector<int> temp(N);
			dirweight.resize(N,temp);
			for(int i=0;i<N;i++)
				for(int j=0;j<N;j++)
					dirweight[i][j] = INT_MIN;
	    }
	
	    void addEdge(vector<int> u, vector<int> v, vector<int> w, int m){
	        for(int i=0;i<m;i++){
	        	inputedges.push_back(edge{u[i],v[i],w[i]});
	        	dirweight[u[i]][v[i]] = w[i];
		    	dirweight[v[i]][u[i]] = w[i];
			}
			
			//adjcent matrix for directed graph
	        for(int i=0;i<N;i++){
		    	for(int j=0;j<N;j++){
		    		if(dirweight[i][j] >= -100){
		    			adjmatrix[i].push_back(j);
		    		}
		    	}
		    }
		    
		    //adjcent list for DFS
	        adj = new list<int>[N];
	    }
		
		// solve unweighted graph
		void kruskal(){
			disjointset vertices(N);
			sort(inputedges.begin(), inputedges.end());
			
			for (int i=M-1;i>=0;i--){
		        if(vertices.findset(inputedges[i].endpoint1) != vertices.findset(inputedges[i].endpoint2))
		            vertices.unionset(inputedges[i].endpoint1, inputedges[i].endpoint2);
		        else{
		        	outputedges.push_back(inputedges[i]);
		        	outputweight += inputedges[i].weight;
				}
	    	}
		}
		
		
		// DFS to detect cycle in directed graph
		bool detectcycle(int startvertex, vector<bool>& visited, vector<bool>& record){ // record will change, call by reference
			if(visited[startvertex] == false){
		    	visited[startvertex] = true;
		        record[startvertex] = true;
				
		        for (auto i = adj[startvertex].begin(); i != adj[startvertex].end();i++){
		            if(!visited[*i] && detectcycle(*i, visited, record))
		                return true;
		            else if(record[*i])
		                return true;
		        }
		    }
		    record[startvertex] = false;
		    return false;
		}
		
		bool existcycle(int setnum, vector<edge> set){ //setnum = how many kinds of endpoint1 in the set
		    vector<bool> visited(N,0);
		    vector<bool> record(N,0);
		    vector<int> vertexindex(setnum,-1);
		    for(int i=0;i<setnum;i++){
		    	vertexindex[i] = set[i].endpoint1;
		    	//cout << " vertexindex: " << vertexindex[i] << endl;
			}		
		    for(int i=0;i<setnum;i++)
		        if(!visited[vertexindex[i]] && detectcycle(vertexindex[i], visited, record)){
		        	//cout << "cycle start from : " << vertexindex[i] <<endl;
					return true;
				}
				
		    return false;
		}
		
		// solve directed graph using direted kruskal
		vector<edge> directedset;
		int directedsetnum = 0; // count the number of vertices in the directed set
		void dkruskal(){
			vector<int> vertexpresent(N,0); // record how many times a vertex present in directed set
			sort(inputedges.begin(), inputedges.end());	
			for(int i=M-1;i>=0;i--){
				directedset.push_back(inputedges[i]);
				if(vertexpresent[inputedges[i].endpoint1] == 0)
					directedsetnum++;
				//cout << "directedsetnum: " << directedsetnum << endl;
				vertexpresent[inputedges[i].endpoint1]++;
	        	adj[inputedges[i].endpoint1].push_back(inputedges[i].endpoint2);	
				
		        if(existcycle(directedsetnum, directedset)){
		        	outputedges.push_back(inputedges[i]);
		        	outputweight += inputedges[i].weight;
		        	directedset.pop_back();
					vertexpresent[inputedges[i].endpoint1]--;
					adj[inputedges[i].endpoint1].pop_back();
					if(vertexpresent[inputedges[i].endpoint1] == 0)
						directedsetnum--;
					//cout << "directedsetnum: " << directedsetnum << endl;
				}
	    	}
		}
		
		// solve direted graph
		void directedsolver(){
			vector<int> parent(N, -1);
		    maxheap heap(N);
			//cout << "check point 1" << endl;
			for(int i=0;i<N;i++){
		    	int currentmax = heap.ExtractMax(); // current node with max edge weight
		    	for(int i=0;i<adjmatrix[currentmax].size();i++){
		    		int adjnode = adjmatrix[currentmax][i];
		    		if(heap.index[adjnode] >= 0 && dirweight[currentmax][adjnode] > heap.key[adjnode]){ // adjnode in the unfinished set and new weight > current
				    	parent[adjnode] = currentmax;
				    	heap.IncreaseKey(adjnode, dirweight[currentmax][adjnode]);
		    		}
		    	}
		    }
		    //cout << "check point 2" << endl;
		    vector<edge> dir_set;
		    vector<edge> remain_set;
		    int dir_setnum = 0; // count the number of vertices(endpoint1) in the directed set
		    vector<int> vertexpresent(N,0); // record how many times a vertex present in directed set
			vector<vector<bool>> choose(N, vector<bool>(N, false));
		    for(int i=0;i<M;i++){
		    	if((parent[inputedges[i].endpoint1] == inputedges[i].endpoint2 || 
				parent[inputedges[i].endpoint2] == inputedges[i].endpoint1) && 
				dirweight[inputedges[i].endpoint1][inputedges[i].endpoint2] == inputedges[i].weight && 
				(choose[inputedges[i].endpoint2][inputedges[i].endpoint1]) == false ){
		    		choose[inputedges[i].endpoint1][inputedges[i].endpoint2] = true;
		    		dir_set.push_back(inputedges[i]);
		    		if(vertexpresent[inputedges[i].endpoint1] == 0)
						dir_setnum++;
					vertexpresent[inputedges[i].endpoint1]++;
	        		adj[inputedges[i].endpoint1].push_back(inputedges[i].endpoint2);
		    	}
		    	else if(inputedges[i].weight <= 0){
		    		outputedges.push_back(inputedges[i]);
		    		outputweight += inputedges[i].weight;
		    	}
		    	else if(inputedges[i].weight > 0){
		    		remain_set.push_back(inputedges[i]);
		    	}
	    	}
	    	//cout << "check point 3" << endl;
	    	sort(remain_set.begin(), remain_set.end());
	    	for(int i=remain_set.size()-1;i>=0;i--){
				dir_set.push_back(remain_set[i]);
				if(vertexpresent[remain_set[i].endpoint1] == 0)
					dir_setnum++;
				vertexpresent[remain_set[i].endpoint1]++;
	        	adj[remain_set[i].endpoint1].push_back(remain_set[i].endpoint2);	
				
		        if(existcycle(dir_setnum, dir_set)){
		        	outputedges.push_back(remain_set[i]);
		        	outputweight += remain_set[i].weight;
		        	dir_set.pop_back();
					vertexpresent[remain_set[i].endpoint1]--;
					adj[remain_set[i].endpoint1].pop_back();
					if(vertexpresent[remain_set[i].endpoint1] == 0)
						dir_setnum--;
					//cout << "directedsetnum: " << directedsetnum << endl;
				}
	    	}
		}
};



int main(int argc, char* argv[]){
	
	fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);
    char type;
    int n, m;
    fin >> type;
	fin >> n >> m;
    Graph graph(n,m);
    
	vector<int> u(m,-1), v(m,-1), w(m,-1);
    for(int i=0;i<m;i++){
        fin >> u[i] >> v[i] >> w[i];   
    }
    
	graph.addEdge(u,v,w,m);
	
    if(type == 'u'){
    	graph.kruskal();
    	
    	fout << graph.outputweight << endl;
    	for(int i=graph.outputedges.size()-1;i>=0;i--){
    		fout << graph.outputedges[i].endpoint1 << " " << graph.outputedges[i].endpoint2 << " " << graph.outputedges[i].weight << endl;
		}
	}
	
	else if(type == 'd'){
		if(m < n)
			fout << 0 << endl;
		else{
			//graph.dkruskal();
	    	graph.directedsolver();
	    	fout << graph.outputweight << endl;
	    	for(int i=graph.outputedges.size()-1;i>=0;i--){
	    		fout << graph.outputedges[i].endpoint1 << " " << graph.outputedges[i].endpoint2 << " " << graph.outputedges[i].weight << endl;
			}
		}
	}
	
	fin.close();
	fout.close();
    return 0;
}


