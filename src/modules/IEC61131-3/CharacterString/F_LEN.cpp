/*******************************************************************************
 * Copyright (c) 2013 ACIN
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_LEN.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_LEN_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_LEN, g_nStringIdF_LEN)

const CStringDictionary::TStringId FORTE_F_LEN::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_LEN::scm_anDataInputTypeIds[] = {g_nStringIdANY_STRING};

const CStringDictionary::TStringId FORTE_F_LEN::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_LEN::scm_anDataOutputTypeIds[] = {g_nStringIdANY_INT};

const TDataIOID FORTE_F_LEN::scm_anEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LEN::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LEN::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_LEN::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LEN::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LEN::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_LEN::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_F_LEN::FORTE_F_LEN(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId),
    var_IN(CIEC_ANY_STRING_VARIANT()),
    var_OUT(CIEC_ANY_INT_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_LEN::executeEvent(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID:
      std::visit([](auto &&paIN, auto&&paOUT) -> void {
          paOUT = func_LEN(paIN);
      }, var_IN, var_OUT);
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_F_LEN::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_LEN::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_LEN::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LEN::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_LEN::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_LEN::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_LEN::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


