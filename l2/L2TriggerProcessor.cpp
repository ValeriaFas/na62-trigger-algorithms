/*
 * L2TriggerProcessor.cpp
 *
 *  Created on: Nov 17, 2011
 *      Author: Jonas Kunze (kunze.jonas@gmail.com)
 */

#include "L2TriggerProcessor.h"

#include <options/Logging.h>
#include <eventBuilding/Event.h>
#include <iostream>

namespace na62 {

uint_fast8_t L2TriggerProcessor::L1bypassTriggerWord;
uint_fast8_t L2TriggerProcessor::bypassTriggerWord;
double L2TriggerProcessor::bypassProbability;

void L2TriggerProcessor::initialize(double _bypassProbability,
		uint _bypassTriggerWord, uint _L1bypassTriggerWord) {
	bypassProbability = _bypassProbability;
	bypassTriggerWord = _bypassTriggerWord;
	L1bypassTriggerWord = _L1bypassTriggerWord;
}

uint8_t L2TriggerProcessor::compute(Event* event) {
	using namespace cream;

	/*
	 * Check if the event should bypass the processing
	 */
	if (event->getTriggerTypeWord() == L1bypassTriggerWord || bypassEvent()) {
		return bypassTriggerWord;
	}

//	std::vector<uint16_t> localCreamIDsToRequestNonZSuppressedData;
//	for (int localCreamID = event->getNumberOfZSuppressedLkrFragments() - 1;
//			localCreamID != -1; localCreamID--) {
//		LkrFragment* fragment = event->getZSuppressedLkrFragment(localCreamID);
//		localCreamIDsToRequestNonZSuppressedData.push_back(
//				fragment->getCrateCREAMID());
//		const char* data = fragment->getDataWithHeader();
//		const uint dataSize = fragment->getEventLength();
//	}

//	async_requestNonZSuppressedLKrData(localCreamIDsToRequestNonZSuppressedData, event);

// Accept event
	return 3;
}

uint8_t L2TriggerProcessor::onNonZSuppressedLKrDataReceived(Event* event) {
	LOG_INFO
			<< "onNonZSuppressedLKrDataReceived - Trigger method not yet implemented!!!!!!!!!!!!"
			<< ENDL;
	return 1;
}

void L2TriggerProcessor::async_requestNonZSuppressedLKrData(
		const std::vector<uint16_t> crateCREAMIDs, Event* event) {
//	event->setNonZSuppressedDataRequestedNum((uint16_t) crateCREAMIDs.size());
//	cream::L1DistributionHandler::Async_RequestLKRDataUnicast(event,
//	true, crateCREAMIDs);
}

} /* namespace na62 */
