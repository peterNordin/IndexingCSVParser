//!
//! @file   IndexingCSVParserImpl.hpp
//! @author Peter Nordin
//! @date   2015-02-03
//!
//! @brief Contains the header only (template) implementation of the IndexingCSVParser
//!

#ifndef INDEXINGCSVPARSERIMPL_HPP
#define INDEXINGCSVPARSERIMPL_HPP

#include <cstdio>
#include <cstdlib>
#include "Converters.hpp"

namespace indcsvp
{

//! @brief Peek help function to peek at the next character in the file
//! @param[in] pStream The stream too peek in
//! @returns The next character (as int)
inline int peek(FILE *pStream)
{
    int c = fgetc(pStream);
    ungetc(c, pStream);
    return c;
}



template <typename T>
bool IndexingCSVParser::getIndexedColumnAs(const size_t col, std::vector<T> &rData)
{
    if (col < numCols(0))
    {
        const size_t nr = numRows();
        rData.reserve(nr); //!< @todo will this shrink reservation ?

        // Loop through each row
        for (size_t r=0; r<nr; ++r)
        {
            // Begin and end positions
            size_t b = mSeparatorPositions[r][col] + size_t(col > 0);
            size_t e = mSeparatorPositions[r][col+1];
            // Move file ptr
            std::fseek(mpFile, b, SEEK_SET);

            // Extract data
            char buff[e-b+1];
            char* rc = fgets(buff, e-b+1, mpFile);
            // Push back data
            if (rc)
            {
                bool parseOK;
                rData.push_back(converter<T>(buff, parseOK));
                if (!parseOK)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <typename T>
bool IndexingCSVParser::getIndexedRowAs(const size_t row, std::vector<T> &rData)
{
    if (row < mSeparatorPositions.size())
    {
        const size_t nc = numCols(row);
        rData.reserve(nc); //!< @todo will this shrink reservation ?

        // Begin position
        size_t b = mSeparatorPositions[row][0];
        // Move file ptr
        fseek(mpFile, b, SEEK_SET);
        // Loop through each column on row
        for (size_t c=1; c<=nc; ++c)
        {
            const size_t e = mSeparatorPositions[row][c];
            char buff[e-b+1];
            char* rc = fgets(buff, e-b+1, mpFile);
            if (rc)
            {
                bool parseOK;
                rData.push_back(converter<T>(buff, parseOK));
                if (!parseOK)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }

            // Update b for next field, skipping the character itself
            b = mSeparatorPositions[row][c]+1;
            // Move the file ptr, 1 char (gobble the separator)
            fgetc(mpFile);
        }
        return true;
    }
    return false;
}


//! @brief Extract the data of a given indexed position row and column (as given template argument)
//! @tparam T The type do convert asci text to
//! @param[in] row The row index (0-based)
//! @param[in] col The column index (0-based)
//! @returns The value at the requested position as given templat type arguemnt default constructed value if position does not exist
template <typename T>
T IndexingCSVParser::getIndexedPosAs(const size_t row, const size_t col, bool &rParseOK)
{
    if (row < mSeparatorPositions.size())
    {
        if (col < mSeparatorPositions[row].size())
        {
            // Begin and end positions
            size_t b = mSeparatorPositions[row][col] + size_t(col > 0);
            size_t e = mSeparatorPositions[row][col+1];
            fseek(mpFile, b, SEEK_SET);

            char buff[e-b+1];
            char* rc = fgets(buff, e-b+1, mpFile);
            if (rc)
            {
                return converter<T>(buff, rParseOK);
            }
        }
    }
    rParseOK = false;
    return T();
}

template <typename T>
bool IndexingCSVParser::getRowAs(std::vector<T> &rData)
{
    bool isSuccess = true;
    size_t b = ftell(mpFile);
    while (true)
    {
        size_t e = ftell(mpFile);
        int c = fgetc(mpFile);

        if (c == mSeparatorChar || c == '\n' || c == '\r')
        {
            // Rewind file pointer to start of field
            fseek(mpFile, b, SEEK_SET);
            char buff[e-b+1];
            char* rc = fgets(buff, e-b+1, mpFile);
            if (rc)
            {
                bool parseOK;
                rData.push_back(converter<T>(buff, parseOK));
                // Indicate we failed to parse, but we still need to gobble the entire line incase we reach EOF
                if (!parseOK)
                {
                    isSuccess = false;
                }
            }
            else
            {
                // Indicate we failed to parse, but we still need to gobble the entire line incase we reach EOF
                isSuccess = false;
            }

            // Eat the separator char, in case of CRLF EOL, then gobble both CR and expected LF
            do
            {
                c = fgetc(mpFile);
                b = ftell(mpFile); //!< @todo maybe can use +1 since binary mode (calc bytes) might be faster
            }while(c == '\r');

            // Break loop when we have reachen EOL
            if (c == '\n')
            {
                // If we got a LF then peek to see if EOF reached, if so gooble char to set EOF flag on file
                if (peek(mpFile) == EOF)
                {
                    fgetc(mpFile);
                }
                break;
            }
        }
    }

    //! @todo try to index line first before extracting data, might be faster since we can reserve (maybe)
    return isSuccess;
}

}

#endif // INDEXINGCSVPARSERIMPL_HPP

