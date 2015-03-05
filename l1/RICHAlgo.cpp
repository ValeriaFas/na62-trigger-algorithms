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
//#include "data_decoder/CedarHits.h"
#include "rich_algorithm/tdcb_buffer.h"

namespace na62 {

RICHAlgo::RICHAlgo() {
}
RICHAlgo::~RICHAlgo() {
}

uint8_t RICHAlgo::checkRICHTrigger(Event* event) {
	using namespace l0;

	TrbDecoder* richPacket = new TrbDecoder[maxNTEL62s]; //max NTel62 boards

	uint noHitsPerTrb[maxNTEL62s];

	uint nWordsPerFPGA[maxNFPGAs];

	uint* noFrame = new uint[maxNFPGAs];
	uint* noNonEmptyFrame = new uint[maxNFPGAs];
	uint* FPGAID = new uint[maxNFPGAs];
	uint* errFlags = new uint[maxNFPGAs];

	uint16_t** coarseFrameTime = new uint16_t*[maxNFPGAs];
	uint** nWordsPerFrame = new uint*[maxNFPGAs];

	for (uint i = 0; i < maxNFPGAs; i++) {
		coarseFrameTime[i] = new uint16_t[maxNFrames];
		nWordsPerFrame[i] = new uint[maxNFrames];
	}

	uint32_t* time = new uint32_t[maxNhits];
	uint* chID = new uint[maxNhits];
	uint* tdcID = new uint[maxNhits];
	uint* ID = new uint[maxNhits];
	uint* trbID = new uint[maxNhits];

	uint pp[maxNhits];
	uint tdc[maxNhits];
	uint box[maxNhits];

	for (uint i = 0; i < maxNTEL62s; i++) {
		noHitsPerTrb[i] = 0;
	}
	for (uint i = 0; i < maxNhits; i++) {
		time[i] = 0;
		chID[i] = 999;
		tdcID[i] = 999;
		ID[i] = 999;
		trbID[i] = 999;
		pp[i] = 999;
		tdc[i] = 999;
		box[i] = 999;
	}

	uint nWords = 0;
	uint nhits_tot = 0;
	uint nhits_tot_check = 0;

	for (uint iFPGA = 0; iFPGA < maxNFPGAs; iFPGA++) {
		nWordsPerFPGA[iFPGA] = 0;
	}

	uint chkmax = 0;

	LOG_INFO<< "Event number = " << event->getEventNumber() << ENDL;
	//LOG_INFO<< "Timestamp = " << std::hex << event->getTimestamp() << std::dec << ENDL;

	l0::Subevent* richSubevent = event->getRICHSubevent();
	LOG_INFO<<"NumberOfFragment " << richSubevent->getNumberOfFragments() <<ENDL;

	for (uint trbNum = 0;
			trbNum != richSubevent->getNumberOfFragments() && chkmax == 0;
			trbNum++) {
		l0::MEPFragment* trbDataFragment = richSubevent->getFragment(trbNum);

		richPacket[trbNum].SetHits(trbNum, trbDataFragment);

		noFrame = richPacket[trbNum].GetNoFrame();
		noNonEmptyFrame = richPacket[trbNum].GetNoNonEmptyFrame();
		FPGAID = richPacket[trbNum].GetFPGAID();
		errFlags = richPacket[trbNum].GetErrFlags();
		coarseFrameTime = richPacket[trbNum].GetCoarseFrameTime();
		nWordsPerFrame = richPacket[trbNum].GetNWordsPerFrame();
		time = richPacket[trbNum].GetTime();
		chID = richPacket[trbNum].GetChID();
		tdcID = richPacket[trbNum].GetTdcID();
		ID = richPacket[trbNum].GetID();
		trbID = richPacket[trbNum].GetTrbID();
		noHitsPerTrb[trbNum] = richPacket[trbNum].GetNoHitsPerTrb();

		for (uint iFPGA = 0; iFPGA < maxNFPGAs; iFPGA++) {

			//			printf("KtagAlgo.cpp: noFrame[%d] %d\n", iFPGA, noFrame[iFPGA]);
			//			printf("KtagAlgo.cpp: noNonEmptyFrame[%d] %d\n", iFPGA,
			//					noNonEmptyFrame[iFPGA]);
			//			printf("KtagAlgo.cpp: FPGAID[%d] %d\n", iFPGA, FPGAID[iFPGA]);
			//			printf("KtagAlgo.cpp: errFlags[%d] %d\n", iFPGA, errFlags[iFPGA]);

			for (uint iFrame = 0; iFrame < maxNFrames; iFrame++) {

				//				printf("KtagAlgo.cpp: coarseFrameTime[%d][%d] %04x\n", iFPGA,
				//						iFrame, coarseFrameTime[iFPGA][iFrame]);
				//				printf("KtagAlgo.cpp: nWordsPerFrame[%d][%d] %d\n", iFPGA,
				//						iFrame, nWordsPerFrame[iFPGA][iFrame]);

				//					coarseFrameTime[iFPGA][iFrame] += (event->getTimestamp()
				//						& 0xFFFF0000);
				//				if ((event->getTimestamp() & 0xf000) == 0xf000
				//					&& (coarseFrameTime[iFPGA][iFrame] & 0xf000) == 0x0000)
				//				coarseFrameTime[iFPGA][iFrame] += 0x10000; //16 bits overflow
				//			if ((event->getTimestamp() & 0xf000) == 0x0000
				//					&& (coarseFrameTime[iFPGA][iFrame] & 0xf000) == 0xf000)
				//				coarseFrameTime[iFPGA][iFrame] -= 0x10000; //16 bits overflow
				//				nWordsPerFPGA[iFPGA] += nWordsPerFrame[iFPGA][iFrame];

				//				nWords += nWordsPerFrame[iFPGA][iFrame];
				//
				//				if (nWordsPerFrame[iFPGA][iFrame])
				//					nhits_tot_check += (nWordsPerFrame[iFPGA][iFrame] - 1);
				//				else
				//					LOG_INFO<< "KtagAlgo.cpp::nWordsPerFrame is zero!"<< ENDL;
			}
		}

		for (uint ihit = 0; ihit < noHitsPerTrb[trbNum]; ihit++) {
			if (ID[ihit]) {
				//LOG_INFO<< "Hit " << ihit + nhits_tot << " ID " << ID[ihit] << ENDL;
				//LOG_INFO << "Hit " << ihit + nhits_tot << " time " << std::hex << time[ihit] << std::dec << ENDL;
				//LOG_INFO << "Hit " << ihit + nhits_tot << " chID " << chID[ihit] << ENDL;
				//LOG_INFO << "Hit " << ihit + nhits_tot << " tdcID " << tdcID[ihit] << ENDL;
				//LOG_INFO << "Hit " << ihit + nhits_tot << " trbID " << trbID[ihit] << ENDL;

				pp[ihit + nhits_tot] = tdcID[ihit] / 4;
				tdc[ihit + nhits_tot] = tdcID[ihit] % 4;
				//LOG_INFO << "pp[" << ihit + nhits_tot << "] " << pp[ihit + nhits_tot] << ENDL;
				//LOG_INFO << "tdc[" << ihit + nhits_tot << "] " << tdc[ihit + nhits_tot] << ENDL;

				int ROChannel = trbID[ihit] * 512 + tdcID[ihit] * 32
						+ chID[ihit];
				if (ID[ihit] == 4) {
					LOG_INFO<< "Event "<<event->getEventNumber()<< " Event TS " << event->getTimestamp() << " time " << time[ihit]<<" tel62 "<< trbID[ihit]<< " RO Channel "<< ROChannel<< "\t"<<ENDL;
				}
			}

			if (ihit == (noHitsPerTrb[trbNum] - 1)) {
				nhits_tot += noHitsPerTrb[trbNum];
			}

		}
		//LOG_INFO << event->getEventNumber() << " nHits " <<  nhits_tot << ENDL;
	}

	LOG_INFO<< event->getEventNumber() << " nHits " << nhits_tot << ENDL;
	//LOG_INFO << event->getEventNumber() << "\t" << std::hex << event->getTimestamp() << std::dec <<  "\t" << nSectors << ENDL;

	uint8_t kaontrigger = 0;

	return 0;

}
}
