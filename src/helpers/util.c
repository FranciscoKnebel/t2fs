#include "libs.h"

struct Constants buildConstants(struct BootBlock bootBlock) {
  struct Constants constants;

  constants.SECTORSIZE = SECTOR_SIZE;
  constants.SECTOR_PER_BLOCK = bootBlock.blockSize;
  constants.BLOCKSIZE = constants.SECTOR_PER_BLOCK * constants.SECTORSIZE;

  constants.DISK_SECTORS = bootBlock.diskSectorSize;
  constants.DISK_BLOCKS = constants.DISK_SECTORS / constants.SECTOR_PER_BLOCK;

  constants.BOOT_BLOCK_SIZE = 1;
  constants.BOOT_SECTOR_SIZE = constants.BOOT_BLOCK_SIZE * constants.SECTOR_PER_BLOCK;

  constants.MFT_BLOCK_SIZE = bootBlock.MFTBlocksSize;
  constants.MFT_SECTOR_SIZE = constants.MFT_BLOCK_SIZE * constants.SECTOR_PER_BLOCK;

  constants.DATA_BLOCK_SIZE = constants.DISK_BLOCKS - (constants.BOOT_BLOCK_SIZE + constants.MFT_BLOCK_SIZE);
  constants.DATA_SECTOR_SIZE = constants.DISK_SECTORS - (constants.BOOT_SECTOR_SIZE + constants.MFT_SECTOR_SIZE);

  constants.BOOT_SECTOR = 0;
  constants.MFT_SECTOR = constants.BOOT_SECTOR + constants.SECTOR_PER_BLOCK;
  constants.DATA_SECTOR = constants.MFT_SECTOR_SIZE + constants.SECTOR_PER_BLOCK;

  constants.BOOT_BLOCK = 0;
  constants.MFT_BLOCK = constants.BOOT_BLOCK + 1;
  constants.DATA_BLOCK = constants.MFT_BLOCK_SIZE + 1;

  return constants;
}

int initConfig() {
  BLOCK_T bootBlock;

  if(readBlock(0, &bootBlock) == FALSE) {
    return FALSE;
  };

  config.bootBlock = parseBootBlock(bootBlock.at);

  config.constants = buildConstants(config.bootBlock);
  constants = config.constants;

  return TRUE;
}
