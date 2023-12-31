#include "InttEvent.cc"
#include <vector>


using namespace std;

// some useful functions
string GetRunType( string data )
{
  /*!
    @brief A function to extract a run type from a file path
   */
  
  // For example,
  // /sphenix/tg/tg01/commissioning/INTT/merged_root_files/cosmics_inttall-00026073-0000_event_base_ana.root
  // -> cosmics_inttall-00026073-0000_event_base_ana.root
  string parts = data.substr( data.find_last_of( "/" ) + 1, data.size() );
  //cout << parts << endl;

  // For example,
  // cosmics_inttall-00026073-0000_event_base_ana.root
  // -> cosmics
  parts = parts.substr( 0, parts.find_first_of( "_" ) );
  //  cout << parts << endl;

  return parts;
}

// this is the main function
int hit_per_event ( string dataname = "/sphenix/tg/tg01/commissioning/INTT/merged_root_files/beam_inttall-00020867-0000_event_base_ana.root" ) {

  char* cstr = new char[dataname.length()];
  strcpy(cstr, dataname.c_str());
  vector<string> data_tok;

  char *p = strtok(cstr, "/_-.");
  while (p != NULL) {
      string str(p);
      data_tok.push_back(p);
      p = strtok(NULL, "/_-.");
  }

  // Open the ROOT file
  TFile* tf = new TFile( dataname.c_str(), "READ" );
  // If something wrong, show error message and quit
  if ( tf == nullptr ) {
    cerr << dataname << " cannot be opened." << endl;
    cerr << "Program is stopped." << endl;
    return -1;
  }

  // Get TTree from the ROOT file
  TTree* tree = (TTree*)tf->Get( "tree" );

  // Make an instance of InttEvent class and assign it to the branch of the TTree
  InttEvent* ev = new InttEvent();
  tree->SetBranchAddress( "event", &ev );

  // some variables for a counter
  int filled_event_counter = 0 ;
  int total_hit_num = 0;

  ofstream myfile;
  myfile.open("/sphenix/u/wctang/workspace/my_INTT/Anatest.txt", std::ios_base::app);
  if (myfile.is_open()) {
    //myfile << "This line is the header.\n";
    cerr << "The file is opened." << endl;
  } else {
    cerr << "There is no such file exist!" << endl;
  }

  // Loop over all events and do some processes
  const int kEvent_num = tree->GetEntries();
  for( int i=0; i<kEvent_num; i++ ) {
      // Store information of this event to the instance ev
      tree->GetEvent( i );
      //ev->show(); // If you want to see this event, use it

      // It's for counting the number of non-empty events
      if( ev->fNhits != 0 ) filled_event_counter++;

      // Add the number of hits in this event to the counter
      total_hit_num += ev->fNhits;

      // It's for debug
      // if( i > 3 )
      // 	break;
  }
  
  myfile << data_tok[10] << " " << filled_event_counter << " " << total_hit_num/kEvent_num << endl;
  myfile.close();

  // Print results in the terminal
  cout << filled_event_counter << " / " << kEvent_num << " = " << 1. * filled_event_counter / kEvent_num 
       << " had at least 1 hit." << endl;
  cout << "#hit / all events = "
       << total_hit_num << " / " << kEvent_num << " = "
       << 1. * total_hit_num / kEvent_num << endl;
  cout << "#hit / filled events = "
       << total_hit_num << " / " << filled_event_counter << " = "
       << 1. * total_hit_num / filled_event_counter << endl;
  
  return 0;
}
