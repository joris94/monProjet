//Update linksetPairs of module given as input 
/*
	Update linksetPairs of module given as input 
	(work only if user has admin right on parent folder)
*/

//! GLOBALS
Skip 		lnkModName = create;
Skip 		lnkModTgt = create;
int 		lengthKey = 0;
string		folderOfDVSTemplates = "/Project_Template_DVS_PLUS_CDA_PMT" //should be changed for production database

/******************************************************************************
	isTemplateMod
******************************************************************************/
bool isTemplateMod(Item i)
{
	string strItem = name (i);
	string strShouldBeXX = strItem[2:3];
	int lenghtStr = length (strItem);
	strShouldBe_tpl = strItem[lenghtStr-4:lenghtStr-1]
	if( (strShouldBeXX != "XX" && strShouldBeXX != "xx") || strShouldBe_tpl != "_tpl"){
		return (false);
	}
	return (true);
}


/******************************************************************************
	errorDisplayExit
******************************************************************************/
void errorDisplayExit(string msg, bool showMsg)
{
	print(msg);
	halt();
}


/******************************************************************************
	scanLinkSetPairingOfFolder
	Scan the folder of the module (global variable: itemIsTheTplate) to find
	linkset Paring with this module as source
******************************************************************************/
void scanLinkSetPairingOfFolder(Folder f, Item i) 
{
	LinkModuleDescriptor lmd
   	int offset, length
	if (null f) return
	for lmd in f do
	{
		if(findPlainText (getSourceName(lmd), name(i), offset, length, true)){
			//print "\nAdd:\n\t"(getSourceName(lmd))"\n\t\t"getName(lmd)"\n\t\t\t"getTargetName(lmd)
			put(lnkModName, lengthKey,getName(lmd))
			put(lnkModTgt, lengthKey,getTargetName(lmd))
			lengthKey++
		}
	}
	Item itm
	for itm in f do //analyze subfolders
	{
		if (null itm) continue
		if (isDeleted(itm)) continue
	   
		if ((type (itm) == "Project") || (type (itm) == "Folder"))
		{
			scanLinkSetPairingOfFolder(folder(itm),i)	   
		}
	}
}


/******************************************************************************
	getTemplateOfModuleFromDVS
	Scan the dvs folder that contains templates and find the corresponding
	template of the input module
******************************************************************************/
string getTemplateOfModuleFromDVS(Item itmWork, Module m) {
	if (type(itmWork) == "Project") {
		Item itmHelp;
		for itmHelp in folder(itmWork) do {
			str = getTemplateOfModuleFromDVS(itmHelp,m);
			if(!null str) return str
		}
	}
	if (type(itmWork) == "Folder") {
		Item itmHelp;
		for itmHelp in folder(itmWork) do {
			str = getTemplateOfModuleFromDVS(itmHelp,m);
			if(!null str) return str
		}
	}
	if (type(itmWork) == "Formal") {
		//print "found "name itmWork"\n"
		if(isTemplateMod(itmWork)){
			tplateCat = (name(itmWork))[0:1];
			tplateName = (name(itmWork))[4:(length(name(itmWork)))-4]
			rexep = tplateCat"[0-9][0-9]"tplateName
			Regexp I_NAME = regexp2 (rexep)
			if(I_NAME name(m)){
				//print "\n "name(itmWork)" is template of " name(inputModule)"\n"
				return fullName (itmWork)
			}
		}
	}
	return ""
}

/******************************************************************************
	getTemplateOfModule
	Scan the folder of the input model to find template
******************************************************************************/
string getTemplateOfModuleInFolder(Folder f, Module m)
{
	Item itemInFolder
	for itemInFolder in f do
	{
		if (type(itemInFolder) == "Formal") {
			if(isTemplateMod(itemInFolder)){
				tplateCat = (name(itemInFolder))[0:1];
				tplateName = (name(itemInFolder))[4:(length(name(itemInFolder)))-4]
				rexep = tplateCat"[0-9][0-9]"tplateName
				//print "\n"rexep"\n"
				Regexp I_NAME = regexp2 (rexep)
				if(I_NAME name(m)){
					//print "\n "name(itemInFolder)" is template \n"
					return fullName (itemInFolder)
				}
			}
		}
	}
	return ""
}

/******************************************************************************
	replaceString
******************************************************************************/
string replaceString (string sSource, string sSearch, string sReplace) {
    int iLen = length sSource
    if (iLen == 0) return ""
    
    int iLenSearch = length(sSearch)
    
    if (iLenSearch == 0) { 
        //raiseError ("Parameter error", "in strings.inc/replace: search string must not be empty")
        return "" 
    }
    
    // read the first char for latter comparison -> speed optimization
    char firstChar = sSearch[0]
    
    Buffer s = create() 
    int pos = 0, d1,d2;    
    int i
    
    while (pos < iLen) { 
        char ch = sSource[pos]; 
        bool found = true
        
        if (ch != firstChar) {pos ++; s+= ch; continue}
        for (i = 1; i < iLenSearch; i++) {
           if (sSource[pos+i] != sSearch[i]) { found = false; break }
        }
        if (!found) {pos++; s+= ch; continue}
        s += sReplace
        pos += iLenSearch
    }
    
    string result = stringOf s
    delete s
    return result
}

/******************************************************************************
	urlInfo
	Analyze the URL
******************************************************************************/
string getModuleNameFromURL(string url)
// DESCRIPTION: Returns a string describing the target of the specified URL string.
{
	string result = null
	ModuleVersion mv
	int objectAbsno
	Item i
	string dbHost = null
	int dbPort
	string dbName
	string dbID = null
	result = decodeURL(url, dbHost, dbPort, dbName, dbID, i, mv, objectAbsno)
	if (!null result)
	{
		errorDisplayExit("An error occured with URL input : "result,true)
	}
	return (fullName mv)
}

void runUpdate(string url)
{
	Item iModule = item(getModuleNameFromURL(url))
	Module inputModule = module(iModule)
	if (null inputModule){
		errorDisplayExit ("This script shall be executed in a module context",true);
	}
	if( isTemplateMod(item (fullName(inputModule)))){
		errorDisplayExit ("This script shall not be executed on a template module",true);
	}
	//Date dateAndTime = dateAndTime(today());
	//print "Running UPDATE LINKSET PAIRS - "dateAndTime"\n";
	//print name(inputModule)"\n"

	/////////////////////////////////////////////////////////////
	///////////////////////Search Template///////////////////////
	/////////////////////////////////////////////////////////////
	Folder currentFolder = getParentFolder(inputModule)
	string templateName = getTemplateOfModuleInFolder(currentFolder,inputModule) 	//find template in current folder	
	if(null templateName){//template not found in the current folder -> search in DVS template folders
		//print "Template not found in current folder. Searching in DVS templates folder...\n"
		templateName = getTemplateOfModuleFromDVS(item folderOfDVSTemplates, inputModule);
		if(null templateName) //DVS template not found -> let the user select the input
		{
			errorDisplayExit("Template not found in DVS templates folder...Exit!",true);
		}
	}
	//print "---> Template of \""name(inputModule) "\" is \""templateName"\".\n"

	/////////////////////////////////////////////////////////////
	///////////////////Compute Linkset Pairing///////////////////
	/////////////////////////////////////////////////////////////
	scanLinkSetPairingOfFolder(getParentFolder(item(templateName)), item(templateName)); //create two skip list "lnkModName" and lnkModTgt"
	if(lengthKey != 0){
		//errorDisplayExit("No linkset pairing to perform according to the template.",false);
		string target,expr, prefixTget, targetName, dirTget, nameLnk
		Regexp EXPR_reg
		Folder fTarget;
		Item itemInF;
		int iKey = 0;
		for (iKey = 0; iKey < lengthKey ; iKey++)
		{
			find(lnkModTgt,iKey,target);
			find(lnkModName,iKey,nameLnk);
			expr = "[0-9][0-9]xx_.*_tpl"
			EXPR_reg = regexp2 (expr)
			if(!EXPR_reg target){//should not happen...
				errorDisplayExit("Error target of template.",false)
			}

			targetName = target[match 0] 
			prefixTget = targetName[0:1]
			int offset, length
			findPlainText(target, targetName, offset, length, true)
			dirTget = target[0:offset-1]
			//Now find all target in fTarget that begin with prefixTget without XX
			int tmpA = 0
			fTarget = folder(dirTget)
			for itemInF in fTarget do
			{
				if (type itemInF == "Formal" && !isTemplateMod(itemInF) && (name(itemInF))[0:1] == prefixTget){
					///////////////////CREATE LINKSET PAIRING///////////////////
					msg = addLinkModuleDescriptor(currentFolder, fullName(inputModule), fullName(itemInF), false, true, nameLnk, "")
					/*if(!null msg){
						print "\nProblem while creating link : \n"name(inputModule)" ->  " nameLnk " -> "fullName(itemInF)"\nERROR: "msg"\n"
					}
					else
					{
						print "\nLinking : "name(inputModule)" ->  " nameLnk " -> "fullName(itemInF)" \n"
					}*/
				}
			}
		}
	}
}
