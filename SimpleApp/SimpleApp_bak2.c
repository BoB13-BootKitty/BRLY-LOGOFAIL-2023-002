/*
 * Copyright (c) 2021, Konstantin Aladyshev <aladyshev22@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
//#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
//#include <Library/UefiHiiServicesLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/Hash2.h>
#include <Protocol/ServiceBinding.h>

#include <Protocol/LoadedImage.h>

static inline UINT64 ReadTSC() {
    UINT32 lo, hi;
    // RDTSC: 현재 타임스탬프를 읽는 명령
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((UINT64)hi << 32) | lo;
}

void WaitForSeconds(UINT64 seconds) {
    const UINT64 CPUFrequency = 2500000000;  // 예시: 2.5 GHz CPU
    UINT64 startTSC = ReadTSC();             // 시작 시점의 타임스탬프 읽기
    UINT64 waitTicks = CPUFrequency * seconds;  // 대기할 클럭 수 계산

    // 목표 시간에 도달할 때까지 루프
    while (ReadTSC() - startTSC < waitTicks);
}

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

//Image Base = 0x00000000067E4000 + 0x1000

  //UINT8* target_address = (UINT8*)0x00000000067E4000 + 0x1000;
  UINT8* target_address = (UINT8*)0x3D9BF000 + 0x1000;

  target_address[0] = 0x50; //P
  target_address[2] = 0x57; //W
  target_address[4] = 0x4e; //N
  target_address[6] = 0x45; //E
  target_address[8] = 0x44; //D
  target_address[10] = 0x0a; //\n

  SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16*)target_address);

// EFI_INPUT_KEY Key;
//   while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) {
//         // 입력 대기 중, 아무 일도 하지 않음
//     }

  WaitForSeconds(5);
  
  return Status;
}
