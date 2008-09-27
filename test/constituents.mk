libnames := app
 
libsrcs_app := EventTest.cc EventOptions.cc

tgtnames          := recordertest eventtest segtest controltest sourcetest bldtest source

tgtsrcs_recordertest := recordertest.cc
tgtlibs_recordertest := pds/service pds/collection pds/xtc pds/utility pds/management pds/client pds/app
tgtslib_recordertest := /usr/lib/rt

tgtsrcs_eventtest := eventtest.cc
tgtlibs_eventtest := pds/service pds/collection pds/xtc pds/utility pds/management pds/client pds/app
tgtslib_eventtest := /usr/lib/rt

tgtsrcs_segtest := segtest.cc
tgtlibs_segtest := pds/service pds/collection pds/xtc pds/utility pds/management pds/client pds/app
tgtslib_segtest := /usr/lib/rt

tgtsrcs_controltest := controltest.cc
tgtlibs_controltest := pds/service pds/collection pds/xtc pds/utility pds/management pds/client pds/app
tgtslib_controltest := /usr/lib/rt

tgtsrcs_source := source.cc
tgtlibs_source := pds/service pds/collection pds/xtc pds/utility pds/management pds/client pds/app
tgtslib_source := /usr/lib/rt

tgtsrcs_sourcetest := sourcetest.cc
tgtlibs_sourcetest := pds/service pds/collection pds/xtc pds/utility pds/management pds/client pds/app
tgtslib_sourcetest := /usr/lib/rt

tgtsrcs_bldtest := bldtest.cc
tgtlibs_bldtest := pds/service pds/collection pds/xtc pds/utility pds/management pds/client pds/app
tgtslib_bldtest := /usr/lib/rt

