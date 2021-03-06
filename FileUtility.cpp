/*
Utilities for manipulating files and folders on Windows. 

Ketan Tang, tkt@ust.hk
*/

# pragma once
#include "FileUtility.h"
#include <iostream>
#pragma warning(disable:4996)

using namespace std;
namespace alphanum
{

  // anonymous namespace for functions we use internally. But if you
  // are coding in C, you can use alphanum_impl() directly, since it
  // uses not C++ features.
  namespace {

    // if you want to honour the locale settings for detecting digit
    // characters, you should define ALPHANUM_LOCALE
#ifdef ALPHANUM_LOCALE
    /** wrapper function for ::isdigit() */
    bool alphanum_isdigit(int c)
    {
      return isdigit(c);
    }
#else
    /** this function does not consider the current locale and only
	works with ASCII digits.
	@return true if c is a digit character
    */
    bool alphanum_isdigit(const char c)
    {
      return c>='0' && c<='9';
    }
#endif

    /**
       compare l and r with strcmp() semantics, but using
       the "Alphanum Algorithm". This function is designed to read
       through the l and r strings only one time, for
       maximum performance. It does not allocate memory for
       substrings. It can either use the C-library functions isdigit()
       and atoi() to honour your locale settings, when recognizing
       digit characters when you "#define ALPHANUM_LOCALE=1" or use
       it's own digit character handling which only works with ASCII
       digit characters, but provides better performance.

       @param l NULL-terminated C-style string
       @param r NULL-terminated C-style string
       @return negative if l<r, 0 if l equals r, positive if l>r
     */
    int alphanum_impl(const char *l, const char *r)
    {
      enum mode_t { STRING, NUMBER } mode=STRING;

      while(*l && *r)
	{
	  if(mode == STRING)
	    {
	      char l_char, r_char;
	      while((l_char=*l) && (r_char=*r))
		{
		  // check if this are digit characters
		  const bool l_digit=alphanum_isdigit(l_char), r_digit=alphanum_isdigit(r_char);
		  // if both characters are digits, we continue in NUMBER mode
		  if(l_digit && r_digit)
		    {
		      mode=NUMBER;
		      break;
		    }
		  // if only the left character is a digit, we have a result
		  if(l_digit) return -1;
		  // if only the right character is a digit, we have a result
		  if(r_digit) return +1;
		  // compute the difference of both characters
		  const int diff=l_char - r_char;
		  // if they differ we have a result
		  if(diff != 0) return diff;
		  // otherwise process the next characters
		  ++l;
		  ++r;
		}
	    }
	  else // mode==NUMBER
	    {
#ifdef ALPHANUM_LOCALE
	      // get the left number
	      char *end;
	      unsigned long l_int=strtoul(l, &end, 0);
	      l=end;

	      // get the right number
	      unsigned long r_int=strtoul(r, &end, 0);
	      r=end;
#else
	      // get the left number
	      unsigned long l_int=0;
	      while(*l && alphanum_isdigit(*l))
		{
		  // TODO: this can overflow
		  l_int=l_int*10 + *l-'0';
		  ++l;
		}

	      // get the right number
	      unsigned long r_int=0;
	      while(*r && alphanum_isdigit(*r))
		{
		  // TODO: this can overflow
		  r_int=r_int*10 + *r-'0';
		  ++r;
		}
#endif

	      // if the difference is not equal to zero, we have a comparison result
	      const long diff=l_int-r_int;
	      if(diff != 0)
		return diff;

	      // otherwise we process the next substring in STRING mode
	      mode=STRING;
	    }
	}

      if(*r) return -1;
      if(*l) return +1;
      return 0;
    }

  }

  /**
     Compare left and right with the same semantics as strcmp(), but with the
     "Alphanum Algorithm" which produces more human-friendly
     results. The classes lT and rT must implement "std::ostream
     operator<< (std::ostream&, const Ty&)".

     @return negative if left<right, 0 if left==right, positive if left>right.
  */
  template <typename lT, typename rT>
  int alphanum_comp(const lT& left, const rT& right)
  {
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<" << typeid(left).name() << "," << typeid(right).name() << "> " << left << "," << right << std::endl;
#endif
    std::ostringstream l; l << left;
    std::ostringstream r; r << right;
    return alphanum_impl(l.str().c_str(), r.str().c_str());
  }

  /**
     Compare l and r with the same semantics as strcmp(), but with
     the "Alphanum Algorithm" which produces more human-friendly
     results.

     @return negative if l<r, 0 if l==r, positive if l>r.
  */
  template <>
  int alphanum_comp<std::string>(const std::string& l, const std::string& r)
  {
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<std::string,std::string> " << l << "," << r << std::endl;
#endif
    return alphanum_impl(l.c_str(), r.c_str());
  }

  ////////////////////////////////////////////////////////////////////////////

  // now follow a lot of overloaded alphanum_comp() functions to get a
  // direct call to alphanum_impl() upon the various combinations of c
  // and c++ strings.

  /**
     Compare l and r with the same semantics as strcmp(), but with
     the "Alphanum Algorithm" which produces more human-friendly
     results.

     @return negative if l<r, 0 if l==r, positive if l>r.
  */
  int alphanum_comp(char* l, char* r)
  {
    assert(l);
    assert(r);
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<char*,char*> " << l << "," << r << std::endl;
#endif
    return alphanum_impl(l, r);
  }

  int alphanum_comp(const char* l, const char* r)
  {
    assert(l);
    assert(r);
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<const char*,const char*> " << l << "," << r << std::endl;
#endif
    return alphanum_impl(l, r);
  }

  int alphanum_comp(char* l, const char* r)
  {
    assert(l);
    assert(r);
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<char*,const char*> " << l << "," << r << std::endl;
#endif
    return alphanum_impl(l, r);
  }

  int alphanum_comp(const char* l, char* r)
  {
    assert(l);
    assert(r);
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<const char*,char*> " << l << "," << r << std::endl;
#endif
    return alphanum_impl(l, r);
  }

  int alphanum_comp(const std::string& l, char* r)
  {
    assert(r);
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<std::string,char*> " << l << "," << r << std::endl;
#endif
    return alphanum_impl(l.c_str(), r);
  }

  int alphanum_comp(char* l, const std::string& r)
  {
    assert(l);
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<char*,std::string> " << l << "," << r << std::endl;
#endif
    return alphanum_impl(l, r.c_str());
  }

  int alphanum_comp(const std::string& l, const char* r)
  {
    assert(r);
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<std::string,const char*> " << l << "," << r << std::endl;
#endif
    return alphanum_impl(l.c_str(), r);
  }

  int alphanum_comp(const char* l, const std::string& r)
  {
    assert(l);
#ifdef alphanumDEBUG
    std::clog << "alphanum_comp<const char*,std::string> " << l << "," << r << std::endl;
#endif
    return alphanum_impl(l, r.c_str());
  }

  ////////////////////////////////////////////////////////////////////////////

  /**
     Functor class to compare two objects with the "Alphanum
     Algorithm". If the objects are no std::string, they must
     implement "std::ostream operator<< (std::ostream&, const Ty&)".
  */
  template<class Ty>
  struct alphanum_less : public std::binary_function<Ty, Ty, bool>
  {
    bool operator()(const Ty& left, const Ty& right) const
    {
      return alphanum_comp(left, right) < 0;
    }
  };

}


namespace tkt{
	/////////////////////// string /////////////////////////////
	void string2stringvec(string s, vector<string>& sv, string delimiter)
	{
		sv.clear();
		size_t pos=0;
		string token;
		while((pos=s.find(delimiter))!=string::npos){
			token=s.substr(0,pos);
			sv.push_back(token);
			s.erase(0,pos+delimiter.length());
		}
		sv.push_back(s);
	}
	
	bool CompString(string s1, string s2)
	{
		if (s1.size() < s2.size())
			return 1;
		else if(s1.size() > s2.size())
			return 0;
		else
		{
			if (s1<s2)
				return 1;
			else 
				return 0;
		}
	}

	bool AlphnumComp(string s1, string s2)
	{
		int x=alphanum::alphanum_comp(s1, s2);
		if (x<0) return true;
		else return false;
	}

	/*
	 fixpath() - Adds \ to the end of a path if not present.
	*/
	int fixpath(const char *inpath, char *outpath)
	{
		int   n=0;

		strcpy(outpath,inpath);

		while(inpath[n]) n++;

		if(inpath[n-1] != '\\' || inpath[n-1] != '/')
		{
			strcat(outpath,"/");
			return 1;
		}

		return 0;
	}

	string fixpath(const string inpath){
		char outpath[260];
		fixpath(inpath.c_str(), outpath);
		return string(outpath);
	}

	/* List files (not including directory, just pure name) in a folder, in a naturally ascending order.
	**
	** Example: 
	fixpath(imdir, imdir);
	string bmpFiles = string(imdir)+"*.bmp";
	vector<string> vFileNames = ListFiles(bmpFiles.c_str());

	*/
	vector<string> ListFiles(const char * path)
	{
		vector<string> vFileName;	
#ifndef WIN32
		//open a directory the POSIX way
		DIR *dp;
		struct dirent *ep;
		dp = opendir(path);
		
		if(dp)
		{
			while((ep=readdir(dp))){
				if(ep->d_name[0] != '.'){
					vFileName.push_back(ep->d_name);
				}
			}
			(void) closedir(dp);
		}
		else{
			cout<<"opendir "<<path<<" fails!\n Error: "<<errno<<endl;
		}
#else		
#if 0
		// the old ugly way of doing this
		WIN32_FIND_DATA fd;
		HANDLE fh = FindFirstFile((LPCSTR)path, &fd);
		if (fh != INVALID_HANDLE_VALUE)
		{
			do
			{
				vFileName.push_back(string(fd.cFileName));			
			}
			while (FindNextFile(fh, &fd));
		}
		FindClose(fh);
#endif

		struct _finddata_t file;
		int status;
		intptr_t HANDLE;
		status = HANDLE = _findfirst( path, &file );
		while( status != -1 )
		{
			vFileName.push_back(string(file.name));
			status = _findnext(HANDLE, &file);
		}
		_findclose( HANDLE );
#endif
		sort(vFileName.begin(), vFileName.end(), AlphnumComp);
		return vFileName;
	}

	vector<string> ListFiles(const string path){
		return ListFiles(path.c_str());
	}

	/* List files including full path.
	Example:
	vector<string> vFileRGB = ListFilesFull(folder+"rgb\\", "*.png");
	*/
	vector<string> ListFilesFull(const string pathin, const char* ext){
		string path = fixpath(pathin);
		vector<string> vFileName = ListFiles(path+ext);
		for (int i = vFileName.size()-1; i>=0; i--)
		{
			if (vFileName[i] == "." || vFileName[i] == ".."){
				vFileName.erase(vFileName.begin() + i);
			}else
				vFileName[i] = path + vFileName[i];
		}
		return vFileName;
	}

	// true: succeed create or dir exists; false: fail to create
    bool makedir(const string path){
		// if dir doesn't exist, make it
#ifdef WIN32		
		if(_access(path.c_str(),0)!=0)
			return _mkdir(path.c_str()) ==0;
#else
        if(opendir(path.c_str())==NULL){
            return mkdir(path.c_str(), 0777) ==0;
		}
#endif
		return true;  // dir already exists
	}



	/* Remove File. Return true if succeed; false if fail.
	Ex: bool ok = RemoveFile("1.txt");
	*/
	bool RemoveFile(const string fname){
		return remove(fname.c_str())==0;
	}

	bool RemoveFileInFolder(const string folder)
	{
		vector<string> files = ListFilesFull(folder, "*");

		bool result = true;
		for (size_t i = 0; i < files.size(); i++)
		{
			result = RemoveFile(files[i]);
			if (!result){
				cout << "Removing file " << files[i] << " fails!\n";
				return result;
			}

			cout << files[i] <<" deleted!\n";
		}

		return result;
	}

	void SplitFilename (const std::string& str, std::string& path, std::string& file_ext)
	{
		std::size_t found = str.find_last_of("/\\");
		path = str.substr(0,found);
		file_ext = str.substr(found+1);		
	}

	void SplitFilename (const std::string& str, std::string& path, std::string& file, std::string& ext)
	{
		std::size_t found = str.find_last_of("/\\");
		path = str.substr(0,found);
		file = str.substr(found+1);		

		found = file.find_last_of(".");
		ext = file.substr(found);
		file = file.substr(0,found);
	}

	FileParts fileparts(std::string filename)
	{
		size_t idx0 = filename.rfind("/");
		if(idx0==std::string::npos) idx0 = filename.rfind("\\");
		size_t idx1 = filename.rfind(".");

		FileParts fp;
		fp.path = filename.substr(0,idx0+1);
		fp.name = filename.substr(idx0+1,idx1-idx0-1);
		fp.ext  = filename.substr(idx1);

		return fp;
	}
	
	////////////////////// time ///////////////////////
	string getNow()
	{
		stringstream ss;
#ifdef WIN32
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		ss	<<setw(2)<<setfill('0')<<int(sysTime.wMonth)<<"-"
			<<setw(2)<<setfill('0')<<int(sysTime.wDay)<<"-"
			<<setw(2)<<setfill('0')<<int(sysTime.wHour)<<"-"
			<<setw(2)<<setfill('0')<<(int(sysTime.wMinute)/10)*10;
#else
		time_t rawtime;
		time(&rawtime);
		struct tm* timeinfo=localtime(&rawtime);
		ss	<<setw(2)<<setfill('0')<<int(timeinfo->tm_mon)<<"-"
			<<setw(2)<<setfill('0')<<int(timeinfo->tm_mday)<<"-"
			<<setw(2)<<setfill('0')<<int(timeinfo->tm_hour)<<"-"
			<<setw(2)<<setfill('0')<<(int(timeinfo->tm_min)/10)*10;
#endif
		return ss.str();
	}

    int isdir(const std::string path)
    {
        struct stat statbuf;
        if (stat(path.c_str(), &statbuf) != 0)
            return 0;
        return S_ISDIR(statbuf.st_mode);
    }

    vector<string> ListDir(const std::string path)
    {
        vector<string> list_dir;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (ent->d_type == DT_DIR && string(ent->d_name)!= "." && string(ent->d_name)!=".."){
                    list_dir.push_back(ent->d_name);
                }
            }
            closedir(dir);
        }
        else {
            /* could not open directory */
            perror("");
        }

        return list_dir;
    }

}

template<typename T>
ostream& operator,(ostream& out, const T& val)
{
	out << ", " << val;
	return out;
}
