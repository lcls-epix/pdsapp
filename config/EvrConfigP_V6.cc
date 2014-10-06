#include "pdsapp/config/EvrConfigP_V6.hh"

#include "pdsapp/config/EvrConfigType_V6.hh"
#include "pdsapp/config/EvrEventCodeTable_V6.hh"
#include "pdsapp/config/EvrPulseTable_V6.hh"
#include "pdsapp/config/SequencerConfig_V6.hh"
#include "pdsapp/config/EventcodeTiming.hh"
#include "pdsapp/config/GlobalCfg.hh"
#include "pdsdata/psddl/evr.ddl.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QTabWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>

#include <stdio.h>

static const unsigned MaxEVRs = 8;
static const double EvrPeriod = 1./119e6;

typedef Pds::EvrData::IOConfigV1  EvrIOConfigType;
static Pds::TypeId _evrIOConfigType(Pds::TypeId::Id_EvrIOConfig,
				    EvrIOConfigType::Version);

#define ADDTAB(p,title) {                       \
    QWidget* w = new QWidget;                   \
    w->setLayout(p->initialize(0));             \
    tab->addTab(w,title); }

namespace Pds_ConfigDb {

  namespace EvrConfig_V6 {

    class EvrPulseTables : public Parameter {
    public:
      EvrPulseTables() : 
        Parameter(NULL), 
        _nevr(0),
        _pulse_buffer (new char[EvrConfigType::MaxPulses *sizeof(PulseType)]),
        _output_buffer(new char[EvrConfigType::MaxOutputs*sizeof(OutputMapType)]),
        _npulses      (0),
        _noutputs     (0)
      {
        for(unsigned i=0; i<MaxEVRs; i++)
          _evr[i] = new EvrPulseTable(i);
      }
      ~EvrPulseTables() {
        delete[] _pulse_buffer;
        delete[] _output_buffer;
        for(unsigned i=0; i<MaxEVRs; i++)
          delete _evr[i];
      }
    public:
      QLayout* initialize(QWidget*) 
      {
        QVBoxLayout* vl = new QVBoxLayout;
        vl->addStretch();
        { QGridLayout* hl = new QGridLayout;
          hl->addWidget(new QLabel("Pulses generated by \"Readout\" EventCode")             ,0,0,Qt::AlignHCenter);
          hl->addWidget(new QLabel("Pulse delay is specified with respect to EventCode 140"),1,0,Qt::AlignHCenter);
          vl->addLayout(hl); }
        vl->addStretch();
        { _tab = new QTabWidget;
          for(unsigned i=0; i<MaxEVRs; i++) {
            QWidget* w = new QWidget;
            w->setLayout(_evr[i]->initialize(0));
            _tab->addTab(w,QString("EVR %1").arg(i));
            _tab->setTabEnabled(i,false);
          }
          vl->addWidget(_tab); }
        vl->addStretch();

        _nevr = 1;
        _tab->setTabEnabled(0,true);

        return vl;
      }
      void     flush     () { for(unsigned i=0; i<_nevr; i++) _evr[i]->flush (); }
      void     update    () { for(unsigned i=0; i<_nevr; i++) _evr[i]->update(); }
      void     enable    (bool) {}
    public:
      void     pull    (const EvrConfigType& tc)
      { 
        for(unsigned i=0; i<MaxEVRs; i++) 
          _evr[i]->pull(tc); 

        //
        //  Read EvrIOConfig
        //
        unsigned nevr = 1;
        { const char* p = reinterpret_cast<const char*>(GlobalCfg::instance().fetch(_evrIOConfigType));
          if (p) {
            nevr = 0;
            do {
              const EvrIOConfigType& iocfg = *reinterpret_cast<const EvrIOConfigType*>(p);
              if (iocfg.nchannels()==0) break;
              p += iocfg._sizeof();
              nevr++;
            } while(1);
          }
        }
      
        for(unsigned i=nevr; i<_nevr; i++)
          _tab->setTabEnabled(i,false);
        for(unsigned i=_nevr; i<nevr; i++)
          _tab->setTabEnabled(i,true);

        _nevr = nevr;
      }

      bool     validate(unsigned ncodes,
                        const EventCodeType* codes)
      {
        bool result  = true;

        int  delay_offset(0);
        int primaryReadout=-1;
        for(unsigned i=0; i<ncodes; i++) {
          if (codes[i].isReadout()) {

            if (primaryReadout<0) {
              primaryReadout=codes[i].code();
              delay_offset =
                EventcodeTiming::timeslot(140) -
                EventcodeTiming::timeslot(codes[i].code());
              printf("Adjusting pulse delays %+d ticks for readout eventcode %d\n",
                     delay_offset, codes[i].code());
            }
            else {
#if 0
              QString msg = QString("Found more than one READOUT code (%1, %2, ...)\n")
                .arg(primaryReadout)
                .arg(codes[i].code());
              msg += QString("Remove all but one.\n");
              QMessageBox::warning(0,"",msg);
              result = false;
#else
              int delay = delay_offset -
                (EventcodeTiming::timeslot(140) -
                 EventcodeTiming::timeslot(codes[i].code()));
              QString msg = QString("Secondary readout eventcode %1 will produce pulses %2 ticks [%3 ns]\n")
                .arg(codes[i].code())
                .arg(delay)
                .arg(delay*EvrPeriod*1e9);
              msg += QString("delayed with respect to primary readout eventcode %1\n").arg(primaryReadout);
              QMessageBox::warning(0,"",msg);
#endif
            }
          }
        }

        unsigned npt = 0;
        PulseType*     pt = 
          reinterpret_cast<PulseType*>(_pulse_buffer);

        unsigned nom = 0;
        OutputMapType* om = 
          reinterpret_cast<OutputMapType*>(_output_buffer);
      
        for(unsigned i=0; i<_nevr; i++) {
          result &= _evr[i]->validate(ncodes, codes, delay_offset, 
                                      npt, pt,
                                      nom, om);
          npt += _evr[i]->npulses();
          pt  += _evr[i]->npulses();
          nom += _evr[i]->noutputs();
          om  += _evr[i]->noutputs();
        }
        _npulses  = npt;
        _noutputs = nom;

        return result;
      }

      unsigned                            npulses () const { return _npulses; }
      const PulseType*     pulses () const 
      { return reinterpret_cast<const PulseType*>(_pulse_buffer); }

      unsigned                            noutputs() const { return _noutputs; }
      const OutputMapType* outputs () const 
      { return reinterpret_cast<const OutputMapType*>(_output_buffer); }

    private:
      EvrPulseTable* _evr[MaxEVRs];
      unsigned       _nevr;
      char*          _pulse_buffer;
      char*          _output_buffer;
      unsigned       _npulses;
      unsigned       _noutputs;
      QTabWidget*    _tab;
    };

    class EvrConfigP::Private_Data : public Parameter {
    public:
      Private_Data() :
        Parameter(NULL),
        _pulse_table (new EvrPulseTables) ,
        _code_table  (new EvrEventCodeTable),
        _seq_config  (new SequencerConfig(*_code_table))
      {}
    public:
      QLayout* initialize(QWidget*) {
        QHBoxLayout* layout = new QHBoxLayout;
        QTabWidget* tab = new QTabWidget;
        ADDTAB(_pulse_table,"Pulses");
        ADDTAB(_code_table ,"EventCodes");
        ADDTAB(_seq_config ,"Sequencer");
        tab->setTabEnabled(2,false);
        layout->addWidget(tab);
        return layout;
      }
      void     update    () { _pulse_table->update(); _code_table ->update(); _seq_config->update(); }
      void     flush     () { _pulse_table->flush (); _code_table ->flush (); _seq_config->flush (); }
      void     enable    (bool) {}
    public:
      int pull(void *from) {
        const EvrConfigType& tc = *reinterpret_cast<const EvrConfigType*>(from);
        _pulse_table->pull(tc);
        _code_table ->pull(tc);
        _seq_config ->pull(tc);
        return size(tc);
      }
      int push(void *to) {
        const_cast<EvrConfigP::Private_Data*>(this)->validate();
        EvrConfigType& tc = *new(to) EvrConfigType(_code_table ->ncodes  (),
                                                   _pulse_table->npulses (),
                                                   _pulse_table->noutputs(),
                                                   _code_table->codes  (),
                                                   _pulse_table->pulses (),
                                                   _pulse_table->outputs(),
                                                   _seq_config ->result() );
        return size(tc);
      }

      int dataSize() const {
        const_cast<EvrConfigP::Private_Data*>(this)->validate();
        return sizeof(EvrConfigType) + 
          _code_table ->ncodes  ()*sizeof(EventCodeType) +
          _pulse_table->npulses ()*sizeof(PulseType) +
          _pulse_table->noutputs()*sizeof(OutputMapType) +
          _seq_config ->result()._sizeof();
      }
      bool validate() {

        bool v = _code_table   ->validate();
        v = v && _pulse_table  ->validate(_code_table->ncodes(),
                                          _code_table->codes ());
        v = v && _seq_config   ->validate();
        return v;
      }
    private:
      EvrPulseTables*    _pulse_table;
      EvrEventCodeTable* _code_table;
      SequencerConfig*   _seq_config;
    };
  };
};

using namespace Pds_ConfigDb::EvrConfig_V6;

EvrConfigP::EvrConfigP():
  Serializer("Evr_Config"), _private_data(new Private_Data)
{
  pList.insert(_private_data);
}

int EvrConfigP::readParameters(void *from)
{
  return _private_data->pull(from);
}

int EvrConfigP::writeParameters(void *to)
{
  return _private_data->push(to);
}

int EvrConfigP::dataSize() const
{
  return _private_data->dataSize();
}

bool EvrConfigP::validate() 
{
  return _private_data->validate();
}
