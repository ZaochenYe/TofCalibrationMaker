#ifndef TOF_CALIBRATION_MAKER
#define TOF_CALIBRATION_MAKER


/**
 * JDB LIB
 */
#include "XmlConfig.h"
#include "Logger.h"
#include "DataSource.h"
#include "HistoBook.h"
#include "LoggerConfig.h"
#include "Reporter.h"
#include "XmlRange.h"
using namespace jdb;

/**
 * STL
 */
#include <memory>
using namespace std;

/**
 * ROOT
 */
#include "TMath.h"

/**
 * Local
 */
#include "TofCorrection.h"



class TofCalibrationMaker
{
protected:

	/**
	 * Config
	 */
	XmlConfig * cfg;
	string nodePath;

	unique_ptr<Logger> logger;

	unique_ptr<DataSource> ds;
	int nEventsToProcess;

	unique_ptr<HistoBook> book;
	unique_ptr<Reporter> reporter;


	string splitMode;
	unique_ptr<XmlRange> trayRange;
	unique_ptr<XmlRange> moduleRange;
	unique_ptr<XmlRange> cellRange;
	vector< unique_ptr<TofCorrection> > corrections;

	int nElements; // the number of separate calibrated elements 

	int iteration;

	const double cLight; //= 29.9792458 cm / ns
	const double mPi; // pi mass = 0.13957 in GeV / c^2

public:
	TofCalibrationMaker( XmlConfig * config, string np, string fileList ="", string jobPrefix = "" );
	~TofCalibrationMaker();

	void make();

	void alignT0();

	void fillTot();
	void correctTot();
	
	void fillZLocal();
	void correctZLocal();


	void inverseBeta();

	static const int nTrays;
	static const int nModules;
	static const int nCells;

protected:

	int absIndex( int tray, int module = 1, int cell = 1 );
	int relIndex( int tray, int module = 1, int cell = 1 );
	vector<int> fromRelIndex( int id ); // [0] = tray, [1] = module, [2] = cell

	void importZParams( string pFile );
	void importTotParams( string totFile );

	void exportT0Params( string pFile );
	void exportTotParams( string pFile );
	void exportZParams( string pFile );

	double expectedTof( double length, double p ){
		return TMath::Sqrt( length*length / (cLight*cLight) * ( 1 + mPi*mPi / (p*p) ) );
	}

	bool keepEvent();
	bool keepTrack( int iHit );

	void makeBins( string var, int nBins, double min, double max );


	/**
	 * Reports
	 */
	void reportTot();
	void reportZLocal();

	string nameFor( int rID ){

		vector<int> place = fromRelIndex( rID );
		if ( "tray" == splitMode )
			return "Tray "+ts(place[0]);
		else if ( "board" == splitMode )
			return "Tray "+ts(place[0]) + " Module " + ts( place[1] );
		else if ( "module" == splitMode )
			return "T"+ts(place[0]) + "M" + ts( place[1] );
		else if ( "cell" == splitMode )
			return "T"+ts(place[0]) + "M" + ts( place[1] )+ "C" + ts( place[2] );
		return "unknown";
	}
	
	
};

#endif