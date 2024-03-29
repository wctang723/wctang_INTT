//____________________________________________________________________________..
//
// This is a template for a Fun4All SubsysReco module with all methods from the
// $OFFLINE_MAIN/include/fun4all/SubsysReco.h baseclass
// You do not have to implement all of them, you can just remove unused methods
// here and in hot_channel_finder_wctang.h.
//
// hot_channel_finder_wctang(const std::string &name = "hot_channel_finder_wctang")
// everything is keyed to hot_channel_finder_wctang, duplicate names do work but it makes
// e.g. finding culprits in logs difficult or getting a pointer to the module
// from the command line
//
// hot_channel_finder_wctang::~hot_channel_finder_wctang()
// this is called when the Fun4AllServer is deleted at the end of running. Be
// mindful what you delete - you do loose ownership of object you put on the node tree
//
// int hot_channel_finder_wctang::Init(PHCompositeNode *topNode)
// This method is called when the module is registered with the Fun4AllServer. You
// can create historgrams here or put objects on the node tree but be aware that
// modules which haven't been registered yet did not put antyhing on the node tree
//
// int hot_channel_finder_wctang::InitRun(PHCompositeNode *topNode)
// This method is called when the first event is read (or generated). At
// this point the run number is known (which is mainly interesting for raw data
// processing). Also all objects are on the node tree in case your module's action
// depends on what else is around. Last chance to put nodes under the DST Node
// We mix events during readback if branches are added after the first event
//
// int hot_channel_finder_wctang::process_event(PHCompositeNode *topNode)
// called for every event. Return codes trigger actions, you find them in
// $OFFLINE_MAIN/include/fun4all/Fun4AllReturnCodes.h
//   everything is good:
//     return Fun4AllReturnCodes::EVENT_OK
//   abort event reconstruction, clear everything and process next event:
//     return Fun4AllReturnCodes::ABORT_EVENT; 
//   proceed but do not save this event in output (needs output manager setting):
//     return Fun4AllReturnCodes::DISCARD_EVENT; 
//   abort processing:
//     return Fun4AllReturnCodes::ABORT_RUN
// all other integers will lead to an error and abort of processing
//
// int hot_channel_finder_wctang::ResetEvent(PHCompositeNode *topNode)
// If you have internal data structures (arrays, stl containers) which needs clearing
// after each event, this is the place to do that. The nodes under the DST node are cleared
// by the framework
//
// int hot_channel_finder_wctang::EndRun(const int runnumber)
// This method is called at the end of a run when an event from a new run is
// encountered. Useful when analyzing multiple runs (raw data). Also called at
// the end of processing (before the End() method)
//
// int hot_channel_finder_wctang::End(PHCompositeNode *topNode)
// This is called at the end of processing. It needs to be called by the macro
// by Fun4AllServer::End(), so do not forget this in your macro
//
// int hot_channel_finder_wctang::Reset(PHCompositeNode *topNode)
// not really used - it is called before the dtor is called
//
// void hot_channel_finder_wctang::Print(const std::string &what) const
// Called from the command line - useful to print information when you need it
//
//____________________________________________________________________________..

#include "hot_channel_finder_wctang.h"
using namespace std;

//____________________________________________________________________________..
hot_channel_finder_wctang::hot_channel_finder_wctang(const std::string &name):
	SubsysReco(name),
	output_(nullptr)
{
	output_name_ = "hot_channel_find_cosmic.root";
	std::cout << "hot_channel_finder_wctang::hot_channel_finder_wctang(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
hot_channel_finder_wctang::~hot_channel_finder_wctang()
{
	std::cout << "hot_channel_finder_wctang::~hot_channel_finder_wctang() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::Init(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::Init(PHCompositeNode *topNode) Initializing" << std::endl;

	//memset(ch_hit_arr, 0, sizeof(ch_hit_arr));
	output_ = new TFile(output_name_.c_str(), "RECREATE");
	hist_hit_num_ = new TH1D("hist_hit_num_","#INTTRAWHIT;The number of INTTRAWHIT;Entries", 50, 0, 400);
	//hist_cluster_ = new TH1D("hist_cluster", "Number of cluster distribution;#Cluster;Entries", 100, 0, 100);
	hist_chanhit_2Dmap_ = new TH2D("hist_chanhit_2Dmap_","RawHit map", 128, 0, 127, 26, 1, 27);
	hist_chanhit_2Dmap_->SetStats(0);

	c1 = new TCanvas("c1", "c1", 900, 800);
	gPad->SetTopMargin(0.07);
    gPad->SetBottomMargin(0.12);
    gPad->SetLeftMargin(0.14);
    gPad->SetRightMargin(0.14);

	tree_ = new TTree( "tree_INTT_raw_hit", "TTree for INTTRAWHIT information" );
	tree_->Branch( "pid", &pid_, "pid/I" );
	tree_->Branch( "fee", &fee_, "fee/I" );
	tree_->Branch( "chip_id", &chip_id_, "chip_id/I" );
	tree_->Branch( "chan_id", &chan_id_, "chan_id/I" );
	tree_->Branch( "bco_full", &bco_full_, "bco_full/L" );
	tree_->Branch( "bco", &bco_, "bco/I" );
	tree_->Branch( "diff", &diff_, "diff/I" );

	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::InitRun(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::process_event(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;

	// Getting Nodes

	// Getting TRKR_CLUSTER node
	/*
	auto *node_cluster_map = findNode::getClass<TrkrClusterContainerv4>(topNode, "TRKR_CLUSTER");
	if (!node_cluster_map) {
		std::cerr << PHWHERE << "TrkrClusterContainer node is missing!" << std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}
	*/

	// Geting ActsGeometry node
	/*
	ActsGeometry *node_acts = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");
	if (!node_acts) {
		std::cout << PHWHERE << "No ActsGeometry on node tree. Bailing." << std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}
	*/

	//* Getting INTTRAWHIT node
	string node_name = "INTTRAWHIT";
	auto *node_inttrawhit_map = findNode::getClass<InttRawHitContainer>(topNode, node_name);

  	if (!node_inttrawhit_map)
    {
      cerr << PHWHERE << node_name << " node is missing." << endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

	//* Loop over TrkrClusters
	/*
	std::vector <TrkrCluster*> clusters;
	for (unsigned int inttlayer = 0; inttlayer < 4; inttlayer++) {
		//std::cout << "INTT layer" << inttlayer << std::endl;
		//int layer = (inttlayer < 2 ? 0 : 1);


		//* Loop over all hits
		for (const auto &hitsetkey : node_cluster_map->getHitSetKeys(TrkrDefs::TrkrId::inttId, inttlayer + 3)) {
			//type: std::pair<ConstIterator, ConstIterator> ConstRange
			//here, MMap::const_iterator ConstIterator;
			auto range = node_cluster_map->getClusters(hitsetkey);


			//* Loop over iterators of this cluster
			for (auto clusIter = range.first; clusIter != range.second; clusIter++) {
				const auto cluskey = clusIter->first;
				const auto cluster = clusIter->second;
				clusters.push_back(cluster);

				const auto globalPos = node_acts->getGlobalPosition(cluskey, cluster);

				//int ladder_z = InttDefs::getLadderZId(cluskey);
				//int ladder_phi = InttDefs::getLadderPhiId(cluskey);
				int size = cluster->getSize();

				//if (nCluster < 5)
				if (Verbosity() > 5) {
					std::cout << "xyz("
							  << std::setprecision(4) << std::setw(8) << cluster->getPosition(0) << ", "
							  << std::setprecision(4) << std::setw(8) << cluster->getPosition(1) << ", "
							  << std::setprecision(4) << std::setw(8) << cluster->getPosition(2) << ") "
							  << "local xy(" 
							  << std::setprecision(4) << std::setw(8) << cluster->getLocalX() << ", "
							  << std::setprecision(4) << std::setw(8) << cluster->getLocalY() << ")\t "
							  << cluster->getAdc() << " "
							  << size << " "
							  << inttlayer << " "
							  //<< ladder_z << " "
							  //<< ladder_phi << " "
							  << std::endl;
				}

				cluster->setPosition(0, globalPos.x());
				cluster->setPosition(1, globalPos.y());
				cluster->setPosition(2, globalPos.z());
			}
		}
	}

	std::cout << clusters.size() << "Clusters in this event" << std::endl;
	hist_cluster_->Fill(clusters.size());
	*/

	int hit_num = node_inttrawhit_map->get_nhits();
  	cout << "# of INTTRAWHIT: " << hit_num << endl;
	cout << "pid : " << "bco_full : " << "felix module : " << "chip_id : " << "chan_id : " << "adc : " << "bco : " << "diff " << endl;
	
	//int ladder_number = 0;
	int pre_pid_ = 0;
	int pre_fee_ = 0;
	int pre_chip_id_ = 0;
	int pre_chan_id_ = 0;

	for (int i=0; i<hit_num; i++) {
		InttRawHit* hit = node_inttrawhit_map->get_hit( i );

		//* Get and assign hit information
		pid_ = hit->get_packetid();					//* pid: 3001~3008
		fee_ = hit->get_fee();						//* felix mod: 0~13
		int chip_id_raw = hit->get_chip_id();		//* chip id: 1~26
		if (chip_id_raw < 27) {chip_id_ = chip_id_raw;} else {chip_id_ = chip_id_raw - 26;} //* get the real chip id
		chan_id_ = hit->get_channel_id();			//* chan id: 0~127
		bco_full_ = hit->get_bco();
		bco_ = hit->get_FPHX_BCO();
		diff_ = abs( (bco_full_ & 0x7f) - bco_ );

		//* Filling TTree
      	tree_->Fill();



		//ladder_number = ((pid_ - 3000)* 100) + (fee_ + 1);		//* used to know which half ladder it is
		int pid_n = pid_ - 3001;			//* fix pid to 0~8
		int chip_id_n = chip_id_ - 1;		//* fix chip id to 0~25

		//* Compare the info from the previous hit, if all the parameters are the same, it is the clone hit
		if (pre_pid_ != pid_ || pre_fee_ != fee_ || pre_chip_id_ != chip_id_ || pre_chan_id_ != chan_id_) {
			ch_hit_arr[pid_n][fee_][chip_id_n][chan_id_] += 1;
		} else {clonehit_num++;}
		
		cout
		<< std::setw(  1 ) << hit->get_packetid() << " "  					//* FELIX server
		<< std::setw( 12 ) << hit->get_bco() << " "       					//* so_called bco_full
		//<< std::setw( 10 ) << hit->get_word() << " "   					//* the data from FELIX
		<< std::setw(  7 ) << hit->get_fee() << " "       					//* module or felix ch
		<< std::setw( 10 ) << hit->get_chip_id() << " "   					//* chip ID
		<< std::setw(  8 ) << hit->get_channel_id() << " "					//* channel ID
		<< std::setw(  8 ) << hit->get_adc() << " "       					//* ADC
		<< std::setw(  5 ) << hit->get_FPHX_BCO() << " "  					//* bco (hit bco, from 0 - 127)
		<< std::setw(  3 ) << (hit->get_bco()&0x7f) - hit->get_FPHX_BCO()	//* the BCO difference
		//<< std::setw(  1 ) << hit->get_full_FPHX() << " " 				//* ?
		//<< std::setw(  1 ) << hit->get_full_ROC() << " " 				//* ?
		//<< std::setw(  2 ) << hit->get_amplitude() << " " 				//* only for calibration
		<< endl;
		
		//cout << "# of hit of a channel:" << ch_hit_arr[pid_n][fee_][chip_id_n][chan_id_] << endl;
		//cout << "ladder number: " << ladder_number << endl;

		//* Saved the info from the previous hit
		pre_pid_ = pid_;
		pre_fee_ = fee_;
		pre_chip_id_ = chip_id_;
		pre_chan_id_ = chan_id_;
    }  

	//* Filling histogram for 2D hist
 	hist_hit_num_->Fill( hit_num );

	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::ResetEvent(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::EndRun(const int runnumber)
{
	std::cout << "hot_channel_finder_wctang::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::End(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::End(PHCompositeNode *topNode) This is the End..." << std::endl;

	c1->cd();
	c1->Print("Channel_raw_hit_dist_2D.pdf(");
	for (int i0 = 0; i0 < 8; i0++) {					//* i0 is pid
		for (int i1 = 0; i1 < 14; i1++){				//* i1 is felix module
			for (int i2 = 0; i2 < 26; i2++){			//* i2 is chip_id - 1
				for (int i3 = 0; i3 < 128; i3++){		//* i3 is channel_id
					if (ch_hit_arr[i0][i1][i2][i3] != 0) cout << "This is the hit for " << "pid = " << i0 << ", fee = " << i1 << " : " <<  ch_hit_arr[i0][i1][i2][i3] << endl;
					hist_chanhit_2Dmap_->SetBinContent(i3,i2+1,ch_hit_arr[i0][i1][i2][i3]);
				}
			}

			//* loop over every ladders (pid & felix module)
			c1->cd();
			hist_chanhit_2Dmap_->SetTitle(Form("intt%i-%i", i0, i1));
			hist_chanhit_2Dmap_->Draw("colz0");
			c1->Print("Channel_raw_hit_dist_2D.pdf");
			hist_chanhit_2Dmap_->Reset("ICESM");
		}
	}
	c1->cd();
	c1->Print("Channel_raw_hit_dist_2D.pdf)");
	c1->Clear();

	cout << "Clone hit numbers: " << clonehit_num << endl;


	output_->WriteTObject( hist_hit_num_, hist_hit_num_->GetName() );
	output_->WriteTObject( tree_, tree_->GetName() );
	output_->Close();

	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int hot_channel_finder_wctang::Reset(PHCompositeNode *topNode)
{
	std::cout << "hot_channel_finder_wctang::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void hot_channel_finder_wctang::Print(const std::string &what) const
{
	std::cout << "hot_channel_finder_wctang::Print(const std::string &what) const Printing info for " << what << std::endl;
}
