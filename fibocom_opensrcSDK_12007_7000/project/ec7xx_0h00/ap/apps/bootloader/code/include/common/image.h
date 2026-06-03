#ifndef __IMAGE_H__
#define __IMAGE_H__

//#include "ARMCM3.h"
#include "ec7xx.h"
#include "mem_map.h"
#include "bl_link_mem_map.h"
//#pragma pack(1)

#define TIMIDENTIFIER		0x54494D48		// "TIMH"
#define OBMIDENTIFIER		0x4F424D49		// "OBMI"
#define DKBIDENTIFIER		0x444B4249		// "DKBI"
#define WTPRESERVEDAREAID	0x4F505448      // "OPTH"
#define DOWNLOAD_EXEC 0
#define DOWNLOAD_BURN 1
#define SHA256_NONE  0xee

typedef struct {
    uint32_t Version;
    uint32_t Identifier;
    uint32_t Date;
    uint32_t OEMReserve;
}VersionInfo;

typedef struct {
    uint32_t ImageId;
    uint32_t BurnAddr; //Flash addr
    uint32_t LoadAddr;
    uint32_t ApImageSze;
    uint32_t CpImageSze;
    uint32_t Reserved[3];
    uint8_t  Hash[28]; //hash 224
    uint8_t  Rsv_0[4];
    uint8_t  ECDSASign[56];
    uint8_t  Rsv_1[72];
}ImageBodyInfo;

typedef struct {
    uint32_t ReservedAreaId;
    uint32_t ReservedSize;
    uint32_t Reserved[2];    
}ReservedArea;

typedef struct {
    VersionInfo Version;
    uint32_t ImageNum;
    uint32_t Type; //bit7-0, hashih type: 0xee-none, other-sha256
    uint32_t reserved[2];
    uint8_t  HashIH[28];
    uint32_t reserved1;
    ImageBodyInfo ImgBodyInfo;
    ReservedArea RsvArea;  //point to reaserved area at tail of ImageHead
}ImageHead;

typedef struct DownloadCtrl
{
    uint32_t type;
}DownloadCtrl;

#ifdef PKGFLXAPP_APP0_LNA
//fibocom
typedef struct {
    uint32_t version;
    uint32_t secUsrheadLen;
    uint32_t imageSize;
    uint32_t hashType;
    uint32_t Reserved1[4];
    uint8_t  Hash[32];
    uint8_t  ECDSASign[64];
    uint32_t Reserved2[8];
}SecUsrHead;

typedef struct
{
    unsigned int ep;
    unsigned int crc32;
    unsigned int len;
    char magic[8];
    char tmp[44];
}app_head;
#endif

extern ImageHead GImageHead;
//0x00800000----0x00820000 reserved 128KB for OBM
//BLIMG_HEAD_FLASH_ENTRY_ADDR     0x00800000 - 0x00802000   8k
//SYSIMG head MAP  4k + 4k 
////SYSIMG1_HEAD_FLASH_ENTRY_ADDR     0x00802000 - 0x00803000   4k
////SYSIMG2_HEAD_FLASH_ENTRY_ADDR     0x00803000 - 0x00804000   4k

//BLIMG_BODY_FLASH_ENTRY_ADDR     0x00804000 - 0x00820000  112k

//0x00820000----0x00C00000 reserved 4M-128K for system
//SYSIMG_BODY_FLASH_ENTRY_ADDR  0x00820000 - 0x00C00000   4M-128K

//LOAD BUFFER FOR VERIFY
//IMG_BODY_LOAD_ADDR                0x00020000 - 0x00038000 96k
//IMG_HEAD_LOAD_ADDR                0x00038000 - 0x0003a000 8k

#ifdef TYPE_EC718M
#define IRAM_IMAGE_LOAD_BASE PSRAM_FOTA_MUXMEM_BASE_ADDR
#else
#define IRAM_IMAGE_LOAD_BASE 0x00420000
#endif

#define IMAGE_HEAD_UNIT_SIZE 0x1000 //4k unit size for image head
#define BLIMG_HEAD_AREA_SIZE (IMAGE_HEAD_UNIT_SIZE)  //4k unit size for bl image head area

//LOAD BUFFER AREA
#define IMG_HEAD_LOAD_ADDR (IRAM_IMAGE_LOAD_BASE)
#define IMG_HEAD_MAX_SIZE  (BLIMG_HEAD_AREA_SIZE) //4k

#define IMG_BODY_LOAD_ADDR (IMG_HEAD_LOAD_ADDR+IMG_HEAD_MAX_SIZE)

//AGENT IMAGE

//IMAGE AREA
#define FLASH_BASE_ADDR AP_FLASH_XIP_ADDR

//IMAGE OFFSET
//spi rw addr, for security boot verify system img
#define BLIMG_HEAD_FLASH_OFFSET_ADDR  0
#define SYSIMG_HEAD_FLASH_OFFSET_ADDR  (BLIMG_HEAD_FLASH_OFFSET_ADDR+BLIMG_HEAD_AREA_SIZE)

//ap 
#define AP_HEAD_FLASH_OFFSET_ADDR  SYSIMG_HEAD_FLASH_OFFSET_ADDR
#define AP_BODY_FLASH_OFFSET_ADDR  (AP_FLASH_LOAD_ADDR - AP_FLASH_XIP_ADDR)
#define AP_BODY_MAX_SIZE           AP_FLASH_LOAD_SIZE

//cp
#define CP_BODY_FLASH_OFFSET_ADDR  (CP_FLASH_LOAD_ADDR - CP_FLASH_XIP_ADDR) 

//FIBOCOM_BASE    second bootloader
#define BL2_BODY_FLASH_ENTRY_ADDR (BOOTLOADEREXT_FLASH_LOAD_ADDR - AP_FLASH_XIP_ADDR)
#define BL2_BODY_MAX_SIAE          BOOTLOADEREXT_FLASH_LOAD_SIZE

//FIBOCOM_BASE    secboot flag_addr
#define FIBO_SECBOOT_FLAG_ADDR     NVRAM_PHYSICAL_BASE + 0x820 + 0x10
#define FIBO_SECBOOT_FLAG_LEN      8
uint32_t fibo_get_secboot_en(void);

uint32_t LoadVerifyImageHead(void);
uint32_t LoadVerifyImageBody(void);

#endif
