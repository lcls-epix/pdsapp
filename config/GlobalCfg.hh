#ifndef PdsConfigDb_GlobalCfg_hh
#define PdsConfigDb_GlobalCfg_hh

#include "pdsdata/xtc/TypeId.hh"
#include "pds/config/PdsDefs.hh"

namespace Pds_ConfigDb {
  class Device;
  class DbClient;

  class GlobalCfg {
  public:
    static const char* name() { return PdsDefs::GlobalCfg_name(); }
    static void  cache(DbClient&,const Device*);
    static void* fetch(Pds::TypeId);    // if data for that type is cached
    static bool  contains(const UTypeName&); // if the type is global
  };
};

#endif
