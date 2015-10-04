// CrDiskInfoLib: include/DiskInfoLib.h
// Copyright (c) 2014, Egtra
//
// This code is released under the MIT License.
// See LICENSE.txt

#pragma once

#include "DiskInfoLibCommon.h"

#include <vector>
#include <string>

struct DiskInfo
{
	struct SmartAttribute
	{
		BYTE Id;
		BYTE CurrentValue;
		BYTE WorstValue;
		BYTE ThresholdValue;
		UINT64 RawValue;
	};

	BYTE SmartReadData[512];
	BYTE SmartReadThreshold[512];

	std::vector<SmartAttribute> Attribute;

	bool IsSmartSupported;
	bool IsCurrentCorrect; ///< SmartAttribute::CurentValue �����p�\�ł��邱�ƁB
	bool IsWorstCorrect; ///< SmartAttribute::WorstValue �����p�\�ł��邱�ƁB
	bool IsThresholdCorrect; ///< SmartAttribute::ThresholdValue �����p�\�ł��邱�ƁB
	bool IsRawValues8; ///< Smart�̐��̒l��8�o�C�g�ŕ\����������ȋ@��B
	bool IsRawValues7; ///< Smart�̐��̒l��7�o�C�g�ŕ\����������ȋ@��B

	bool IsLba48Supported;
	bool IsApmSupported;
	bool IsAamSupported;
	bool IsNcqSupported;
	bool IsDeviceSleepSupported;
	bool IsMaxtorMinute; ///< �g�p���Ԃ��s���m�ȉ\��������Maxtor���@��B
	bool IsSsd;
	bool IsTrimSupported;

	int PhysicalDriveId; ///< OS�̕����h���C�uID�B"\\\\.\\PhysicalDrive${N}"�ŎQ�Ƃł���B��Y���܂��͕s���ȏꍇ��-1�B
	int ScsiPort; ///< SCSI�|�[�g�ԍ��B"\\\\.\\scsi${N}:"�ŎQ�Ƃł���B��Y���܂��͕s���ȏꍇ��-1�B
	int ScsiTargetId; ///< SCSI�^�[�Q�b�gID�B��Y���܂��͕s���ȏꍇ��-1�B
	int ScsiBus; ///< SCSI�o�X�ԍ��B��Y���܂��͕s���ȏꍇ��-1�B
	int SiliconImageType; ///< Silicon Image���R���g���[���̌^�Ԃ̈ꕔ�B��Y���̏ꍇ��0�B
	DWORD TotalDiskSize;
	DWORD Cylinder;
	DWORD Head;
	DWORD Sector;
	DWORD Sector28;
	ULONGLONG Sector48;
	ULONGLONG NumberOfSectors;
	DWORD DiskSizeChs;
	DWORD DiskSizeLba28;
	DWORD DiskSizeLba48;
	DWORD DiskSizeWmi;
	DWORD BufferSize;
	ULONGLONG NvCacheSize;
	TRANSFER_MODE TransferModeType;
	int DetectedPowerOnHours; ///< �g�p���ԁB�s���ȏꍇ��-1�B
	//int MeasuredPowerOnHours;
	DWORD PowerOnCount;
	int HostWrites; ///< �������� (GiB)�B�s���ȏꍇ��-1�B
	int HostReads; ///< ���Ǎ��� (GiB)�B�s���ȏꍇ��-1�B
	int GBytesErased; ///< �������� (GiB)�B�s���ȏꍇ��-1�B
	int NandWrites; ///< NAND���x���ł̑������� (GiB)�B�s���ȏꍇ��-1�B
	int WearLevelingCount; ///< �E�F�A���x���J�E���g�B�s���ȏꍇ��-1�B
	int Life; ///< �����B0�`100�B�s���ȏꍇ��-1�B

	DWORD Major;
	DWORD Minor;

	INTERFACE_TYPE InterfaceType;
	COMMAND_TYPE CommandType;

	DWORD DiskVendorId;
	DWORD UsbVendorId;
	DWORD UsbProductId;
	BYTE Target;

	CSMI_SAS_PHY_ENTITY SasPhyEntity;

	std::wstring SerialNumber;
	std::wstring FirmwareRev;
	std::wstring Model;
	std::wstring ModelWmi;
	std::wstring ModelSerial;
	std::wstring MaxTransferMode;
	std::wstring CurrentTransferMode;
	std::wstring MajorVersion;
	std::wstring MinorVersion;
	std::wstring Interface;
	std::wstring Enclosure;
	std::wstring SsdVendorString;
	std::wstring DeviceNominalFormFactor;
	std::wstring PnpDeviceId; ///< �f�o�C�X�C���X�^���X�p�X�iWindows XP�܂ł̓f�o�C�X�C���X�^���XID�ƕ\�L����Ă����j�B

	DWORD Temperature; /// ���x (��)�B�s���ȏꍇ��0�B
	IDENTIFY_DEVICE IdentifyDevice;
	DWORD NominalMediaRotationRate; ///< ��]���B�s���ȏꍇ��0�BATA8-ACS�Ή�SSD�̏ꍇ��1�B
};

namespace CrDiskInfoLib
{
	std::vector<DiskInfo> GetDiskInfo();

#if CR_DISK_INFO_LIB_CUSTOM_DEBUG_PRINT
	void CrDiskInfoLib_DebugPrint(PCWSTR s); // User-defined
#else
	inline void DebugPrint(PCWSTR s)
	{
		OutputDebugStringW(s);
	}
#endif
}
