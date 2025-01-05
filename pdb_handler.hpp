#pragma once

#include <fstream>
#include <string>
#include <vector>

#define PRINT
#define DEBUG
#ifdef DEBUG
#ifndef PRINT
#define PRINT
#endif
#endif

class Atom;
class Residue;

class Connection {
  Atom* atoms[2];
  Residue* residues[2];

public:

};

class Atom {
private:
  bool isatom;
  std::string type, atomname, resname, chain, symbol, charge;
  std::string rawline;
  int atomsnum, resseq;
  double x, y, z;
  Residue* partof;

public:
  Atom();
  Atom(std::string line);
  bool read(std::string line);
  int get_resseq();
  std::string get_resname();
  std::string get_chain();
  std::vector<double> position();
  std::string get_symbol();
};

class Residue {
  int resseq;
  std::string resname;
  std::vector<Atom> atoms;

public:
  void init();
  Residue();
  Residue(Atom record);
  Residue(int resseq, std::vector<Atom> records);
  void addrecord(Atom record);
  int get_resseq();
  int get_reccount();
  std::string get_resname();
  std::string get_reschain();
  std::vector<Atom> get_atomvec();
};

class PDB {
  std::ifstream file;
  std::vector<Residue> residues;
  bool readonly = true;

public:
  PDB(std::string filename);
  ~PDB();
  void addresidue(Residue residue);
  int get_rescount();
  Residue get_res(int i);
  std::vector<Residue> get_resvec();

private:
  void initresidues();
  void readresidue();
};
