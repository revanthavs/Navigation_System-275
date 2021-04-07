#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>     // socket, bind, listen, accept
#include <sys/time.h>       // timeval
#include <arpa/inet.h>      // inet_ntoa, htonl, htons, ntohl, ntohs
#include <netdb.h>          // getaddrinfo, freeaddrinfo, INADDR_ANY (superset of netinet/in.h)
#include <cstdlib>          // atoi
#include <cstring>          // strlen, strcmp
#include <string>       
#include <vector> // Need to remove this
// Add more libraries, macros, functions, and global variables if needed

using namespace std;

int create_and_open_fifo(const char * pname, int mode) {
    // creating a fifo special file in the current working directory
    // with read-write permissions for communication with the plotter
    // both proecsses must open the fifo before they can perform
    // read and write operations on it
    if (mkfifo(pname, 0666) == -1) {
        cout << "Unable to make a fifo. Ensure that this pipe does not exist already!" << endl;
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
    int MSG_SIZE = 1024;
    int SERVER_PORT = 50000;
    string server_IP="";

    const char *inpipe = "inpipe";
    const char *outpipe = "outpipe";

    if (argc == 3){
        SERVER_PORT = atoi(argv[1]);
        server_IP=argv[2];
    }
    else{
        cout << "Enter port number and server IP address: \n";
        return 0;
    }

    int in = create_and_open_fifo(inpipe, O_RDONLY);
    cout << "inpipe opened..." << endl;
    int out = create_and_open_fifo(outpipe, O_WRONLY);
    cout << "outpipe opened..." << endl;

    // Your code starts here



    // 1. Establish a connection with the server
    struct sockaddr_in my_addr, peer_addr;

    // zero out the structor variable because it has an unused part
    memset(&my_addr, '\0', sizeof my_addr);

    // Declare socket descriptor
    int socket_desc;

   // char outbound[BUFFER_SIZE] = {};
   // char inbound[BUFFER_SIZE] = {};

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        cerr << "Listening socket creation failed!\n";
        return 1;
    }

    cout << "socket created\n";

    // Prepare sockaddr_in structure variable
    peer_addr.sin_family = AF_INET;                         // address family (2 bytes)
    peer_addr.sin_port = htons(SERVER_PORT);                // port in network byte order (2 bytes)
                                                            // htons takes care of host-order to short network-order conversion.

    peer_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);     // internet address (4 bytes). INADDR_LOOPBACK is localhost address

    if (connect(socket_desc, (struct sockaddr *) &peer_addr, sizeof peer_addr) == -1) {
        std::cerr << "Cannot connect to the host!\n";
        close(socket_desc);
        return 1;
    }

    cout << "Connected to the socket\n";

    struct timeval timer = {.tv_sec = 1};

    if (setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (void *) &timer, sizeof(timer)) == -1) {
        std::cerr << "Cannot set socket options!\n";
        close(socket_desc);
        return 1;
      }

    char line[MSG_SIZE] = {};
    bool timeout=false;
    string route_req="";

    while(true){
        vector<string> A_Waypoints;
        bool flag= false;
        //2. Read coordinates of start and end points from inpipe (blocks until they are selected)
        //char line[MSG_SIZE] = {};
        char msg_rec[MSG_SIZE]={};
        //char Out_buff[MSG_SIZE]={};
        int bytes_written;
        int bytes_read;
        string Ack = "A";

        int num;
        

        if(!timeout)
        {
            route_req="";
            route_req+="R";

            bytes_read = read(in, line, MSG_SIZE);

            for (auto _: line){
                cout << _;
            }
            cout << endl;

            if (strcmp("Q", line) == 0) {
                cout << "Quit checkpoint\n";
                send(socket_desc, line, strlen(line) + 1, 0);
                // send(socket_desc, "Q", strlen("Q") + 1, 0);
                cout << "Send Q message to server\n";
                break;
            }

            // char * coord[2];
            string coord[2] = {};
            num = 0;
            for (auto ch : line) {
                if (ch == ' ') {
                // cout << "Check " << coord[num] << endl;
                  ++num;
                }
                else {
                  coord[num] += ch;
                }
            }

            cout << "Checkpoint1\n";

            char coord1[coord[0].length()];
            for (int i = 0; i < coord[0].length(); i++){
                coord1[i] = coord[0][i];
            }

            char coord2[coord[1].length()];
            for (int i = 0; i < coord[1].length(); i++){
                coord2[i] = coord[1][i];
            }

            // route_req+=" "+to_string(atof(coord[0])*100000);
            // route_req+=" "+to_string(atof(coord[1])*100000);

            long long temp1 = static_cast<long long>(atof(coord1)*100000);
            long long temp2 = static_cast<long long>(atof(coord2)*100000);


            route_req+=" "+to_string(temp1);
            route_req+=" "+to_string(temp2);


            cout << "Checkpoint3 " << route_req << endl;

            num=0;

            string s_coord[2];

            bytes_read = read(in, line, MSG_SIZE);

            for (auto _: line){
                cout << _;
            }
            cout << endl;


            if (strcmp("Q", line) == 0) {
                send(socket_desc, line, strlen(line) + 1, 0);
                cout << "Quit checkpoint\n";
                // send(socket_desc, "Q", strlen("Q") + 1, 0);
                cout << "Sended Q message to server\n";
                break;
            }
            for (auto ch : line) {
                if (ch == ' ') {
                    ++num;
                }
                else {
                  s_coord[num] += ch;
                }
            }

            cout << "Checkpoint2\n";

            char coord3[s_coord[0].length()];
            for (int i = 0; i < s_coord[0].length(); i++){
                coord3[i] = s_coord[0][i];
            }

            char coord4[s_coord[1].length()];
            for (int i = 0; i < coord[1].length(); i++){
                coord4[i] = s_coord[1][i];
            }


            long long temp3 = static_cast<long long>(atof(coord3)*100000);
            long long temp4 = static_cast<long long>(atof(coord4)*100000);

            route_req+=" "+to_string(temp3);
            route_req+=" "+to_string(temp4);
            cout << route_req << endl;
        }
        

        // route_req+=" "+to_string(atof(coord[0])*100000);
        // route_req+=" "+to_string(atof(coord[1])*100000);

        cout << "Received coordinates\n";
       // cout << route_req << endl;

        // 3. Write to the socket
        send(socket_desc, route_req.c_str(), route_req.length() + 1, 0);

        cout << "After sending route request\n";

        int rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
        if (rec_size == -1) {
            cout << "Timeout occurred... state reset!\n";
            timeout=true;
            continue;
        }

        num=0;
        string N_msg[2];
        cout << "Got N number message\n";
        if(msg_rec[0] == 'N'){
            if(msg_rec[0] == 'N' && msg_rec[2] == '0'){
                timeout = false;
                continue;
            }
            send(socket_desc, Ack.c_str() , Ack.length()+1, 0);
            for(auto ch : msg_rec){
                // (ch == ' ') ? num++ : N_msg[num] += ch;
                if(ch == ' ')
                    num++;
                else
                    N_msg[num] += ch;
            }
        }
        else{
            cout << "Received unexpected message!\n";
            timeout = true;
            continue; 
        }
        
        cout << Ack << endl;

        int iter=stoi(N_msg[1]);
        cout << iter << endl;
        for(int i=0; i<iter ; i++)
        {
            // string msg[3];
            rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
            if (rec_size == -1) {
                cout << "Timeout occurred... state reset!\n";
                timeout=true;
                flag=true;
                break;
            }

            num=0;
            if(msg_rec[0] == 'W'){
                // Waypoint.push_back(msg_rec);
                string temp_s = "";
                for (auto ch: msg_rec){
                    temp_s += ch;
                }
                A_Waypoints.push_back(temp_s);
                // for(auto ch : msg_rec){
                //     // (ch == ' ') ? num++ : N_msg[num] += ch; 
                //     if(ch == ' '){
                //         num++;
                //     }
                //     else{
                //         msg[num] += ch;
                //     }
                // }
            }
            else{
                cout << "Received unexpected message!\n";
                flag=true;
                timeout = true;
                break; 
            }

            // string Waypoint="";
            // cout << msg[0] << " " << msg[1] << " " << msg[2] << " waypoints\n";
            // cout << "Checkpoint after getting waypoints\n";
            // Waypoint+=to_string( stod(msg[1]) / 100000 ) + " " + to_string( stod(msg[2]) / 100000);

            // char Out_buff[Waypoint.length()+1]={};

            // int j;
            // for(j = 0; j < Waypoint.length(); j++)
            // {
            //     Out_buff[j]=Waypoint[j];
            // }
            // Out_buff[j]='\n';

                
            // bytes_written = write(out, Out_buff , sizeof Out_buff);

            cout << "Waypoint to plotter " << Ack << endl;

            
            send(socket_desc, Ack.c_str() , Ack.length()+1, 0);
        }

        if(flag){
            timeout = true;
            continue;
        }
            
        rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
        if (rec_size == -1) {
            cout << "Timeout occurred... state reset!\n";
            timeout=true;
            continue;
        }


        char E= 'E' ;

        if (msg_rec[0] != E){
            cout << "Received unexpected message!\n";
            timeout = true;
            continue;
          }
        else{
            num = 0;
            for (auto it: A_Waypoints){
                string Newmsg[3];
                num = 0;
                for(auto ch : it){
                    // (ch == ' ') ? num++ : N_Newmsg[num] += ch; 
                    if(ch == ' '){
                        num++;
                    }
                    else{
                        Newmsg[num] += ch;
                    }
                }

                string Waypoint="";
                // cout << Newmsg[0] << " " << Newmsg[1] << " " << Newmsg[2] << " waypoints\n";
                // cout << "Checkpoint after getting waypoints\n";
                Waypoint+=to_string( stod(Newmsg[1]) / 100000 ) + " " + to_string( stod(Newmsg[2]) / 100000);

                char Out_buff[Waypoint.length()+1]={};

                int j;
                for(j = 0; j < Waypoint.length(); j++)
                {
                    Out_buff[j]=Waypoint[j];
                }
                Out_buff[j]='\n';
                
                bytes_written = write(out, Out_buff , sizeof Out_buff);
            }

            char out_msg[2]={};
            out_msg[0]=msg_rec[0];
            out_msg[1]='\n';
            bytes_written = write(out, out_msg , sizeof out_msg);
            timeout = false;
        }
    }


    // Here is what you need to do:
    // 1. Establish a connection with the server
    // 2. Read coordinates of start and end points from inpipe (blocks until they are selected)
    //    If 'Q' is read instead of the coordinates then go to Step 7
    // 3. Write to the socket
    // 4. Read coordinates of waypoints one at a time (blocks until server writes them)
    // 5. Write these coordinates to outpipe
    // 6. Go to Step 2
    // 7. Close the socket and pipes


    // Your code ends here
    close(socket_desc);
    close(in);
    close(out);
    unlink(inpipe);
    unlink(outpipe);
    return 0;
}


// #include <iostream>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <sys/socket.h>     // socket, bind, listen, accept
// #include <sys/time.h>       // timeval
// #include <arpa/inet.h>      // inet_ntoa, htonl, htons, ntohl, ntohs
// #include <netdb.h>          // getaddrinfo, freeaddrinfo, INADDR_ANY (superset of netinet/in.h)
// #include <cstdlib>          // atoi
// #include <cstring>          // strlen, strcmp
// #include <string>       
// // Add more libraries, macros, functions, and global variables if needed

// using namespace std;

// int create_and_open_fifo(const char * pname, int mode) {
//     // creating a fifo special file in the current working directory
//     // with read-write permissions for communication with the plotter
//     // both proecsses must open the fifo before they can perform
//     // read and write operations on it
//     if (mkfifo(pname, 0666) == -1) {
//         cout << "Unable to make a fifo. Ensure that this pipe does not exist already!" << endl;
//         exit(-1);
//     }


//     // opening the fifo for read-only or write-only access
//     // a file descriptor that refers to the open file description is
//     // returned
//     int fd = open(pname, mode);

//     if (fd == -1) {
//         cout << "Error: failed on opening named pipe." << endl;
//         exit(-1);
//     }

//     return fd;
// }

// int main(int argc, char const *argv[]) {
//     int MSG_SIZE = 1024;
//     int SERVER_PORT = 50000;
//     string server_IP="";

//     const char *inpipe = "inpipe";
//     const char *outpipe = "outpipe";

//     if (argc == 3){
//         SERVER_PORT = atoi(argv[1]);
//         server_IP=argv[2];
//     }
//     else{
//         cout << "Enter port number and server IP address: \n";
//         return 0;
//     }

//     int in = create_and_open_fifo(inpipe, O_RDONLY);
//     cout << "inpipe opened..." << endl;
//     int out = create_and_open_fifo(outpipe, O_WRONLY);
//     cout << "outpipe opened..." << endl;

//     // Your code starts here



//     // 1. Establish a connection with the server
//     struct sockaddr_in my_addr, peer_addr;

//     // zero out the structor variable because it has an unused part
//     memset(&my_addr, '\0', sizeof my_addr);

//     // Declare socket descriptor
//     int socket_desc;

//    // char outbound[BUFFER_SIZE] = {};
//    // char inbound[BUFFER_SIZE] = {};

//     socket_desc = socket(AF_INET, SOCK_STREAM, 0);
//     if (socket_desc == -1) {
//         cerr << "Listening socket creation failed!\n";
//         return 1;
//     }

//     cout << "socket created\n";

//     // Prepare sockaddr_in structure variable
//     peer_addr.sin_family = AF_INET;                         // address family (2 bytes)
//     peer_addr.sin_port = htons(SERVER_PORT);                // port in network byte order (2 bytes)
//                                                             // htons takes care of host-order to short network-order conversion.

//     peer_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);     // internet address (4 bytes). INADDR_LOOPBACK is localhost address

//     if (connect(socket_desc, (struct sockaddr *) &peer_addr, sizeof peer_addr) == -1) {
//         std::cerr << "Cannot connect to the host!\n";
//         close(socket_desc);
//         return 1;
//     }

//     cout << "Connected to the socket\n";

//     struct timeval timer = {.tv_sec = 1};

//     if (setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (void *) &timer, sizeof(timer)) == -1) {
//         std::cerr << "Cannot set socket options!\n";
//         close(socket_desc);
//         return 1;
//       }

//     char line[MSG_SIZE] = {};
//     bool timeout=false;
//     string route_req="";

//     while(true){
//         bool flag= false;
//         //2. Read coordinates of start and end points from inpipe (blocks until they are selected)
//         //char line[MSG_SIZE] = {};
//         char msg_rec[MSG_SIZE]={};
//         //char Out_buff[MSG_SIZE]={};
//         int bytes_written;
//         int bytes_read;
//         string Ack = "A";

//         int num;
        

//         if(!timeout)
//         {
//             route_req="";
//             route_req+="R";

//             bytes_read = read(in, line, MSG_SIZE);

//             for (auto _: line){
//                 cout << _;
//             }
//             cout << endl;

//             if (strcmp("Q", line) == 0) {
//                 send(socket_desc, line, strlen(line) + 1, 0);
//                 break;
//             }

//             // char * coord[2];
//             string coord[2] = {};
//             num = 0;
//             for (auto ch : line) {
//                 if (ch == ' ') {
//                 // cout << "Check " << coord[num] << endl;
//                   ++num;
//                 }
//                 else {
//                   coord[num] += ch;
//                 }
//             }

//             cout << "Checkpoint1\n";

//             char coord1[coord[0].length()];
//             for (int i = 0; i < coord[0].length(); i++){
//                 coord1[i] = coord[0][i];
//             }

//             char coord2[coord[1].length()];
//             for (int i = 0; i < coord[1].length(); i++){
//                 coord2[i] = coord[1][i];
//             }

//             // route_req+=" "+to_string(atof(coord[0])*100000);
//             // route_req+=" "+to_string(atof(coord[1])*100000);

//             long long temp1 = static_cast<long long>(atof(coord1)*100000);
//             long long temp2 = static_cast<long long>(atof(coord2)*100000);


//             route_req+=" "+to_string(temp1);
//             route_req+=" "+to_string(temp2);


//             cout << "Checkpoint3 " << route_req << endl;

//             num=0;

//             string s_coord[2];

//             bytes_read = read(in, line, MSG_SIZE);

//             for (auto _: line){
//                 cout << _;
//             }
//             cout << endl;


//             if (strcmp("Q", line) == 0) {
//                 send(socket_desc, line, strlen(line) + 1, 0);
//                 break;
//             }
//             for (auto ch : line) {
//                 if (ch == ' ') {
//                     ++num;
//                 }
//                 else {
//                   s_coord[num] += ch;
//                 }
//             }

//             cout << "Checkpoint2\n";

//             char coord3[s_coord[0].length()];
//             for (int i = 0; i < s_coord[0].length(); i++){
//                 coord3[i] = s_coord[0][i];
//             }

//             char coord4[s_coord[1].length()];
//             for (int i = 0; i < coord[1].length(); i++){
//                 coord4[i] = s_coord[1][i];
//             }


//             long long temp3 = static_cast<long long>(atof(coord3)*100000);
//             long long temp4 = static_cast<long long>(atof(coord4)*100000);

//             route_req+=" "+to_string(temp3);
//             route_req+=" "+to_string(temp4);
//             cout << route_req << endl;


//         }
        

//         // route_req+=" "+to_string(atof(coord[0])*100000);
//         // route_req+=" "+to_string(atof(coord[1])*100000);

//         cout << "Received coordinates\n";
//        // cout << route_req << endl;

//         // 3. Write to the socket
//         send(socket_desc, route_req.c_str(), route_req.length() + 1, 0);

//         cout << "After sending route request\n";

//         int rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
//         if (rec_size == -1) {
//             cout << "Timeout occurred... state reset!\n";
//             timeout=true;
//             continue;
//         }

//         num=0;
//         string N_msg[2];
//         cout << "Got N number message\n";
//         if(msg_rec[0] == 'N'){
//             send(socket_desc, Ack.c_str() , Ack.length()+1, 0);
//             for(auto ch : msg_rec){
//                 // (ch == ' ') ? num++ : N_msg[num] += ch;
//                 if(ch == ' ')
//                     num++;
//                 else
//                     N_msg[num] += ch;
//             }
//         }
//         else{
//             cout << "Received unexpected message!\n";
//             continue; 
//         }
        
//         cout << Ack << endl;

//         int iter=stoi(N_msg[1]);
//         cout << iter << endl;
//         for(int i=0; i<iter ; i++)
//         {
//             string msg[3];
//             rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
//             if (rec_size == -1) {
//                 cout << "Timeout occurred... state reset!\n";
//                 timeout=true;
//                 flag=true;
//                 break;
//             }

//             num=0;
//             if(msg_rec[0] == 'W'){
//                 for(auto ch : msg_rec){
//                     // (ch == ' ') ? num++ : N_msg[num] += ch; 
//                     if(ch == ' '){
//                         num++;
//                     }
//                     else{
//                         msg[num] += ch;
//                     }
//                 }
//             }
//             else{
//                 cout << "Received unexpected message!\n";
//                 flag=true;
//                 break; 
//             }

//             string Waypoint="";
//             cout << msg[0] << " " << msg[1] << " " << msg[2] << " waypoints\n";
//             cout << "Checkpoint after getting waypoints\n";
//             Waypoint+=to_string( stod(msg[1]) / 100000 ) + " " + to_string( stod(msg[2]) / 100000);

//             char Out_buff[Waypoint.length()+1]={};

//             int j;
//             for(j = 0; j < Waypoint.length(); j++)
//             {
//                 Out_buff[j]=Waypoint[j];
//             }
//             Out_buff[j]='\n';

                
//             bytes_written = write(out, Out_buff , sizeof Out_buff);

//             cout << "Waypoint to plotter " << Ack << endl;

            
//             send(socket_desc, Ack.c_str() , Ack.length()+1, 0);
//         }

//         if(flag){
//             continue;
//         }
            
//         rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
//         if (rec_size == -1) {
//             cout << "Timeout occurred... state reset!\n";
//             timeout=true;
//             continue;
//         }


//         char E= 'E' ;

//         if (msg_rec[0] != E){
//             cout << "Received unexpected message!\n";
//             continue;
//           }
//         else{
//             char out_msg[2]={};
//             out_msg[0]=msg_rec[0];
//             out_msg[1]='\n';
//             bytes_written = write(out, out_msg , sizeof out_msg);

//         }
//     }


//     // Here is what you need to do:
//     // 1. Establish a connection with the server
//     // 2. Read coordinates of start and end points from inpipe (blocks until they are selected)
//     //    If 'Q' is read instead of the coordinates then go to Step 7
//     // 3. Write to the socket
//     // 4. Read coordinates of waypoints one at a time (blocks until server writes them)
//     // 5. Write these coordinates to outpipe
//     // 6. Go to Step 2
//     // 7. Close the socket and pipes


//     // Your code ends here
//     close(socket_desc);
//     close(in);
//     close(out);
//     unlink(inpipe);
//     unlink(outpipe);
//     return 0;
// }


// #include <iostream>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <sys/socket.h>     // socket, bind, listen, accept
// #include <sys/time.h>       // timeval
// #include <arpa/inet.h>      // inet_ntoa, htonl, htons, ntohl, ntohs
// #include <netdb.h>          // getaddrinfo, freeaddrinfo, INADDR_ANY (superset of netinet/in.h)
// #include <cstdlib>          // atoi
// #include <cstring>          // strlen, strcmp
// #include <string>       
// // Add more libraries, macros, functions, and global variables if needed

// using namespace std;

// int create_and_open_fifo(const char * pname, int mode) {
//     // creating a fifo special file in the current working directory
//     // with read-write permissions for communication with the plotter
//     // both proecsses must open the fifo before they can perform
//     // read and write operations on it
//     if (mkfifo(pname, 0666) == -1) {
//         cout << "Unable to make a fifo. Ensure that this pipe does not exist already!" << endl;
//         exit(-1);
//     }

//     // opening the fifo for read-only or write-only access
//     // a file descriptor that refers to the open file description is
//     // returned
//     int fd = open(pname, mode);

//     if (fd == -1) {
//         cout << "Error: failed on opening named pipe." << endl;
//         exit(-1);
//     }

//     return fd;
// }

// int main(int argc, char const *argv[]) {
//     int MSG_SIZE = 1024;
//     int SERVER_PORT = 50000;
//     string server_IP="";

//     const char *inpipe = "inpipe";
//     const char *outpipe = "outpipe";

//     if (argc == 3){
//         SERVER_PORT = atoi(argv[1]);
//         server_IP=argv[2];
//     }
//     else{
//         cout << "Enter port number and server IP address: \n";
//         return 0;
//     }

//     int in = create_and_open_fifo(inpipe, O_RDONLY);
//     cout << "inpipe opened..." << endl;
//     int out = create_and_open_fifo(outpipe, O_WRONLY);
//     cout << "outpipe opened..." << endl;

//     // Your code starts here



//     // 1. Establish a connection with the server
//     struct sockaddr_in my_addr, peer_addr;

//     // zero out the structor variable because it has an unused part
//     memset(&my_addr, '\0', sizeof my_addr);

//     // Declare socket descriptor
//     int socket_desc;

//    // char outbound[BUFFER_SIZE] = {};
//    // char inbound[BUFFER_SIZE] = {};

//     socket_desc = socket(AF_INET, SOCK_STREAM, 0);
//     if (socket_desc == -1) {
//         cerr << "Listening socket creation failed!\n";
//         return 1;
//     }

//     // Prepare sockaddr_in structure variable
//     peer_addr.sin_family = AF_INET;                         // address family (2 bytes)
//     peer_addr.sin_port = htons(SERVER_PORT);                // port in network byte order (2 bytes)
//                                                             // htons takes care of host-order to short network-order conversion.

//     peer_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);     // internet address (4 bytes). INADDR_LOOPBACK is localhost address

//     if (connect(socket_desc, (struct sockaddr *) &peer_addr, sizeof peer_addr) == -1) {
//         std::cerr << "Cannot connect to the host!\n";
//         close(socket_desc);
//         return 1;
//     }

//     struct timeval timer = {.tv_sec = 1};

//     if (setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (void *) &timer, sizeof(timer)) == -1) {
//         std::cerr << "Cannot set socket options!\n";
//         close(socket_desc);
//         return 1;
//       }


//     while(true){
//         bool flag= false;
//         //2. Read coordinates of start and end points from inpipe (blocks until they are selected)
//         char line[MSG_SIZE] = {};
//         char msg_rec[MSG_SIZE]={};
//         //char Out_buff[MSG_SIZE]={};
//         int bytes_written;
//         int bytes_read;
//         string Ack = "A";
        

//         string route_req="";

//         route_req+="R";

//         bytes_read = read(in, line, MSG_SIZE);

//         if (strcmp("Q", line) == 0) {
//             send(socket_desc, line, strlen(line) + 1, 0);
//             break;
//         }

//         char * coord[2];
//         int num = 0;
//         for (auto ch : line) {
//             if (ch == ' ') {
//               ++num;
//             }
//             else {
//               coord[num] += ch;
//             }
//         }

//         route_req+=" "+to_string(atof(coord[0])*100000);
//         route_req+=" "+to_string(atof(coord[1])*100000);

//         num=0;

//         bytes_read = read(in, line, MSG_SIZE);

//         if (strcmp("Q", line) == 0) {
//             send(socket_desc, line, strlen(line) + 1, 0);
//             break;
//         }
//         for (auto ch : line) {
//             if (ch == ' ') {
//                 ++num;
//             }
//             else {
//               coord[num] += ch;
//             }
//         }

//         route_req+=" "+to_string(atof(coord[0])*100000);
//         route_req+=" "+to_string(atof(coord[1])*100000);

//         // 3. Write to the socket
//         send(socket_desc, route_req.c_str(), route_req.length() + 1, 0);

//         int rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
//         if (rec_size == -1) {
//             cout << "Timeout occurred... state reset!\n";
//             continue;
//         }

//         num=0;
//         string msg[2];
//         if(msg_rec[0] == 'N'){
//             send(socket_desc, Ack.c_str() , Ack.length()+1, 0);
//             for(auto ch : msg_rec){ 
//                 if(ch == ' '){
//                     num++;
//                 }
//                 else if(ch == '\0'){
//                     break;
//                 }
//                 else{
//                     msg[num] += ch;
//                 }
//             }
//         }
//         else{
//             cout << "Received unexpected message!\n";
//             continue; 
//         }
        

//         int iter=stoi(msg[1]);
//         for(int i=0; i<iter ; i++)
//         {
//             string msg[3];
//             rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
//             if (rec_size == -1) {
//                 cout << "Timeout occurred... state reset!\n";
//                 flag=true;
//                 break;
//             }

//             num=0;
//             if(msg_rec[0] == 'W'){
//                 for(auto ch : msg_rec){ 
//                     if(ch == ' '){
//                         num++;
//                     }
//                     else if(ch == '\0'){
//                         break;
//                     }
//                     else{
//                         msg[num] += ch;
//                     }
//                 }
//             }
//             else{
//                 cout << "Received unexpected message!\n";
//                 flag=true;
//                 break; 
//             }

//             string Waypoint="";
//             Waypoint+=to_string( stod(msg[1]) / 100000 ) + " " + to_string( stod(msg[2]) / 100000);

//             char Out_buff[Waypoint.length()+1]={};

//             int j;
//             for(j = 0; j < Waypoint.length(); j++)
//             {
//                 Out_buff[j]=Waypoint[j];
//             }
//             Out_buff[j]='\n';

                
//             bytes_written = write(out, Out_buff , sizeof Out_buff);

            
//             send(socket_desc, Ack.c_str() , Ack.length()+1, 0);
//         }

//         if(flag){
//             continue;
//         }
            
//         rec_size = recv(socket_desc, msg_rec, MSG_SIZE, 0);
//         if (rec_size == -1) {
//             cout << "Timeout occurred... state reset!\n";
            
//             continue;
//         }


//         char E= 'E' ;

//         if (msg_rec[0] != E){
//             cout << "Received unexpected message!\n";
//             continue;
//           }
//         else{
//             char out_msg[2]={};
//             out_msg[0]=msg_rec[0];
//             out_msg[1]='\n';
//             bytes_written = write(out, out_msg , sizeof out_msg);

//         }
//     }


//     // Here is what you need to do:
//     // 1. Establish a connection with the server
//     // 2. Read coordinates of start and end points from inpipe (blocks until they are selected)
//     //    If 'Q' is read instead of the coordinates then go to Step 7
//     // 3. Write to the socket
//     // 4. Read coordinates of waypoints one at a time (blocks until server writes them)
//     // 5. Write these coordinates to outpipe
//     // 6. Go to Step 2
//     // 7. Close the socket and pipes


//     // Your code ends here
//     close(socket_desc);
//     close(in);
//     close(out);
//     unlink(inpipe);
//     unlink(outpipe);
//     return 0;
// }
