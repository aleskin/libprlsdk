/*
 * PrlHandleResult.h
 *
 * Copyright (C) 1999-2014 Parallels IP Holdings GmbH
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


#ifndef __PARALLELS_HANDLE_RESULT_H__
#define __PARALLELS_HANDLE_RESULT_H__


#include "PrlHandleBase.h"
#include <prlxmlmodel/Messaging/CResult.h>

/**
 * Base handle object - all objects representation on the client
 * side should follow it's interface by the means of inheritance.
 */
class PrlHandleResult : public PrlHandleBase
{

public:

	/**
	 * Class constructor.
	 * @param pointer to the server connection object
	 * @param pointer to the parent VM object (if result generating by VM request)
	 * @param reference to the result data object
	 */
	PrlHandleResult(const PrlHandleServerPtr& server, const PrlHandleVmPtr &pVm, const CResult& Result);

	/**
	 * Class destructor.
	 */
	~PrlHandleResult();


	/**
	 * Get count of parameters
	 * @param specifies a pointer to the count of parameters
	 * @return PRL_ERR_SUCCESS if the method succeeds
	 */
	PRL_RESULT GetParamsCount(PRL_UINT32_PTR pCount);

	/**
	 * Get param token by index
	 *
	 * @param index of parameter
	 * @param pointer to a command specific object that contains parameter value
	 * @return PRL_ERR_SUCCESS if the method succeeds
	 */
	 PRL_RESULT GetParamToken(PRL_UINT32 index, PRL_VOID_PTR_PTR data_ptr);

	/**
	 * Get error description
	 *
	 * @param pointer for getting result
	 * @return PRL_ERR_SUCCESS if the method succeeds
	 */
	 PRL_RESULT GetError(PRL_VOID_PTR_PTR data_ptr);

	/**
	 * Get param by index
	 *
	 * @param index of parameter
	 * @param pointer to buffer for storing handle to object that represents parameter
	 * @return PRL_ERR_SUCCESS if the method succeeds
	 */
	 PRL_RESULT GetParamByIndex(PRL_UINT32 nIndex, PRL_HANDLE_PTR phParam);

	/**
	 * Returns handle to result representation object
	 *
	 * @param pointer for getting result
	 * @return PRL_ERR_SUCCESS if the method succeeds
	 */
	 PRL_RESULT GetResultAsHandle(PRL_HANDLE_PTR pHandle);

	/**
	 * Get param string representation by index
	 *
	 * @param index of parameter
	 * @param pointer to string buffer
	 * @param pointer to string buffer size
	 * @return PRL_RESULT. Possible values:
	 * PRL_ERR_INVALID_ARG 		- out of range index was specified
	 * PRL_ERR_BUFFER_OVERRUN	- not enough buffer size to completely storing string value (in that case output
	 * buffer will be contain non full string value)
	 * PRL_ERR_SUCCESS 				- method executed successfully
	 */
	 PRL_RESULT GetParamByIndexAsString(PRL_UINT32 nIndex, PRL_STR sParamBuf, PRL_UINT32_PTR pnParamBufSize);

	/**
	 * Initializes result object from specified string XML representation
	 *
	 * @param [in] result object XML representation
	 * @return PRL_RESULT. Possible values:
	 *		PRL_ERR_INVALID_ARG		- wrong result object XML representation
	 *		PRL_ERR_SUCCESS				- operation completed successfully
	 */
	PRL_RESULT FromString(PRL_CONST_STR sXml);

private:
	/**
	 * Represents result as SDK object and returns handle to it.
	 */
	PRL_HANDLE ResultAsHandle();
	/**
	 * Internal helper that returns actual result parameters count
	 * Note: method using just for internal purposes and not contains class memebers access synchronization mech
	 */
	PRL_UINT32 InternalGetParamsCount();

private:

	/// Result object
	CResult m_Result;

	PrlHandleServerPtr m_pServer;

	/** Pointer to the parent VM object (if result was generated by VM request) */
	PrlHandleVmPtr m_pVm;
};

#endif // __PARALLELS_HANDLE_RESULT_H__
