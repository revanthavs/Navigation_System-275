/*
Name: Venkata Sai Revanth Atmakuri
ID  : 1684293
Name: Jasmine Kaur Wadhwa
ID  : 1619400
CMPUT 275 WINTER 2021
Assignment #1  Trivial Nagivation System
*/


#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>     // socket, bind, listen, accept
#include <sys/time.h>       // timeval
#include <arpa/inet.h>      // inet_ntoa, htonl, htons, ntohl, ntohs
#include <netdb.h>          // getaddrinfo, freeaddrinfo, INADDR_ANY
#include <cstdlib>          // atoi
#include <cstring>          // strlen, strcmp
#include <string>       
#include <vector>

using namespace std;

int create_and_open_fifo(const char * pname, int mode) {
    // creating a fifo special file in the current working directory
    // with read-write permissions for communication with the plotter
    // both proecsses must open the fifo before they can perform
    // read and write operations on it
    if (mkfifo(pname, 0666) == -1) {
        cout << "Unable to make a fifo. Ensure that this pipe does not";
        cout << "exist already!" << endl;
        exit(-1);
    }


    // opening the fifo for read-only or write-only access
    // a file descriptor that refers to the open file description is
    // returned
    int fd = open(pname, mode);

    if (fd == -1) {
        cout << "Error: failed on opening named pipe." << endl;
        exit(-1);
    }

    return fd; 
}

int main(int argc, char const *argv[]) {
    /* 
    This is the main function of client program. The client program
    establishes a two way communication channel with the plotter and
    the server. It uses pipes to communicate with the plotter program
    and sockets to communicate with the server program. It reads the
    coordinates from the plotter, and sends them to the route finding 
    server. It then receives the waypoints from the server and
    sends to the plotter.
    */


    //Declaring variables
    int MSG_SIZE = 1024;
    int SERVER_PORT = 50000;
    string server_IP="";

    const char *inpipe = "inpipe";
    const char *outpipe = "outpipe";

    //reading the command line arguments
    if (argc == 3){
        SERVER_PORT = atoi(argv[1]);
        server_IP=argv[2];
    }
    else{
        // send an error message and display the running instructions
        cout << "Enter port number and server IP address: \n";
        return 0;
    }

    // calling create_and_openfifo() to open inpipe and outpipe with read only
    // and write only modes respectively. 
    // these pipes will be used to read from and write to the plotter program
    int in = create_and_open_fifo(inpipe, O_RDONLY);
    cout << "inpipe opened..." << endl;
    int out = create_and_open_fifo(outpipe, O_WRONLY);
    cout << "outpipe opened..." << endl;



    // 1. Establish a connection with the server
    struct sockaddr_in my_addr, peer_addr;

    // zero out the structor variable because it has an unused part
    memset(&my_addr, '\0', sizeof my_addr);

    // Declare socket descriptor
    int socket_desc;


    // creating a socket to communicate with the server
    /*
        socket() input arguments are:
        socket domain (AF_INET):    IPv4 Internet protocols
        socket type (SOCK_STREAM):  sequenced, reliable, two-way,
                                    connection-based byte streams
        socket protocol (0):        OS selects a protocol that supports
                                    the requested socket type
                                    (in this case: IPPROTO_TCP)
        socket() returns a socket descriptor
    */
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        cerr << "Listening socket creation failed!\n";
        return 1;
    }

    cout << "socket created\n";

    // Prepare sockaddr_in structure variable
    // address family (2 bytes)
    peer_addr.sin_family = AF_INET;   
    // port in network byte order (2 bytes)
    // htons takes care of host-order to short network-order conversion.                      
    peer_addr.sin_port = htons(SERVER_PORT);               
                                                            
    // internet address (4 bytes). INADDR_LOOPBACK is localhost address
    peer_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);     

    // connecting to the server socket
    if (connect(socket_desc, (struct sockaddr *) &peer_addr,
    sizeof peer_addr) == -1) {
        std::cerr << "Cannot connect to the host!\n";
        close(socket_desc);
        return 1;
    }

    cout << "Connected to the socket\n";

    // initializing the struct timer to initialize the timeout interval
    struct timeval timer = {.tv_sec = 1};

    // calling setsockopt() to set the receiving timeout interval
    if (setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (void *) &timer,
    sizeof(timer)) == -1) {
        std::cerr << "Cannot set socket options!\n";
        close(socket_desc);
        return 1;
      }

    // declaring timeout to be false initially
    bool timeout=false;

    // a string to store the route request sent by the plotter
    string route_req="";

    while(true){
        // this while loop runs until the plotter program exits,
        // i.e there are no more route requests to be catered for.

        // A vector of type string to store all the Waypoints along the path
        vector<string> A_Waypoints;

        char line[MSG_SIZE] = {};
        bool flag= false;


        // Declaring variables
        
        char msg_rec[MSG_SIZE]={};
        
        int bytes_written = 0;
        int bytes_read = 0;
        if (false){
            cout << bytes_written << " " << bytes_read << endl;
        }
        //bytes_written=0;
        //bytes_read=0;
        string Ack = "A";

        int num;
        

        if(!timeout)
        {  // This if loop runs when there is no timeout and new coordinates
           // are to be read from the plotter and transferred to the server

            //building the string route_req
            route_req="";
            route_req+="R";

            //reading the coordinates of the first point from the plotter
            bytes_read = read(in, line, MSG_SIZE);

        
            // checking for 'Q' message
            if (line[0] == 'Q'){
                // if a 'Q' message is received, the client and server
                // must exit.
        
                send(socket_desc, line, strlen(line) + 1, 0);

                break;
            }

            string coord[2] = {};
            num = 0;
            for (auto ch : line) {
                // for loop to read and strore the coordinates
                if (ch == ' ') {
                  ++num;
                }
                else {
                  coord[num] += ch;
                }
            }

            long long temp1 = static_cast<long long>(stod(coord[0])*100000);
            long long temp2 = static_cast<long long>(stod(coord[1])*100000);

            // converting coordinates to string and appending them to the
            // route request

            route_req+=" "+to_string(temp1);
            route_req+=" "+to_string(temp2);
            num=0;

            string s_coord[2];

            //reading the coordinates of the end point
            bytes_read = read(in, line, MSG_SIZE);

            // checking for 'Q' message
            if (line[0] == 'Q'){
                // if a 'Q' message is received, the client and server
                // must exit.
                
                send(socket_desc, line, strlen(line) + 1, 0);
                break;
            }
            for (auto ch : line) {
                // for loop to read and strore the coordinates
                if (ch == ' ') {
                    ++num;
                }
                else {
                  s_coord[num] += ch;
                }
            }

            long long temp3 = static_cast<long long>(stod(s_coord[0])*100000);
            long long temp4 = static_cast<long long>(stod(s_coord[1])*100000);

            // converting coordinates to string and appending them to the
            // route request
            route_req+=" "+to_string(temp3);
            route_req+=" "+to_string(temp4);
            
        }
        
        // send route request to the socket
        send(socket_desc, route_req.c_str(), route_req.length() + 1, 0);

        
        // receiving the message with the number of coordinates from
        // the server
        int rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
        if (rec_size == -1) {
            //cout << "Timeout occurred... state reset!\n";
            timeout=true;
            continue;
        }

        num=0;
        string N_msg[2];
    
        if(msg_rec[0] == 'N'){
            // checking if the message is valid
            if(msg_rec[0] == 'N' && msg_rec[2] == '0'){
                // in case there is no path between the selected coordinates

                //send an E message to the plotter
                char o_msg[2];
                o_msg[0]='E'; 
                o_msg[1]='\n';
                bytes_written=write(out,o_msg, sizeof o_msg);
                //update the timeout flag
                timeout = false;
                continue;
            }

            // sending the Acknowledgement A to the server
            send(socket_desc, Ack.c_str() , Ack.length()+1, 0);

            for(auto ch : msg_rec){
                // a for loop to read and store the message received.
                if(ch == ' ')
                    num++;
                else
                    N_msg[num] += ch;
            }
        }
        else{
            // if an unexpected message is received, reset the state

            // cout << "Received unexpected message!\n";
            timeout = true;
            continue; 
        }
        
        
        // converting the number of waypoints to an integer
        int iter=stoi(N_msg[1]);

        for(int i=0; i<iter ; i++)
        {
            // a for loop to receive all the waypoints from the server 
            // and send them to the plotter

            rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
            if (rec_size == -1) {
                // if a timeout occurs.

                //cout << "Timeout occurred... state reset!\n";

                //updating the flags
                timeout=true;
                flag=true;
                break;
            }

            num=0;
            if(msg_rec[0] == 'W'){
                // checking if the correct message is received.
                string temp_s = "";
                for (auto ch: msg_rec){
                    temp_s += ch;
                }
                // storing the waypoints in the vector
                A_Waypoints.push_back(temp_s);
            }
            else{
                // if an unexpected message is received
                //cout << "Received unexpected message!\n";

                //updating the flags
                flag=true;
                timeout = true;
                break; 
            }

            
            // sending the Acknowledgement 'A' to the plotter
            send(socket_desc, Ack.c_str() , Ack.length()+1, 0);
        }

        if(flag){
            // if flag is true, continue to the next iteration
            timeout = true;
            continue;
        }
        
        // receiving the 'E' message from the server
        rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
        if (rec_size == -1) {
            //cout << "Timeout occurred... state reset!\n";

            // updating the timeout flag
            timeout=true;
            continue;
        }


        char E= 'E' ;

        if (msg_rec[0] != E){
            //if an unexpected message is received, update the flag
            //cout << "Received unexpected message!\n";
            timeout = true;
            continue;
          }
        else{

            // build and send coordinates of the waypoints to the plotter
            num = 0;
            for (auto it: A_Waypoints){
                
                string Newmsg[3];
                num = 0;
                for(auto ch : it){
                
                    if(ch == ' '){
                        num++;
                    }
                    else{
                        Newmsg[num] += ch;
                    }
                }

                string Waypoint="";
                
                Waypoint+= to_string( stod(Newmsg[1]) / 100000 );
                Waypoint +=" " + to_string( stod(Newmsg[2]) / 100000);

                char Out_buff[Waypoint.length()+1]={};

                unsigned int j;
                for(j = 0; j < Waypoint.length(); j++)
                {
                    Out_buff[j]=Waypoint[j];
                }

                // adding the newline character at the end of each waypoint
                Out_buff[j]='\n';
                    
                // writing the waypoints to the plotter
                bytes_written = write(out, Out_buff , sizeof Out_buff);
            }


            // sending the 'E' message to the plotter
            char out_msg[2]={};
            out_msg[0]=msg_rec[0];
            out_msg[1]='\n';
            bytes_written = write(out, out_msg , sizeof out_msg);

            //reset the timeout flag to receive new coordinates
            timeout = false;
        }
    }


    // close the socket connection with the server
    close(socket_desc);

    // close and unlink the inpipe and outpipe
    close(in);
    close(out);
    unlink(inpipe);
    unlink(outpipe);
    return 0;
}