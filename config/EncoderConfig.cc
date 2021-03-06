#include "EncoderConfig.hh"

#include "pdsapp/config/Parameters.hh"
#include "pdsapp/config/ParameterSet.hh"
#include "pdsapp/config/BitCount.hh"
#include "pds/config/EncoderConfigType.hh"

#include <new>

using namespace Pds_ConfigDb;

// FIXME: This is blatant duplication of code in
// pds/encoder/driver/pci3e-wrapper.cc.  Yuck!
//
// These must end in NULL.
namespace PCI3E
{
   static const char* count_mode_to_name[] = {
      "COUNT_MODE_WRAP_FULL",
      "COUNT_MODE_LIMIT",
      "COUNT_MODE_HALT",
      "COUNT_MODE_WRAP_PRESET",
      NULL
   };
   static const char* quad_mode_to_name[] = {
      "QUAD_MODE_CLOCK_DIR",
      "QUAD_MODE_X1",
      "QUAD_MODE_X2",
      "QUAD_MODE_X4",
      NULL
   };
}

class Pds_ConfigDb::EncoderConfig::Private_Data {
 public:
   Private_Data();

   void insert( Pds::LinkedList<Parameter>& pList );
   int pull( void* from );
   int push( void* to );
   int dataSize() const
      { return sizeof(EncoderConfigType); }

   NumericInt<uint8_t>     _chan_mask;
   Enumerated<Pds::Encoder::ConfigV2::count_mode_type> _count_mode;
   Enumerated<Pds::Encoder::ConfigV2::quad_mode      > _quadrature_mode;
   NumericInt<uint8_t>     _input_num;
   Enumerated<Enums::Bool> _input_rising;   
   NumericInt<uint32_t>    _ticks_per_sec;
};

Pds_ConfigDb::EncoderConfig::Private_Data::Private_Data() 
   : _chan_mask(        "Channel Mask ",
                       0, 0, 2 ),
     _count_mode(      "Counter Mode ",
                       Pds::Encoder::ConfigV2::WRAP_FULL,
                       PCI3E::count_mode_to_name ),
     _quadrature_mode( "Quadrature Mode ",
                       Pds::Encoder::ConfigV2::X1,
                       PCI3E::quad_mode_to_name ),
     _input_num(       "Trigger Input Number ",
                       0, 0, 3 ),
     _input_rising(    "Trigger on Rising Edge ",
                       Enums::True,
                       Enums::Bool_Names ),
     _ticks_per_sec(   "Timestamp ticks per second ",
                       33000000, 32000000, 34000000)
{}

void Pds_ConfigDb::EncoderConfig::Private_Data::insert( Pds::LinkedList<Parameter>& pList )
{
   pList.insert( &_chan_mask );
   pList.insert( &_count_mode );
   pList.insert( &_quadrature_mode );
   pList.insert( &_input_num );
   pList.insert( &_input_rising );
   pList.insert( &_ticks_per_sec );
}

int Pds_ConfigDb::EncoderConfig::Private_Data::pull( void* from )
{
  EncoderConfigType& encoderConf = *reinterpret_cast<EncoderConfigType*>(from);

  _chan_mask.value       = encoderConf.chan_mask();
  _count_mode.value      = encoderConf.count_mode();
  _quadrature_mode.value = encoderConf.quadrature_mode();
  _input_num.value       = encoderConf.input_num();
  _input_rising.value    = encoderConf.input_rising() ? Enums::True : Enums::False;
  _ticks_per_sec.value   = encoderConf.ticks_per_sec();

  return sizeof(EncoderConfigType);
}

int Pds_ConfigDb::EncoderConfig::Private_Data::push(void* to)
{
   new (to) EncoderConfigType( _chan_mask.value,
                               _count_mode.value,
                               _quadrature_mode.value,
                               _input_num.value,
                               _input_rising.value == Enums::True,
                               _ticks_per_sec.value );

   return sizeof(EncoderConfigType);
}

Pds_ConfigDb::EncoderConfig::EncoderConfig()
   : Serializer("Encoder_Config"),
     _private_data( new Private_Data )
{
   _private_data->insert(pList);
}

int Pds_ConfigDb::EncoderConfig::readParameters (void* from)
{
   return _private_data->pull(from);
}

int Pds_ConfigDb::EncoderConfig::writeParameters(void* to)
{
   return _private_data->push(to);
}

int Pds_ConfigDb::EncoderConfig::dataSize() const
{
   return _private_data->dataSize();
}

#include "Parameters.icc"
