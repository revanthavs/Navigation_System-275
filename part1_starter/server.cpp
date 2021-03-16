#include <iostream>
#include "heap.h"
#include "wdigraph.h"

using namespace std;

// R: added this template file to make things easier

struct Point {
    long long Lat; // Latitude of the point
    long long lon; // Longitude of the point
};

void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {

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

}

long longmanhattan(constPoint& pt1,constPoint& pt2) {
    // Return the Manhattan distance between the two given points
}

void dijkstra(constWDigraph& graph,intstartVertex, unordered_map<int, PIL>& tree) {
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