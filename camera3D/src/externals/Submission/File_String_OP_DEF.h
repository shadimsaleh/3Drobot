//FILE_OP CLASS Class requires string and vector headers; see stdafx.h (by MKopp)

class File_OP
{
public:
	File_OP(string filename){my_filename = filename;mode = 0;}
	File_OP(){}
	~File_OP(){}
	void OpenWriter(string filename,bool append);
    void OpenWriter(bool append);
	void OpenReader(string filename);
	void CloseStream();
	//schreibt String als Zeile( mit Zeilenumbruch)
	void WriteLine(string line);
	//schreibt String in Datei
	void Write(string strX);
	string ReadLine();
	string Read();
    void ReadFileIntoVector();
	string ReadFromVector(int index);
	string ReadFromVector();
	int GetVectorSize();

private:
string my_filename;
ofstream file_out;
ifstream file_in;
vector<string> line_of_file;
vector<string>::iterator it_line;
int mode;

};

//FILE_OP CLASS ENDE

//STRING_OP (by MKopp)################################################################

class String_OP
{
public:
	String_OP(){}
	~String_OP(){}
    //zerlegt string anhand von delim in teile speichert teile in vector
	void Tokenize(string & str, string& delim);
	// F�gt an string eine Integerzahl an und gibt string zur�ck
	string AddInteger(string & str,int cipher);
	string Int_to_String(int cipher);
	//�bernimmt string zerlegt ihn und gibt ihn als vector zur�ck
	vector<string> Get_Tokens(string &, string & delim);//Zerlegt und gibt String zur�ck
	//gibt zerlegten string als vector zur�ck
	vector<string> Get_Tokens();
	//gibt Teilstring aus dem Vector zur�ck
	string Get_Token(int index);
	//Konvert String in Integerzahl
	int ConvertToInteger(string  str);
	//Wandelt Integer Vector in einzelnen String um, Trennstring angeben z.B: \t
	string IntVector_to_String(vector<int> & vec, string& separator);
	// Wandelt Integer Array in String um, Trennstring muss angegeben werden z.B: \t
	string IntArr_to_String(int arr[],int arr_lenght,string& separator);
	// Wandelt String mit Interger Zahlen in Vector um
	vector<int> String_to_IntVector(string str,string& separator);
	//gibt Gr�sse des internen Vectors zur�ck
	int Get_Size();
	//gibt internen Vector als String zur�ck
	string Internal_Vector_as_String();
	//Gibt String aus ACHTUNG: MEthode nur geeignet f�r Konsolenprogramm
    void PrintStrings();
private:
	vector<string> str_vector;//Template Klasse Vector, nimmt string Objekte auf
	vector<string>::iterator it;// iterator l�sst sich wie Zeiger bedienen und auf
	
};
