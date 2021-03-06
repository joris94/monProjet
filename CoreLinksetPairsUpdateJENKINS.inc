//run jenkins job to update linksetPairs of module given as input
/*
	run jenkins job to update linksetPairs of module given as input
	then display output message into dxl console
*/

const string fileToRead = "D:\\Users\\"doorsname"\\tempUpdateLinkset.txt"
void requestUpdate(Module m){
	User u = find()
	string users_email = u.(UserElement_ email)
	module_link = getURL(m)
	string jarClient = "D:\\Users\\"doorsname()"\\jenkins-cli.jar"
	string cmdLine =  "java -jar "jarClient" -s http://bie2-sv00031.vnet.valeo.com/ build DOORS/DOORS_LINKSETS -p EMAIL="users_email" -p MLINK=\""module_link"\" -f -v"
	//print cmdLine

	if(canOpenFile(jarClient,false))
	{
		string msg = ""
		//the command line call a Jenkins job - This Jenkins job call a batch which call a DXL script with admin right - As the called script (UpdateLinksetPairsBatch) create a file  we read it after the command
		int returnCode = win32SystemWait_(cmdLine, msg, -1) //win32SystemWait_(cmdLine, msg, TIME_OUT (in ms))
		string msgFromUpdateLinksetPairsBatch = readFile fileToRead
		//print "\n"
		if(returnCode == 3){
			print "TIME-OUT...\n"msg"\n"
			print msgFromUpdateLinksetPairsBatch
		}else if (returnCode == 1){
			print "Error while executing the command"
		}/*else{
			print "Output: "msg"\n"
			print msgFromUpdateLinksetPairsBatch
		}*/
		deleteFile (fileToRead)
	}
	else
	{
		result = "Invalid Path for Jenkins jar file"
		infoBox ("Invalid Path for Jenkins jar file")
	}
	logEnd()
}