#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Record {
  bool isatom = false;
  std::string type, atomname, resname, chain, symbol, charge;
  std::string rawline;
  int atomsnum, resseq;
  double x, y, z;

public:
  Record(std::string line);
  int get_resseq();
};

class Residue {
  int resseq;
  std::vector<Record> records;

public:
  Residue(Record record);
  Residue(int resseq, std::vector<Record> records);
};

class PDB {
  std::ifstream file;
  std::vector<Residue> residues;
  bool readonly = true;

  public:
    PDB(std::string filename);
    ~PDB();

  private:
  void initresidues();
};
