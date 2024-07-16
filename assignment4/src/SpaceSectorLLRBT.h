#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void deleteTree(Sector* root_of_sub);
    void readSectorsFromFile(const std::string& filename);
    void fixViolations(Sector*& root_of_sub);
    Sector* rotateRight(Sector*& root_of_sub);
    Sector* rotateLeft(Sector*& root_of_sub);
    void flipColors(Sector*& root_of_sub);
    void recursiveInsert(int x, int y, int z, Sector* root_of_sub, Sector* sector_new);
    void insertSectorByCoordinates(int x, int y, int z);
    void traverseInorder(Sector* sector_prt);
    void traversePreorder(Sector* sector_prt);
    void traversePostorder(Sector* sector_prt);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    void findSector(Sector* current, const std::string& sector_code, Sector*& last_sector);
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
};

#endif // SPACESECTORLLRBT_H
