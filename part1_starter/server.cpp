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

	while(!fin.eof())
  	{// a loop to read from the stdin until EOF is reached

	    // declaring variables
	    Point pt;
	    string str,value;
	    //reading a line from stdin using istream object cin
	    getline(cin,str);
	    //storing the choice, 'V' or 'E'
	    int pos=str.find(",");
	    value=str.substr(0,pos);
	    str=str.substr(pos+1);

	    if(value=="V")
	    {// if a vertex is given
	      string temp;
	      //read and store the ID of the vertex
	      pos=str.find(",");
	      temp=str.substr(0,pos);
	      int v=stoi(temp);
	      // add the vertex to the graph
	      graph.addVertex(v);

	      str=str.substr(pos+1);
	      pos=str.find(",");
	      temp = str.substr(0,pos);
	      double coord= stod(temp);
	      pt.lat=static_cast<long long>(coord*100000);

	      temp =str.substr(pos+1);
	      coord= stod(temp);
	      pt.lon=static_cast<long long>(coord*100000);
	      points[v]=pt;
	    }
	    if(choice=="E"){
	    	
	    }

	}

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