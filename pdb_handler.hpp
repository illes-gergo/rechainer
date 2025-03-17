#pragma once

#include <fstream>
#include <string>
#include <vector>

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
  std::vector<int> atomIndex;
} RESIDUE;

class PDBFile {
public:
  std::string fileName;
  std::ifstream file;
  std::vector<RESIDUE *> residues;
  std::vector<ATOM *> atoms;
  int atomCount;
  int resCount;

  PDBFile(std::string);

  void openPDB(std::string fileName);
  void parsePDB();
};
