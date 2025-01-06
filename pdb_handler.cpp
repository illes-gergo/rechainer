#include "./pdb_handler.hpp"
#include <cstdio>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <sstream>
#include <vector>

// Implementation of records
Atom::Atom() {}

Atom::Atom(std::string line) { read(line); }

bool Atom::read(std::string line) {
  rawline = line;
  if (line.substr(0, 4) == "ATOM" || line.substr(0,4) == "HETA") {
    if (line.length() < 79) {
#ifdef PRINT
      std::cerr << "Warning, bad line width, PDB is not in current format!"
                << std::endl;
#endif
      isatom = false;
      resseq = -1;
      return false;
    }
    isatom = true;
    type = line.substr(0, 4);
    atomsnum = stoi(line.substr(6, 5));
    atomname = line.substr(12, 4);
    resname = line.substr(17, 3);
    chain = line.substr(21, 1);
    resseq = stoi(line.substr(22, 4));
    x = stod(line.substr(30, 8));
    y = stod(line.substr(38, 8));
    z = stod(line.substr(46, 8));
    symbol = line.substr(76, 2);
    charge = line.substr(78, 2);
    return true;
  } else {
    isatom = false;
    resseq = -1;
#ifdef PRINT
    std::cerr << "Line does not contain atom" << std::endl;
#endif
    return true;
  }
}

int Atom::get_resseq() { return resseq; }
int Atom::get_id() { return atomsnum; }
std::string Atom::get_resname() { return resname; }
std::string Atom::get_chain() { return chain; }

// Implementation of residues

Residue::Residue(Atom record) {
  resseq = record.get_resseq();
  atoms = std::vector<Atom>();
  atoms.push_back(record);
}

Residue::Residue() { init(); }

void Residue::init() {
  resseq = -1;
  resname = "";
  atoms = std::vector<Atom>();
}

Residue::Residue(int resseq, std::vector<Atom> records) {
  this->resseq = resseq;
  this->atoms = std::vector<Atom>();
  for (uint i = 0; i < records.size(); i++) {
    if (records.at(i).get_resseq() == this->resseq) {
      this->atoms.push_back(records.at(i));
    }
  }
}

void Residue::addrecord(Atom record) {
  if (atoms.size() == 0 && resname == "") {
    resseq = record.get_resseq();
    resname = record.get_resname();
    atoms.push_back(record);
    return;
  } else {
    if (resseq != record.get_resseq() && resname != record.get_resname()) {
#ifdef PRINT
      std::cout << "Warning, the record got does not belong to this resiude, "
                   "aborting..."
                << std::endl;
#endif
      return;
    }
    atoms.push_back(record);
  }
}

int Residue::get_resseq() { return this->atoms.back().get_resseq(); }
std::string Residue::get_resname() { return this->atoms.back().get_resname(); }

int Residue::get_reccount() { return atoms.size(); }
std::string Residue::get_reschain() { return atoms.back().get_chain(); }

// Implementation of whole PDB

void PDB::addresidue(Residue residue) { this->residues.push_back(residue); }

void PDB::readresidues() {
  std::string line;
  std::streampos filepos = file.tellg();
  Residue loader;
  Atom scratch;
  getline(file, line);
  file.seekg(filepos);
  scratch.read(line);
  int seekseq = scratch.get_resseq();
  int currentseq = seekseq;
  while (!file.eof() && !file.bad() && !file.fail() && seekseq == currentseq) {
    filepos = file.tellg();
    getline(file, line);
    scratch.read(line);
    currentseq = scratch.get_resseq();
    if (currentseq == seekseq && currentseq != -1)
      loader.addrecord(scratch);
    if (currentseq == -1) {
#ifdef PRINT
      std::cout << "No atom in line, skipping... " << std::endl;
#endif
      currentseq = seekseq;
    }
  }
  addresidue(loader);
  file.seekg(filepos);
  return;
}

PDB::PDB(std::string filename) {
#ifdef PRINT
  std::cout << "Entering PDB constructor..." << std::endl;
#endif
  file = std::ifstream(filename, std::ios_base::in);
  residues = std::vector<Residue>();
  connections = std::vector<Connection>();
  if (file.fail() || file.bad()) {
#ifdef PRINT
    std::cout << "File " << filename << " cannot be opened!" << std::endl;
#endif
  } else {
    // initresidues();
    while (!file.fail() && !file.bad()) {
      readresidues();
#ifdef PRINT
      std::cout << "Reading residue " << this->residues.size() << " finished."
                << std::endl;
#endif
    }
    file.clear();
    file.seekg(0);
    while (!file.fail() && !file.bad() && !file.eof()) {
      readconnections();
    }
  }
}

void PDB::readconnections() {
  std::string line;
  getline(file, line);
  if (line.substr(0, 6) != "CONECT") {
  } else {
    auto conndata = line.substr(6);
    auto nums = extractValues(conndata);
    // std::cout << "Valid connection data found" << std::endl;
    addif_needed(nums);
  }
}

InternalCoordinate PDB::findInternal(int id) {
  int base_res = 0, base_atom = 0;
  for (auto res : residues) {
    base_atom = 0;
    for (auto atom : res.get_atomvec()) {
      if (atom.get_id() == id)
        goto the_way_out;
      base_atom++;
    }
    base_res++;
  }
the_way_out:
  return InternalCoordinate{base_res, base_atom};
}

bool sameInternal(InternalCoordinate first, InternalCoordinate second) {
  if (first.AtomCoord == second.AtomCoord &&
      first.ResCoord == second.ResCoord) {
    return true;
  } else {
    return false;
  }
}

bool sameConnection(Connection first, Connection second) {
  if ((sameInternal(first[0], second[0]) &&
       sameInternal(first[1], second[1])) ||
      (sameInternal(first[1], second[0]) &&
       sameInternal(first[0], second[1]))) {
    return true;
  } else {
    return false;
  }
}

void PDB::addif_needed(std::vector<int> ids) {
  int base_id = ids[0];
  std::vector<int> connect_ids(ids.begin() + 1, ids.end());
  InternalCoordinate base_coord = findInternal(base_id);
  InternalCoordinate test_coord;
  Connection test_connection;
  for (auto id : connect_ids) {
    test_coord = findInternal(id);
    test_connection = Connection{base_coord, test_coord};
    for (auto connection : connections) {
      if (sameConnection(test_connection, connection)) {
        goto skip_add;
      }
    }
    connections.push_back(test_connection);
    //std::cout << "Unique connection found" << std::endl;
  skip_add:
    continue;
  }
}

std::vector<int> extractValues(std::string line) {
  auto retval = std::vector<int>();
  int f;
  std::stringstream ss(line);
  while (ss >> f) {
    retval.push_back(f);
  }
  return retval;
}

PDB::~PDB() {
  file.close();
  return;
}
int PDB::get_rescount() { return residues.size(); }

Residue PDB::get_res(int i) { return residues.at(i); }

std::vector<Residue> PDB::get_resvec() { return this->residues; }

std::vector<Atom> Residue::get_atomvec() { return this->atoms; }

std::vector<double> Atom::position() {
  return std::vector<double>{this->x, this->y, this->z};
}
std::string Atom::get_symbol() { return this->symbol; }

std::vector<Connection> PDB::get_connections() { return this->connections; }
