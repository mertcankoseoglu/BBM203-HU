#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    deleteTree(root);
}

void SpaceSectorBST::deleteTree(Sector* root_of_sub){
    if(root_of_sub != nullptr){
        deleteTree(root_of_sub->left);
        deleteTree(root_of_sub->right);
        delete root_of_sub;
    }   
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
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

void SpaceSectorBST::recursiveInsert(int x, int y, int z, Sector* root_of_sub, Sector* sector_new){
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
    else {
        if(root_of_sub->left == nullptr){
            root_of_sub->left = sector_new;
            sector_new->parent = root_of_sub;
        }
        else{
            recursiveInsert(x, y, z, root_of_sub->left, sector_new);
        }
    }
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
    Sector* sector_new = new Sector(x, y, z);

    // if the BST is empty, new sector is root
    if(root == nullptr){
        root = sector_new;
    }
    else{
        recursiveInsert(x, y, z, root, sector_new);
    }
}

void SpaceSectorBST::findSector(Sector* current, const std::string& sector_code, Sector*& last_sector){
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

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    if(root == nullptr){
        return;
    }
    Sector *deleted_sec = nullptr;
    findSector(root, sector_code, deleted_sec);

    if(deleted_sec != nullptr){
        // if deleted sector is leaf, no child
        if(deleted_sec->left == nullptr && deleted_sec->right == nullptr){
            if (deleted_sec->parent != nullptr) {
                if (deleted_sec->parent->left == deleted_sec) {
                    deleted_sec->parent->left = nullptr;
                } else {
                    deleted_sec->parent->right = nullptr;
                }
            } 
            else {
                // Deleting root 
                root = nullptr;
            }
            
        }
        // if deleted sector has only right subtree
        else if(deleted_sec->left == nullptr){
            if (deleted_sec->parent != nullptr) {
                if (deleted_sec->parent->left == deleted_sec) {
                    deleted_sec->parent->left = deleted_sec->right;
                } else {
                    deleted_sec->parent->right = deleted_sec->right;
                }
                deleted_sec->right->parent = deleted_sec->parent;
            } else {
                // Deleting root node with only right subtree
                root = deleted_sec->right;
                root->parent = nullptr;
            }
            
        }
        // if deleted sector has only left subtree
        else if(deleted_sec->right == nullptr){
            if (deleted_sec->parent != nullptr) {
                if (deleted_sec->parent->left == deleted_sec) {
                    deleted_sec->parent->left = deleted_sec->left;
                } else {
                    deleted_sec->parent->right = deleted_sec->left;
                }
                deleted_sec->left->parent = deleted_sec->parent;
            } else {
                // Deleting root node with only left subtree
                root = deleted_sec->left;
                root->parent = nullptr;
            }
            
        }
        // if deleted sector has two children
        else{
            // find the min in right subtree of deleted sector
            Sector* in_order_suc = deleted_sec->right;
            while(in_order_suc->left != nullptr){
                in_order_suc = in_order_suc->left;
            }
            string temp_code = in_order_suc->sector_code;
            double temp_dist = in_order_suc->distance_from_earth;
            int temp_x = in_order_suc->x;
            int temp_y = in_order_suc->y;
            int temp_z = in_order_suc->z;

            in_order_suc->sector_code = deleted_sec->sector_code;
            in_order_suc->distance_from_earth = deleted_sec->distance_from_earth;
            in_order_suc->x = deleted_sec->x;
            in_order_suc->y = deleted_sec->y;
            in_order_suc->z = deleted_sec->z;
            
            deleted_sec->sector_code = temp_code;
            deleted_sec->distance_from_earth = temp_dist;
            deleted_sec->x = temp_x;
            deleted_sec->y = temp_y;
            deleted_sec->z = temp_z;
            deleteSector(in_order_suc->sector_code);    
        }        
    }
}

void SpaceSectorBST::traverseInorder(Sector* sector_prt){
    if(sector_prt != nullptr){
        traverseInorder(sector_prt->left);
        cout << sector_prt->sector_code << endl;
        traverseInorder(sector_prt->right);
    }   
}

void SpaceSectorBST::traversePreorder(Sector* sector_prt){
    if(sector_prt != nullptr){
        cout << sector_prt->sector_code << endl;
        traversePreorder(sector_prt->left);
        traversePreorder(sector_prt->right);
    }   
}

void SpaceSectorBST::traversePostorder(Sector* sector_prt){
    if(sector_prt != nullptr){
        traversePostorder(sector_prt->left);
        traversePostorder(sector_prt->right);
        cout << sector_prt->sector_code << endl;
    }   
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    traverseInorder(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    traversePreorder(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    traversePostorder(root);
    cout << endl;
}

void SpaceSectorBST::reverseVector(std::vector<Sector*> &path) {
    int size_path = path.size();
    for (int i = 0; i < size_path / 2; ++i) {
        std::swap(path[i], path[size_path - i - 1]);
    }
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    if(root == nullptr){
        return path;
    }

    Sector* destination_sec = nullptr;
    findSector(root, sector_code, destination_sec);
    
    if(destination_sec != nullptr){
        Sector* current_sector = destination_sec;

        while (current_sector != nullptr) {
            path.push_back(current_sector);
            current_sector = current_sector->parent;
        }
        //delete current_sector;
        destination_sec = nullptr;
    }      
    reverseVector(path);  
    //delete destination_sec;
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
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