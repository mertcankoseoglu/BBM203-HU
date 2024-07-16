#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
    ifstream block_file(filename);
    if(!block_file.is_open()){
        cerr << "Cannot open file!" << endl;
        return;
    }
    string line;
    // first line is X,Y,Z
    getline(block_file, line);

    while(getline(block_file, line)){
        istringstream iss(line);
        int x, y, z;
        char comma;
        iss >> x >> comma >> y >> comma >> z;

        insertSectorByCoordinates(x, y, z);
    }
}

Sector* SpaceSectorLLRBT::rotateRight(Sector*& p_sector){   
    Sector* x = p_sector->left;
    
    p_sector->left = x->right;
    if(x->right){
        x->right->parent = p_sector;
    }
    x->right = p_sector;
    x->parent = p_sector->parent;
    p_sector->parent = x;   
    x->color = p_sector->color;
    x->right->color = true;

    if(p_sector == root){
        root = x;
    }   
    return x;
}

Sector* SpaceSectorLLRBT::rotateLeft(Sector*& p_sector){   
    Sector* x = p_sector->right;
   
    p_sector->right = x->left;
    if(x->left){
        x->left->parent = p_sector;
    }
    x->parent = p_sector->parent;
    p_sector->parent = x;
    x->left = p_sector;
    x->color = p_sector->color;
    x->left->color = true;

    if(p_sector == root){
        root = x;
    }
    return x;
}

void SpaceSectorLLRBT::flipColors(Sector*& p_sector){   
    if(p_sector != root){
        p_sector->color = true;
    }
    else{
        p_sector->color = false;
    }       
    p_sector->left->color = false;
    p_sector->right->color = false;   
}

void SpaceSectorLLRBT::fixViolations(Sector*& root_of_sub){  
    if((root_of_sub->right != nullptr && root_of_sub->right->color) && (root_of_sub->left == nullptr || !root_of_sub->left->color)){
        root_of_sub = rotateLeft(root_of_sub);               
    }
    if(root_of_sub->left != nullptr && root_of_sub->left->left != nullptr && root_of_sub->left->color && root_of_sub->left->left->color){
        root_of_sub = rotateRight(root_of_sub);        
    }
    if(root_of_sub->right != nullptr && root_of_sub->left != nullptr && root_of_sub->left->color && root_of_sub->right->color){
        flipColors(root_of_sub);
    }
    if(root_of_sub->left != nullptr){
        fixViolations(root_of_sub->left);
    }
    if(root_of_sub->right != nullptr){
        fixViolations(root_of_sub->right);
    }
}

void SpaceSectorLLRBT::recursiveInsert(int x, int y, int z, Sector* root_of_sub, Sector* sector_new){
    //if new is greater, check right
    if(x > root_of_sub->x || (x == root_of_sub->x && y > root_of_sub->y) || (x == root_of_sub->x && y == root_of_sub->y && z > root_of_sub->z)){
        if(root_of_sub->right == nullptr){
            root_of_sub->right = sector_new;
            sector_new->parent = root_of_sub; 
        }
        else{
            recursiveInsert(x, y, z, root_of_sub->right, sector_new);
        }
    }
    //if new is smaller, check left
    else{
        if(root_of_sub->left == nullptr){
            root_of_sub->left = sector_new;
            sector_new->parent = root_of_sub;
        }
        else{
            recursiveInsert(x, y, z, root_of_sub->left, sector_new);
        }
    }
    fixViolations(root);
    root->color = false;
}

void SpaceSectorLLRBT::deleteTree(Sector* root_of_sub){
    if(root_of_sub != nullptr){
        deleteTree(root_of_sub->left);
        deleteTree(root_of_sub->right);
        delete root_of_sub;
    }   
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    deleteTree(root);
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
    Sector* sector_new = new Sector(x, y, z);

    if(root == nullptr){
        root = sector_new;
        root->color = false;
    }
    else{
        sector_new->color = true;
        recursiveInsert(x, y, z, root, sector_new);
    }
    
}

void SpaceSectorLLRBT::traverseInorder(Sector* sector_prt){
    if(sector_prt != nullptr){
        traverseInorder(sector_prt->left);
        if(sector_prt->color){
            cout << "RED sector: ";
        }
        else{
            cout << "BLACK sector: ";
        }
        cout << sector_prt->sector_code << endl;
        traverseInorder(sector_prt->right);
    }   
}

void SpaceSectorLLRBT::traversePreorder(Sector* sector_prt){
    if(sector_prt != nullptr){
        if(sector_prt->color){
            cout << "RED sector: ";
        }
        else{
            cout << "BLACK sector: ";
        }
        cout << sector_prt->sector_code << endl;
        traversePreorder(sector_prt->left);
        traversePreorder(sector_prt->right);
    }   
}

void SpaceSectorLLRBT::traversePostorder(Sector* sector_prt){
    if(sector_prt != nullptr){
        traversePostorder(sector_prt->left);
        traversePostorder(sector_prt->right);
        if(sector_prt->color){
            cout << "RED sector: ";
        }
        else{
            cout << "BLACK sector: ";
        }
        cout << sector_prt->sector_code << endl;
    }   
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    traverseInorder(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    traversePreorder(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    traversePostorder(root);
    cout << endl;
}

void SpaceSectorLLRBT::findSector(Sector* current, const std::string& sector_code, Sector*& last_sector){
    if(current == nullptr){
        return;
    }
    if(current->sector_code == sector_code){
        last_sector = current;
        return;
    }
    if(current->right != nullptr){
        findSector(current->right, sector_code, last_sector); 
    }
    if(current->left != nullptr){
        findSector(current->left, sector_code, last_sector); 
    }      
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    if(root == nullptr){
        return path;
    }

    Sector* destination_sec = nullptr;
    Sector* earth_sec = nullptr;
    findSector(root, sector_code, destination_sec);
    findSector(root, "0SSS", earth_sec);

    vector<Sector*> part_earth;
    vector<Sector*> part_dest;

    if(earth_sec != nullptr){
        Sector* current_sector = earth_sec;

        while (current_sector != nullptr) {
            part_earth.push_back(current_sector);
            current_sector = current_sector->parent;
        }
        earth_sec = nullptr;
    }
    if(destination_sec != nullptr){
        Sector* current_sector = destination_sec;

        while (current_sector != nullptr) {
            part_dest.push_back(current_sector);
            current_sector = current_sector->parent;
        }
        
        destination_sec = nullptr;
    }
   
    for(int i = 0; i < part_earth.size(); i++){
        for(int j = 0; j < part_dest.size(); j++){
            if(part_earth[i]->sector_code == part_dest[j]->sector_code){
                for(int k = 0; k <= i; k++){
                    path.push_back(part_earth[k]);
                }
                while (j > 0)
                {
                    path.push_back(part_dest[j-1]);
                    j--;
                }               
                return path;
            }
        }
    }
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
    if(path.empty()){
        cout << "A path to Dr. Elara could not be found." << endl;
        return;
    }

    cout << "The stellar path to Dr. Elara: ";
    for(int i = 0; i < path.size() - 1; i++){
        cout << path[i]->sector_code << "->";
    }
    cout << path[path.size() - 1]->sector_code << endl;
}