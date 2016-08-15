#ifndef _H_DEDISP_READ_FILFILE
#define _H_DEDISP_READ_FILFILE

#include <fstream>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

struct header_s
{
	std::string raw_file;		// raw data file name
	std::string source_name;	// source name
	int mach_id;			// machine id
	int tel_id;			// telescope id
	double ra;			// source right ascension
	double dec;			// source declinatio
	double az;			// azimuth angle in deg
	double zn;			// zenith angle in deg
	int data_type;			// data type ID
	double rdm;			// reference DM
	int nchans;			// number of channels
	double top_chn;			// frequency of the top channel MHz
	double band;			// channel bandwidth in MHz
	int nbeams;			// number of beams
	int ibeam;			// beam number
	int nbits;			// bits per sample
	double tstart;			// observation start time in MJD format
	double tsamp;			// sampling time in seconds
	int nifs;			// something
	size_t nsamps;			// number of time samples per channel
};

void read_filterbank(std::string in_file, header_s &header, unsigned char *indata)
{
	cout << "Reading the file " << in_file << "...\n";
	cout << "Reading the header...\n";

	std::ifstream input_file(in_file.c_str(), std::ifstream::in | std::ifstream::binary);

	// reading the header

//	bool result = read_header(input_file);

	std::string read_param;
	char field[60];
	int strlen;

	while(true)		// go 4eva
	{
		input_file.read((char *)&strlen, sizeof(int));
		input_file.read(field, strlen * sizeof(char));
		field[strlen] = '\0';
		read_param = field;
		if (read_param == "HEADER_END") break;		// finish reading the header when its end is reached
		else if (read_param == "rawdatafile")		// need to read some long filename
		{
			input_file.read((char *)&strlen, sizeof(int));		// reads the length of the raw data file name
			input_file.read(field, strlen * sizeof(char));
			field[strlen] = '\0';
			header.raw_file = field;
		}
		else if (read_param == "source_name")		// need to read source name
		{
			input_file.read((char *)&strlen, sizeof(int));
			input_file.read(field, strlen * sizeof(char));
			field[strlen] = '\0';
			header.source_name = field;
		}
		else if (read_param == "machine_id")	input_file.read((char *)&header.mach_id, sizeof(int));
		else if (read_param == "telescope_id")	input_file.read((char *)&header.tel_id, sizeof(int));
		else if (read_param == "src_raj")	input_file.read((char *)&header.ra, sizeof(double));
		else if (read_param == "src_dej")	input_file.read((char *)&header.dec, sizeof(double));
		else if (read_param == "az_start")	input_file.read((char *)&header.az, sizeof(double));
		else if (read_param == "za_start")	input_file.read((char *)&header.zn, sizeof(double));
		else if (read_param == "data_type")	input_file.read((char *)&header.data_type, sizeof(int));
		else if (read_param == "refdm")		input_file.read((char *)&header.rdm, sizeof(double));
		else if (read_param == "nchans")	input_file.read((char *)&header.nchans, sizeof(int));
		else if (read_param == "fch1")		input_file.read((char *)&header.top_chn, sizeof(double));
		else if (read_param == "foff")		input_file.read((char *)&header.band, sizeof(double));
		else if (read_param == "nbeams")	input_file.read((char *)&header.nbeams, sizeof(int));
		else if (read_param == "ibeam")		input_file.read((char *)&header.ibeam, sizeof(int));
		else if (read_param == "nbits")		input_file.read((char *)&header.nbits, sizeof(int));
		else if (read_param == "tstart")	input_file.read((char *)&header.tstart, sizeof(double));
		else if (read_param == "tsamp")		input_file.read((char *)&header.tsamp, sizeof(double));
		else if (read_param == "nifs")		input_file.read((char *)&header.nifs, sizeof(int));

	}

	// get the number of samples

	size_t header_end_pos = input_file.tellg();	// header size
	input_file.seekg(0, input_file.end);
	size_t file_end_pos = input_file.tellg();

	// go back to end of header position
	input_file.seekg(header_end_pos, input_file.beg);

	header.nsamps = (file_end_pos - header_end_pos) / header.nchans * (8 / header.nbits);

	// read the real data now

	size_t to_read = header.nsamps * header.nchans * header.nbits / 8;

	indata = new unsigned char[to_read];

	// treat the binary representation of the data as char
	input_file.read(reinterpret_cast<char*>(indata), to_read);
	// reinterpret_cast converts any pointer type
	// to any other pointer type, even of unrelated classes.
	// The operation result is a simple binary copy of
	// the value from one pointer to the other

	cout << "Printing some data...\n";

        cout << *indata << " " << *(indata + 1) << " " << *(indata + 2)
                << " " << *(indata + header.nchans) << endl;

	input_file.close();

}

#endif
