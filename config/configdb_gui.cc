#include "pdsapp/config/Ui.hh"
#include "pdsapp/config/Experiment.hh"
#include "pdsapp/config/Parameters.hh"  // set edit mode

#include <QtGui/QApplication>

#include <stdio.h>
#include <getopt.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

using namespace Pds_ConfigDb;

//
// pathOK - verify that path exists and is nonempty
//
static bool pathOK(const char *cmd, const char *pathname)
{
  struct stat buf;
  bool goodFlag = false;
  bool emptyFlag = false;
  char errmsg[256];

  if (pathname && cmd) {
    snprintf(errmsg, 255, "%s: %s", cmd, pathname);
    if (stat(pathname, &buf)) {
      perror(errmsg);
    } else {
      if (S_ISDIR(buf.st_mode)) {
        DIR *dir = opendir(pathname);
        if (dir == NULL) {
          perror(errmsg);
        } else {
          struct dirent *entry;
          // search directory
          emptyFlag = true;
          while ((entry = readdir(dir))) {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
              // directory is not empty
              goodFlag = true;
              emptyFlag = false;
              break;
            }
          }
          (void) closedir(dir);
        }
      } else if (buf.st_size > 0) {
        // file is not empty
        goodFlag = true;
      } else {
        emptyFlag = true;
      }
    }
    if (emptyFlag) {
      fprintf(stderr, "%s: %s is empty\n", cmd, pathname);
    }
  }
  return (goodFlag);
}

int main(int argc, char** argv)
{
  extern int optind;
  bool edit=false;
  bool dbnamed=false;
  string path;
  bool lusage=false;
  int c;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"db", 1, 0, 'd'},
      {"edit", 0, 0, 'e'},
      {"help", 0, 0, 'h'},
      {0, 0, 0, 0}
    };
    c = getopt_long(argc, argv, "h", long_options, &option_index);
    if (c == -1) {
      break;
    }
    switch (c) {
      case 'd':
        if (optarg) {
          if (!strcmp(optarg, "-h") || !strcmp(optarg, "--help")) {
            lusage = true;
          } else {
            path = optarg;
            dbnamed = true;
          }
        } else {
          exit(1);  // should not get here: missing arg
        }
        break;
      case 'e':
        edit = true;
        break;
      case 'h':
        lusage = true;
        break;
      default:
      case '?':
        // error
        lusage = true;
     }
  }

  if (optind < argc) {
    printf("%s: invalid argument -- %s\n", argv[0], argv[optind]);
    lusage = true;
  }

  if (lusage || !dbnamed || !pathOK(argv[0], path.c_str())) {
    printf("Usage: %s --db <path> [--edit] [-h]\n",argv[0]);
    return 1;
  }

  QApplication app(argc, argv);

  Experiment db(path.c_str(),edit?Experiment::Lock:Experiment::NoLock);

  Ui* ui = new Ui(db,edit);
  ui->show();
  app.exec();

  //  printf("\n==AFTER==\n");
  //  db.dump();
}
