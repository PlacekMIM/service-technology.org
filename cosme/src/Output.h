#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <iostream>
#include <string>

/*!
 \brief output file organization

 This file is a wrapper class around an STL ostream object and organizes the
 lifecycle of an output. An output can be a regular file that is created by
 the tool, a temporary file that is used to communicate with other tools,
 or a redirection to the standard output stream.

 To create a regular file, call the constructor with a filename and a
 description of the output (for verbose messages). To create a temporary
 file, call the constructor without arguments. Temporary files will be
 deleted when the object is destroyed unless the paramter '--noClean' is
 given. To redirect to standard output, use the the filename "-".

 The quite tricky constructor was taken from the Ubuntu Forums at
 http://ubuntuforums.org/showthread.php?p=5929211
 */
class Output {
    public: /* member functions */
        /// constructor (creates temp file)
        Output();

        /// constructor (opens named stream or directs to std::cout)
        Output(std::string, std::string);

        /// destructor (closes stream)
        ~Output();

        /// implicit conversation operator to ostream
        operator std::ostream&();

        /// returns the filename of this object
        std::string name() const;

        /// returns the stream of this object
        std::ostream& stream() const;

    private: /* member attributes */
        /// the stream itself
        std::ostream& os;

        /// filename of the output stream
        std::string filename;

        /// its a temp file if its not NULL
        char* temp;

        /// the kind of the file
        std::string kind;
};

#endif
