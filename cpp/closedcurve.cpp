#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <queue>
#include <boost/algorithm/string.hpp>
#include <math.h>
#include <chrono>
#include <stdlib.h>

using namespace std;

typedef std::chrono::high_resolution_clock Clock;
typedef std::vector<vector<int>> AdjList;


double distance(const tuple<int, int> &p1, const tuple<int, int> &p2) {
  return sqrt( (std::get<0>(p1) - std::get<0>(p2)) * (std::get<0>(p1) - std::get<0>(p2)) + (std::get<1>(p1) - std::get<1>(p2)) * (std::get<1>(p1) - std::get<1>(p2)) );
}

std::vector<std::vector<double>> createAdjMatrix (const std::vector<std::tuple<int, int>> &nodes){
  std::vector<std::vector<double>> adjMatrix(nodes.size(), vector<double>(nodes.size()));
  for (int i = 0; i < nodes.size(); i++) {
    for (int j = 0; j < nodes.size(); j++){
      adjMatrix[i][j] = distance(nodes[i], nodes[j]); 
    }
  }
  return adjMatrix;
}

double pathWeightVsReverse(std::vector<int> &path, int i, int j, const std::vector<std::vector<double>> &adjMatrix){ 
  if (j-1 < 0)
    return ( adjMatrix[path[i]][path[(i+1)%path.size()]] + adjMatrix[path[path.size()-1]][path[j]] ) - ( adjMatrix[path[i]][path[path.size()-1]] + adjMatrix[path[(i+1)%path.size()]][path[j]] ); 
  else
    return ( adjMatrix[path[i]][path[(i+1)%path.size()]] + adjMatrix[path[j-1]][path[j]] ) - ( adjMatrix[path[i]][path[j-1]] + adjMatrix[path[(i+1)%path.size()]][path[j]] ); 
}

void printTour(std::vector<int> &tour, const std::vector<tuple<int,int>> &nodes, std::string name){ 
  //cout << name << "  TOUR: "; 
  for ( const auto &i : tour) {
     cout << "(" << get<0>(nodes[i]) << ", " << get<1>(nodes[i]) << ")" << endl;
  }
}

void printTour(std::vector<int> &tour){ 
  cout << "TOUR: "; 
  for ( const auto &i : tour) {
    cout << i << " ";
  }
  cout << endl;
}

void reverseSubtour (std::vector<int> &tour, int &i, int &j) {
  if (j <= i) {
    /**/
    vector <int> subtour = vector<int>(tour.begin() + i + 1, tour.end()); 
    subtour.insert(subtour.end(),  tour.begin(), tour.begin() + j);  

    int index = i+1;
    for (std::vector<int>::reverse_iterator it = subtour.rbegin(); it != subtour.rend(); it++) {
      tour[index%tour.size()] = *it;
      index += 1;
    }
    
    //cout << "REVERSING SUBOTUR: " << i << ", " << j << endl;
    /* 
    int tourSize = tour.size();
    int a = i + 1;
    int b = (j-1) < 0 ? tourSize - 1 : j -1;
    int tmp;
    for (int c = 0; c < (tourSize - i + j - 2)/2; c++) {
        //printTour(tour);
        //cout << tourSize - i + j - 1 - c << "\tswapping a: " << a << ", b: " << b << endl; 
        //cout << "\tswapping tour[a]: " << tour[a] << ", tour[b]: " << tour[b] << endl; 
        tmp = tour[a];
        tour[a] = tour[b];
        tour[b] = tmp;
        a = (a + 1) % tourSize;
        b = (b - 1) < 0 ? tourSize - 1 : b - 1;
        //cout <<"\t\t" << a << ", " << b << endl; 
    }
   */ 
  } else {
    std::reverse(tour.begin() + i + 1, tour.begin() + j);
  }
}

double tourWeight(std::vector<int> tour, std::vector<std::vector<double>> adjMatrix) {
  double tourWeight = 0;
  for (int i = 0; i < tour.size(); i++)
    tourWeight += adjMatrix[tour[i]][tour[(i+1)%tour.size()]];
  return tourWeight;
}

std::vector<int> reverseTours (std::vector<int> &tour, const std::vector<std::tuple<int, int>> &nodes, const std::vector<std::vector<double>> &adjMatrix){
  int counter = 0;
  int tourSize = tour.size();
  int tourSizeSq = tourSize*tourSize;
  int subtourSize;
  
  while (counter < tourSizeSq) {
    for (int i = 0; i < tourSize; i++) {
      for (int j = 0; j < tourSize; j++){
        
        subtourSize = ((j <= i) ? ((tourSize - i) + j) : (j - i));

        if (subtourSize > 3) {
           if (pathWeightVsReverse(tour, i, j, adjMatrix) > 0) {
             reverseSubtour(tour, i, j); 
             counter = 0;
           }
        }
        counter += 1; 
        if (counter > tourSizeSq) { goto label2; }
      }
    } 
  }
label2:
  
  return tour;
}

std::vector<std::tuple<int,int>> readPipedInput() {
  std::string line;
  std::vector<std::string> strs;
  std::vector<std::tuple<int, int>> nodes;
  int i = 0; int j = 0;
  std::string black_dot = "0";
  while (std::getline(std::cin, line))  {
    boost::split(strs, line, boost::is_any_of(" \n"));
    for (std::vector<std::string>::const_iterator it = strs.begin(); it != strs.end(); ++it) {
        if (*it == black_dot){nodes.push_back(std::make_tuple(i, j));} 
        j += 1;
     }
    i += 1;
    j = 0;
  }
  return nodes;
}
std::vector<std::tuple<int, int>> readInput(string const &str1) {
  string line;
  std::vector<std::string> strs;
  std::vector<std::tuple<int, int>> nodes;
  ifstream myfile (str1);
  int i = 0; int j = 0;
  std::basic_string<char> black_dot = "0";
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      boost::split(strs, line, boost::is_any_of(" \n"));
      for (std::vector<std::string>::const_iterator it = strs.begin(); it != strs.end(); ++it) {
          if (*it == black_dot){nodes.push_back(std::make_tuple(i, j));} 
          j += 1;
       }
      i += 1;
      j = 0;
    }
    myfile.close();
  }
  else cout << "Unable to open file"; 
  return nodes;
}

struct compareNodeWeight {
  bool operator()(const std::tuple<double, int, int> &lhs, std::tuple<double,int,int> &rhs) {
    return std::get<0>(lhs) >  std::get<0>(rhs); 
  }
};

int searchHeap(std::vector<std::tuple<double,int,int>> &heap, int vertex) {
  for (int i = 0; i < heap.size(); i++) {
    if (std::get<1>(heap[i]) == vertex) {
      return i; 
    }
  }
  return -1;
}

void siftUp(std::vector<std::tuple<double,int,int>> &heap, int index) {
  int parent = (index - 1) / 2;
  double indexValue = std::get<0>(heap[parent]);
  double parentValue = std::get<0>(heap[index]);
  std::tuple<double,int,int> tmp; 

  while (index != 0 && std::get<0>(heap[(index-1)/2]) > std::get<0>(heap[index]) ) {
    parent = (index - 1) / 2;
    indexValue = std::get<0>(heap[index]);
    parentValue = std::get<0>(heap[parent]);
    tmp = heap[index];
    heap[index] = heap[parent];
    heap[parent] = tmp;
    index = parent;
  }
}

std::vector<std::tuple<int,int>> prims (std::vector<std::vector<double>> adjMatrix) {
  std::vector<int> knownNodes(adjMatrix.size(), 0); // knownNodes[i] = 0 iff i is not known, else 1
  
  std::vector<std::tuple<double,int,int>> heap;
  for (int i = 1; i < adjMatrix.size(); i++ ){
    heap.push_back(std::make_tuple(INT_MAX, i, -1)); 
  } 
  
  heap.push_back(std::make_tuple(0, 0, -1)); 
  std::make_heap(heap.begin(), heap.end(), compareNodeWeight());

  std::tuple<double, int, int> minVal;
  double weight; int u; int u_prev;
  std::vector<std::tuple<int,int>> edges;  
  int index;

  while (heap.size() > 0) {
    std::pop_heap(heap.begin(), heap.end(), compareNodeWeight());
    minVal = heap.back(); 
    heap.pop_back();
    weight = std::get<0>(minVal);
    u = std::get<1>(minVal);
    u_prev = std::get<2>(minVal);
    
    if (knownNodes[u] == 0) {
      knownNodes[u] = 1;
      if (u_prev != -1) {
        edges.push_back(std::make_tuple(u, u_prev));
      }
    }
    for (int v = 0; v < adjMatrix.size(); v++ ) { 
      if (knownNodes[v] == 0) {
        index = searchHeap(heap, v);
        if (index != -1) {
          if (weight + adjMatrix[u][v] < std::get<0>(heap[index])) {
            heap[index] = std::make_tuple(weight + adjMatrix[u][v], v, u);
            siftUp(heap, index);
          }
        }
      }
    }   
  } 
  return edges;
}

std::vector<vector<int>> constructAdjList(std::vector<std::tuple<int,int>> &MST, const std::vector<std::tuple<int,int>> &nodes) {
  std::vector<std::vector<int>> adjList(nodes.size(), std::vector<int>());

  for (int i = 0; i < MST.size(); i++) {
    std::tuple<int,int> edge = MST[i];
    adjList[std::get<0>(edge)].push_back(std::get<1>(edge));
    adjList[std::get<1>(edge)].push_back(std::get<0>(edge));
  }
  return adjList;
}

std::vector<int> DFSUtil(int v, bool visited[], const std::vector<std::vector<int>> &adjList, std::vector<int> &tour) {
    visited[v] = true;
    tour.push_back(v);
 
    for (std::vector<int>::const_iterator i = adjList[v].begin(); i != adjList[v].end(); ++i)
        if (!visited[*i])
            DFSUtil(*i, visited, adjList, tour);
    return tour;
}
 
std::vector<int> DFS(int v, const std::vector<std::vector<int>> adjList) {
    bool *visited = new bool[adjList.size()];
    for (int i = 0; i < adjList.size(); i++)
        visited[i] = false;
    std::vector<int> dfsTour; 
    std::vector<int> tour = DFSUtil(v, visited, adjList, dfsTour);
    return tour;
}

std::vector<int> nearestNeighbor(std::vector<int> &tour, const std::vector<std::vector<double>> &adjMatrix) {
  int visitedVertices[adjMatrix.size()] = {0};
  int visitedCount = 0;

  int currentVertex = 0;
  int nextVertex;
  double minEdgeWeight;
   

  std::vector<int> nnTour;
 
  while(visitedCount < adjMatrix.size()) {
    nnTour.push_back(currentVertex);
    minEdgeWeight = INT_MAX;
    for (int v = 0; v < adjMatrix.size(); v++){
      if (!visitedVertices[v] && currentVertex != v && adjMatrix[currentVertex][v] < minEdgeWeight) {
        minEdgeWeight = adjMatrix[currentVertex][v];
        nextVertex = v;
      }
    }
    visitedVertices[currentVertex] = 1;
    currentVertex = nextVertex; 
    visitedCount+=1;
  }
  return nnTour;

}


int main (int argc, char *argv[]) {
  /*
  std::string input_file = argv[1];
  cout << "INPUT FILE: " << input_file << endl;
  const std::basic_string<char> T = input_file;
  */
  const std::vector<std::tuple<int, int>> nodes = readPipedInput(); 

  std::vector<int> tour;
  for (int i=0; i<nodes.size(); ++i) tour.push_back(i);
  //printTour(tour, nodes, "ORIGINAL");
  
  //cout << "Number of cities: " << tour.size() << endl;
  auto t1 = Clock::now();
  const std::vector<std::vector<double>> adjMatrix = createAdjMatrix(nodes);
  auto t2 = Clock::now();
  /*std::cout << "Create AdjMatrix Delta t2-t1: " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
            << " milliseconds" << std::endl;
  */
//  tour = nearestNeighbor(tour, adjMatrix);
//  printTour(tour, nodes, "NEAREST NEIGHBOR");
  
  t1 = Clock::now();
  std::vector<tuple<int,int>> MST = prims(adjMatrix);
  t2 = Clock::now();
  /*std::cout << "PRIMS Delta t2-t1: " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
            << " milliseconds" << std::endl;
  */
  
  t1 = Clock::now();
  const std::vector<vector<int>> adjList = constructAdjList(MST, nodes);
  t2 = Clock::now();
  /*std::cout << "Construct AdjList  Delta t2-t1: " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
            << " milliseconds" << std::endl;
  */
  t1 = Clock::now();
  tour = DFS(0, adjList); 
  t2 = Clock::now();
  //printTour(tour, nodes, "DFS");
  /*std::cout << "DFS  Delta t2-t1: " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
            << " seconds" << std::endl;
  */

  t1 = Clock::now();
  std::vector<int> reversedTour = reverseTours(tour, nodes, adjMatrix);
  t2 = Clock::now();
  printTour(tour, nodes, "REVERSED");
  /*cout << "Number of cities: " << tour.size() << endl;
  std::cout << "Reverse Tour Delta t2-t1: " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
            << " milliseconds" << std::endl;
  */
  return 0;
}
