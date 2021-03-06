#ifndef Pds_Reconfig_Ui_hh
#define Pds_Reconfig_Ui_hh

#include "pdsapp/config/SerializerDictionary.hh"
#include "pdsapp/config/ExpertDictionary.hh"
#include "pds/service/Routine.hh"

#include <QtGui/QDialog>

#include <string>
using std::string;

class QListWidget;
class QListWidgetItem;
class QPushButton;
class QButtonGroup;

namespace Pds_ConfigDb {

  class Device;
  class Experiment;
  class Serializer;
  class TableEntry;
  class FileEntry;
  class UTypeName;

  class Reconfig_Ui : public QDialog, public Pds::Routine {
    Q_OBJECT
  public:
    Reconfig_Ui(QWidget*, Experiment&);
    ~Reconfig_Ui();
  public:
    void enable(bool);
    void routine();
  public slots:
    void apply();
    void set_run_type(const QString&);
    void update_device_list();
    void update_component_list();
    void change_component     ();
    void expert_mode();
    void user_mode();
    void reset();
  private:
    Device* _device() const;
    const TableEntry* _device_entry() const;
    Serializer* lookup(const UTypeName&);
    bool _has_dialog(const FileEntry&);
  signals:
    void changed();
    void gchanged();
  private:
    Experiment&  _expt;
    const TableEntry* _entry;
    SerializerDictionary _dict;
    ExpertDictionary     _xdict;
    QListWidget* _devlist;
    QListWidget* _cmplist;
    QPushButton* _applyB;
    QButtonGroup* _modeG;
    bool _expert_mode;
  };
};

#endif


        
        
