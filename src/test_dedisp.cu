#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "DedispPlan.hpp"
#include "filterbank.hpp"

using std::cout;
using std::endl;


int main(int argc, char *argv[])
{

    std::string infilename = std::string(argv[1]);
    unsigned char *indata;
    unsigned char *outdata;
    header_s header;
    read_filterbank(infilename, header, indata)

    DedispPlan plandedisp(header.nchans, header.tsamp, header.top_chn, header.band);
    plandedisp.generate_dm_list(atof(argv[2]), atof(argv[3]), 64.0f, 1.10f);
    outdata = new unsigned char[(header.nsamps - plandedisp.get_max_delay()) * plandedisp.get_dm_count()];
    plandedisp.execute(header.nsamps, indata, header.nbits, outdata, header.nbits);

    return 0;
}
