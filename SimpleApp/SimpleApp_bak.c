/*
 * Copyright (c) 2021, Konstantin Aladyshev <aladyshev22@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>


// 프로토콜 GUID 정의
#define MY_PROTOCOL_GUID \
    { 0xabcd1234, 0x5678, 0x9abc, { 0xde, 0xf0, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab } }

typedef struct {
    VOID (*MyFunction)(VOID);
} MY_PROTOCOL;

EFI_GUID gMyProtocolGuid = MY_PROTOCOL_GUID;

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
                  &gMyProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    Print(L"Error! Can't find any handle with gMyProtocolGuid: %r\n", Status);
    return Status;
  }

  Print(L"Found %d handles with MyProtocol\n", HandleCount);

    // 찾은 핸들 중 첫 번째 핸들에 대해 MyProtocol을 검색
    Status = gBS->HandleProtocol(HandleBuffer[0], &gMyProtocolGuid, (VOID**)&MyProtocol);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get protocol interface: %r\n", Status);
        FreePool(HandleBuffer);
        return Status;
    }

    // 프로토콜의 MyFunction 호출
    if (MyProtocol != NULL && MyProtocol->MyFunction != NULL) {
        MyProtocol->MyFunction();  // "MyFunction is called!" 출력
    } else {
        Print(L"MyProtocol or MyFunction is NULL\n");
    }

    // 핸들 버퍼 메모리 해제
    if(HandleBuffer != NULL)
      FreePool(HandleBuffer);

  return Status;
}
