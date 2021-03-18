#include <iostream>
#include <cstdlib>
#include "heap.h"
#include "wdigraph.h"
//#include "digraph.h"
#include "dijkstra.h"
#include <fstream>

using namespace std;

// R: Alias for pair<int, long long>
typedef pair<int, long long> PIL;

// R: added this template file to make things easier

struct Point 
{
    long long lat; // Latitude of the point
    long long lon; // Longitude of the point
};

void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) 
{
    /*
    Read the Edmonton map data from the provided
    fileand load it into the given WDigraph object.
    Store vertex coordinates in Point struct and map
    each vertex to its corresponding Point struct.

    PARAMETERS:
    filename: name of the file describing a road network
    graph: an instance of the weighted directed graph (WDigraph)class
    points: a mapping between vertex identifiers and their coordinates
*/
	ifstream fin;
	fin.open(filename);

	// R: I changed the name of variable str to input_line to
	//increase readability
	while(!fin.eof())
  	{// a loop to read from the stdin until EOF is reached

	    // declaring variables
	    Point pt;
	    string input_line,value; char V = 'V', E = 'E', comma = ',';
	    //reading a line from stdin using istream object cin
	    getline(cin, input_line);
	    //storing the choice, 'V' or 'E'
	    int pos = input_line.find(",");
	    // value = input_line.substr(0,pos);
	    // input_line = input_line.substr(pos+1);

	    if(input_line[0] == V)
	    {// if a vertex is given
	      string temp;
	      input_line = input_line.substr(pos+1);
	      //read and store the ID of the vertex
	      pos=input_line.find(",");
	      temp=input_line.substr(0,pos);
	      int v=stoi(temp);
	      // add the vertex to the graph
	      graph.addVertex(v);

	      input_line=input_line.substr(pos+1);
	      pos=input_line.find(",");
	      temp = input_line.substr(0,pos);
	      double coord= stod(temp);
	      pt.lat=static_cast<long long>(coord*100000);

	      temp =input_line.substr(pos+1);
	      coord= stod(temp);
	      pt.lon=static_cast<long long>(coord*100000);
	      points[v]=pt;
	    }
    	// R: Since the start of the input line would either be 'V' or 'E'
    	else if (input_line[0] == E){
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
      		graph.addEdge(ver1, ver2);
    	}
  	}
  	return;
}

long long manhattan(const Point& pt1,const Point& pt2) 
{
    // Computes and returns the Manhattan distance between the two given points
    long long manDist= abs(pt1.lat-pt2.lat)+abs(pt1.lon-pt2.lon);
    return manDist;
}

void dijkstra(const WDigraph& graph,int startVertex, unordered_map<int, PIL>& tree) 
{
    /*Compute least cost paths that start from a given vertex.
    Use a binary heap to efficiently retrieve an unexplored vertex that
    has the minimum distance from the start vertex at every iteration.

    PIL is an alias for "pair<int, long long>" type as discussed in class
    PARAMETERS:
    WDigraph: an instance of the weighted directed graph (WDigraph) class
    startVertex: a vertex in this graph which serves as the root of the search tree
    tree: a search tree used to construct the least cost path to some vertex

    */
}
int main()
{
	WDigraph wgraph;
	string filename="edmonton-roads-2.0.1.txt";
	//Point locations;
	unordered_map<int, Point> points;
	readGraph(filename, wgraph, points);
	return 0;
}