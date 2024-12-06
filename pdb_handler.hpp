#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define PRINT
#define DEBUG
#ifdef DEBUG
#ifndef PRINT
#define PRINT
#endif
#endif

class Record {
  bool isatom;
  std::string type, atomname, resname, chain, symbol, charge;
  std::string rawline;
  int atomsnum, resseq;
  double x, y, z;

public:
  Record();
  Record(std::string line);
  bool read(std::string line);
  int get_resseq();
};

class Residue {
  int resseq;
  std::vector<Record> records;

public:
  void init();
  Residue();
  Residue(Record record);
  Residue(int resseq, std::vector<Record> records);
  void addrecord(Record record);
  int get_resseq();
};

class PDB {
  std::ifstream file;
  std::vector<Residue> residues;
  bool readonly = true;

public:
  PDB(std::string filename);
  ~PDB();
  void addresidue(Residue residue);

private:
  void initresidues();
  void readresidue();
};
