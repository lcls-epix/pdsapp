tgtnames := control_gui

tgtsrcs_control_gui := control.cc 
tgtsrcs_control_gui += MainWindow.cc ConfigSelect.cc PartitionSelect.cc StateSelect.cc 
tgtsrcs_control_gui += NodeSelect.cc SelectDialog.cc
tgtsrcs_control_gui += MainWindow_moc.cc ConfigSelect_moc.cc PartitionSelect_moc.cc StateSelect_moc.cc
tgtsrcs_control_gui += NodeSelect_moc.cc SelectDialog_moc.cc
tgtsrcs_control_gui += SeqAppliance.cc
tgtlibs_control_gui := pdsdata/xtcdata pdsdata/opal1kdata pdsdata/pulnixdata pdsdata/camdata pdsdata/evrdata pdsdata/controldata
tgtlibs_control_gui += pds/service pds/collection pds/xtc pds/mon pds/vmon pds/utility pds/client pds/config pds/management
tgtlibs_control_gui += pdsapp/configdb
tgtlibs_control_gui += qt/QtGui qt/QtCore
tgtslib_control_gui := /usr/lib/rt
tgtincs_control_gui := qt/include
tgtincs_control_gui += epics/include epics/include/os/Linux

tgtsrcs_catest := catest.cc RunMonitor.cc EpicsCA.cc
tgtslib_catest := /usr/lib/rt
tgtincs_catest := epics/include epics/include/os/Linux
tgtlibs_catest := epics/ca epics/Com
tgtlibs_catest += pdsdata/controldata pdsdata/xtcdata
tgtlibs_catest += pds/service pds/mon pds/vmon pds/collection pds/xtc pds/utility pds/config