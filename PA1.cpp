#include "PA1.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <bitset>
#include <iomanip>
#include <sstream>

// http://en.cppreference.com/w/cpp/language/type_alias
using byte = unsigned char ;

// http://en.cppreference.com/w/cpp/types/numeric_limits
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<byte>::digits ;

// http://en.cppreference.com/w/cpp/utility/bitset
using bits_in_byte = std::bitset<BITS_PER_BYTE> ;



using namespace std;

int main(int argc, char* argv[]){



	//ENSURE THAT ALL ARGUMENTS ARE VALID
        if (argc > 6) {
                cerr << "Too many arguments entered. Exiting " << argv[0] << "\n";
                return -1;
        }

        else if(argc < 6) {
                 cerr << "Not enough arguments entered. Exiting " << argv[0] << "\n";
                 return -1;
        }


	string argv1 = argv[1];

	if(argv1 == ("B")){
		//Use Block Cypher
	}

	else if(argv1 == "S"){
		//Use Stream Cypher
	}
	
	else{
		cerr << "Invalid first argument. Only a B or an S will be accepted. Exiting " << argv[0] << "\n";
               	return -1;
	}
	

	string file = "";	

	ifstream infile(argv[2]); //open the file

        //Read the file into the words vector
        if (infile.is_open() && infile.good()) {
		infile.close();
		file = readfile(argv[2]);

	}
	
	else{
                cerr << "Invalid file name for second argument: " << argv[2] << " Exiting "<< argv[0] << "\n";
        	return -1;
	}


	
	ifstream outfile(argv[3]); //open the file

        //Read the file into the words vector
        if (outfile.is_open() && outfile.good()) {
               outfile.close(); 
        }

        else{
                cerr << "Invalid file name for third argument: " << argv[3] << " Exiting "<< argv[0] << "\n";
        	return -1;
	}


	string key = "";

	
	ifstream keyfile(argv[4]); //open the file

        //Read the file into the words vector
        if (keyfile.is_open() && keyfile.good()) {
                keyfile.close();
                key = readfile(argv[4]);
        }

        else{
                cerr << "Invalid file name for fourth argument: " << argv[4] << " Exiting "<< argv[0] << "\n";
        	return -1;
	}


	string argv5 = argv[5];
	if(argv5 == "E"){
                //Use Block Cypher
        }

        else if(argv5 == "D"){
                //Use Stream Cypher
        }

        else{
                cerr << "Invalid fifth argument. Only an E or a D will be accepted. Exiting " << argv[0] << "\n";
                return -1;
        }


	string result = "";


	if(argv1 == "B" && argv5 == "E"){
		if(key.length() < 64){
			cerr << "Key is not 64 bits as required for block cypher. Exiting " 
			<< argv[0] << "\n";
			return -1;
		}
		result = blockencrypt(key, file);
	}

	else if(argv1 == "B" && argv5 == "D"){
		if(key.length() < 64){
                        cerr << "Key is not 64 bits as required for block cypher. Exiting " 
			<< argv[0] << "\n";
                        return -1;
                }
                result = blockdecrypt(key, file);
        }

	else if(argv1 == "S"){
                result = stream(key, file);
        }

	
	writefile(argv[3], result);


	return 0;

}



string blockencrypt(string key, string file){
	//PAD DATA IF NECESSARY
	int padding = file.length() % 64;
	if(padding != 0){
		for(int i = 0; i < padding; i + 8){
			file + "10000000";
		}
		cout << "file after padding: " << file << "\n";
	}
	
	//PASS TO XOR
	string paddedXORed = stringxor(key, file);
	
	//PASS TO SWAP AND RETURN
	return swap(key, paddedXORed);
}



string blockdecrypt(string key, string file){

	string swapped = swap(key, file);
	
	string swapXOR = stringxor(key, swapped);

	//UNPAD DATA
	for(int i = 0; i < swapXOR.length(); i+8){
		string bytestring = swapXOR.substr(i, 8);
		if(bytestring == "10000000"){
			swapXOR = swapXOR.substr(0, i-1);
		}
	}

        return swapXOR;
}


string swap(string key, string file){
	string readytoswap = "";
	std::bitset<8> bs;
	stringstream ss;
	std::istringstream f(file);
	while(f >> bs){
        	ss << char(bs.to_ulong());
	}
	ss >> readytoswap;

	string keychars = "";
	std::bitset<8> keybs;
	stringstream ss1;
	std::istringstream k(key);
        while(k >> keybs){
                ss1 << char(keybs.to_ulong());
        }
	ss1 >> keychars;


	int j = 0;
	int end = readytoswap.size() - 1;
	for(int start = 0; start < end; start++){
		if((keychars[j] % 2) == 1){
			std::swap(readytoswap[start], readytoswap[end]);
			end--;
		}
		j++;
		if(j == keybs.size()){
			j = 0;
		}
	}
	
	return readytoswap;

}


string stream(string key, string file){
        return stringxor(key, file);
}



std::string readfile(string filename){
	std::string bitstring;
    	std::ifstream file(filename, std::ios::binary) ; // open in binary mode

    	char c ;
    	while(file.get(c)){
		 // read byte by byte
        	bitstring += bits_in_byte(byte(c)).to_string() ; // append as string of '0' '1'
	}

    	return bitstring ;
}



std::string stringxor(string key, string file){
	string result = "";
	for(int i = 0; i < file.length(); i++){
		for(int j = 0; j < key.length(); j++){
                	result[i] = ((key[j]-'0') ^ (file[i]-'0')) + '0';
			if(j + 1 == key.length()){
				j = 0;
			}
		}
        }
	return result;
}



void writefile(string filename, string result){
	ofstream writer;
	writer.open(filename);
	writer << result;
	writer.close();
}
