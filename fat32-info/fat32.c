//Noah Redden
//7841009
//COMP 3430, Assignment 4
//fat32

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

#include "fat32.h"

fat32BootSector bs;

//name of image we are using
char* imagename;

//reads imagefile, fills fat32 struct with required info
void readToFat32Struct(){
    //read parts of image into struct

    int file = open(imagename, O_RDONLY);
    char currByte[1];
    
    //jump boot
    for(int i = 0; i < 3; i++){
        read(file, currByte, 1);
        bs.BS_jmpBoot[i] = currByte[0];
        //printf("jmpBoot: 0x%x\n", bs.BS_jmpBoot[i]);
    }

    //oem name
    read(file, bs.BS_OEMName, BS_OEMName_LENGTH);
    //printf("Name: %s\n", bs.BS_OEMName);

    //bytes per sec
    read(file, currByte, 1);
    bs.BPB_BytesPerSec = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_BytesPerSec = bs.BPB_BytesPerSec | currByte[0];
    //printf("Bytes per sec: %i\n", bs.BPB_BytesPerSec);

    //sectors per cluster
    read(file, currByte, 1);
    bs.BPB_SecPerClus = currByte[0];
    //printf("Sectors per cluster: %i\n", bs.BPB_SecPerClus);

    //number of reserved sectors
    read(file, currByte, 1);
    bs.BPB_RsvdSecCnt = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_RsvdSecCnt = bs.BPB_RsvdSecCnt | currByte[0];
    //printf("Reserved sectors: %i\n", bs.BPB_RsvdSecCnt);

    //number of FAT data structures in the volume
    read(file, currByte, 1);
    bs.BPB_NumFATs = currByte[0];
    //printf("Number of FATs: %i\n", bs.BPB_NumFATs);

    //number of 32byte directory entries in root directory
    read(file, currByte, 1);
    bs.BPB_RootEntCnt = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_RootEntCnt = bs.BPB_RootEntCnt | currByte[0];
    //printf("Root entries: %i\n", bs.BPB_RootEntCnt);

    //number of sectors on volume (16bit)
    read(file, currByte, 1);
    bs.BPB_TotSec16 = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_TotSec16 = bs.BPB_TotSec16 | currByte[0];
    //printf("Number of sectors (16bit): %i\n", bs.BPB_TotSec16);

    //media
    read(file, currByte, 1);
    bs.BPB_Media = currByte[0];
    //printf("Media: 0x%x\n", bs.BPB_Media);

    //FATz16
    read(file, currByte, 1);
    bs.BPB_FATSz16 = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_FATSz16 = bs.BPB_FATSz16 | currByte[0];
    //printf("FATz16: %i\n", bs.BPB_FATSz16);

    //sectors per track
    read(file, currByte, 1);
    bs.BPB_SecPerTrk = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_SecPerTrk = bs.BPB_SecPerTrk | currByte[0];
    //printf("Sectors per track: %i\n", bs.BPB_SecPerTrk);

    //number of heads
    read(file, currByte, 1);
    bs.BPB_NumHeads = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_NumHeads = bs.BPB_NumHeads | currByte[0];
    //printf("Number of heads: %i\n", bs.BPB_NumHeads);

    //number of hidden sectors
    read(file, currByte, 1);
    bs.BPB_HiddSec = currByte[0]<<24;
    read(file, currByte, 1);
    bs.BPB_HiddSec = bs.BPB_HiddSec | (currByte[0]<<16);
    read(file, currByte, 1);
    bs.BPB_HiddSec = bs.BPB_HiddSec | (currByte[0]<<8);    
    read(file, currByte, 1);
    bs.BPB_HiddSec = bs.BPB_HiddSec | currByte[0];
    //printf("Number of hiden sectors: %i\n", bs.BPB_HiddSec);

    //number of sectors on volume (32bit)
    read(file, currByte, 1);
    bs.BPB_TotSec32 = currByte[0]<<24;
    read(file, currByte, 1);
    bs.BPB_TotSec32 = bs.BPB_TotSec32 | (currByte[0]<<16);
    read(file, currByte, 1);
    bs.BPB_TotSec32 = bs.BPB_TotSec32 | (currByte[0]<<8);    
    read(file, currByte, 1);
    bs.BPB_TotSec32 = bs.BPB_TotSec32 | currByte[0];
    //printf("Number of sectors (32bit): %i\n", bs.BPB_TotSec32);

    //number of sectors occupied by FATs
    read(file, currByte, 1);
    bs.BPB_FATSz32 = currByte[0]<<24;
    read(file, currByte, 1);
    bs.BPB_FATSz32 = bs.BPB_FATSz32 | (currByte[0]<<16);
    read(file, currByte, 1);
    bs.BPB_FATSz32 = bs.BPB_FATSz32 | (currByte[0]<<8);    
    read(file, currByte, 1);
    bs.BPB_FATSz32 = bs.BPB_FATSz32 | currByte[0];
    //printf("Number of sectors occupied by FATs: %i\n", bs.BPB_FATSz32);

    //ext flags
    read(file, currByte, 1);
    bs.BPB_ExtFlags = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_ExtFlags = bs.BPB_ExtFlags | currByte[0];
    //printf("Ext Flags: 0x%x\n", bs.BPB_ExtFlags);

    //FAT32 revision number
    read(file, currByte, 1);
    bs.BPB_FSVerLow = currByte[0];
    read(file, currByte, 1);
    bs.BPB_FSVerHigh = currByte[0];
    //printf("FAT32 revision: %i.%i\n", bs.BPB_FSVerHigh, bs.BPB_FSVerLow);

    //root cluster
    read(file, currByte, 1);
    bs.BPB_RootClus = currByte[0]<<24;
    read(file, currByte, 1);
    bs.BPB_RootClus = bs.BPB_RootClus | (currByte[0]<<16);
    read(file, currByte, 1);
    bs.BPB_RootClus = bs.BPB_RootClus | (currByte[0]<<8);    
    read(file, currByte, 1);
    bs.BPB_RootClus = bs.BPB_RootClus | currByte[0];
    //printf("Root cluster number: %i\n", bs.BPB_RootClus);

    //sector number for FSINFO
    read(file, currByte, 1);
    bs.BPB_FSInfo = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_FSInfo = bs.BPB_FSInfo | currByte[0];
    //printf("FSINFO Sector: %i\n", bs.BPB_FSInfo);

    //sector number for boot record
    read(file, currByte, 1);
    bs.BPB_BkBootSec = currByte[0]<<8;
    read(file, currByte, 1);
    bs.BPB_BkBootSec = bs.BPB_BkBootSec | currByte[0];
    //printf("Boot Sector: %i\n", bs.BPB_BkBootSec);

    //reserved for future
    for(int i = 0; i < 12; i++){
        read(file, currByte, 1);
        bs.BPB_reserved[i] = currByte[0];
        //printf("Reserved %i: 0x%x\n",i,bs.BPB_reserved[i]);
    }

    //drive number
    read(file, currByte, 1);
    bs.BS_DrvNum = currByte[0];
    //printf("Drive number: 0x%x\n", bs.BS_DrvNum);

    //reserved1
    read(file, currByte, 1);
    bs.BS_Reserved1 = currByte[0];
    //printf("Reserved1: 0x%x\n", bs.BS_Reserved1);

    //BootSig
    read(file, currByte, 1);
    bs.BS_BootSig = currByte[0];
    //printf("BootSig: 0x%x\n", bs.BS_BootSig);

    //volume serial number
    read(file, currByte, 1);
    bs.BS_VolID = currByte[0]<<24;
    read(file, currByte, 1);
    bs.BS_VolID = bs.BS_VolID | (currByte[0]<<16);
    read(file, currByte, 1);
    bs.BS_VolID = bs.BS_VolID | (currByte[0]<<8);    
    read(file, currByte, 1);
    bs.BS_VolID = bs.BS_VolID | currByte[0];
    //printf("Volume ID: 0x%x\n", bs.BS_VolID);

    //volume label
    read(file, bs.BS_VolLab, BS_VolLab_LENGTH-1);
    //printf("Volume label: %s\n", bs.BS_VolLab);

    //file system type
    read(file, bs.BS_FilSysType, BS_FilSysType_LENGTH-1);
    //printf("File System Type: %s\n", bs.BS_FilSysType);

    //clean up
    close(file);
}

//info command
//print info about the volume in the image
//This includes drive name, free space in kB, usable space, cluster size
void info(){
    readToFat32Struct();

    //volume name
    printf("Volume label: %s\n", bs.BS_VolLab);

    //free space on volume
    //# free clusters * sectors per cluster * bytes per sector
    int freeClusters = 0;//TODO
    printf("Free space: %i bytes\n", freeClusters * bs.BPB_SecPerClus * bs.BPB_BytesPerSec);

    //usable storage on volume
    //(sectors per cluster * bytes per sector) - reserved 
    int reserved = 0;//TODO
    printf("Usable storage: %i bytes\n", (bs.BPB_TotSec32 * bs.BPB_BytesPerSec)-reserved);

    //cluster size, in sectors and bytes
    printf("Cluster size (sectors): %i sectors\n",bs.BPB_SecPerClus);
    printf("Cluster size (bytes): %i bytes\n", bs.BPB_SecPerClus*bs.BPB_BytesPerSec);
    
}

//list command
//print a list of all files and directories in volume
void list(){
    printf("Sorry, this command is not ready yet.\n");
}

//get command
//fetch file from the volume
void get(char* filename){
    printf("Get %s...\n", filename);
    printf("Sorry, this command is not ready yet.\n");
}

//program should be run with `./fat32 (imagename) (command)`
int main(int argc, char *argv[]){
    if(argc == 3){
        imagename = argv[1];

        if(strcmp(argv[2],"info")==0){
            info();
            
        }else if(strcmp(argv[2],"list")==0){
            list();
            
        }else if(strcmp(argv[2],"get")==0){
            printf("Please specify file to get.\nUsage: `./fat32 (imagename) get (filename)`\n");
            
        }else{
            printf("Invalid Command. Try `info`, `list`, or `get`.\n");
            return 0;
        }
    }else if(argc == 4){
        if(strcmp(argv[2],"get")==0){
            get(argv[3]);            
        }else{
            printf("Invalid Command. Try `info`, `list`, or `get`.\n");
            return 0;
        }
    }else{
        printf("Invalid Arguments. Usage: `./fat32 (imagename) (command)`\n");
    }

    return 0;
}
