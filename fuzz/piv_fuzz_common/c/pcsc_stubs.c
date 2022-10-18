#include <string.h>

#include <internal.h>
#include <ykpiv.h>

#include <harness.h>
#include "memcpy_rollover.h"

const SCARD_IO_REQUEST g_rgSCardT0Pci = { SCARD_PROTOCOL_T0, sizeof(SCARD_IO_REQUEST) };
const SCARD_IO_REQUEST g_rgSCardT1Pci = { SCARD_PROTOCOL_T1, sizeof(SCARD_IO_REQUEST) };
const SCARD_IO_REQUEST g_rgSCardRawPci = { SCARD_PROTOCOL_RAW, sizeof(SCARD_IO_REQUEST) };

LONG SCardEstablishContext(
    DWORD           dwScope,
    LPCVOID         pvReserved1,
    LPCVOID         pvReserved2,
    LPSCARDCONTEXT  phContext) {
    *phContext = (SCARDCONTEXT)1337;
    return SCARD_S_SUCCESS;
}

LONG SCardReleaseContext(SCARDCONTEXT hContext) {
    return SCARD_S_SUCCESS;
}

LONG SCardIsValidContext(SCARDCONTEXT hContext) {
    if (hContext == (SCARDCONTEXT)-1) {
        return SCARD_E_INVALID_HANDLE;
    } else {
        return SCARD_S_SUCCESS;
    }
}

LONG SCardDisconnect(
    SCARDHANDLE hCard,
    DWORD       dwDisposition) {
    return SCARD_LEAVE_CARD;
}

LONG SCardStatus(
    SCARDHANDLE hCard,
    LPSTR       mszReaderNames,
    LPDWORD     pcchReaderLen,
    LPDWORD     pdwState,
    LPDWORD     pdwProtocol,
    LPBYTE      pbAtr,
    LPDWORD     pcbAtrLen
) {
    return SCARD_S_SUCCESS;
}

LONG SCardConnect(
    SCARDCONTEXT    hContext,
    LPCSTR          szReader,
    DWORD           dwShareMode,
    DWORD           dwPreferredProtocols,
    LPSCARDHANDLE   phCard,
    LPDWORD         pdwActiveProtocol
) {
    return SCARD_S_SUCCESS;
}

LONG SCardTransmit(
    SCARDHANDLE hCard,
    const SCARD_IO_REQUEST *pioSendPci,
    LPCBYTE pbSendBuffer,
    DWORD cbSendLength,
    SCARD_IO_REQUEST *pioRecvPci,
    LPBYTE pbRecvBuffer,
    LPDWORD pcbRecvLength) {

    if (harness_state.test_case->pcsc_data != NULL && harness_state.test_case->pcsc_data_len > 0) {
        memcpy_rollover(
            pbRecvBuffer,
            harness_state.test_case->pcsc_data,
            *pcbRecvLength,
            harness_state.test_case->pcsc_data_len,
            &harness_state.pcsc_data_offset
        );
    } else {
        memset(pbRecvBuffer, 0, *pcbRecvLength);
    }

    return SCARD_S_SUCCESS;
}

LONG SCardListReaders(
    SCARDCONTEXT    hContext,
    LPCSTR          mszGroups,
    LPSTR           mszReaders,
    LPDWORD         pcchReaders
) {
    if (mszReaders != NULL) {
        if (harness_state.test_case->pcsc_data != NULL && harness_state.test_case->pcsc_data_len > 0) {
            memcpy_rollover(
                mszReaders,
                harness_state.test_case->pcsc_data,
                *pcchReaders,
                harness_state.test_case->pcsc_data_len,
                &harness_state.pcsc_data_offset
            );
        } else {
            memset(mszReaders, 0, *pcchReaders);
            *pcchReaders = 0;
        }
    }
    return SCARD_S_SUCCESS;
}