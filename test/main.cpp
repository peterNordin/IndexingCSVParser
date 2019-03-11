//!
//! @file   main.cpp
//! @author Peter Nordin
//! @date   2015-02-03
//!
//! @brief Example main program that uses the IndexingCSVParser class to parse and output csv data in different ways.
//!

#include <iostream>
#define INDCSVP_REPLACEDECIMALCOMMA
#include "IndexingCSVParser.h"

using namespace std;

template<typename T>
void printVector(vector<T> &rVector)
{
    for (size_t i=0; i<rVector.size(); ++i)
    {
        cout << rVector[i] << " ";
    }
    cout << endl;
}


int main(int argc, char *argv[])
{
    string filePath;
    if (argc < 2)
    {
        filePath = "./Test1_d_snc_c_crlf.csv";
        cout << "Warning to few arguments! Using default file: " << filePath << endl;
    }
    else
    {
        filePath = argv[1];
    }
    char sepChar = '0';
    if (argc == 3)
    {
        sepChar = string(argv[2])[0];
    }

    cout << "Using CSV file: " << filePath << endl;

    indcsvp::IndexingCSVParser icsvp;
    if (!icsvp.openFile(filePath.c_str()))
    {
        cout << "Error; Could not open file: " << filePath << endl;
        return 1;
    }

    icsvp.setCommentChar('#');

    vector<char> possibleSeparators;
    possibleSeparators.push_back(';');
    possibleSeparators.push_back(',');
    char sep = icsvp.autoSetSeparatorChar(possibleSeparators);
    cout << "Auto choosing separator char: " << sep << endl;
    if (sepChar != '0')
    {
        icsvp.setSeparatorChar(';');
    }
    sepChar = icsvp.getSeparatorChar();
    cout << "Using separator char: " << sepChar << endl;


    // Index the file
    icsvp.indexFile();
    cout << "File: " << filePath << " has nRows: " << icsvp.numRows() << " nCols: " << icsvp.numCols() << " All rows have same num cols (1 = true): " << icsvp.allRowsHaveSameNumCols() << endl;

    if (icsvp.numRows() < 100 && icsvp.numCols() < 100)
    {
        cout << "---------- Contents as string (position by position) ----------" << endl;
        bool parseOK;
        for (size_t r=0; r<icsvp.numRows(); ++r)
        {
            for (size_t c=0; c<icsvp.numCols(r); ++c)
            {
                cout << icsvp.getIndexedPosAs<std::string>(r,c, parseOK) << " ";
            }
            cout << endl;
        }

        cout << endl << "---------- Fetching all indexed rows ----------" << endl;
        for (size_t r=0; r<icsvp.numRows(); ++r)
        {
            vector<double> row;
            bool rc = icsvp.getIndexedRowAs<double>(r, row);
            if (rc)
            {
                printVector(row);
            }
            else
            {
                cout << "There were errors (aborting) parsing row: " << r << endl;
            }
        }

        cout << endl << "---------- Fetching Rows Columns 0->"<< icsvp.numCols()/2-1 << " ----------" << endl;
        for (size_t r=0; r<icsvp.numRows(); ++r)
        {
            vector<double> row;
            bool rc = icsvp.getIndexedRowColumnRangeAs<double>(r, 0, icsvp.numCols(r)/2, row);
            if (rc)
            {
                printVector(row);
            }
            else
            {
                cout << "There were errors (aborting) parsing row: " << r << endl;
            }
        }

        cout << endl << "---------- Fetching Rows Columns " << icsvp.numCols()/2 << "->" << icsvp.numCols()-1 << " ----------" << endl;
        for (size_t r=0; r<icsvp.numRows(); ++r)
        {
            vector<double> row;
            bool rc = icsvp.getIndexedRowColumnRangeAs<double>(r, icsvp.numCols(r)/2, icsvp.numCols()-icsvp.numCols(r)/2, row);
            if (rc)
            {
                printVector(row);
            }
            else
            {
                cout << "There were errors (aborting) parsing row: " << r << endl;
            }
        }


        cout << endl << "---------- Fetching all indexed columns (printed transposed) ----------" << endl;
        for (size_t c=0; c<icsvp.numCols(); ++c)
        {
            vector<double> col;
            bool rc = icsvp.getIndexedColumnAs<double>(c, col);
            if (rc)
            {
                printVector(col);
            }
            else
            {
                cout << "There were errors (aborting) parsing column: " << c << endl;
            }
        }

        cout << endl << "---------- Fetching Columns Rows: 0->" << icsvp.numRows()/2-1 << " (printed transposed) ----------" << endl;
        for (size_t c=0; c<icsvp.numCols(); ++c)
        {
            vector<double> col;
            bool rc = icsvp.getIndexedColumnRowRangeAs<double>(c, 0, icsvp.numRows()/2, col);
            if (rc)
            {
                printVector(col);
            }
            else
            {
                cout << "There were errors (aborting) parsing column: " << c << endl;
            }
        }

        cout << endl << "---------- Fetching Columns Rows: " << icsvp.numRows()/2 << "->" << icsvp.numRows()-1 << " (printed transposed) ----------" << endl;
        for (size_t c=0; c<icsvp.numCols(); ++c)
        {
            vector<double> col;
            bool rc = icsvp.getIndexedColumnRowRangeAs<double>(c, icsvp.numRows()/2, icsvp.numRows()-icsvp.numRows()/2 , col);
            if (rc)
            {
                printVector(col);
            }
            else
            {
                cout << "There were errors (aborting) parsing column: " << c << endl;
            }
        }



        cout << endl << "---------- Get non-indexed rows one by one until EOF ----------" << endl;
        icsvp.rewindFile();
        icsvp.readUntilData();
        while (icsvp.hasMoreDataRows())
        {
            vector<double> data;
            bool isok = icsvp.getRowAs<double>(data);
            for (size_t i=0; i<data.size(); ++i)
            {
                cout << data[i] << " ";
            }
            if (!isok)
            {
                cout << "    <<<" << "There were parsing errors on this row";
            }
            cout << endl;
        }
    }
    else
    {
        cout << "File very large, not showing contents!" << endl;
    }


    icsvp.closeFile();
    cout << endl << "Done!" << endl;
    return 0;
}

