/*
 * PrlHandleDisk.cpp
 *
 * @modifier mperevedentsev
 *
 * Copyright (C) 2005-2016 Parallels IP Holdings GmbH
 *
 * This file is part of Parallels SDK. Parallels SDK is free
 * software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/> or write to Free Software Foundation,
 * 51 Franklin Street, Fifth Floor Boston, MA 02110, USA.
 *
 * Our contact details: Parallels IP Holdings GmbH, Vordergasse 59, 8200
 * Schaffhausen, Switzerland; http://www.parallels.com/.
 */

#include "PrlHandleDisk.h"
#include <prlcommon/VirtualDisk/Qcow2Disk.h>

using namespace VirtualDisk;

PrlHandleDisk::PrlHandleDisk() :
	PrlHandleBase(PHT_VIRTUAL_DISK), m_bLocalObject(false)
{
}

PrlHandleDisk::PrlHandleDisk(bool bLocal) :
	PrlHandleBase(PHT_VIRTUAL_DISK), m_bLocalObject(bLocal)
{
}

/**
 * Method let to create disk and return handle
 *
 * @param File name in UTF8
 * @param Parameters of the disk
 * @param Disk callback for long operations
 * @param Parameter for the callback
 *
 * @return Error code in PRL_RESULT format
 */
PRL_RESULT PrlHandleDisk::CreateDisk(
	// File name
	PRL_CONST_STR pFileName,
	// Parameters for image
	PRL_CONST_DISK_PARAMETERS_PTR pParams,
	// Callback for long procedures of image creation
	const PRL_DISK_CALLBACK pCallback,
	// Callback parameter
	PRL_CONST_VOID_PTR pAdditional)
{
	Q_UNUSED(pAdditional);

	// Unimplemented for non local objects
	if (!m_bLocalObject)
		return PRL_ERR_UNIMPLEMENTED;

	// Syncronious only
	if (pCallback != NULL)
		return PRL_ERR_UNIMPLEMENTED;

	VirtualDisk::Parameters::Disk params(pParams);
	// Call disk creation
	PRL_RESULT res = VirtualDisk::Qcow2::create(pFileName, params);
	if (PRL_FAILED(res))
		return res;

	m_pDisk = QSharedPointer<VirtualDisk::Format>(new VirtualDisk::Qcow2);
	if (PRL_FAILED(res = m_pDisk->open(pFileName, PRL_DISK_READ | PRL_DISK_WRITE)))
		m_pDisk.clear();

	return res;
};

/**
 * Method let to open disk and return handle
 *
 * @param File name in UTF8
 * @param Open flags
 *
 * @return Error code in PRL_RESULT format
 */
PRL_RESULT PrlHandleDisk::OpenDisk(
	// File name
	PRL_CONST_STR pFileName,
	// Open flags
	const PRL_DISK_OPEN_FLAGS OpenFlags)
{
	if (!m_bLocalObject)
		return PRL_ERR_UNIMPLEMENTED;

	m_pDisk = QSharedPointer<VirtualDisk::Format>(
			VirtualDisk::detectImageFormat(pFileName));
	if (!m_pDisk)
		return PRL_ERR_INVALID_ARG;

	PRL_RESULT res = m_pDisk->open(pFileName, OpenFlags);
	if (PRL_FAILED(res))
		m_pDisk.clear();

	return res;
};

/**
 * Wait for completion of the work
 *
 * @return Error code in PRL_RESULT format
 */
PRL_RESULT PrlHandleDisk::WaitForCompletion()
{
	return PRL_ERR_UNIMPLEMENTED;
};

/**
 * Start switching to state procedure
 *
 * @param UID of the state to switch to
 * @param Callback to signal progress/completion, if it is equal to NULL
 *        function will not return until completion
 * @param Callback parameter
 *
 * @return Error code in PRL_RESULT format
 */
PRL_RESULT PrlHandleDisk::SwitchToState(
		// UID of the state
		PRL_CONST_GUID_PTR pUid,
		// Callback
		const PRL_STATES_CALLBACK pCallback,
		// Parameter to callback
		PRL_VOID_PTR pParameter)
{
	Q_UNUSED(pUid);
	Q_UNUSED(pParameter);
	Q_UNUSED(pCallback);

	return PRL_ERR_UNIMPLEMENTED;
};

/**
 * Write data to disk
 *
 * @param Valid PRL_HANDLE
 * @param Pointer to sector/page aligned data to write (depends from host OS)
 * @param Size of data to write
 * @param Offset of the block on the disk in sectors
 *
 * @return PRL_RESULT.
 */
PRL_RESULT PrlHandleDisk::Write(
		// Block to write
		PRL_CONST_VOID_PTR pBlock,
		// Size of block
		const PRL_UINT32 uiSize,
		// Offset of block (in sectors)
		const PRL_UINT64 uiBlockOffset)
{
	// Check up
	if (!m_pDisk)
		return PRL_ERR_UNINITIALIZED;

	return m_pDisk->write(pBlock, uiSize, uiBlockOffset);
};

/**
 * Read data from disk
 *
 * @param Valid PRL_HANDLE
 * @param Pointer to sector/page aligned data to write (depends from host OS)
 * @param Size of data to write
 * @param Offset of the block on the disk in sectors
 *
 * @return PRL_RESULT.
 */
PRL_RESULT PrlHandleDisk::Read(
		// Block to write
		PRL_VOID_PTR pBlock,
		// Size of block
		const PRL_UINT32 uiSize,
		// Offset of block (in sectors)
		const PRL_UINT64 uiBlockOffset)
{
	// Check up
	if (!m_pDisk)
		return PRL_ERR_UNINITIALIZED;

	return m_pDisk->read(pBlock, uiSize, uiBlockOffset);
};

/**
 * Get disk information
 *
 * @param Valid PRL_HANDLE
 * @param Pointer to the result
 * @param Pointer to parameter value buffer size, if specified size is
 *        lower than needed, here will be returned corrected size
 *
 * @return PRL_RESULT.
 */
PRL_RESULT PrlHandleDisk::GetDiskInfo(
		// Pointer to memory, that receive parameters
		PRL_DISK_PARAMETERS *ppDDesc,
		// Parameter value buffer size
		PRL_UINT32_PTR BufferSize)
{
	// Check up
	if (!m_pDisk)
		return PRL_ERR_UNINITIALIZED;

	VirtualDisk::Parameters::disk_type params = m_pDisk->getInfo();
	if (params.isFailed())
		return params.error().code();

	if (*BufferSize)
		return params.value().fillBuffer(ppDDesc, *BufferSize);

	*BufferSize = params.value().getBufferSize();
	return PRL_ERR_SUCCESS;
};