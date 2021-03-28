#include <iostream>
#include <cstdlib> // To type cast the double value to long long
#include "wdigraph.h"
#include "digraph.h"
#include "dijkstra.h"
#include <fstream> // To open the input file
#include <list> // To store the path between Start and End Vertexs

using namespace std;

struct Point 
{
/*
	This struct stores the latitude and longitude of each Vertex in the graph
*/

    long long lat; // Latitude of the point
    long long lon; // Longitude of the point
};

long long manhattan(const Point& pt1,const Point& pt2) 
{
/*	
	This function takes in the coordinates of two vertices in the graph, computes 
	and returns the Manhattan distance between them.
*/


    // Computes and returns the Manhattan distance between the two given points
    long long manDist= abs(pt1.lat-pt2.lat)+abs(pt1.lon-pt2.lon);
    return manDist;
}


void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) 
{
/*
    This function reads the Edmonton map data from the provided
    file and loads it into the given WDigraph object.
    It uses the Point struct to Store vertex coordinates and maps
    the ID of each vertex to its corresponding Point struct.

    PARAMETERS:
    filename: name of the file describing a road network
    graph: an instance of the weighted directed graph (WDigraph)class
    points: a mapping between vertex identifiers and their coordinates 
    		stored in Point struct  
*/

	// instantiating an object of input file stream class to read from the provided 
	// file
	ifstream fin;

	//opening the file
	fin.open(filename);

	
	while(!fin.eof())
  	{// a loop to read from the given file until EOF is reached

	    // declaring variables
	    Point pt;
	    string input_line,value; char V = 'V', E = 'E', comma = ',';
	    //reading a line from the file using ifstream object fin
	    getline(fin, input_line);
	    
	    int pos = input_line.find(",");
	    
	    if(input_line[0] == V)
	    {// if a vertex is given

	      string temp;
	      input_line = input_line.substr(pos+1);
	      //reading and storing the ID of the vertex
	      pos=input_line.find(",");
	      temp=input_line.substr(0,pos);
	      int v=stoi(temp);

	      // add the vertex to the graph
	      graph.addVertex(v);

	      // reading the coordinates of the vertex 
	      input_line=input_line.substr(pos+1);
	      pos=input_line.find(",");
	      temp = input_line.substr(0,pos);
	      double coord= stod(temp);
	      // storing the latitude of the vertex in 100,000-ths of degrees 
	      pt.lat=static_cast<long long>(coord*100000);

	      // reading and storing the longitude of the vertex in 100,000-ths 
	      // of degrees
	      temp =input_line.substr(pos+1);
	      coord= stod(temp);
	      pt.lon=static_cast<long long>(coord*100000);

	      // maping the vertex to its coordinates
	      points[v]=pt;
	    }
    	// Since the start of the input line would either be 'V' or 'E'
    	else if (input_line[0] == E){
    		// If an edge is given 

      		int bounds = input_line.size()+1;
      		int index = 2, substr_i = 0; char temp = input_line[index];
      		string temp_string = "", sub_str[2];
      		for (int i = index; i < bounds; i++) {
        		temp = input_line[i];
        		if (temp == comma){
          			sub_str[substr_i] = temp_string;
          			temp_string = "";
          			substr_i++;
          			continue;
        		}
        		else{
          			temp_string += temp;
        		}
      		}

      		int ver1 = stoi(sub_str[0]);
      		int ver2 = stoi(sub_str[1]);

      		// calculating and adding the weight for this edge, i.e
      		// the manhattan distance between the points
      		long long weight= manhattan(points[ver1],points[ver2]);
      		graph.addEdge(ver1, ver2, weight);
    	}
  	}
  	// closig the file
  	fin.close();
  	return;
}

int main()
{/* 
	This is the main function of the program. It calls readGraph() to read from the
	file "edmonton-roads-2.0.1.txt" and then responds to the client requests by 
	displaying the waypoints along the shortest path between the coordinates provided 
	by the client.
*/

	// instantiating an object of WDigraph class
	WDigraph wgraph;
	string filename="edmonton-roads-2.0.1.txt";
	
	// Point locations;
	unordered_map<int, Point> points;
	// calling the readGraph() to read data from file and build a graph
	readGraph(filename, wgraph, points);

	// declaring two points to store the coordinates of the starting and ending vertices
	Point start_point, end_point;
	string temp;
	cin >> temp;
	if (temp == "R"){
		//reading the coordinates of the vertices
		cin >> temp;
		start_point.lat = stol(temp);
		cin >> temp;
		start_point.lon = stol(temp);
		cin >> temp;
		end_point.lat = stol(temp);
		cin >> temp;
		end_point.lon = stol(temp);
	}

	int startVertex = 0, endVertex = 0;
	int distance = 1000000000;

	for (auto it: points){
		//computing the vertex nearest to start vertex
		long long temp = manhattan(it.second, start_point);
		if (temp <= distance){
			distance = temp;
			startVertex = it.first;
		}
	}

	distance = 1000000000;
	for (auto it: points){
		//computing the vertex nearest to end vertex
		long long temp = manhattan(it.second, end_point);
		if (temp <= distance){
			distance = temp;
			endVertex = it.first;
		}
	}
	
	unordered_map<int, PIL> searchTree;
	//computing the shortest path between start and end vertex
	dijkstra(wgraph, startVertex, searchTree);

	//declaring a list to store the waypoints along the shortest path
	list<int> path;

	if (searchTree.find(endVertex) == searchTree.end()){
		//if there is no path between the start and end vertex
		cout << "N " << 0 << endl;
	}
	else {
		//stepping through the searchTree to build the shortest path
		int stepping = endVertex;
		while (stepping != startVertex) {
			path.push_front(stepping);
			stepping = searchTree[stepping].first;
		}
		path.push_front(startVertex);
	}
	cout << "N " << path.size() << endl;
	for (auto it: path){
		//printing the waypoints along the shortest path
		cin >> temp;
		if (temp[0] == 'A'){
			cout << "W " << points[it].lat << " " << points[it].lon << endl;
		}
	}
	//printing 'E' to indicate end of a query
	cout << "E" << endl;
	return 0;
}