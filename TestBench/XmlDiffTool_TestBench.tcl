#####################################################################
## TEST BENCH FOR XMLDIFF TOOL 										#
## @author : Amine elhattami										#
## Email : elhattami@gmail.com										#
## Date : 11/2013													#
#####################################################################

# Extentions 
set expectedOutputExtention ".expectedOutput.txt"
set file1Extention ".file1.xml"
set file2Extention ".file2.xml"
set testDescriptionExtention ".testDescription.txt"

# Global options
set stopAfterHelp 0 
set skipRecompile 0
set singleTest 0

# file listes 
set testFilesList ""

# PRINT TEST HEADER 
proc printTestHeader {} {
	puts "**********************************************"
	puts "            TEST XML DIFF TOOL                "
	puts "**********************************************"
}

# PRINT HELP 
proc printHelp {} {

#Do not indent the set command below, cause it will affect the output
set helpMsg {
Usage : tclsh XmlDiffTool_TestBench.tcl [-option] [file1] [file2]

Options :
	-h : Print help.
	-c : Skip recompling the xml diff tool
	-s : Do do a signle test between [file1] [file2]}
		
	puts $helpMsg
}

# PARSE ARGUMENTS 
proc parseArguments {} {

	global stopAfterHelp 
	global skipRecompile
	global singleTest
	
	if { $argc > 1 } {
		for { set i 0 } { i <= $argc } { incr $i } {
			if { [lindex $argc $argv] == "-h" } {
				set stopAterHelp 1
			}
		
		}
	
	
	
	}
	 
}

# RECONPILE XMLDIFFTOOL
proc recompileDiffTool {} {
 #do it later 
}

# TEST MAIN FUNCTION
proc startTest {} {

# PRINT TEST HEADER 
printTestHeader
}

# START TEST 
startTest

