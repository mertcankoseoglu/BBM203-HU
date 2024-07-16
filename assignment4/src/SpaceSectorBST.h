#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <string>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    
    void findSector(Sector* current, const std::string& sector_code, Sector*& last_sector);
    void readSectorsFromFile(const std::string& filename); 
    void deleteTree(Sector* root_of_sub);
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    void reverseVector(std::vector<Sector*> &path);
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    void recursiveInsert(int x, int y, int z, Sector* root_of_sub, Sector* sector_new);
    void traverseInorder(Sector* sector_prt);
    void traversePreorder(Sector* sector_prt);
    void traversePostorder(Sector* sector_prt);
};

#endif // SPACESECTORBST_H
