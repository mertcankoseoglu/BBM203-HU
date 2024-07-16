    #include "ApplicationLayerPacket.h"

    ApplicationLayerPacket::ApplicationLayerPacket(int _layer_ID, const string &_sender_ID, const string &_receiver_ID, const string& _message_data) : Packet(_layer_ID) {
        sender_ID = _sender_ID;
        receiver_ID = _receiver_ID;
        message_data = _message_data;
    }

    void ApplicationLayerPacket::print() {
        // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.
        
        cout << "Sender ID: " << sender_ID << ", Receiver ID: " << receiver_ID << endl;
        cout << "Message chunk carried: \"" << message_data << "\"" << endl;
    }

    void ApplicationLayerPacket::show_frame(){
        int layer_num = 0;
        cout << "Carried Message: \"" << message_data << "\"" << endl;
        cout << "Layer " << layer_num << " info: Sender ID: " << sender_ID << ", Receiver ID: " << receiver_ID << endl;
    }

    ApplicationLayerPacket::~ApplicationLayerPacket() {
        // TODO: Free any dynamically allocated memory if necessary.
    }
