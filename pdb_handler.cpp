#include "./pdb_handler.hpp"
#include <fstream>

// Implementation of records
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
  }
}

int Record::get_resseq() { return resseq; }

// Implementation of residues

Residue::Residue(Record record) {
  resseq = record.get_resseq();
  records = std::vector<Record>();
  records.push_back(record);
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

// Implementation of whole PDB

void PDB::initresidues() {
  bool foundfirst = false;
  int currentseq;
  std::string line;
  while (!foundfirst) {
    if (std::getline(file, line)) {
      if (line.substr(0,4) == "ATOM") {
        Record scratch(line);
        Residue loader(scratch);
        currentseq = scratch.get_resseq();
        foundfirst = true;
      }

    } else {
      std::cerr << "First residue not found!" << std::endl;
      break;
    }
  }
}

PDB::PDB(std::string filename) {
  file = std::ifstream(filename, std::ios_base::in);
  residues = std::vector<Residue>();
  initresidues();
}

PDB::~PDB() {
  file.close();
}
