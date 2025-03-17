#include "./pdb_handler.hpp"
#include <cstring>
#include <iostream>
#include <vector>

bool contains(std::string large, std::string sub) { return large.find(sub) != std::string::npos; }

void setAtom(ATOM *atom, std::string record) {
  atom->serialNumber = std::stoi(record.substr(6, 5));           // set atom serial number
  std::memcpy(atom->atomName, record.substr(12, 4).c_str(), 4); // set atom name
  std::memcpy(atom->resName, record.substr(17, 3).c_str(), 3);  // set residue name
  atom->chainID = record.at(21);                                 // set chain ID
  atom->resSeq = std::stoi(record.substr(22, 4));                // set residue serial number
  atom->x = std::stod(record.substr(30, 8));                     // set x coordinate
  atom->y = std::stod(record.substr(38, 8));                     // set y coordinate
  atom->z = std::stod(record.substr(46, 8));                     // set z coordinate
  atom->occupancy = std::stod(record.substr(46, 8));             // set occupancy number
  std::memcpy(atom->element, record.substr(76, 2).c_str(), 2);  // set element name
  atom->charge = 0;                                              // for now it is set to zero;
}

void PDBFile::openPDB(std::string fileName) {
  file.open(fileName);
  ATOM helperAtom;
  atomCount = 0, resCount = 0;
  int currRes = -1, prevRes = -1;
  std::string buffer;
  file.seekg(0);
  while (getline(file, buffer)) {
    if (contains(buffer, "ATOM") || contains(buffer, "HETATM")) {
      atomCount++;
      setAtom(&helperAtom, buffer);
      currRes = helperAtom.resSeq;
      if (currRes != prevRes) {
        prevRes = currRes;
        resCount++;
      }
    }
  }
#ifdef PRINT
  std::cout << "Atom count: " << atomCount << '\n';
  std::cout << "Residue count: " << resCount << std::endl;
#endif
}

void PDBFile::parsePDB() {
  atoms = std::vector<ATOM *>(atomCount);
  residues = std::vector<RESIDUE *>(resCount);
  int posCounter = 0, resPosCounter = -1;
  int currRes = -1, prevRes = -1;
  file.clear();
  file.seekg(0);
  std::string buffer;
  while (getline(file, buffer)) {
    if (contains(buffer, "ATOM") || contains(buffer, "HETATM")) {
      atoms.at(posCounter) = new ATOM;
      setAtom(atoms.at(posCounter), buffer);
      currRes = atoms.at(posCounter)->resSeq;
      if (currRes != prevRes) {
        prevRes = currRes;
        resPosCounter++;
        residues.at(resPosCounter) = new RESIDUE;
        residues.at(resPosCounter)->atomIndex = std::vector<int>();
        residues.at(resPosCounter)->resSeq = atoms.at(posCounter)->resSeq;
        residues.at(resPosCounter)->chainID = atoms.at(posCounter)->chainID;
        std::memcpy(residues.at(resPosCounter)->resname,atoms.at(posCounter)->resName,3);
      }
      residues.at(resPosCounter)->atomIndex.push_back(posCounter);
      posCounter++;
    }
  }
#ifdef PRINT
  std::cout << posCounter << " atoms parsed." << std::endl;
#endif
}

PDBFile::PDBFile(std::string fileName) {
  this->openPDB(fileName);
  this->parsePDB();
}

//int main(int argc, char *argv[]) { PDBFile pdb(argv[1]); }
