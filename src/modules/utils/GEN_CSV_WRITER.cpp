/*******************************************************************************
 * Copyright (c) 2012 - 2015 ACIN, fortiss GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/
#include "GEN_CSV_WRITER.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_CSV_WRITER_gen.cpp"
#endif
#include <errno.h>
#include <devlog.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_CSV_WRITER, g_nStringIdGEN_CSV_WRITER);

const CStringDictionary::TStringId GEN_CSV_WRITER::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId GEN_CSV_WRITER::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING };

const CStringDictionary::TStringId GEN_CSV_WRITER::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TForteInt16 GEN_CSV_WRITER::scm_anEIWithIndexes[] = { 0, 3 };
const TDataIOID GEN_CSV_WRITER::scm_anEOWith[] = { 0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter };
const TForteInt16 GEN_CSV_WRITER::scm_anEOWithIndexes[] = { 0, 3, -1 };
const CStringDictionary::TStringId GEN_CSV_WRITER::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const CIEC_STRING GEN_CSV_WRITER::scmOK = "OK"_STRING;
const CIEC_STRING GEN_CSV_WRITER::scmFileAlreadyOpened = "File already opened"_STRING;
const CIEC_STRING GEN_CSV_WRITER::scmFileNotOpened = "File not opened"_STRING;

void GEN_CSV_WRITER::executeEvent(TEventID paEIID) {
  if(scm_nEventINITID == paEIID) {
    if(QI()) {
      openCSVFile();
    } else {
      closeCSVFile();
    }
    sendOutputEvent(scm_nEventINITOID);
  } else if(scm_nEventREQID == paEIID) {
    QO() = QI();
    if(QI()) {
      writeCSVFileLine();
    }
    sendOutputEvent(scm_nEventCNFID);
  }
}

GEN_CSV_WRITER::GEN_CSV_WRITER(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId), mCSVFile(nullptr), m_anDataInputNames(nullptr), m_anDataInputTypeIds(nullptr), m_anEIWith(nullptr){
}

GEN_CSV_WRITER::~GEN_CSV_WRITER(){
  delete[] m_anDataInputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anEIWith;
  closeCSVFile();
}

bool GEN_CSV_WRITER::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  const char *acPos = strrchr(paConfigString, '_');
  if(nullptr != acPos){
    acPos++;
    paInterfaceSpec.m_nNumDIs = static_cast<TPortId>(forte::core::util::strtoul(acPos, nullptr, 10) + 2); // we have in addition to the SDs a QI and FILE_NAME data inputs

    m_anDataInputNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDIs];
    m_anDataInputTypeIds = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDIs];

    m_anDataInputNames[0] = g_nStringIdQI;
    m_anDataInputTypeIds[0] = g_nStringIdBOOL;
    m_anDataInputNames[1] = g_nStringIdFILE_NAME;
    m_anDataInputTypeIds[1] = g_nStringIdSTRING;

    generateGenericDataPointArrays("SD_", &(m_anDataInputTypeIds[2]), &(m_anDataInputNames[2]), paInterfaceSpec.m_nNumDIs - 2);

    m_anEIWith = new TDataIOID[3 + paInterfaceSpec.m_nNumDIs];

    m_anEIWith[0] = 0;
    m_anEIWith[1] = 1;
    m_anEIWith[2] = scmWithListDelimiter;
    m_anEIWith[3] = 0;

    for(TDataIOID i = 2; i < paInterfaceSpec.m_nNumDIs; i++){
      m_anEIWith[i + 2] = i;
    }

    m_anEIWith[2 + paInterfaceSpec.m_nNumDIs] = scmWithListDelimiter;

    //create the interface Specification
    paInterfaceSpec.m_nNumEIs = 2;
    paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
    paInterfaceSpec.m_anEIWith = m_anEIWith;
    paInterfaceSpec.m_anEIWithIndexes = scm_anEIWithIndexes;
    paInterfaceSpec.m_nNumEOs = 2;
    paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
    paInterfaceSpec.m_anEOWith = scm_anEOWith;
    paInterfaceSpec.m_anEOWithIndexes = scm_anEOWithIndexes;
    paInterfaceSpec.m_aunDINames = m_anDataInputNames;
    paInterfaceSpec.m_aunDIDataTypeNames = m_anDataInputTypeIds;
    paInterfaceSpec.m_nNumDOs = 2;
    paInterfaceSpec.m_aunDONames = scm_anDataOutputNames;
    paInterfaceSpec.m_aunDODataTypeNames = scm_anDataOutputTypeIds;
    return true;
  }
  return false;
}

void GEN_CSV_WRITER::openCSVFile() {
  QO() = CIEC_BOOL(false);
  if(nullptr == mCSVFile) {
    mCSVFile = fopen(FILE_NAME().getStorage().c_str(), "w+");
    if(nullptr != mCSVFile) {
      QO() = CIEC_BOOL(true);
      STATUS() = scmOK;
      DEVLOG_INFO("[GEN_CSV_WRITER]: File %s successfully opened\n", FILE_NAME().getStorage().c_str());
    } else {
      const char* errorCode = strerror(errno); 
      STATUS() = CIEC_STRING(errorCode, strlen(errorCode));
      DEVLOG_ERROR("[GEN_CSV_WRITER]: Couldn't open file %s. Error: %s\n", FILE_NAME().getStorage().c_str(), STATUS().getStorage().c_str());
    }
  } else {
    STATUS() = scmFileAlreadyOpened;
    DEVLOG_ERROR("[GEN_CSV_WRITER]: Can't open file %s since it is already opened\n", FILE_NAME().getStorage().c_str());
  }
}

void GEN_CSV_WRITER::closeCSVFile() {
  QO() = CIEC_BOOL(false);
  if(nullptr != mCSVFile) {
    if(0 == fclose(mCSVFile)) {
      STATUS() = scmOK;
      DEVLOG_INFO("[GEN_CSV_WRITER]: File %s successfully closed\n", FILE_NAME().getStorage().c_str());
    } else {
      const char *errorCode = strerror(errno);
      STATUS() = CIEC_STRING(errorCode, strlen(errorCode));
      DEVLOG_ERROR("[GEN_CSV_WRITER]: Couldn't close file %s. Error: %s\n", FILE_NAME().getStorage().c_str(), STATUS().getStorage().c_str());
    }
    mCSVFile = nullptr;
  }
}

void GEN_CSV_WRITER::writeCSVFileLine() {
  if(nullptr != mCSVFile) {
    char acBuffer[scmWriteBufferSize];
    for(TPortId i = 2; i < mInterfaceSpec->m_nNumDIs; i++) {
      int nLen = getDI(i)->unwrap().toString(acBuffer, scmWriteBufferSize);
      if(nLen >= 0) {
        fwrite(acBuffer, 1, static_cast<size_t>(nLen), mCSVFile);
      } else {
        DEVLOG_ERROR("[GEN_CSV_WRITER]: Can't get string value for input %d\n", i);
      }
      fwrite("; ", 1, 2, mCSVFile);
    }
    fwrite("\n", 1, 1, mCSVFile);
  } else {
    QO() = CIEC_BOOL(false);
    STATUS() = scmFileNotOpened;
    DEVLOG_ERROR("[GEN_CSV_WRITER]: Can't write to file %s since it is not opened\n", FILE_NAME().getStorage().c_str());
  }
}
