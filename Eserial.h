// ----------------------------------------------------------------------------
// Trivial Linux serial driver class

// Copyright (C) 2013  Embecosm Limited <info@embecosm.com>

// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// ----------------------------------------------------------------------------

// Based on a subset of the Tserial interface proposed by Thierry Schneider
// and inspired by the ideas on Arduino playground about serial connections
// between Linux and Arduino. This only provides the functionality needed to
// run the Instructables face tracker example

#ifndef ESERIAL_H
#define ESERIAL_H

#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sstream>


using std::cerr;
using std::endl;
using std::ostringstream;
using std::string;


//! Enumeration for parity types
enum serial_parity {
  spNONE,
  spODD,
  spEVEN };


class Eserial
{
 private:
  int  handle;				//!< File handle to serial line.


 public:
  //! Constructor
  Eserial()
    {
      handle = -1;
    }

  //! Destructor
  ~Eserial()
    {
      disconnect ();
    }

  //! Close the connection
  void disconnect(void)
  {
    // Don't care if it fails
    if (-1 != handle)
      {
	close (handle);
	handle = -1;
    }
  }

  //! Open the connection.
  
  //! @param[in] portname  The name of the port to use
  //! @param[in] rate      The bit rate to set
  //! @param[in] parity    The parity to use - ignored
  int  connect (const char    *portname,
		int            rate,
		serial_parity  parity)
  {
    string portstr = portname;
    string ratestr = 
      static_cast<ostringstream*>(&(ostringstream() << rate))->str();
    string comm = "stty -F " + portstr + " cs8 " + ratestr
      + " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig"
      + " -icanon -iexten -echo -echoe -echok -echoctl -echoke"
      + " noflsh -ixon -crtscts";
    std::cout << comm << endl;
    
    if (-1 == system (comm.c_str()))
      {
	cerr << "ERROR: stty failed: " << strerror (errno) << endl;
      }
    
    handle = open (portname, O_RDWR);
    if (-1 == handle)
      {
	cerr << "ERROR: open failed: " << strerror (errno) << endl;
      }
  }


  //! Write a byte

  //! @param[in] data  The byte to write.
  void sendChar(char data)
  {
    if (-1 == write (handle, &data, 1))
      {
	cerr << "ERROR: write failed: " << strerror (errno) << endl;
      }
  }
};

#endif  // ESERIAL_H


