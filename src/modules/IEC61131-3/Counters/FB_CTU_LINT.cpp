/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "FB_CTU_LINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_CTU_LINT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "forte_lint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_FB_CTU_LINT, g_nStringIdFB_CTU_LINT)

const CStringDictionary::TStringId FORTE_FB_CTU_LINT::scm_anDataInputNames[] = {g_nStringIdCU, g_nStringIdR, g_nStringIdPV};

const CStringDictionary::TStringId FORTE_FB_CTU_LINT::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLINT};

const CStringDictionary::TStringId FORTE_FB_CTU_LINT::scm_anDataOutputNames[] = {g_nStringIdQ, g_nStringIdCV};

const CStringDictionary::TStringId FORTE_FB_CTU_LINT::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdLINT};

const TDataIOID FORTE_FB_CTU_LINT::scm_anEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_CTU_LINT::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTU_LINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FB_CTU_LINT::scm_anEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_CTU_LINT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTU_LINT::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_FB_CTU_LINT::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  3, scm_anDataInputNames, scm_anDataInputTypeIds,
  2, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};


FORTE_FB_CTU_LINT::FORTE_FB_CTU_LINT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CSimpleFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_CU(CIEC_BOOL(0)),
    var_R(CIEC_BOOL(0)),
    var_PV(CIEC_LINT(0)),
    var_Q(CIEC_BOOL(0)),
    var_CV(CIEC_LINT(0)),
    var_conn_Q(var_Q),
    var_conn_CV(var_CV),
    conn_CNF(this, 0),
    conn_CU(nullptr),
    conn_R(nullptr),
    conn_PV(nullptr),
    conn_Q(this, 0, &var_conn_Q),
    conn_CV(this, 1, &var_conn_CV) {
}

void FORTE_FB_CTU_LINT::alg_REQ(void) {
  
  if (var_R) {
    var_CV = CIEC_LINT(0);
  }
  else if (func_AND<CIEC_BOOL>(var_CU, func_LT(var_CV, CIEC_LINT(std::numeric_limits<CIEC_LINT::TValueType>::max())))) {
    var_CV = func_ADD<CIEC_LINT>(var_CV, CIEC_LINT(1));
  }
  var_Q = func_GE(var_CV, var_PV);
}


void FORTE_FB_CTU_LINT::executeEvent(TEventID pa_nEIID){
  switch(pa_nEIID) {
    case scm_nEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scm_nEventCNFID);
}

void FORTE_FB_CTU_LINT::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_CU, conn_CU);
      readData(1, var_R, conn_R);
      break;
    }
    default:
      break;
  }
}

void FORTE_FB_CTU_LINT::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_Q, conn_Q);
      writeData(1, var_CV, conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_FB_CTU_LINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CU;
    case 1: return &var_R;
    case 2: return &var_PV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTU_LINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
    case 1: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_CTU_LINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_CTU_LINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CU;
    case 1: return &conn_R;
    case 2: return &conn_PV;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_CTU_LINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTU_LINT::getVarInternal(size_t) {
  return nullptr;
}


