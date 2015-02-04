//!
//! @file   main.cpp
//! @author Peter Nordin
//! @date   2015-02-03
//!
//! @brief Example main program that uses the IndexingCSVParser class to parse and output csv data in different ways.
//!

#include <iostream>
#include "IndexingCSVParser.h"

using namespace std;


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Error to few arguments!" << endl;
        return 1;
    }

    cout << "Starting with file: " << argv[1] << endl;

    indcsvp::IndexingCSVParser csv;
    if (!csv.openFile(argv[1]))
    {
        cout << "Error; Could not open file: " << argv[1] << endl;
        return 1;
    }

    csv.setCommentChar('#');

    vector<char> possibleSeparators;
    possibleSeparators.push_back(';');
    possibleSeparators.push_back(',');
    char sep = csv.autoSetSeparatorChar(possibleSeparators);
    cout << "Choosing separator char: " << sep << endl;

    // Index the file
    csv.indexFile();

    cout << "File: " << argv[1] << " has nRows: " << csv.numRows() << " nCols: " << csv.numCols() << " All rows have same num cols (1 = true): " << csv.allRowsHaveSameNumCols() << endl;

    if (csv.numRows() < 100 && csv.numCols() < 100)
    {
        cout << "---------- Contents ----------" << endl;
        bool parseOK;
        for (size_t r=0; r<csv.numRows(); ++r)
        {
            for (size_t c=0; c<csv.numCols(r); ++c)
            {
                cout << csv.getIndexedPosAs<std::string>(r,c, parseOK) << " ";
            }
            cout << endl;
        }

        cout << endl << "---------- Rows ----------" << endl;
        for (size_t r=0; r<csv.numRows(); ++r)
        {
            vector<double> row;
            bool rc = csv.getIndexedRowAs<double>(r, row);
            if (rc)
            {
                for (size_t i=0; i<row.size(); ++i)
                {
                    cout << row[i] << " ";
                }
            }
            else
            {
                cout << "There were errors (aborting) parsing row: " << r;
            }
            cout << endl;
        }

        cout << endl << "---------- Cols ----------" << endl;
        for (size_t c=0; c<csv.numCols(); ++c)
        {
            vector<long int> col;
            bool rc = csv.getIndexedColumnAs<long int>(c, col);
            if (rc)
            {
                for (size_t i=0; i<col.size(); ++i)
                {
                    cout << col[i] << " ";
                }
            }
            else
            {
                cout << "There were errors (aborting) parsing column: " << c;
            }
            cout << endl;
        }

        cout << endl << "---------- Get non-indexed rows ----------" << endl;
        csv.rewindFile();
        csv.readUntilData();
        while (csv.hasMoreDataRows())
        {
            vector<double> data;
            csv.getRowAs<double>(data);
            for (size_t i=0; i<data.size(); ++i)
            {
                cout << data[i] << " ";
            }
            cout << endl;
        }
    }
    else
    {
        cout << "File very large, not showing contents!" << endl;
    }


    csv.closeFile();
    cout << endl << "Done!" << endl;
    return 0;
}

