void requestUpdate(Module m){
	User u = find()
	string users_email = u.(UserElement_ email)
	module_link = getURL(m)
	string jarClient = "D:\\Users\\jgonza11\\jenkins-cli.jar"
	string cmdLine =  "java -jar "jarClient" -s http://bie2-sv00031.vnet.valeo.com/ build DOORS/DOORS_LINKSETS -p EMAIL="users_email" -p MLINK=\""module_link"\" -f -v"
	print cmdLine
	if(canOpenFile(jarClient,false))
	{
		int timeOut = 180000 //wait for 3min = 180000ms
		string msg = ""
		int returnCode = win32SystemWait_(cmdLine, msg, 180000)
		print "\n"
		if(returnCode == 3){
			print "TIME-OUT...\n"
			print "Output (if any): "msg"\n"
		}else if (returnCode == 1){
			print "Error while executing the command"
		}else{
			print "Output: "msg"\n"
		}
	}
	else
	{
		result = "Invalid Path"
		infoBox ("Invalid path")
	}
	logEnd()
}