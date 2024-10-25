/*
 * Copyright (c) 2021, Konstantin Aladyshev <aladyshev22@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>

// 검색할 패턴 정의 (8바이트)
const UINT8 PATTERN[3] = { 0x42, 0x4B, 0x54 };

// 특정 메모리 주소에서 패턴이 일치하는지 검사
BOOLEAN IsPatternMatch(UINT8 *Address) {
    for (UINTN i = 0; i < sizeof(PATTERN); i++) {
        if (Address[i] != PATTERN[i]) {
            return FALSE;
        }
    }
    return TRUE;
}


EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8 *Address = NULL;

  Print(L"Searching mem point...\n");

  // 0부터 0x7FFFFFFFFF까지 메모리 순회
    for (UINTN i = 0x30000000; i <= 0x7FFFFFFFFF; i++) {
        Address = (UINT8 *)i;

        if(i % 100000000 == 0) {
          Print(L"Continung... current mem: 0x%p\n", Address);
        }

        // 메모리 접근 시도, 실패 시 예외 처리
        if (IsPatternMatch(Address)) {
            Print(L"Pattern found at address: 0x%p\n", Address);
        }
    }

  Print(L"Search completed.\n");

  return Status;
}
