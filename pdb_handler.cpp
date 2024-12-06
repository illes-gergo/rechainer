#include "./pdb_handler.hpp"
#include <fstream>

// Implementation of records
Record::Record() {}

Record::Record(std::string line) {
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
    return;
  }
#ifdef PRINT
  std::cerr << "Line does not contain atom" << std::endl;
#endif
}

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
  }
#ifdef PRINT
  std::cerr << "Line does not contain atom" << std::endl;
#endif
  return false;
}

int Record::get_resseq() { return resseq; }

// Implementation of residues

Residue::Residue(Record record) {
  resseq = record.get_resseq();
  records = std::vector<Record>();
  records.push_back(record);
}

Residue::Residue() {}

Residue::Residue(int resseq, std::vector<Record> records) {
  this->resseq = resseq;
  this->records = std::vector<Record>();
  for (uint i = 0; i < records.size(); i++) {
    if (records.at(i).get_resseq() == this->resseq) {
      this->records.push_back(records.at(i));
    }
  }
}

void Residue::addrecord(Record record) { records.push_back(record); }

// Implementation of whole PDB

void PDB::addresidue(Residue residue) { residues.push_back(residue); }

void PDB::initresidues() {
  bool ended = false;
  bool foundfirst = false;
  int currentseq, scanseq;
  std::string line;
  Record scratch;
  Residue loader;
  // Find first residue
  while (!foundfirst) {
    if (!std::getline(file, line).eof()) {
      if (line.substr(0, 4) == "ATOM") {
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
    loader.addrecord(scratch);
    std::getline(file, line);
    scratch.read(line);
    currentseq = scratch.get_resseq();
  }
#ifdef PRINT
  std::cout << "First residue is finished reading.\nAdding resiude to the "
               "internal resiude list."
            << std::endl;
#endif
  residues.push_back(loader);
}

PDB::PDB(std::string filename) {
  file = std::ifstream(filename, std::ios_base::in);
  residues = std::vector<Residue>();
  initresidues();
}

PDB::~PDB() { file.close(); }
