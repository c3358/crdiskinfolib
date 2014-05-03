/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2008-2009 hiyohiyo. All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskInfo.h"
#include "DiskInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString CDiskInfoDlg::__Number(DWORD value)
{
	CString cstr;

	if(value >= 1000)
	{
		cstr = _T("");
	}
	else if(value >= 100)
	{
		cstr.Format(_T("%3d"), value);
	}
	else if(value >= 10)
	{
		cstr.Format(_T("_%2d"), value);
	}
	else
	{
		cstr.Format(_T("__%1d"), value);
	}

	return cstr;
}

void CDiskInfoDlg::OnEditCopy()
{
	CString cstr, clip, driveTemplate, drive, feature, temp, line, csd;

	UpdateData(TRUE);

	clip = _T("\
----------------------------------------------------------------------------\r\n\
%PRODUCT% %VERSION% (C) %COPY_YEAR% hiyohiyo\r\n\
                                Crystal Dew World : http://crystalmark.info/\r\n\
----------------------------------------------------------------------------\r\n\
\r\n\
    OS : %OS%\r\n\
  Date : %DATE%\r\n\
\r\n\
-- Controller Map ----------------------------------------------------------\r\n\
%CONTROLLER_MAP%\
\r\n\
-- Disk List ---------------------------------------------------------------\r\n\
%DISK_LIST%\
\r\n");
	clip.Replace(_T("%PRODUCT%"), PRODUCT_NAME);
	clip.Replace(_T("%VERSION%"), PRODUCT_VERSION);
	clip.Replace(_T("%COPY_YEAR%"), PRODUCT_COPY_YEAR);

	SYSTEMTIME st;
	GetLocalTime(&st);
	cstr.Format(_T("%04d/%02d/%02d %d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	clip.Replace(_T("%DATE%"), cstr);

	clip.Replace(_T("%CONTROLLER_MAP%"), m_Ata.m_ControllerMap);

	GetOsName(cstr);
	clip.Replace(_T("%OS%"), cstr);

	cstr = _T("");
	temp = _T("");
	for(int i = 0; i < m_Ata.vars.GetCount(); i++)
	{
#ifdef BENCHMARK
		temp.Format(_T(" (%d) %s : %.1f GB (%.1f MB/s)"), i + 1,
			m_Ata.vars[i].Model, m_Ata.vars[i].TotalDiskSize / 1000.0, m_Ata.vars[i].Speed);
#else
		if(m_Ata.vars[i].TotalDiskSize < 1000)
		{
			temp.Format(_T(" (%d) %s : %.2f GB"), i + 1,
				m_Ata.vars[i].Model, m_Ata.vars[i].TotalDiskSize / 1000.0);
		}
		else
		{
			temp.Format(_T(" (%d) %s : %.1f GB"), i + 1,
				m_Ata.vars[i].Model, m_Ata.vars[i].TotalDiskSize / 1000.0);
		}
#endif
		cstr += temp;
		
		if(m_Ata.vars[i].PhysicalDriveId < 0)
		{
			temp.Format(_T(" [X-%d-%d, %s]"),
				m_Ata.vars[i].ScsiPort, m_Ata.vars[i].ScsiTargetId, m_Ata.vars[i].CommandTypeString);
		}
		else if(m_Ata.vars[i].ScsiPort < 0)
		{
			if(m_Ata.vars[i].VendorId > 0 && m_Ata.vars[i].ProductId > 0)
			{
				temp.Format(_T(" [%d-X-X, %s] (VID=%04Xh, PID=%04Xh)"),
					m_Ata.vars[i].PhysicalDriveId, m_Ata.vars[i].CommandTypeString, m_Ata.vars[i].VendorId, m_Ata.vars[i].ProductId);
			}
			else
			{
				temp.Format(_T(" [%d-X-X, %s]"),
					m_Ata.vars[i].PhysicalDriveId, m_Ata.vars[i].CommandTypeString);
			}
		}
		else
		{
			temp.Format(_T(" [%d-%d-%d, %s]"),
				m_Ata.vars[i].PhysicalDriveId, m_Ata.vars[i].ScsiPort, m_Ata.vars[i].ScsiTargetId, m_Ata.vars[i].CommandTypeString);
		}

		cstr += temp;

		if(! m_Ata.vars[i].SsdVendorString.IsEmpty())
		{
			cstr += _T(" - ") + m_Ata.vars[i].SsdVendorString;
		}

		cstr += _T("\r\n");
	}
	clip.Replace(_T("%DISK_LIST%"), cstr);

	driveTemplate = _T("\
----------------------------------------------------------------------------\r\n\
 (%I%) %MODEL%\r\n\
----------------------------------------------------------------------------\r\n\
%ENCLODURE%\
            Model : %MODEL%\r\n\
         Firmware : %FIRMWARE%\r\n\
    Serial Number : %SERIAL_NUMBER%\r\n\
  Total Disk Size : %TOTAL_DISK_SIZE%\r\n\
      Buffer Size : %BUFFER_SIZE%\r\n\
    NV Cache Size : %NV_CACHE_SIZE%\r\n\
Number of Sectors : %NUMBER_OF_SECTORS%\r\n\
    Rotation Rate : %ROTATION_RATE%\r\n\
        Interface : %INTERFACE%\r\n\
    Major Version : %MAJOR_VERSION%\r\n\
    Minor Version : %MINOR_VERSION%\r\n\
    Transfer Mode : %TRANSFER_MODE%\r\n\
   Power On Hours : %POWER_ON_HOURS%\r\n\
   Power On Count : %POWER_ON_COUNT%\r\n\
      Temparature : %TEMPERATURE%\r\n\
    Health Status : %DISK_STATUS%\r\n\
         Features : %SUPPORTED_FEATURE%\r\n\
        APM Level : %APM_LEVEL%\r\n\
        AAM Level : %AAM_LEVEL%\r\n\
");
#ifdef BENCHMARK
	driveTemplate += _T("  Sequential Read : %BENCHMARK%\r\n");
#endif
	driveTemplate += _T("\r\n");

	for(int i = 0; i < m_Ata.vars.GetCount(); i++)
	{
		drive = driveTemplate;
		cstr.Format(_T("%d"), i + 1);
		drive.Replace(_T("%I%"), cstr);
		if(m_Ata.vars[i].Enclosure.IsEmpty())
		{
			drive.Replace(_T("%ENCLODURE%"), _T(""));
		}
		else
		{
			cstr.Format(_T("        Enclodure : %s (VID=%04Xh, PID=%04Xh, %s)\r\n"),
				m_Ata.vars[i].Enclosure, m_Ata.vars[i].VendorId, m_Ata.vars[i].ProductId, m_Ata.vars[i].CommandTypeString);		
			drive.Replace(_T("%ENCLODURE%"), cstr);
		}
		drive.Replace(_T("%MODEL%"), m_Ata.vars[i].Model);
//		drive.Replace(_T("%MODEL_WMI%"), m_Ata.vars[i].ModelWmi);
		drive.Replace(_T("%FIRMWARE%"), m_Ata.vars[i].FirmwareRev);
		if(m_FlagHideSerialNumber)
		{
			CString temp = _T("");
			for(int j = 0; j < m_Ata.vars[i].SerialNumber.GetLength(); j++)
			{
				temp += _T("*");
			}
			drive.Replace(_T("%SERIAL_NUMBER%"), temp);
		}
		else
		{
			drive.Replace(_T("%SERIAL_NUMBER%"), m_Ata.vars[i].SerialNumber);
		}
		drive.Replace(_T("%INTERFACE%"), m_Ata.vars[i].Interface);
		drive.Replace(_T("%MAJOR_VERSION%"), m_Ata.vars[i].MajorVersion);
		drive.Replace(_T("%MINOR_VERSION%"), m_Ata.vars[i].MinorVersion);
		drive.Replace(_T("%TRANSFER_MODE%"), m_Ata.vars[i].MaxTransferMode);
		temp.Format(_T("%I64d"), m_Ata.vars[i].NumberOfSectors);
		drive.Replace(_T("%NUMBER_OF_SECTORS%"), temp);
		CString diskStatus;
		diskStatus = GetDiskStatus(m_Ata.vars[i].DiskStatus);
		if(m_Ata.vars[i].Life > 0)
		{
			cstr.Format(_T(" (%d %%)"), m_Ata.vars[i].Life);
			diskStatus += cstr; 
		}
		drive.Replace(_T("%DISK_STATUS%"), diskStatus);

		CString IsMinutes;

		if(m_Ata.vars[i].MeasuredPowerOnHours > 0)
		{
			if(m_Ata.vars[i].MeasuredTimeUnitType == CAtaSmart::POWER_ON_MINUTES)
			{
				if(m_Ata.vars[i].IsMaxtorMinute)
				{
					IsMinutes = _T(" (?)");
				}
			}
			else
			{
				IsMinutes = _T("");
			}
			cstr.Format(_T("%d %s%s"), m_Ata.vars[i].MeasuredPowerOnHours, i18n(_T("Dialog"), _T("POWER_ON_HOURS_UNIT")), IsMinutes);
		}
		else if(m_Ata.vars[i].DetectedPowerOnHours >= 0)
		{
			if(m_Ata.vars[i].DetectedTimeUnitType == CAtaSmart::POWER_ON_MINUTES)
			{
				if(m_Ata.vars[i].IsMaxtorMinute)
				{
					IsMinutes = _T(" (?)");
				}
			}
			else
			{
				IsMinutes = _T("");
			}
			cstr.Format(_T("%d %s"), m_Ata.vars[i].DetectedPowerOnHours, i18n(_T("Dialog"), _T("POWER_ON_HOURS_UNIT")), IsMinutes);
		}
		else
		{
			cstr = i18n(_T("Dialog"), _T("UNKNOWN"));
		}
		drive.Replace(_T("%POWER_ON_HOURS%"), cstr);

		if(m_Ata.vars[i].PowerOnCount > 0)
		{
			cstr.Format(_T("%d %s"), m_Ata.vars[i].PowerOnCount, i18n(_T("Dialog"), _T("POWER_ON_COUNT_UNIT")));
		}
		else
		{
			cstr = i18n(_T("Dialog"), _T("UNKNOWN"));
		}
		drive.Replace(_T("%POWER_ON_COUNT%"), cstr);

		if(m_Ata.vars[i].Temperature > 0)
		{
			cstr.Format(_T("%d C (%d F)"), m_Ata.vars[i].Temperature, m_Ata.vars[i].Temperature * 9 / 5 + 32);
		}
		else
		{
			cstr = i18n(_T("Dialog"), _T("UNKNOWN"));
		}
		drive.Replace(_T("%TEMPERATURE%"), cstr);

		if(m_Ata.vars[i].TotalDiskSize < 1000)
		{
			cstr.Format(_T("%.2f GB (%.2f/----/----)"),
				m_Ata.vars[i].TotalDiskSize / 1000.0,
				m_Ata.vars[i].DiskSizeChs / 1000.0);
		}
		else if(m_Ata.vars[i].IsLba48Supported)
		{
			cstr.Format(_T("%.1f GB (%.1f/%.1f/%.1f)"),
				m_Ata.vars[i].TotalDiskSize / 1000.0,
				m_Ata.vars[i].DiskSizeChs / 1000.0,
				m_Ata.vars[i].DiskSizeLba28 / 1000.0,
				m_Ata.vars[i].DiskSizeLba48 / 1000.0
				);
		}
		else
		{
			cstr.Format(_T("%.1f GB (%.1f/%.1f/----)"),
				m_Ata.vars[i].TotalDiskSize / 1000.0,
				m_Ata.vars[i].DiskSizeChs / 1000.0,
				m_Ata.vars[i].DiskSizeLba28 / 1000.0
				);
		}
		drive.Replace(_T("%TOTAL_DISK_SIZE%"), cstr);


		if(m_Ata.vars[i].IsSsd && m_Ata.vars[i].BufferSize == 0xFFFF * 512)
		{
			cstr.Format(_T(">= %d KB"), m_Ata.vars[i].BufferSize / 1024);
		}
		else if(m_Ata.vars[i].BufferSize > 0)
		{
			cstr.Format(_T("%d KB"), m_Ata.vars[i].BufferSize / 1024);
		}
		else
		{
			cstr = i18n(_T("Dialog"), _T("UNKNOWN"));
		}
		drive.Replace(_T("%BUFFER_SIZE%"), cstr);

		if(m_Ata.vars[i].NvCacheSize > 0)
		{
			cstr.Format(_T("%d MB"), m_Ata.vars[i].NvCacheSize / 1024 / 1024);
		}
		else
		{
			cstr = _T("----");
		}
		drive.Replace(_T("%NV_CACHE_SIZE%"), cstr);

		if(m_Ata.vars[i].NominalMediaRotationRate == 1)
		{
			cstr = _T("---- (SSD)");
		}
		else if(m_Ata.vars[i].NominalMediaRotationRate > 0)
		{
			cstr.Format(_T("%d RPM"), m_Ata.vars[i].NominalMediaRotationRate);
		}		
		else
		{
			cstr = i18n(_T("Dialog"), _T("UNKNOWN"));
		}
		drive.Replace(_T("%ROTATION_RATE%"), cstr);


		feature = _T("");
		if(m_Ata.vars[i].IsSmartSupported)
		{
			feature += _T("S.M.A.R.T., ");
		}
		if(m_Ata.vars[i].IsApmSupported)
		{
			feature += _T("APM, ");
		}
		if(m_Ata.vars[i].IsAamSupported)
		{
			feature += _T("AAM, ");
		}
		if(m_Ata.vars[i].IsLba48Supported)
		{
			feature += _T("48bit LBA, ");
		}
		if(m_Ata.vars[i].IsNcqSupported)
		{
			feature += _T("NCQ, ");
		}
		if(m_Ata.vars[i].IsTrimSupported)
		{
			feature += _T("TRIM, ");
		}

		/*
		if(m_Ata.vars[i].IsNvCacheSupported)
		{
			feature += _T("NV Cache, ");
		}

		if(m_Ata.vars[i].IsSsd)
		{
			feature += _T("SSD, ");
		}
		*/
		
		if(! feature.IsEmpty())
		{
			feature.Delete(feature.GetLength() - 2, 2);
		}
		drive.Replace(_T("%SUPPORTED_FEATURE%"), feature);

#ifdef BENCHMARK
		cstr.Format(_T("%.1f MB/s"), m_Ata.vars[i].Speed);
		drive.Replace(_T("%BENCHMARK%"), cstr);
#endif
		if(m_Ata.vars[i].IsAamSupported)
		{
			cstr.Format(_T("%04Xh"), m_Ata.vars[i].IdentifyDevice.AcoustricManagement);
			if(m_Ata.vars[i].IsAamEnabled)
			{
				cstr += _T(" [ON]");
			}
			else
			{
				cstr += _T(" [OFF]");
			}
		}
		else
		{
			cstr = _T("----");
		}
		drive.Replace(_T("%AAM_LEVEL%"), cstr);

		if(m_Ata.vars[i].IsApmSupported)
		{
			cstr.Format(_T("%04Xh"), m_Ata.vars[i].IdentifyDevice.CurrentPowerManagement);
			if(m_Ata.vars[i].IsApmEnabled)
			{
				cstr += _T(" [ON]");
			}
			else
			{
				cstr += _T(" [OFF]");
			}
		}
		else
		{
			cstr = _T("----");
		}
		drive.Replace(_T("%APM_LEVEL%"), cstr);

		clip += drive;

		if(m_Ata.vars[i].AttributeCount > 0)
		{
			cstr.Format(_T("-- S.M.A.R.T. --------------------------------------------------------------\r\n"));
			line = cstr;

			if(m_Ata.vars[i].IsRawValues8)
			{
				line += _T("ID Raw Values (8)   Attribute Name\r\n");
			}
			else
			{
				line += _T("ID Cur Wor Thr RawValues(6) Attribute Name\r\n");
			}

			for(DWORD j = 0; j < m_Ata.vars[i].AttributeCount; j++)
			{
				TCHAR str[256];
				TCHAR unknown[256];
				TCHAR vendorSpecific[256];
				cstr.Format(_T("%02X"), m_Ata.vars[i].Attribute[j].Id);
				GetPrivateProfileString(_T("Smart"), _T("UNKNOWN"), _T("Unknown"), unknown, 256, m_CurrentLangPath);
				GetPrivateProfileString(_T("Smart"), _T("VENDOR_SPECIFIC"), _T("Vendor Specific"), vendorSpecific, 256, m_CurrentLangPath);

				BYTE id = m_Ata.vars[i].Attribute[j].Id;

				if(id == 0xBB || id == 0xBD || id == 0xBE || id == 0xE5
				|| (0xE8 <= id && id <= 0xEF) || (0xF1 <= id && id <= 0xF9) || (0xFB <= id && id <= 0xFF))
				{
					GetPrivateProfileString(m_Ata.vars[i].SmartKeyName, cstr, vendorSpecific, str, 256, m_CurrentLangPath);
				}
				else
				{
					GetPrivateProfileString(m_Ata.vars[i].SmartKeyName, cstr, unknown, str, 256, m_CurrentLangPath);
				}

				if(m_Ata.vars[i].VendorId == m_Ata.SSD_VENDOR_JMICRON)
				{
					cstr.Format(_T("%02X %s %02X%02X%02X%02X%02X%02X%02X%02X %s\r\n"),
						m_Ata.vars[i].Attribute[j].Id,
						__Number(m_Ata.vars[i].Attribute[j].CurrentValue),
						m_Ata.vars[i].Attribute[j].Reserved,
						m_Ata.vars[i].Attribute[j].RawValue[5],
						m_Ata.vars[i].Attribute[j].RawValue[4],
						m_Ata.vars[i].Attribute[j].RawValue[3],
						m_Ata.vars[i].Attribute[j].RawValue[2],
						m_Ata.vars[i].Attribute[j].RawValue[1],
						m_Ata.vars[i].Attribute[j].RawValue[0],
						m_Ata.vars[i].Attribute[j].WorstValue,
						str
						);
				}
				else if(m_Ata.vars[i].VendorId == m_Ata.SSD_VENDOR_INDILINX)
				{
					cstr.Format(_T("%02X %02X%02X%02X%02X%02X%02X%02X%02X %s\r\n"),
						m_Ata.vars[i].Attribute[j].Id,
						m_Ata.vars[i].Attribute[j].RawValue[5],
						m_Ata.vars[i].Attribute[j].RawValue[4],
						m_Ata.vars[i].Attribute[j].RawValue[3],
						m_Ata.vars[i].Attribute[j].RawValue[2],
						m_Ata.vars[i].Attribute[j].RawValue[1],
						m_Ata.vars[i].Attribute[j].RawValue[0],
						m_Ata.vars[i].Attribute[j].WorstValue,
						m_Ata.vars[i].Attribute[j].CurrentValue,
						str
						);
				}
				else
				{
					cstr.Format(_T("%02X %s %s %s %02X%02X%02X%02X%02X%02X %s\r\n"),
						m_Ata.vars[i].Attribute[j].Id,
						__Number(m_Ata.vars[i].Attribute[j].CurrentValue),
						__Number(m_Ata.vars[i].Attribute[j].WorstValue),
						__Number(m_Ata.vars[i].Threshold[j].ThresholdValue),
						m_Ata.vars[i].Attribute[j].RawValue[5],
						m_Ata.vars[i].Attribute[j].RawValue[4],
						m_Ata.vars[i].Attribute[j].RawValue[3],
						m_Ata.vars[i].Attribute[j].RawValue[2],
						m_Ata.vars[i].Attribute[j].RawValue[1],
						m_Ata.vars[i].Attribute[j].RawValue[0],
						str
						);
				}
				line += cstr;
			}
			clip += line;
			clip += _T("\r\n");
		}

		WORD data[256];
		if(m_FlagDumpIdentifyDevice)
		{
			memcpy(data, &(m_Ata.vars[i].IdentifyDevice), 512);
			if(m_FlagHideSerialNumber)
			{
				for(int j = 10; j < 19; j++) // Serial Number : WORD[10-19]
				{
					data[j] = 0xFFFF;
				}
			}
			cstr.Format(_T("-- IDENTIFY_DEVICE ---------------------------------------------------------\r\n"));
			line = cstr;
			line += _T("     +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F\r\n");
			for(int k = 0; k < 32; k++)
			{
				cstr.Format(_T("%03X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\r\n"),
								k * 16,
								LOBYTE(data[8 * k + 0]), HIBYTE(data[8 * k + 0]),
								LOBYTE(data[8 * k + 1]), HIBYTE(data[8 * k + 1]),
								LOBYTE(data[8 * k + 2]), HIBYTE(data[8 * k + 2]),
								LOBYTE(data[8 * k + 3]), HIBYTE(data[8 * k + 3]),
								LOBYTE(data[8 * k + 4]), HIBYTE(data[8 * k + 4]),
								LOBYTE(data[8 * k + 5]), HIBYTE(data[8 * k + 5]),
								LOBYTE(data[8 * k + 6]), HIBYTE(data[8 * k + 6]),
								LOBYTE(data[8 * k + 7]), HIBYTE(data[8 * k + 7]));
				line += cstr;
			}
			clip += line;
			clip += _T("\r\n");
		}

		if(m_FlagDumpSmartReadData)
		{
			memcpy(data, &(m_Ata.vars[i].SmartReadData), 512);
			cstr.Format(_T("-- SMART_READ_DATA ---------------------------------------------------------\r\n"));
			line = cstr;
			line += _T("     +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F\r\n");
			for(int k = 0; k < 32; k++)
			{
				cstr.Format(_T("%03X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\r\n"),
								k * 16,
								LOBYTE(data[8 * k + 0]), HIBYTE(data[8 * k + 0]),
								LOBYTE(data[8 * k + 1]), HIBYTE(data[8 * k + 1]),
								LOBYTE(data[8 * k + 2]), HIBYTE(data[8 * k + 2]),
								LOBYTE(data[8 * k + 3]), HIBYTE(data[8 * k + 3]),
								LOBYTE(data[8 * k + 4]), HIBYTE(data[8 * k + 4]),
								LOBYTE(data[8 * k + 5]), HIBYTE(data[8 * k + 5]),
								LOBYTE(data[8 * k + 6]), HIBYTE(data[8 * k + 6]),
								LOBYTE(data[8 * k + 7]), HIBYTE(data[8 * k + 7]));
				line += cstr;
			}
			clip += line;
			clip += _T("\r\n");
		}

		if(m_FlagDumpSmartReadData)
		{
			memcpy(data, &(m_Ata.vars[i].SmartReadThreshold), 512);
			cstr.Format(_T("-- SMART_READ_THRESHOLD ----------------------------------------------------\r\n"));
			line = cstr;
			line += _T("     +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F\r\n");
			for(int k = 0; k < 32; k++)
			{
				cstr.Format(_T("%03X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\r\n"),
								k * 16,
								LOBYTE(data[8 * k + 0]), HIBYTE(data[8 * k + 0]),
								LOBYTE(data[8 * k + 1]), HIBYTE(data[8 * k + 1]),
								LOBYTE(data[8 * k + 2]), HIBYTE(data[8 * k + 2]),
								LOBYTE(data[8 * k + 3]), HIBYTE(data[8 * k + 3]),
								LOBYTE(data[8 * k + 4]), HIBYTE(data[8 * k + 4]),
								LOBYTE(data[8 * k + 5]), HIBYTE(data[8 * k + 5]),
								LOBYTE(data[8 * k + 6]), HIBYTE(data[8 * k + 6]),
								LOBYTE(data[8 * k + 7]), HIBYTE(data[8 * k + 7]));
				line += cstr;
			}
			clip += line;
			clip += _T("\r\n");
		}
	}

	if(OpenClipboard())
	{
		HGLOBAL clipbuffer;
		TCHAR* buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, sizeof(TCHAR) * (clip.GetLength() + 1));
		buffer = (TCHAR*)GlobalLock(clipbuffer);
		_tcscpy_s(buffer, clip.GetLength() + 1, LPCTSTR(clip));
		GlobalUnlock(clipbuffer);
#ifdef _UNICODE
		SetClipboardData(CF_UNICODETEXT, clipbuffer);
#else
		SetClipboardData(CF_OEMTEXT, clipbuffer);
#endif
		CloseClipboard();
	}
}