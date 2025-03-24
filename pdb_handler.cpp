#include "./pdb_handler.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

bool contains(std::string large, std::string sub) {
  return large.find(sub) != std::string::npos;
}

void setAtom(ATOM *atom, std::string record) {
  atom->serialNumber = std::stoi(record.substr(6, 5)); // set atom serial number
  std::memcpy(atom->atomName, record.substr(12, 4).c_str(), 4); // set atom name
  std::memcpy(atom->resName, record.substr(17, 3).c_str(),
              3);                                 // set residue name
  atom->chainID = record.at(21);                  // set chain ID
  atom->resSeq = std::stoi(record.substr(22, 4)); // set residue serial number
  atom->x = std::stod(record.substr(30, 8));      // set x coordinate
  atom->y = std::stod(record.substr(38, 8));      // set y coordinate
  atom->z = std::stod(record.substr(46, 8));      // set z coordinate
  atom->occupancy = std::stod(record.substr(46, 8)); // set occupancy number
  std::memcpy(atom->element, record.substr(76, 2).c_str(),
              2);            // set element name
  atom->charge = 0;          // for now it is set to zero;
  atom->region = REGION::MM; // by default atoms are in the MM region;
}

void PDBFile::openPDB(std::string fileName) {
  file.open(fileName);
  ATOM helperAtom;
  atomCount = 0, resCount = 0, raw_connectionCount = 0;
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
    if (contains(buffer, "CONECT"))
      raw_connectionCount++;
  }
#ifdef PRINT
  std::cout << "Atom count: " << atomCount << '\n';
  std::cout << "Residue count: " << resCount << '\n';
  std::cout << "Connections: " << raw_connectionCount << std::endl;
#endif
}

void PDBFile::parseConnection(std::string buffer) {}

void PDBFile::parsePDB() {
  ATOM helperAtom;
  std::string connData;
  std::stringstream ss;
  int baseToConn = 0, token = 0;
  atoms = std::unordered_map<int, ATOM>(atomCount);
  residues = std::unordered_map<int, RESIDUE>(resCount);
  residues = std::unordered_map<int, RESIDUE>(raw_connectionCount);
  int posCounter = 0;
  int currRes = 0;
  file.clear();
  file.seekg(0);
  std::string buffer;
  while (getline(file, buffer)) {
    if (contains(buffer, "ATOM") || contains(buffer, "HETATM")) {
      setAtom(&helperAtom, buffer);
      posCounter = helperAtom.serialNumber;
      atoms[posCounter] = helperAtom;
      currRes = atoms.at(posCounter).resSeq;
      if (residues.count(currRes) == 0) {
        residues[currRes] = RESIDUE();
        residues.at(currRes).atomID = std::unordered_set<int>();
        residues.at(currRes).resSeq = atoms.at(posCounter).resSeq;
        residues.at(currRes).chainID = atoms.at(posCounter).chainID;
        std::memcpy(residues.at(currRes).resname, atoms.at(posCounter).resName,
                    3);
      }
      residues.at(currRes).atomID.insert(posCounter);
    }
    if (contains(buffer, "CONECT")) {
      connData = buffer.substr(6);
      ss = std::stringstream(connData);
      ss >> baseToConn;
      connections[baseToConn] = CONNECTION();
      connections.at(baseToConn).baseAtomID = baseToConn;
      while (ss >> token) {
        connections.at(baseToConn).connectedTo.insert(token);
      }
    }
  }
  countActualConnections();
#ifdef PRINT
  std::cout << posCounter << " atoms parsed." << '\n';
  std::cout << connectionCount << " unique connections parsed." << std::endl;
#endif
}

PDBFile::PDBFile(std::string fileName) {
  this->openPDB(fileName);
  this->parsePDB();
}

void PDBFile::countActualConnections() {
  connectionCount = 0;
  for (auto pairRec : connections) {
    auto data = pairRec.second;
    for (auto connedAtom : data.connectedTo) {
      if (connedAtom < data.baseAtomID) {
        continue;
      } else {
        connectionCount++;
      }
    }
  }
}

uniquePairList PDBFile::getUniquePairs() {
  uniquePairList retval = uniquePairList(connectionCount);
  int i = 0;
  for (auto pairRec : connections) {
    auto data = pairRec.second;
    for (auto connedAtom : data.connectedTo) {
      if (connedAtom < data.baseAtomID) {
        continue;
      } else {
        retval.at(i).first[0] = atoms.at(connedAtom).x;
        retval.at(i).first[1] = atoms.at(connedAtom).y;
        retval.at(i).first[2] = atoms.at(connedAtom).z;
        retval.at(i).second[0] = atoms.at(data.baseAtomID).x;
        retval.at(i).second[1] = atoms.at(data.baseAtomID).y;
        retval.at(i).second[2] = atoms.at(data.baseAtomID).z;
        i++;
      }
    }
  }

  return retval;
}

// int main(int argc, char *argv[]) { PDBFile pdb(argv[1]); }
