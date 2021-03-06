#ifndef Pds_EvrOutputMap_V1_hh
#define Pds_EvrOutputMap_V1_hh

#include "pdsdata/psddl/evr.ddl.h"

#include "pdsapp/config/Parameters.hh"
#include "pdsapp/config/ParameterSet.hh"

namespace Pds_ConfigDb {
  class EvrOutputMap_V1 {
  public:
    EvrOutputMap_V1();
  public:   
    void insert(Pds::LinkedList<Parameter>& pList);
    bool pull  (const Pds::EvrData::OutputMap&);
    int  push  (void* to);
  private:
    Enumerated<Pds::EvrData::OutputMap::Source> _source;
    NumericInt<unsigned>                        _source_id;
    Enumerated<Pds::EvrData::OutputMap::Conn>   _conn;
    NumericInt<unsigned>                        _conn_id;
  };
};

#endif
