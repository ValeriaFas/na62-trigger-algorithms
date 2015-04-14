/*
 * RICHAlgo.h
 *
 *  Created on: Mar 2, 2015
 *      Author: vfascian
 */
#pragma once
#ifndef RICHALGO_H_
#define RICHALGO_H_

#include <sys/types.h>
#include <atomic>
#include <cstdint>

namespace na62 {

class Event;

class RICHAlgo {
public:
	/**
	 * @param event Event* This is a pointer to the built Event containing all subevents (except those from the LKr)
	 *
	 * The event pointer may not be deleted by this method as it is deleted by the EventBuilder
	 *
	 * @return uint8_t <0> if the event is rejected, the L1 trigger type word in other cases.
	 */

	RICHAlgo();
	~RICHAlgo();
	static uint8_t checkRICHTrigger(Event* event);
	static uint searchPMT(uint tel62ID, uint fpgaID);

private:

};

} /* namespace na62 */
#endif /* RICHALGORITHM_H_ */


