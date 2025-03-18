#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#define PRINT
// #define DEBUG
#ifdef DEBUG
#ifndef PRINT
#define PRINT
#endif
#endif

typedef struct {
  int serialNumber, resSeq, charge;
  char atomName[4], resName[3], chainID, element[2];
  double x, y, z;
  double occupancy;
} ATOM;

typedef struct {
  char resname[3], chainID;
  int resSeq;
  std::vector<int> atomID;
} RESIDUE;

class PDBFile {
public:
  std::string fileName;
  std::ifstream file;
  std::unordered_map<int, RESIDUE> residues;
  std::unordered_map<int, ATOM> atoms;
  int atomCount;
  int resCount;

  PDBFile(std::string);

  void openPDB(std::string fileName);
  void parsePDB();
};
