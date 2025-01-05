#pragma once

#include <array>
#include <fstream>
#include <string>
#include <vector>

// #define PRINT
// #define DEBUG
#ifdef DEBUG
#ifndef PRINT
#define PRINT
#endif
#endif

class Atom;
class Residue;

typedef struct {
  int ResCoord;
  int AtomCoord;
} InternalCoordinate;

typedef std::array<InternalCoordinate, 2> Connection;

class Atom {
private:
  bool isatom;
  std::string type, atomname, resname, chain, symbol, charge;
  std::string rawline;
  int atomsnum, resseq;
  double x, y, z;
  Residue *partof;

public:
  Atom();
  Atom(std::string line);
  bool read(std::string line);
  int get_resseq();
  int get_id();
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
  std::vector<Connection> connections;

public:
  PDB(std::string filename);
  ~PDB();
  void addresidue(Residue residue);
  int get_rescount();
  Residue get_res(int i);
  std::vector<Residue> get_resvec();
  std::vector<Connection> get_connections();

private:
  void readresidues();
  void readconnections();
  void addif_needed(std::vector<int> ids);
  InternalCoordinate findInternal(int id);
};

std::vector<int> extractValues(std::string line);
bool sameConnection(Connection first, Connection second);
bool sameInternal(InternalCoordinate first, InternalCoordinate second);
