/*******************************************************************************
 * Copyright (c) 2016, 2018 fortiss GmbH, TU Vienna/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Martin Melik-Merkumians - adds timed wait and try and no wait
 *    and documentation
 *******************************************************************************/

#include "forte_sem.h"
#include "../devlog.h"
#include <errno.h>
#include <string.h>

namespace forte {
  namespace arch {

    CEcosSemaphore::CEcosSemaphore(unsigned int paInitialValue) : mSemaphore(paInitialValue > 0){
    }

    CEcosSemaphore::~CEcosSemaphore(){
    }

    void CEcosSemaphore::inc(){
      mSemaphore->post();
    }

    void CEcosSemaphore::waitIndefinitely(){
      mSemaphore->wait();
    }

    bool CEcosSemaphore::timedWait(TForteUInt64 paRelativeTimeout){
      cyg_tick_count_t absWaitTime = cyg_current_time() + paRelativeTimeout * CYGNUM_HAL_RTC_NUMERATOR / CYGNUM_HAL_RTC_DENOMINATOR;
      do{
        if(true == mSemaphore->sem_trywait()){
          return true;
        }
      } while(cyg_current_time() < absWaitTime);
      return false;
    }

    bool CEcosSemaphore::tryNoWait(){
      return (0 != mSemaphore->trywait());
    }

  } /* namespace arch */
} /* namespace forte */
