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

// Keep in mind that in Part I, your program must handle 1 request
// but in Part 2 you must serve the next request as soon as you are
// done handling the previous one
// int main(int argc, char* argv[]) {
//   WDigraph graph;
//   unordered_map<int, Point> points;

//   // build the graph
//   readGraph("edmonton-roads-2.0.1.txt", graph, points);

//   // In Part 2, client and server communicate using a pair of sockets
//   // modify the part below so that the route request is read from a socket
//   // (instead of stdin) and the route information is written to a socket

//   // read a request
//   char c;
//   Point sPoint, ePoint;
//   cin >> c >> sPoint.lat >> sPoint.lon >> ePoint.lat >> ePoint.lon;

//   // c is guaranteed to be 'R' in part 1, no need to error check until part 2

//   // get the points closest to the two points we read
//   int start = findClosest(sPoint, points), end = findClosest(ePoint, points);

//   // run dijkstra's, this is the unoptimized version that does not stop
//   // when the end is reached but it is still fast enough
//   unordered_map<int, PIL> tree;
//   dijkstra(graph, start, tree);

//   // no path
//   if (tree.find(end) == tree.end()) {
//       cout << "N 0" << endl;
//   }
//   else {
//     // read off the path by stepping back through the search tree
//     list<int> path;
//     while (end != start) {
//       path.push_front(end);
//       end = tree[end].first;
//     }
//     path.push_front(start);

//     // output the path
//     cout << "N " << path.size() << endl;
//     for (int v : path) {
//       cout << "W " << points[v].lat << ' ' << points[v].lon << endl;
//     }
//     cout << "E" << endl;
//   }

//   return 0;
// }

// Keep in mind that in Part I, your program must handle 1 request
// but in Part 2 you must serve the next request as soon as you are
// done handling the previous one
int main(int argc, char* argv[]) {
  // need to make it unsign
  int PORT = 50000, LISTEN_BACKLOG = 50, BUFFER_SIZE = 1024;
  WDigraph graph;
  unordered_map<int, Point> points;

  // build the graph
  readGraph("edmonton-roads-2.0.1.txt", graph, points);

  // In Part 2, client and server communicate using a pair of sockets
  // modify the part below so that the route request is read from a socket
  // (instead of stdin) and the route information is written to a socket

  if (argc == 2){
    PORT = atoi(argv[1]);
  }
  else{
    std::cout << "Enter port number: \n";
  }

  struct sockaddr_in my_addr, peer_addr;

  memset(&my_addr, '\0', sizeof my_addr);

  int lstn_socket_desc, conn_socket_desc;

  char buffer[BUFFER_SIZE] = {};

  lstn_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (lstn_socket_desc == -1){
    std::cerr << "Listening socket creation failed!\n";
    return 1;
  }

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);

  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

  conn_socket_desc = accept(lstn_socket_desc, (struct sockaddr *) &peer_addr, &peer_addr_size);
  if (conn_socket_desc == -1){
    std::cerr << "Connection socket creation failed!\n";

    // Need to check weather to continue
    // Need to check if we need to close the socket descriptor
    return 1;
  }
  std::cout << "Connection request accepted from " << inet_ntoa(peer_addr.sin_addr);
  std::cout << ":" << ntohs(peer_addr.sin_port) << "\n";

  struct timeval timer = {.tv_sec = 1, .tv_usec = 10000};

  if (setsockopt(conn_socket_desc, SOL_SOCKET, SO_RCVTIMEO, (void *) &timer, sizeof(timer)) == -1) {
    std::cerr << "Cannot set socket options!\n";
    close(conn_socket_desc);
    return 1;
  }


  while(true) {
    int rec_size = recv(conn_socket_desc, buffer, BUFFER_SIZE, 0);
    if (rec_size == -1){
      std::cout << "Timeout occurred.. still waiting!\n";
      continue;
    }
    std::cout << "Message received\n";
    Point sPoint, ePoint;
    unordered_map<int, PIL> tree;
    int start, end;
    sPoint.lat = 0; sPoint.lon = 0; ePoint.lat = 0; ePoint.lat = 0;
        
    if (strcmp("Q", buffer) == 0) {
      std::cout << "Connection will be closed\n";
      // Need to check since we need to close connection
      // I am thinking to closing the file destrctor would make more sense
      // and then returning from the program
      break;
    }
    char R = 'R';
    if (buffer[0] == R){
      // Need to check this way
      char *p[5];
      int at = 0;
      for (auto c : line) {
        if (c == ' ') {
          // starting a new string
          ++at;
        }
        else {
          // appending a character to the string we are building
          p[at] += c;
        }
      }

      sPoint.lat = atoi(p[1]);sPoint.lon = atoi(p[2]);
      ePoint.lat = atoi(p[3]);ePoint.lon = atoi(p[4]);

      start = findClosest(sPoint, points); end = findClosest(ePoint, points);

      dijkstra(graph, start, tree);
      if (tree.find(end) == tree.end()) {
            std::string str = "N 0";
            send(conn_socket_desc, str.c_str(), str.length() + 1, 0);
            std::cout << "No path\n";
            continue;
        }
        else {
          // read off the path by stepping back through the search tree
          list<int> path;
          while (end != start) {
            path.push_front(end);
            end = tree[end].first;
          }
          path.push_front(start);

          // output the path
          // cout << "N " << path.size() << endl;
          std::string str = "N ";
          str += std::to_string(path.size());
          send(conn_socket_desc, str.c_str(), str.length() + 1, 0);
          rec_size = recv(conn_socket_desc, buffer, BUFFER_SIZE, 0);
          if (rec_size == -1){
            std::cout << "Timeout occurred.. still waiting!\n";
            continue;
          }
          char A = 'A';
          if (buffer[0] != A){
            std::cout << "Received unexpected message!\n";
            continue;
          }
          for (int v : path) {
            std::string Way_point = "W ";
            str += std::to_string(points[v].lat);
            str += " ";
            str += std::to_string(points[v].lon);
            // str += "\n"; need to check if I need to send newline character
            send(conn_socket_desc, Way_point.c_str(), Way_point.length() + 1, 0);
            rec_size = recv(conn_socket_desc, buffer, BUFFER_SIZE, 0);
            if (rec_size == -1) {
              std::cout << "Timeout occurred.. still waiting!\n";
              // Need to find a way to get to ther outer loop
              continue;
            }
            else{
              if (buffer[0] != A){
                std::cout << "Received unexpected message!\n";
                // Need to find a way to get to the outer loop
                continue;
              }
            }
          }
          str = "E";
          send(conn_socket_desc, str.c_str(), str.length() + 1, 0);
          continue; // Need to conform
        }
    }
    else{
      std::cout << "Received unexpected message!\n";
      continue;
    }
  }

  return 0;
}
