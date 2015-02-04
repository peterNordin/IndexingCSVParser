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

    //csv.setSeparatorChar(';');

    // Index the file
    csv.indexFile();

    cout << "File: " << argv[1] << " has nRows: " << csv.numRows() << " nCols: " << csv.numCols() << " All rows have same num cols (1 = true): " << csv.allRowsHaveSameNumCols() << endl;

    if (csv.numRows() < 100 && csv.numCols() < 100)
    {
        cout << "---------- Contents as string (position by position) ----------" << endl;
        bool parseOK;
        for (size_t r=0; r<csv.numRows(); ++r)
        {
            for (size_t c=0; c<csv.numCols(r); ++c)
            {
                cout << csv.getIndexedPosAs<std::string>(r,c, parseOK) << " ";
            }
            cout << endl;
        }

        cout << endl << "---------- Fetching all indexed rows ----------" << endl;
        for (size_t r=0; r<csv.numRows(); ++r)
        {
            vector<double> row;
            bool rc = csv.getIndexedRowAs<double>(r, row);
            if (rc)
            {
                printVector(row);
            }
            else
            {
                cout << "There were errors (aborting) parsing row: " << r << endl;
            }
        }

        cout << endl << "---------- Fetching Rows Columns 0->"<< csv.numCols()/2-1 << " ----------" << endl;
        for (size_t r=0; r<csv.numRows(); ++r)
        {
            vector<double> row;
            bool rc = csv.getIndexedRowColumnRangeAs<double>(r, 0, csv.numCols(r)/2, row);
            if (rc)
            {
                printVector(row);
            }
            else
            {
                cout << "There were errors (aborting) parsing row: " << r << endl;
            }
        }

        cout << endl << "---------- Fetching Rows Columns " << csv.numCols()/2 << "->" << csv.numCols()-1 << " ----------" << endl;
        for (size_t r=0; r<csv.numRows(); ++r)
        {
            vector<double> row;
            bool rc = csv.getIndexedRowColumnRangeAs<double>(r, csv.numCols(r)/2, csv.numCols()-csv.numCols(r)/2, row);
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
        for (size_t c=0; c<csv.numCols(); ++c)
        {
            vector<double> col;
            bool rc = csv.getIndexedColumnAs<double>(c, col);
            if (rc)
            {
                printVector(col);
            }
            else
            {
                cout << "There were errors (aborting) parsing column: " << c << endl;
            }
        }

        cout << endl << "---------- Fetching Columns Rows: 0->" << csv.numRows()/2-1 << " (printed transposed) ----------" << endl;
        for (size_t c=0; c<csv.numCols(); ++c)
        {
            vector<double> col;
            bool rc = csv.getIndexedColumnRowRangeAs<double>(c, 0, csv.numRows()/2, col);
            if (rc)
            {
                printVector(col);
            }
            else
            {
                cout << "There were errors (aborting) parsing column: " << c << endl;
            }
        }

        cout << endl << "---------- Fetching Columns Rows: " << csv.numRows()/2 << "->" << csv.numRows()-1 << " (printed transposed) ----------" << endl;
        for (size_t c=0; c<csv.numCols(); ++c)
        {
            vector<double> col;
            bool rc = csv.getIndexedColumnRowRangeAs<double>(c, csv.numRows()/2, csv.numRows()-csv.numRows()/2 , col);
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

