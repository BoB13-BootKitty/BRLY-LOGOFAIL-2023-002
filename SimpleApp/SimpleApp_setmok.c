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

#include "cert.h"

EFI_GUID SHIM_LOCK_GUID = { 0x605dab50, 0xe046, 0x4300, {0xab, 0xb6, 0x3d, 0xd8, 0x10, 0xdd, 0x8b, 0x23 } };

VOID set_mok(IN EFI_SYSTEM_TABLE *SystemTable) {


  SystemTable->RuntimeServices->SetVariable((CHAR16*)L"MokList", &SHIM_LOCK_GUID, EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS, sizeof(MOK_DATA), MOK_DATA);
}

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if((VOID*)ImageHandle && SystemTable)
    set_mok(SystemTable);

  return Status;
}
