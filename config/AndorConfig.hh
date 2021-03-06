#ifndef Pds_AndorConfig_hh
#define Pds_AndorConfig_hh

#include "pdsapp/config/Serializer.hh"

namespace Pds_ConfigDb {

  class AndorConfig : public Serializer {
  public:
    AndorConfig();
    ~AndorConfig() {}
  public:
    int  readParameters (void* from);
    int  writeParameters(void* to);
    int  dataSize() const;
  private:
    class Private_Data;
    Private_Data* _private_data;
  };

};

#endif
