#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define PRINT
// #define DEBUG
#ifdef DEBUG
#ifndef PRINT
#define PRINT
#endif
#endif

enum REGION { QM, MM };

typedef std::vector<std::pair<double[3], double[3]>> uniquePairList;
typedef std::pair<double[3], double[3]> uniquePair;

typedef struct {
  int serialNumber, resSeq, charge;
  int region;
  char atomName[4], resName[3], chainID, element[2];
  double x, y, z;
  double occupancy;
} ATOM;

typedef struct {
  char resname[3], chainID;
  int resSeq;
  std::unordered_set<int> atomID;
} RESIDUE;

typedef struct {
  int baseAtomID;
  std::unordered_set<int> connectedTo;
} CONNECTION;

class PDBFile {
public:
  std::string fileName;
  std::ifstream file;
  std::unordered_map<int, RESIDUE> residues;
  std::unordered_map<int, ATOM> atoms;
  std::unordered_map<int, CONNECTION> connections;
  int atomCount;
  int resCount;
  int raw_connectionCount;
  int connectionCount;

  PDBFile(std::string);

  void openPDB(std::string fileName);
  void parsePDB();
  void parseConnection(std::string buffer);
  void countActualConnections();
  uniquePairList getUniquePairs();
};
