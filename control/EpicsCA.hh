#ifndef odfEpicsCA_hh
#define odfEpicsCA_hh

//--------------------------------------------------------------------------
//
// File and Version Information:
// 	$Id$
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//      This class was copied from OdcCompProxy/EpicsCAProxy for use
//      in the online environment.
//
//
//------------------------------------------------------------------------

// epics includes
#include "cadef.h"

class EpicsCA;

//==============================================================================
class EpicsCAChannel {
public:
  enum ConnStatus { NotConnected, Connecting, Connected };

  EpicsCAChannel(const char* channelName,
		 bool        monitor,
		 EpicsCA&    proxy);
  ~EpicsCAChannel();
    
  void connect           (void);
  void getDataCallback   (struct event_handler_args ehArgs);
  void putDataCallback   (struct event_handler_args ehArgs);
  void connStatusCallback(struct connection_handler_args chArgs);
  const char* epicsName  (void) { return _epicsName; }
  chtype     type        () const { return _type; }
  int        nelements   () const { return _nelements; }
  ConnStatus connected   () const { return _connected; }
  
 protected:
  char        _epicsName[32];
  int         _nelements;
  chid	      _epicsChanID;
  chtype      _type;
  evid        _event;
  ConnStatus  _connected;
  bool        _monitor;
  EpicsCA&    _proxy;
};

//==============================================================================
class EpicsCA {
public:
  EpicsCA(const char *channelName, bool);
  virtual ~EpicsCA();
  
  virtual void  connected(bool) = 0;
  virtual void  getData  (const void* value) = 0;
  virtual void* putData  () = 0; 
  virtual void  putStatus(bool) = 0;
protected:
  EpicsCAChannel _channel;
};

#endif
