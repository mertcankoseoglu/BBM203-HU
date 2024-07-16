#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {
public:
    Network();
    ~Network();

    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port,
                     const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename);
    
    void show_frame_info(const vector<Client>& clients, const string& line);
    void message(vector<Client>& clients, string line, int message_limit, const string &sender_port, const string &receiver_port); 
    void send(vector<Client>& clients);
    void print_command(const string line);
    void logs_print(vector<Client>& clients, string line);
    void receive(vector<Client>& clients);
    void print_frame_queue(const string queue_type, const string show_id, int frame_number, queue<stack<Packet*>>& print_queue, int num_of_hops);
    void show_queue(const vector<Client> clients, const string line);
    vector<Client> clients;
};

#endif  // NETWORK_H
