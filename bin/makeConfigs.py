#!/usr/bin/python

import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)


dataDir = sys.argv[ 1 ]


template = """
<?xml version="1.0" encoding="UTF-8"?>
<!-- A root node is required -->
<!--  It can be anything <root></root>, it just has to exist -->
<config>
	<job>calibrateTof</job>

	<Logger>
		<logLevel>info</logLevel>
	</Logger>

	<output>
		<data>dTof{treeIndex}.root</data>
	</output>

	<Reporter>
		<output url="rpTof{treeIndex}.pdf"/>
	</Reporter>

	<TofCalibration splitMode="cell" nIterations="6" >
		

		<Trays min="{tray}" max="{tray}" />
		<Modules min="{sMod}" max="{eMod}" />
		<Cells min="1" max="6" />

		<Spline tot="true" zLocal="true" zLocalPol1="false"/>

		<Bins>
			<tot variable="true" nBins="40" min="10" max="30"/>
			<zLocal variable="false" nBins="40" min="-3.2" max="3.2"/>
		</Bins>

	</TofCalibration>

	<histograms>

	</histograms>
	
	<DataSource treeName="tof" url="{dataDir}/tree_{treeIndex}.root" >
		<EvaluatedLeaf name="vR" value="TMath::Sqrt( [0]*[0] + [1]*[1] )" p0="vertexX" p1="vertexY" />
	</DataSource>
	
	<b>
		<trays width="1.0" min="0.5" max="120.5" />
		<modules width="1.0" min="0.5" max="32.5" />
		<cells width="1.0" min="0.5" max="6.5" />
		<p width="0.05" min="0" max="4.1" />
		<iBetaFirst width="0.05" min="-5" max="5.0" />
		<iBeta width="0.005" min=".75" max="3.0" />
		<dtWide width="1" min="-250" max="250" />
		<dt width=".01" min="-2.50" max="2.50" />
		<dtZ width=".001" min="-0.50" max="0.50" />
		<z width="0.1" min="-3.0" max="3.0" />
		<tot width=".5" min="10" max="30" />


	</b>



</config>
"""



for iT in range( 1, 2 ) :
	for iB in range( 1, 9 ) :
		treeIndex = (iT-1) * 8 + iB
		print "tree_", treeIndex
		print "Tray ", iT, " Board ", iB
		sMod = ( iB - 1 ) * 4 + 1;
		eMod = ( iB ) * 4
		context = {
 		"treeIndex" : treeIndex,
 		"tray" : iT,
 		"sMod" : sMod,
 		"eMod" : eMod,
 		"dataDir" : dataDir
 		}
 		fName = "tof" + `treeIndex` + ".xml"
 		aFile = open( fName, 'w' )
 		aFile.write(template.format(**context))
 		aFile.close()
 		
    	

