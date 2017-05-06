/*
Utilities for manipulating files and folders on Windows. 

Ketan Tang, tkt@ust.hk
*/

# pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <functional>
#include <sstream>
#include <stdio.h>	// for remove
#include <iomanip>  // std::setfill, std::setw
#ifdef _MSC_VER
#include <io.h>
#include <direct.h>  // for mkdir
#include <wtypes.h>  // for SYSTEMTIME
#include "dirent.h"
#else
#include <sys/io.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#pragma warning(disable:4996)

using namespace std;

namespace tkt{
	/////////////////////// string /////////////////////////////
    void string2stringvec(string s, vector<string>& sv, string delimiter);
	
    bool CompString(string s1, string s2);

    bool AlphnumComp(string s1, string s2);

	/*
	 fixpath() - Adds \ to the end of a path if not present.
	*/
    int fixpath(const char *inpath, char *outpath);

    string fixpath(const string inpath);

	/* List files (not including directory, just pure name) in a folder, in a naturally ascending order.
	**
	** Example: 
	fixpath(imdir, imdir);
	string bmpFiles = string(imdir)+"*.bmp";
	vector<string> vFileNames = ListFiles(bmpFiles.c_str());
	*/
    vector<string> ListFiles(const char * path);

    vector<string> ListFiles(const string path);

	/* List files including full path.
	Example:
	vector<string> vFileRGB = ListFilesFull(folder+"rgb\\", "*.png");
	*/
    vector<string> ListFilesFull(const string pathin, const char* ext);

	// true: succeed create or dir exists; false: fail to create
    bool makedir(const string path);

	/* Remove File. Return true if succeed; false if fail.
	Ex: bool ok = RemoveFile("1.txt");
	*/
    bool RemoveFile(const string fname);

    bool RemoveFileInFolder(const string folder);

    void SplitFilename(const std::string& str, std::string& path, std::string& file_ext);

    void SplitFilename(const std::string& str, std::string& path, std::string& file, std::string& ext);

	struct FileParts
	{
		std::string path;
		std::string name;
		std::string ext;
	};

    FileParts fileparts(std::string filename);

    ////////////////////// directories /////////////////
    int isdir(const std::string path);

    // list directories in path without "." and ".."
    vector<string> ListDir(const std::string path);
	
	////////////////////// time ///////////////////////
    string getNow();
}

template<typename T> ostream& operator,(ostream& out, const T& val);
