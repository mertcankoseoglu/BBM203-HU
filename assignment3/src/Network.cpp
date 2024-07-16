#include "Network.h"
#include <queue>
#include <ctime>

Network::Network() {

}

void Network::message(vector<Client>& clients, string line, int message_limit, const string &sender_port, const string &receiver_port){

    string send_id = line.substr(8,1);
    string last_id = line.substr(10,1);
    string last_ip;

    for(auto& client : clients){
        if(client.client_id == send_id){

            // split message according to message_limit
            int start_msg = line.find("#");
            int end_msg = line.find("#", start_msg + 1);
            string full_msg = line.substr(start_msg + 1, end_msg - start_msg - 1);

            // number of stacks in outgoing queue
            int frame_num = (full_msg.length() + message_limit - 1) /message_limit;
            
            // holds the time as string
            time_t current_time = time(nullptr);
            std::tm* time_info = std::localtime(&current_time);
            char buffer[20];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);           
            std::string time_stamp(buffer);

            // create log for message
            Log msg_log(time_stamp, full_msg, frame_num, 0, client.client_id, last_id, true, ActivityType::MESSAGE_SENT);
            msg_log.prev_id = client.client_id;

            // find the next client to link
            for(const auto& entry : client.routing_table){
                if(entry.first == last_id){
                    msg_log.next_id = entry.second;
                    break;
                }
            }
            for(const auto& other_client : clients){
                // find the mac address of next client               
                if(other_client.client_id == msg_log.next_id){
                    msg_log.next_mac = other_client.client_mac;
                }
                
                // find the ip of receiver
                if(other_client.client_id == last_id){
                    last_ip = other_client.client_ip;
                }
            }

            client.log_entries.emplace_back(msg_log);
            // temp queue to print
            queue<stack<Packet*>> temp_queue;

            cout << "Message to be sent: \"" << full_msg << "\"\n\n";
            // create stacks for each message part
            for (int i = 0; i < frame_num; i++) {
                // message part
                cout << "Frame #" << i+1 << endl;
                string frame_msg = full_msg.substr(i * message_limit, message_limit);

                stack<Packet*> frame_new_stack;
                Packet* application_layer = new ApplicationLayerPacket(0, send_id, last_id, frame_msg);
                Packet* transport_layer = new TransportLayerPacket(1, sender_port, receiver_port);
                Packet* network_layer = new NetworkLayerPacket(2, client.client_ip, last_ip);
                Packet* physical_layer = new PhysicalLayerPacket(3,client.client_mac, msg_log.next_mac);

                frame_new_stack.push(application_layer);
                frame_new_stack.push(transport_layer);
                frame_new_stack.push(network_layer);
                frame_new_stack.push(physical_layer);

                physical_layer->print();                
                network_layer->print();
                transport_layer->print();
                application_layer->print();
                cout << "Number of hops so far: " << 0 << endl;
                cout << "--------\n";
                client.outgoing_queue.push(frame_new_stack);
                temp_queue.push(frame_new_stack);                   
            }  
            
              
        }
    } 
}

void Network::print_command(const string line){
    int dash_length = line.length() + 9; 
    std::cout << std::string(dash_length, '-') << "\n";
    std::cout << "Command: " << line << "\n";
    std::cout << std::string(dash_length, '-') << "\n";
}

void Network::send(vector<Client>& clients) {
    for (auto& sender : clients) {
        if (!sender.outgoing_queue.empty()) {
            for(auto& msg_log : sender.log_entries){   
                // find the next link to send
                for (auto& next_link : clients) {
                    if(msg_log.next_id == next_link.client_id){ 
                        if(msg_log.end_of_log){
                            continue;
                        }
                        if(!msg_log.in_outging){
                            continue;
                        }

                        //print send entry
                        queue<stack<Packet*>> temp_queue = sender.outgoing_queue;
                        int num  = 1;
                        while (num <= msg_log.number_of_frames)
                        {                        
                            stack<Packet*> crnt_stack = temp_queue.front();
                            cout << "Client " << sender.client_id << " sending frame #" << num << " to client " << next_link.client_id << endl;
                            cout << "Sender MAC address: " << sender.client_mac << ", Receiver MAC address: " << next_link.client_mac << endl;

                            crnt_stack.pop();
                            while (!crnt_stack.empty())
                            {           
                                Packet* current_packet = crnt_stack.top();
                                current_packet->print();
                                crnt_stack.pop();
                            }
                            temp_queue.pop();
                            cout << "Number of hops so far: " << msg_log.number_of_hops + 1 << endl;
                            cout << "--------\n";
                            num++;                       
                        }
                        while(!temp_queue.empty()){
                            temp_queue.pop();
                        }

                        // holds the time as string
                        string time_stamp = msg_log.timestamp;
                        string move_msg = msg_log.message_content;
                        int num_of_f = msg_log.number_of_frames;
                        int num_of_h = msg_log.number_of_hops + 1;
                        string s_id = msg_log.sender_id;
                        string r_id = msg_log.receiver_id;

                        // this is the last link message received
                        if(msg_log.next_id == msg_log.receiver_id){                           
                            // create new log to last client
                            Log log_new(time_stamp, move_msg, num_of_f, num_of_h, s_id, r_id, true, ActivityType::MESSAGE_RECEIVED);
                            log_new.prev_id = sender.client_id;
                            log_new.in_outging = false;
                            msg_log.end_of_log = true;
                            next_link.log_entries.emplace_back(log_new);
    
                            // delete the message in outcoming queue add it into incoming queue
                            int len_msg = 1;
                            while (len_msg <= msg_log.number_of_frames ) {
                                stack<Packet*> temp_stack = sender.outgoing_queue.front();
                                next_link.incoming_queue.push(temp_stack);
                                sender.outgoing_queue.pop();    
                                len_msg++;                        
                            }
                        }
                        // not last forwarded or drop
                        else{
                            // check the next next id 
                            string check_id;
                            for (const auto& entry : next_link.routing_table) {
                                if (entry.first == msg_log.receiver_id) {
                                    check_id = entry.second;
                                    break;
                                }
                            }
                            // bool for drop or forward
                            bool temp_bool = true;
                            for (auto& next_next : clients){
                                if(check_id == next_next.client_id){
                                    temp_bool = true;
                                    break;
                                }
                                else{
                                    temp_bool = false;
                                }
                            }
                            // drop
                            if(!temp_bool){
                                Log log_new(time_stamp, move_msg, num_of_f, num_of_h, s_id, r_id, false, ActivityType::MESSAGE_DROPPED);
                                log_new.prev_id = sender.client_id;
                                log_new.in_outging = false;
                                msg_log.end_of_log = true;
                                next_link.log_entries.emplace_back(log_new);

                                // delete the message in outcoming queue add it into incoming queue
                                int len_msg = 1;
                                while (len_msg <= msg_log.number_of_frames ) {
                                    stack<Packet*> temp_stack = sender.outgoing_queue.front();
                                    next_link.incoming_queue.push(temp_stack);
                                    sender.outgoing_queue.pop();    
                                    len_msg++;                        
                                }
                            }
                            //forwaded
                            else{
                                Log log_new(time_stamp, move_msg, num_of_f, num_of_h, s_id, r_id, true, ActivityType::MESSAGE_FORWARDED);
                                log_new.prev_id = sender.client_id;
                                log_new.in_outging = false;
                                msg_log.end_of_log = true;
                                for (const auto& entry : next_link.routing_table) {
                                    if (entry.first == msg_log.receiver_id) {
                                        log_new.next_id = entry.second;
                                        break;
                                    }
                                }
                                for (const auto& after_next : clients) {
                                    if (after_next.client_id == log_new.next_id) {
                                        log_new.next_mac = after_next.client_mac;
                                        break;
                                    }
                                }
                                next_link.log_entries.emplace_back(log_new);                               

                                // delete the message in outcoming queue add it into incoming queue
                                int len_msg = 1;
                                while (len_msg <= msg_log.number_of_frames ) {
                                    stack<Packet*> temp_stack = sender.outgoing_queue.front();
                                    temp_stack.pop();
                                    Packet* phsc_new_layer = new PhysicalLayerPacket(3, next_link.client_id, log_new.next_mac);
                                    temp_stack.push(phsc_new_layer);
                                    next_link.incoming_queue.push(temp_stack);
                                    sender.outgoing_queue.pop();    
                                    len_msg++;                        
                                }
                            }
                        }
                    }    
                }
            }
        }
    }
}

void Network::show_queue(const vector<Client> clients, const string line){
    string show_id = line.substr(12,1);   // id of client from command
    string type_queue = line.substr(14,1);// i or o queue type

    for(const auto& client : clients){
        // find the client according to its id
        if(client.client_id == show_id){
            // i = incoming queue
            if(type_queue == "i"){
                cout << "Client " << client.client_id << " Incoming Queue Status\n";
                if(client.incoming_queue.empty()){
                    cout << "Current total number of frames: 0\n"; 
                }
                else{
                    cout << "Current total number of frames: " << client.incoming_queue.size() << endl;
                }
            }
            // o = outgoing queue
            if(type_queue == "o"){
                cout << "Client " << client.client_id << " Outgoing Queue Status\n";
                if(client.outgoing_queue.empty()){
                    cout << "Current total number of frames: 0\n"; 
                }
                else{
                    cout << "Current total number of frames: " << client.outgoing_queue.size() << endl;
                }
            }
        }
    }                       
}

void Network::show_frame_info(const vector<Client>& clients, const string& line) {
    string show_id_f = line.substr(16, 1);  // client id
    string queue_type_f = line.substr(18, 1);  // i or o queue type

    // find the wanted frame
    for (const auto& client : clients) {
        // find the client according to id
        if (client.client_id == show_id_f) {
            // incoming queue
            if (queue_type_f == "i") {
                int number_frame_incoming = 0;
                queue<stack<Packet*>> sum_queue = client.incoming_queue;
                while(!sum_queue.empty()){
                    number_frame_incoming++;
                    sum_queue.pop();
                }
                int end_position = line.find(' ', 21);
                string number_str = line.substr(21, end_position - 21);
                int frame_number = stoi(number_str);
                if(frame_number > number_frame_incoming){
                    cout << "No such frame." << endl;
                    return;
                }
                int temp_f_num = frame_number;
                int num_of_hop = 0;
                for(auto& enty_of_frame : client.log_entries){
                    if(temp_f_num <= enty_of_frame.number_of_frames){
                        num_of_hop = enty_of_frame.number_of_hops;
                        break;
                    }
                    temp_f_num -= enty_of_frame.number_of_frames;
                }
                
                queue<stack<Packet*>> print_queue = client.incoming_queue;
                print_frame_queue("i", show_id_f, frame_number, print_queue, num_of_hop);
            }

            // outcoming queue
            if (queue_type_f == "o") {
                int number_frame_outgoing = 0;
                queue<stack<Packet*>> sum_queue = client.outgoing_queue;
                while(!sum_queue.empty()){
                    number_frame_outgoing++;
                    sum_queue.pop();
                }
                int end_position = line.find(' ', 22);
                string number_str = line.substr(22, end_position - 22);
                int frame_number = stoi(number_str);
                if(frame_number > number_frame_outgoing){
                    cout << "No such frame." << endl;
                    return;
                }
                int temp_f_num = frame_number;
                int num_of_hop = 0;
                for(auto& enty_of_frame : client.log_entries){
                    if(temp_f_num <= enty_of_frame.number_of_frames){
                        num_of_hop = enty_of_frame.number_of_hops;
                        break;
                    }
                    temp_f_num -= enty_of_frame.number_of_frames;
                }
                queue<stack<Packet*>> print_queue = client.outgoing_queue;
                print_frame_queue("o", show_id_f, frame_number, print_queue, num_of_hop);    
            }
        }
    }
}

void Network::print_frame_queue(const string queue_type, const string show_id,
 int frame_number, queue<stack<Packet*>>& print_queue, int num_of_hops) {
    
    int temp_num = 1;
    while (!print_queue.empty()) {
        // find the wanted frame
        if (temp_num == frame_number) {
            cout << "Current Frame #" << frame_number << " on the " << 
            (queue_type == "i" ? "incoming" : "outgoing") << " queue of client " << show_id << endl;

            // create temp stack
            stack<Packet*> print_stack = print_queue.front();
            // reserve the stack
            stack<Packet*> reserved_stack;
            while (!print_stack.empty()) {
                Packet* print_packet = print_stack.top();
                reserved_stack.push(print_packet);
                print_stack.pop();
            }
            // print the reserved stack
            while (!reserved_stack.empty()) {
                Packet* reserved_packet = reserved_stack.top();
                reserved_packet->show_frame();
                reserved_stack.pop();
            }
            cout << "Number of hops so far: " << num_of_hops << endl;
            break;
        } 
        else {
            // if it is not found, pop the queue
            print_queue.pop();
            temp_num++;
        }
    }
}

void Network::receive(vector<Client>& clients){
    for(auto& received_client : clients){
        if(!received_client.incoming_queue.empty()){
            for(auto& received_log : received_client.log_entries){
                if(received_log.end_of_log){
                    continue;
                }
                if(received_log.in_outging){
                    received_log.end_of_log = true;
                    continue;
                }
                // received
                if(received_log.receiver_id == received_client.client_id){
                    int num  = 1;
                    while (num <= received_log.number_of_frames)
                    {                        
                        stack<Packet*> current_stack = received_client.incoming_queue.front();
                        cout << "Client " << received_client.client_id << " receiving frame #" <<
                        num << " from client " << received_log.prev_id << ", originating from client " << received_log.sender_id << endl;

                        current_stack.pop();
                        string prev_mac;
                        for(const auto& previous : clients){
                            if (previous.client_id == received_log.prev_id){
                                prev_mac = previous.client_mac;
                            }
                        }
                        cout << "Sender MAC address: " << prev_mac << ", Receiver MAC address: " << received_client.client_mac << endl;
                        while (!current_stack.empty())
                        {           
                            Packet* current_packet = current_stack.top();
                            current_packet->print();
                            current_stack.pop();

                        }
                        received_client.incoming_queue.pop();
                        cout << "Number of hops so far: " << received_log.number_of_hops << endl;
                        cout << "--------\n";
                        num++;                       
                    }
                    cout << "Client " << received_client.client_id << " received the message " << "\""
                     << received_log.message_content << "\"" << " from client " << received_log.sender_id << ".\n";
                    cout << "--------\n";

                    received_log.end_of_log = true;
                    received_log.in_outging = true;

                }
                // drop
                else if(!received_log.success_status){
                    int num  = 1;
                    while (num <= received_log.number_of_frames)
                    {                                               
                        received_client.incoming_queue.pop();                      
                        num++;                       
                    }
                    //print drop situation
                    for(int i = 1; i <= received_log.number_of_frames; i++){
                        cout << "Client " << received_client.client_id << " receiving frame #" << i << " from client " <<
                         received_log.sender_id << ", but intended for client " << received_log.receiver_id << ". Forwarding...\n";

                        cout << "Error: Unreachable destination. Packets are dropped after " << received_log.number_of_hops << " hops!\n";
                    }
                    cout << "--------\n";

                    received_log.in_outging = true;
                    received_log.end_of_log = true;
                }
                //forward
                else{
                    cout << "Client " << received_client.client_id << " receiving a message from client " <<
                     received_log.prev_id << ", but intended for client " << received_log.receiver_id << ". Forwarding...\n";

                    int temp_frame = 1;
                    while (temp_frame <= received_log.number_of_frames) {                   
                        stack<Packet*> temp_incoming = received_client.incoming_queue.front();
                        cout << "Frame #" << temp_frame << " MAC address change: New sender MAC " <<
                        received_client.client_mac << ", new receiver MAC " << received_log.next_mac << endl;

                        temp_frame++;
                        received_client.outgoing_queue.push(temp_incoming);
                        received_client.incoming_queue.pop();
                    }
                    cout << "--------\n";

                    received_log.in_outging = true;
                }
            }
        }
    }
}

void Network::logs_print(vector<Client>& clients, string line){
    string print_id = line.substr(10,1);
    for (auto& client_print : clients){
        if(client_print.client_id == print_id){
            if(client_print.log_entries.empty()){
                break;
            }
            cout << "Client " <<  print_id << " Logs:" << endl;
            cout << "--------------\n";

            int entry_number = 1;
            for(auto& log_print : client_print.log_entries){
                // convert to string from enum class
                ActivityType activity = log_print.activity_type;
                string str_activity;
                switch (activity)
                {
                case ActivityType::MESSAGE_SENT:
                    str_activity = "Message Sent";
                    break;
                case ActivityType::MESSAGE_DROPPED:
                    str_activity = "Message Dropped";
                    break;
                case ActivityType::MESSAGE_FORWARDED:
                    str_activity = "Message Forwarded";
                    break;
                case ActivityType::MESSAGE_RECEIVED:
                    str_activity = "Message Received";
                    break;
                default:
                    break;
                }
                // print log
                cout << "Log Entry #" << entry_number <<":\n";
                cout << "Activity: " << str_activity << endl;
                cout << "Timestamp: " << log_print.timestamp << endl;
                cout << "Number of frames: " << log_print.number_of_frames << endl;
                cout << "Number of hops: " << log_print.number_of_hops << endl;
                cout << "Sender ID: " << log_print.sender_id << endl;
                cout << "Receiver ID: " << log_print.receiver_id << endl;
                cout << "Success: " << (log_print.success_status ? "Yes" : "No") << endl;
                if(str_activity == "Message Sent" || str_activity == "Message Received"){
                    cout << "Message: \"" << log_print.message_content << "\"" << endl;
                }
                if (&log_print != &client_print.log_entries.back()) {
                    cout << "--------------\n";
                }            
                entry_number++;
            }
        }
    }
}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */

    for (const auto& line : commands) {
        print_command(line);
        // message command
        if(line.substr(0,7) == "MESSAGE"){           
            message(clients, line, message_limit, sender_port, receiver_port);          
        }
        // send command
        else if(line == "SEND"){
            send(clients);
        }
        // show queue command
        else if(line.substr(0,11) == "SHOW_Q_INFO"){
            show_queue(clients, line);
        }
        // show frame command
        else if(line.substr(0,15) == "SHOW_FRAME_INFO"){
            show_frame_info(clients, line);
        }
        // receive command
        else if(line == "RECEIVE"){
            receive(clients);
        }
        // print log command
        else if(line.substr(0,9) == "PRINT_LOG"){
            logs_print(clients, line);
        }
        // invalid command
        else{
            cout << "Invalid command.\n";
        }
    } 
}

vector<Client> Network::read_clients(const string &filename) {
    // TODO: Read clients from the given input file and return a vector of Client instances.

    ifstream client_file(filename);
    if(!client_file.is_open()){
        std::cerr << "Cannot open file!" << std::endl;
    }

    string line;
    while (getline(client_file, line)) {
        // Use a stringstream to parse the line
        istringstream iss(line);

        // Read id, ip, mac
        string id, ip, mac;
        if (iss >> id >> ip >> mac) {
            // Create a Client object and add it to the 2D vector
            Client client_new(id, ip, mac);
            clients.emplace_back(client_new);
        }
    }

    client_file.close();
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.

    ifstream routing_file(filename);
    if(!routing_file.is_open()){
        std::cerr << "Cannot open file!" << std::endl;
    }

    string line;
    //it is used to increment the clients vector 
    int num_client = 0;

    while (getline(routing_file, line)) {
        // Use a stringstream to get the line
        istringstream iss(line);
        string receiver_id, next_hop_id;

        if (iss >> receiver_id >> next_hop_id) {
            // receiver id is the first, next hop id is the second in map
            clients[num_client].routing_table[receiver_id] = next_hop_id;
        }
        else {
            // when the input line is " - ", increment the num in order to move to the next client
            num_client++;
        }
    }

    routing_file.close();
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.

    ifstream command_file(filename);
    if(!command_file.is_open()){
        std::cerr << "Cannot open file!" << std::endl;
    }

    string first_line;
    getline(command_file, first_line);

    string line;
    while (getline(command_file, line)){
        commands.emplace_back(line);
    }
    
    command_file.close();
    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
    // Free dynamically allocated memory for Packet objects in outgoing queues
    for (auto& client : clients) {
        while (!client.outgoing_queue.empty()) {
            stack<Packet*> frame_stack = client.outgoing_queue.front();
            while (!frame_stack.empty()) {
                delete frame_stack.top();// Deallocate memory
                frame_stack.pop();
            }
            client.outgoing_queue.pop();
        }
        while (!client.incoming_queue.empty()) {
            stack<Packet*> frame_stack = client.outgoing_queue.front();
            while (!frame_stack.empty()) {
                delete frame_stack.top(); // Deallocate memory
                frame_stack.pop();
            }
            client.incoming_queue.pop();
        }
    }
}