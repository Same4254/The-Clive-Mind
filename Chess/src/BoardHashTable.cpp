#include "BoardHashTable.hpp"

BoardHashTable::BoardHashTable(uint32_t numPieceTypes, uint32_t numRows, uint32_t numCols) {
    //Deterministic Behavior
    random.seed(1729);

    pieceLocationToHash.clear();
    pieceLocationIndexToHash.clear();

    // For every piece at every location, give it a random number
    // Support for 1D and 2D access
    for(PieceFunctionalityIndex pieceIndex = 0; pieceIndex < numPieceTypes; pieceIndex++) {
        //2D
        std::vector<std::vector<BoardHash>> rowColMatrix;
        //1D
        std::vector<BoardHash> rowColVector;

        for(uint32_t row = 0; row < numRows; row++) {
            std::vector<BoardHash> rowVector;
            for(uint32_t col = 0; col < numCols; col++) {
                BoardHash value = uniformDistribution(random);
                rowVector.push_back(value);
                rowColVector.push_back(value);
            }

            rowColMatrix.push_back(rowVector);
        }

        pieceLocationToHash.push_back(rowColMatrix);
        pieceLocationIndexToHash.push_back(rowColVector);
    }
}