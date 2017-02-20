/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Ben Schneider
 *      - initial implementation and documentation
 *******************************************************************************/

#ifndef _TRIGGER_SERVICE_CLIENT_H_
#define _TRIGGER_SERVICE_CLIENT_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_string.h>
#include <ros/ros.h>
#include <std_srvs/Trigger.h>
#include "ROSActionManager.h"

class FORTE_TRIGGER_SERVICE_CLIENT : public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_TRIGGER_SERVICE_CLIENT)

  private:

    bool m_Initiated;
    ros::NodeHandle* m_nh;
    ros::ServiceClient m_triggerClient;
    std_srvs::Trigger m_srv;
    std::string m_RosNamespace;
    std::string m_RosMsgName;

    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }
    ;

    CIEC_STRING &NAMESPACE(){
      return *static_cast<CIEC_STRING*>(getDI(1));
    }
    ;

    CIEC_STRING &SRVNAME(){
      return *static_cast<CIEC_STRING*>(getDI(2));
    }
    ;

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }
    ;

    CIEC_STRING &STATUS(){
      return *static_cast<CIEC_STRING*>(getDO(1));
    }
    ;

    CIEC_BOOL &SUCCESS(){
      return *static_cast<CIEC_BOOL*>(getDO(2));
    }
    ;

    CIEC_STRING &MESSAGE(){
      return *static_cast<CIEC_STRING*>(getDO(3));
    }
    ;

    static const TEventID scm_nEventINITID = 0;
    static const TEventID scm_nEventREQID = 1;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventINITOID = 0;
    static const TEventID scm_nEventCNFID = 1;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(2, 3, 4, 0)
    ;

    void executeEvent(int pa_nEIID);

  public:
    FUNCTION_BLOCK_CTOR(FORTE_TRIGGER_SERVICE_CLIENT), m_Initiated(false) , m_RosNamespace(""), m_RosMsgName(""){
    };

    virtual ~FORTE_TRIGGER_SERVICE_CLIENT(){};

  };

#endif //close the ifdef sequence from the beginning of the file
