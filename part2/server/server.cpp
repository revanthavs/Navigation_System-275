/*
 Name: Atmakuri Venkata Sai Revanth
 ID  : 1684293

 Name: Jasmine Kaur Wadhwa
 ID  : 1619400

 CMPUT 275 WINTER 2021
 Assignment #1  Trivial Nagivation System
*/

#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <list>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "wdigraph.h"
#include "dijkstra.h"

struct Point {
    long long lat, lon;
};

// returns the manhattan distance between two points
long long manhattan(const Point& pt1, const Point& pt2) {
  long long dLat = pt1.lat - pt2.lat, dLon = pt1.lon - pt2.lon;
  return abs(dLat) + abs(dLon);
}

// finds the point that is closest to a given point, pt
int findClosest(const Point& pt, const unordered_map<int, Point>& points) {
  pair<int, Point> best = *points.begin();

  for (const auto& check : points) {
    if (manhattan(pt, check.second) < manhattan(pt, best.second)) {
      best = check;
    }
  }
  return best.first;
}

// reads graph description from the input file and builts a graph instance
void readGraph(const string& filename, WDigraph& g, unordered_map<int, Point>& points) {
  ifstream fin(filename);
  string line;

  while (getline(fin, line)) {
    // split the string around the commas, there will be 4 substrings either way
    string p[4];
    int at = 0;
    for (auto c : line) {
      if (c == ',') {
        // starting a new string
        ++at;
      }
      else {
        // appending a character to the string we are building
        p[at] += c;
      }
    }

    if (at != 3) {
      // empty line
      break;
    }

    if (p[0] == "V") {
      // adding a new vertex
      int id = stoi(p[1]);
      assert(id == stoll(p[1])); // sanity check: asserts if some id is not 32-bit
      points[id].lat = static_cast<long long>(stod(p[2])*100000);
      points[id].lon = static_cast<long long>(stod(p[3])*100000);
      g.addVertex(id);
    }
    else {
      // adding a new directed edge
      int u = stoi(p[1]), v = stoi(p[2]);
      g.addEdge(u, v, manhattan(points[u], points[v]));
    }
  }
}


int main(int argc, char* argv[]) {
  // need to make it unsign
  ///
  unsigned int PORT = 50000, LISTEN_BACKLOG = 50, BUFFER_SIZE = 1024;
  WDigraph graph;
  // To store the Vertex Id's mapping to their coordinate points
  unordered_map<int, Point> points;

  // To build the edmonton graph
  readGraph("edmonton-roads-2.0.1.txt", graph, points);

  if (argc == 2){
    PORT = atoi(argv[1]);
  }
  else{
    std::cout << "Run server program with a port number: ./server [port]\n";
    return 1;
  }

  // Declare structure variables that store local and peer socket addresses
  // sockaddr_in is the address sturcture used for IPv4 
  // sockaddr is the protocol independent address structure
  struct sockaddr_in my_addr, peer_addr;

  // zero out the structor variable because it has an unused part
  memset(&my_addr, '\0', sizeof my_addr);

  // To store the socket descriptors
  int lstn_socket_desc, conn_socket_desc;

  /*
    socket() input arguments are:
    socket domain (AF_INET):  IPv4 Internet protocols
    socket type (SOCK_STREAM):  sequenced, reliable, two-way, connection-based
                  byte streams
    socket protocol (0):    OS selects a protocol that supports the requested
                  socket type (in this case: IPPROTO_TCP)
    socket() returns a socket descriptor
  */
  lstn_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (lstn_socket_desc == -1){
    std::cerr << "Listening socket creation failed!\n";
    return 1;
  }

  // address family (2 bytes)
  my_addr.sin_family = AF_INET;

  // port in network byte order (2 bytes)
  // htons takes care of host-order to short network-order conversion.
  my_addr.sin_port = htons(PORT);

  // internet address (4 bytes) INADDR_ANY is all local interfaces
  // htons takes care of host-order to long network-order conversion.
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // note bind takes in a protocol independent address structure
  // hence we need to cast sockaddr_in* to sockaddr*
  if (bind(lstn_socket_desc, (struct sockaddr *) &my_addr, sizeof my_addr) == -1){
    std::cerr << "Binding failed!\n";
    close(lstn_socket_desc);
    return 1;
  }
  std::cout<< "Binding was successful\n";

  if(listen(lstn_socket_desc, LISTEN_BACKLOG) == -1){
    std::cerr << "Cannot listen to the specified socket!\n";
    close(lstn_socket_desc);
    return 1;
  }
  
  socklen_t peer_addr_size = sizeof my_addr;

  // Extract the first connection request from the queue of pending connection requests
  // Return a new connection socket descriptor which is not in the listening state
  conn_socket_desc = accept(lstn_socket_desc, (struct sockaddr *) &peer_addr, &peer_addr_size);
  if (conn_socket_desc == -1){
    std::cerr << "Connection socket creation failed!\n";
    // close(lstn_socket_desc);///
    // Need to check weather to continue
    // Need to check if we need to close the socket descriptor
    return 1;
  }
  std::cout << "Connection request accepted from " << inet_ntoa(peer_addr.sin_addr);
  std::cout << ":" << ntohs(peer_addr.sin_port) << "\n";

  // Declare structure variable that represents an elapsed time 
  // It stores the number of whole seconds and the number of microseconds
  struct timeval timer = {.tv_sec = 1, .tv_usec = 10000};

  /*   setsockopt sets a socket option
    it takes a socket descriptor, an integer that represents the level at which the option resides,
    an integer that can be mapped to the option name, a buffer pointed to by a const void * that 
    contains the option value, and an integer for the size of that buffer

    to manipulate socket API options, use SOL_SOCKET for the level
    SO_RCVTIMEO and SO_SNDTIMEO are option names for receiving and sending timeouts respectively
    send and recv functions return -1 if timeout occurs
  */
  if (setsockopt(conn_socket_desc, SOL_SOCKET, SO_RCVTIMEO, (void *) &timer, sizeof(timer)) == -1) {
    std::cerr << "Cannot set socket options!\n";
    close(lstn_socket_desc);
    return 1;
  }

  // Loop ends when Client sends "Q" message
  while(true) {

    // To recieve new data from client
    char buffer[BUFFER_SIZE] = {};

    // Receiving the route request from client
    int rec_size = recv(conn_socket_desc, buffer, BUFFER_SIZE, 0);

    // To store the start and end points of the route
    Point sPoint, ePoint;
    // To store the Waypoints
    unordered_map<int, PIL> tree;
    // To store the start and end vertex's
    int start, end;
    sPoint.lat = 0; sPoint.lon = 0; ePoint.lat = 0; ePoint.lat = 0;

    // Q message is recieved from client connection will be closed and exits
    if (buffer[0] == 'Q'){

      std::cout << "Connection will be closed\n";
      close(lstn_socket_desc);
      return 0;
    }

    // If it's a route request from the client
    char R = 'R';
    if (buffer[0] == R){

      // To store the coordinate
      std::string coords[5];
      int at = 0;
      for (auto c : buffer) {
        if (c == ' ') {
          // starting a new string
          ++at;
        }
        else {
          // appending a character to the string we are building
          coords[at] += c;
        }
      }
      
      std::string::size_type sz = 0;   // alias of size_t

      sPoint.lat = stoll(coords[1], &sz, 0); sPoint.lon = stoll(coords[2], &sz, 0);
      ePoint.lat = stoll(coords[3], &sz, 0); ePoint.lon = stoll(coords[4], &sz, 0);

      // get the points closest to the two points we read
      start = findClosest(sPoint, points); end = findClosest(ePoint, points);

      // To find the closet paths in the given graph
      dijkstra(graph, start, tree);
      if (tree.find(end) == tree.end()) {
            std::string str = "N 0";
            send(conn_socket_desc, str.c_str(), str.length() + 1, 0);
            std::cout << "No path\n";
            // Since it's no path continue to accept new route request
            goto Recv_new_request;
        }
        else {
          // read off the path by stepping back through the search tree
          list<int> path;
          while (end != start) {
            path.push_front(end);
            end = tree[end].first;
          }

          path.push_front(start);

          // Sending the number of waypoints
          std::string str = "N ";
          str += std::to_string(path.size());
          send(conn_socket_desc, str.c_str(), str.length() + 1, 0);
          rec_size = recv(conn_socket_desc, buffer, BUFFER_SIZE, 0);
          if (rec_size == -1){
            std::cout << "Timeout occurred.. still waiting!\n";
            // Since it's a timeout continue to recieve new route request
            continue;
          }

          // Q message is recieved from client connection will be closed and exits
          if (buffer[0] == 'Q'){
            std::cout << "Connection will be closed\n";
            close(lstn_socket_desc);
            return 0;
          }

          char A = 'A';
          if (buffer[0] != A){
            std::cout << "Received unexpected message!\n";
            // Continuew to recieve new route request from client
            continue;
          }
          // Sending the waypoints
          for (auto v : path) {
            buffer[BUFFER_SIZE] = {};

            std::string Way_point = "W ";
            Way_point += std::to_string(points[v].lat);
            Way_point += " ";
            Way_point += std::to_string(points[v].lon);

            send(conn_socket_desc, Way_point.c_str(), Way_point.length() + 1, 0);
            rec_size = recv(conn_socket_desc, buffer, BUFFER_SIZE, 0);

            if (rec_size == -1) {
              
              std::cout << "Timeout occurred.. still waiting!\n";
              // Since it's a timeout continue to recieve new route request
              goto Recv_new_request;

            }

            // Q message is recieved from client connection will be closed and exits
            if (buffer[0] == 'Q'){
              std::cout << "Connection will be closed\n";
              close(lstn_socket_desc);
              return 0;

            }

            if (buffer[0] != A){
              std::cout << "Received unexpected message!\n";
              // Continuew to recieve new route request from client
              goto Recv_new_request;
            }
          }

          str = "E";
          send(conn_socket_desc, str.c_str(), str.length() + 1, 0);
          // To accept the new route request
          continue;
        }
    }
    Recv_new_request:
      continue;
  }

  close(lstn_socket_desc);
  return 0;
}
