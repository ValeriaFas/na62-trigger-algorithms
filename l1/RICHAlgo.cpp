/*
 * RICHAlgo.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: vfascian
 */

#include "RICHAlgo.h"

#include <eventBuilding/Event.h>
#include <eventBuilding/SourceIDManager.h>
#include <l0/MEPFragment.h>
#include <l0/Subevent.h>
#include <options/Logging.h>
#include "data_decoder/TrbDecoder.h"

namespace na62 {

RICHAlgo::RICHAlgo() {
}
RICHAlgo::~RICHAlgo() {
}

uint8_t RICHAlgo::checkRICHTrigger(Event* event) {
	using namespace l0;

	l0::Subevent* richSubevent = event->getRICHSubevent();

	uint nTel62 = richSubevent->getNumberOfFragments();

	LOG_INFO<<"nTel62 " << nTel62 <<ENDL;

	TrbDecoder* richPacket = new TrbDecoder[nTel62]; //max NTel62 boards

	uint noHitsPerTrb[nTel62];

	double* time;
	uint* chID;
	uint* tdcID;
	uint* ID;
	uint* trbID;

	uint pp[maxNhits];
	uint tdc[maxNhits];

	for (uint i = 0; i < nTel62; i++) {
		noHitsPerTrb[i] = 0;
	}

	for (uint i = 0; i < maxNhits; i++) {
		pp[i] = 999;
		tdc[i] = 999;
	}

	uint nhits_tot = 0;
	uint nTotLeadEdge = 0;
	uint chkmax = 0;

	LOG_INFO<< "Event number = " << event->getEventNumber() << ENDL;
	//LOG_INFO<< "Timestamp = " << std::hex << event->getTimestamp() << std::dec << ENDL;

	for (uint trbNum = 0;
			trbNum != richSubevent->getNumberOfFragments() && chkmax == 0;
			trbNum++) {

		l0::MEPFragment* trbDataFragment = richSubevent->getFragment(trbNum);

		richPacket[trbNum].GetData(trbNum, trbDataFragment, event);
		time = richPacket[trbNum].GetTimes();
		chID = richPacket[trbNum].GetChIDs();
		tdcID = richPacket[trbNum].GetTdcIDs();
		ID = richPacket[trbNum].GetIDs();
		trbID = richPacket[trbNum].GetTrbIDs();
		noHitsPerTrb[trbNum] = richPacket[trbNum].GetNoEdgesPerTrb();

		for (uint ihit = 0; ihit < noHitsPerTrb[trbNum]; ihit++) {

			int ROChannel = trbID[ihit + nhits_tot] * 512 + tdcID[ihit+nhits_tot] * 32 + chID[ihit+nhits_tot];

			if (ID[ihit+nhits_tot] == 4) {
				nTotLeadEdge += 1;
				LOG_INFO<<"Event TS " << event->getTimestamp() << " time " << time[ihit+nhits_tot]<<" tel62 "<< trbID[ihit+nhits_tot]<< " RO Channel "<< ROChannel<< "\t"<<ENDL;
			}

			if (ID[ihit+nhits_tot]) {
				//LOG_INFO<< "Hit " << ihit + nhits_tot << " ID " << ID[ihit] << ENDL;
				//LOG_INFO << "Hit " << ihit + nhits_tot << " time " << std::hex << time[ihit] << std::dec << ENDL;
				//LOG_INFO << "Hit " << ihit + nhits_tot << " chID " << chID[ihit] << ENDL;
				//LOG_INFO << "Hit " << ihit + nhits_tot << " tdcID " << tdcID[ihit] << ENDL;
				//LOG_INFO << "Hit " << ihit + nhits_tot << " trbID " << trbID[ihit] << ENDL;

				pp[ihit + nhits_tot] = tdcID[ihit] / 4;
				tdc[ihit + nhits_tot] = tdcID[ihit] % 4;
				//LOG_INFO << "pp[" << ihit + nhits_tot << "] " << pp[ihit + nhits_tot] << ENDL;
				//LOG_INFO << "tdc[" << ihit + nhits_tot << "] " << tdc[ihit + nhits_tot] << ENDL;

				//int ROChannel = trbID[ihit] * 512 + tdcID[ihit] * 32
				//		+ chID[ihit];

//				if (ID[ihit] == 4) {
//					nTotLeadEdge += 1;
//					LOG_INFO<<"Event TS " << event->getTimestamp() << " time " << time[ihit]<<" tel62 "<< trbID[ihit]<< " RO Channel "<< ROChannel<< "\t"<<ENDL;
//				}

				if (ihit == (noHitsPerTrb[trbNum] - 1)) {
					nhits_tot += noHitsPerTrb[trbNum];
				}

			}
		}
	}
	LOG_INFO<<" nLeadEdges " << nTotLeadEdge << ENDL;
	//LOG_INFO << event->getEventNumber() << " nHits " <<  nhits_tot << ENDL;

//LOG_INFO << event->getEventNumber() << "\t" << std::hex << event->getTimestamp() << std::dec <<  "\t" << nSectors << ENDL;

	//uint8_t kaontrigger = 0;

	return 0;

}
}
