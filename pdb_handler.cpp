#include "./pdb_handler.hpp"
#include <cstdio>
#include <fstream>
#include <iosfwd>
#include <vector>

// Implementation of records
Record::Record() {}

Record::Record(std::string line) { read(line); }

bool Record::read(std::string line) {
  rawline = line;
  if (line.substr(0, 4) == "ATOM") {
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
#ifdef PRINT
    std::cerr << "Line does not contain atom" << std::endl;
#endif
    return true;
  }
}

int Record::get_resseq() { return resseq; }

// Implementation of residues

Residue::Residue(Record record) {
  resseq = record.get_resseq();
  records = std::vector<Record *>(0);
  records.push_back(&record);
}

Residue::Residue() { init(); }

void Residue::init() {
  resseq = 0;
  records = std::vector<Record *>(0);
}

Residue::Residue(int resseq, std::vector<Record *> records) {
  this->resseq = resseq;
  this->records = std::vector<Record *>(0);
  for (uint i = 0; i < records.size(); i++) {
    if (records.at(i)->get_resseq() == this->resseq) {
      this->records.push_back(records.at(i));
    }
  }
}

void Residue::addrecord(Record record) { records.push_back(&record); }

int Residue::get_resseq() { return (*this->records.end())->get_resseq(); }

// Implementation of whole PDB

void PDB::addresidue(Residue *residue) { this->residues.push_back(residue); }

void PDB::initresidues() {
  file.clear();
  file.seekg(0);
  std::streampos filepos = file.tellg();
  bool foundfirst = false;
  int currentseq, scanseq;
  std::string line;
  Record scratch;
  Residue loader;
#ifdef DEBUG
  int counter = 0;
#endif
  // Find first residue
  while (!foundfirst) {
    if (!std::getline(file, line).eof()) {
      if (line.substr(0, 4) == "ATOM") {
#ifdef DEBUG
        std::cout << counter++ << std::endl;
#endif
        scratch.read(line);
        currentseq = scratch.get_resseq();
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
  std::getline(file, line); // Advance to the next record
  scratch.read(line);
  currentseq = scratch.get_resseq();
  scanseq = currentseq;
  while (currentseq == scanseq && !file.eof()) {
#ifdef DEBUG
    std::cout << counter++ << std::endl;
#endif
    loader.addrecord(scratch);
    filepos = file.tellg();
    std::getline(file, line);
    scratch.read(line);
    currentseq = scratch.get_resseq();
    if (currentseq == scanseq)
      loader.addrecord(scratch);
  }
  file.seekg(filepos);
#ifdef PRINT
  std::cout << "First residue is finished reading.\nAdding resiude to the "
               "internal resiude list."
            << std::endl;
#endif
  addresidue(&loader);
  return;
}

void PDB::readresidue() {
  std::string line;
  std::streampos filepos = file.tellg();
  Residue loader;
  Record scratch;
  int seekseq = (*residues.end())->get_resseq() + 1;
#ifdef DEBUG
  std::cout << "Passed first get" << std::endl;
#endif
  int currentseq = seekseq;
#ifdef DEBUG
  int counter = 0;
#endif
  while (!file.eof() && !file.bad() && !file.fail() && seekseq == currentseq) {
#ifdef DEBUG
    std::cout << counter++ << std::endl;
#endif
    filepos = file.tellg();
    getline(file, line);
    scratch.read(line);
    currentseq = scratch.get_resseq();
    if (currentseq == seekseq && currentseq != -1)
      loader.addrecord(scratch);
    if (currentseq == -1) {
#ifdef PRINT
      std::cout << "No atom in line, skipping... " << std::endl;
      currentseq = seekseq;
#endif
    }
  }
  addresidue(&loader);
  file.seekg(filepos);
  return;
}

PDB::PDB(std::string filename) {
#ifdef PRINT
  std::cout << "Entering PDB constructor..." << std::endl;
#endif
  file = std::ifstream(filename, std::ios_base::in);
  residues = std::vector<Residue *>(0);
  if (file.fail() || file.bad()) {
#ifdef PRINT
    std::cout << "File " << filename << " cannot be opened!" << std::endl;
#endif
  } else {
    initresidues();
    while (!file.fail() && !file.bad()) {
      readresidue();
    }
  }
}

PDB::~PDB() {
  file.close();
  return;
}
