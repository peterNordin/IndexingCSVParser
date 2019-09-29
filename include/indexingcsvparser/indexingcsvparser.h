//!
//! @file   indexingcsvparser.h
//! @author Peter Nordin
//! @date   2015-02-03
//!
//! @brief Contains the definition of the IndexingCSVParser
//!

#ifndef INDEXINGCSVPARSER_H
#define INDEXINGCSVPARSER_H

#include <string>
#include <vector>

//! @brief The Indexing CSV Parser namespace
namespace indcsvp
{

enum Direction {Row, Column};
enum TrimSpaceOption {NoTrim, TrimLeadingTrailingSpace};

class HeaderSetting {

public:
    HeaderSetting();
    void setHeaderInfo(Direction direction, size_t rowOrCol);
    bool isValid() const;
    Direction direction() const;
    size_t rowOrColumn() const;

protected:
    size_t mHeaderIndex;
    Direction mDirection;
};

//! @brief The Indexing CSV Parser Class
class IndexingCSVParser
{
public:
    IndexingCSVParser();

    // ----- Configuration methods -----
    void setHeaderInfo(Direction direction, size_t rowOrColumn);
    void setSeparatorChar(char sep);
    void setCommentChar(char com);
    void setNumLinesToSkip(size_t num);
    char autoSetSeparatorChar(const std::vector<char> &rAlternatives);
    HeaderSetting getHeaderSetting() const;
    char getSeparatorChar() const;
    char getCommentChar() const;
    size_t getNumLinesToSkip() const;

    // ----- File methods -----
    bool openFile(const char* filePath);
    void closeFile();
    void rewindFile();
    void readUntilData();
    void skipNumLines(size_t num);

    // ----- Indexing access methods -----
    void indexFile();

    size_t numRows() const;
    size_t numCols(size_t row=0) const;
    bool allRowsHaveSameNumCols() const;
    void minMaxNumCols(size_t &rMin, size_t &rMax);
    const std::vector<std::string>& header() const;

    bool getIndexedColumn(const size_t col, std::vector<std::string> &rData, TrimSpaceOption trim=NoTrim);
    bool getIndexedRow(const size_t row, std::vector<std::string> &rData, TrimSpaceOption trim=NoTrim);
    std::string getIndexedPos(const size_t row, const size_t col, bool &rParseOK, TrimSpaceOption trim=NoTrim);

    template <typename T> bool getIndexedColumnAs(const size_t col, std::vector<T> &rData, TrimSpaceOption trim=NoTrim);
    template <typename T> bool getIndexedColumnRowRangeAs(const size_t col, const size_t startRow, const size_t numRows, std::vector<T> &rData, TrimSpaceOption trim=NoTrim);
    template <typename T> bool getIndexedRowAs(const size_t row, std::vector<T> &rData, TrimSpaceOption trim=NoTrim);
    template <typename T> bool getIndexedRowColumnRangeAs(const size_t row, const size_t startCol, const size_t numCols, std::vector<T> &rData, TrimSpaceOption trim=NoTrim);
    template <typename T> T getIndexedPosAs(const size_t row, const size_t col, bool &rParseOK, TrimSpaceOption trim=NoTrim);

    // ----- Non-indexing access methods -----
    bool getRow(std::vector<std::string> &rData, TrimSpaceOption trim=NoTrim);
    template <typename T> bool getRowAs(std::vector<T> &rData, TrimSpaceOption trim=NoTrim);
    bool hasMoreDataRows();

protected:
    bool getHeaderRow(std::vector<std::string> &rData);

    FILE *mpFile;           //!< @brief The internal file pointer
    char mSeparatorChar;    //!< @brief The chosen separator character
    char mCommentChar;      //!< @brief The chosen comment character
    size_t mNumSkipLines;   //!< @brief The initial lines to skip
    std::vector< std::vector<size_t> > mSeparatorMatrix; //!< @brief The index of separators
    std::vector< std::string > mHeader;
    HeaderSetting mHeaderSetting;
};

}

// Include the header-only implementation (template functions and methods)
#include "indexingcsvparserimpl.hpp"

#endif // INDEXINGCSVPARSER_H
