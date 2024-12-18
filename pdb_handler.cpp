#include "./pdb_handler.hpp"
#include <cstdio>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <vector>

// Implementation of records
Record::Record() {}

Record::Record(std::string line) { read(line); }

bool Record::read(std::string line) {
  rawline = line;
  if (line.substr(0, 4) == "ATOM") {
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

int Record::get_resseq() { return resseq; }
std::string Record::get_resname() { return resname; }
std::string Record::get_chain() { return chain; }

// Implementation of residues

Residue::Residue(Record record) {
  resseq = record.get_resseq();
  records = std::vector<Record>();
  records.push_back(record);
}

Residue::Residue() { init(); }

void Residue::init() {
  resseq = -1;
  resname = "";
  records = std::vector<Record>();
}

Residue::Residue(int resseq, std::vector<Record> records) {
  this->resseq = resseq;
  this->records = std::vector<Record>();
  for (uint i = 0; i < records.size(); i++) {
    if (records.at(i).get_resseq() == this->resseq) {
      this->records.push_back(records.at(i));
    }
  }
}

void Residue::addrecord(Record record) {
  if (records.size() == 0 && resname == "") {
    resseq = record.get_resseq();
    resname = record.get_resname();
    records.push_back(record);
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
    records.push_back(record);
  }
}

int Residue::get_resseq() { return this->records.back().get_resseq(); }
std::string Residue::get_resname() {
  return this->records.back().get_resname();
}

int Residue::get_reccount() { return records.size(); }
std::string Residue::get_reschain() { return records.back().get_chain(); }

// Implementation of whole PDB

void PDB::addresidue(Residue residue) { this->residues.push_back(residue); }

void PDB::initresidues() {
  file.clear();
  file.seekg(0);
  std::streampos filepos = file.tellg();
  bool foundfirst = false;
  int scanseq;
  std::string line;
  Record scratch;
  Residue loader;
  // #ifdef DEBUG
  //   int counter = 0;
  // #endif
  // Find first residue
  while (!foundfirst) {
    if (!std::getline(file, line).eof()) {
      if (line.substr(0, 4) == "ATOM") {
        // #ifdef DEBUG
        //         std::cout << counter++ << std::endl;
        // #endif
        scratch.read(line);
        foundfirst = true;
        loader.addrecord(scratch);
      }

    } else {
#ifdef PRINT
      std::cerr << "First residue not found!" << std::endl;
#endif
      break;
    }
  }
#ifdef PRINT
  std::cout << "First residue found, keep reading..." << std::endl;
#endif
  // If first residue found, then read it
  scanseq = scratch.get_resseq();
  while (scratch.get_resseq() == scanseq && !file.eof()) {
    // #ifdef DEBUG
    //     std::cout << counter++ << std::endl;
    // #endif
    filepos = file.tellg();
    std::getline(file, line);
    scratch.read(line);
    if (scratch.get_resseq() == scanseq)
      loader.addrecord(scratch);
  }
  file.seekg(filepos);
#ifdef PRINT
  std::cout << "First residue is finished reading.\nAdding resiude to the "
               "internal resiude list."
            << std::endl;
#endif
  addresidue(loader);
  return;
}

void PDB::readresidue() {
  std::string line;
  std::streampos filepos = file.tellg();
  Residue loader;
  Record scratch;
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
  if (file.fail() || file.bad()) {
#ifdef PRINT
    std::cout << "File " << filename << " cannot be opened!" << std::endl;
#endif
  } else {
    initresidues();
    while (!file.fail() && !file.bad()) {
      readresidue();
#ifdef PRINT
      std::cout << "Reading residue " << this->residues.size() << " finished."
                << std::endl;
#endif
    }
  }
}

PDB::~PDB() {
  file.close();
  return;
}
int PDB::get_rescount() { return residues.size(); }

Residue PDB::get_res(int i) { return residues.at(i); }
