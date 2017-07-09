/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel

  Header para simplificar o include dentro dos arquivos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apidisk.h"
#include "bitmap2.h"
#include "t2fs.h"

#include "constants.h"
#include "types.h"

#include "helpers/print.h"
#include "helpers/util.h"
#include "helpers/files.h"
#include "helpers/mft.h"
#include "helpers/ldaa.h"

#include "disk.h"
#include "parse.h"
#include "files.h"

extern struct Configuration config;
extern struct Constants constants;
