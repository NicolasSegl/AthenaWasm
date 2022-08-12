#include <random>

#include "Constants.h"
#include "ZobristKeyGenerator.h"

uint64_t ZobristKeyGenerator::getRandom64()
{
	return (rand()) | (rand() << 16) | ((uint64_t)rand() << 32) | ((uint64_t)rand() << 48);
}

void ZobristKeyGenerator::initHashKeys()
{
	for (int pieceType = 0; pieceType < 12; pieceType++)
		for (int square = 0; square < 64; square++)
			mPieceHashKeys[pieceType][square] = getRandom64();

	// for the sake of the tranposition table, should we store the square of the enPassant instead of a bitboard? i think yes? 
	for (int castle = 0; castle < 16; castle++)
		mCastleHashKeys[castle] = getRandom64();

	for (int square = 0; square < 64; square++)
		mEnpassantHashKeys[square] = getRandom64();

	mSideToPlayHashKey = getRandom64();
}

uint64_t ZobristKeyGenerator::getPieceHashKeyForGeneration(ChessPosition* chessPosition, Byte square)
{
	if (BB::boardSquares[square] & chessPosition->whitePiecesBB)
	{
		if	    (BB::boardSquares[square] & chessPosition->whitePawnsBB)   return mPieceHashKeys[PieceTypes::WHITE_PAWN][square];
		else if (BB::boardSquares[square] & chessPosition->whiteRooksBB)   return mPieceHashKeys[PieceTypes::WHITE_ROOK][square];
		else if (BB::boardSquares[square] & chessPosition->whiteBishopsBB) return mPieceHashKeys[PieceTypes::WHITE_BISHOP][square];
		else if (BB::boardSquares[square] & chessPosition->whiteQueensBB)  return mPieceHashKeys[PieceTypes::WHITE_QUEEN][square];
		else if (BB::boardSquares[square] & chessPosition->whiteKnightsBB) return mPieceHashKeys[PieceTypes::WHITE_KNIGHT][square];
		else if (BB::boardSquares[square] & chessPosition->whiteKingBB)	   return mPieceHashKeys[PieceTypes::WHITE_KING][square];
	}
	else // the occupied square is a black piece...
	{
		if		(BB::boardSquares[square] & chessPosition->blackPawnsBB)   return mPieceHashKeys[PieceTypes::BLACK_PAWN][square];
		else if (BB::boardSquares[square] & chessPosition->blackRooksBB)   return mPieceHashKeys[PieceTypes::BLACK_ROOK][square];
		else if (BB::boardSquares[square] & chessPosition->blackBishopsBB) return mPieceHashKeys[PieceTypes::BLACK_BISHOP][square];
		else if (BB::boardSquares[square] & chessPosition->blackQueensBB)  return mPieceHashKeys[PieceTypes::BLACK_QUEEN][square];
		else if (BB::boardSquares[square] & chessPosition->blackKnightsBB) return mPieceHashKeys[PieceTypes::BLACK_KNIGHT][square];
		else if (BB::boardSquares[square] & chessPosition->blackKingBB)	   return mPieceHashKeys[PieceTypes::BLACK_KING][square];
	}
}

uint64_t ZobristKeyGenerator::getPieceHashKeyForUpdate(ChessPosition* chessPosition, Bitboard* pieceBB, Byte square)
{
	if (!pieceBB)
		return 0;

	if (*pieceBB & chessPosition->whitePiecesBB)
	{
		if		(*pieceBB & chessPosition->whitePawnsBB)   return mPieceHashKeys[PieceTypes::WHITE_PAWN][square];
		else if (*pieceBB & chessPosition->whiteRooksBB)   return mPieceHashKeys[PieceTypes::WHITE_ROOK][square];
		else if (*pieceBB & chessPosition->whiteBishopsBB) return mPieceHashKeys[PieceTypes::WHITE_BISHOP][square];
		else if (*pieceBB & chessPosition->whiteQueensBB)  return mPieceHashKeys[PieceTypes::WHITE_QUEEN][square];
		else if (*pieceBB & chessPosition->whiteKnightsBB) return mPieceHashKeys[PieceTypes::WHITE_KNIGHT][square];
		else if (*pieceBB & chessPosition->whiteKingBB)	   return mPieceHashKeys[PieceTypes::WHITE_KING][square];
	}
	else // the occupied square is a black piece...
	{
		if		(*pieceBB & chessPosition->blackPawnsBB)   return mPieceHashKeys[PieceTypes::BLACK_PAWN][square];
		else if (*pieceBB & chessPosition->blackRooksBB)   return mPieceHashKeys[PieceTypes::BLACK_ROOK][square];
		else if (*pieceBB & chessPosition->blackBishopsBB) return mPieceHashKeys[PieceTypes::BLACK_BISHOP][square];
		else if (*pieceBB & chessPosition->blackQueensBB)  return mPieceHashKeys[PieceTypes::BLACK_QUEEN][square];
		else if (*pieceBB & chessPosition->blackKnightsBB) return mPieceHashKeys[PieceTypes::BLACK_KNIGHT][square];
		else if (*pieceBB & chessPosition->blackKingBB)	   return mPieceHashKeys[PieceTypes::BLACK_KING][square];
	}
}
ZobristKey ZobristKeyGenerator::generateKey(ChessPosition* chessPosition)
{
	ZobristKey zobristKey = 0;

	for (int square = 0; square < 64; square++)
	{
		if (BB::boardSquares[square] & chessPosition->emptyBB) continue;

		zobristKey ^= getPieceHashKeyForGeneration(chessPosition, square);
		zobristKey ^= getPieceHashKeyForGeneration(chessPosition, square);
	}

	zobristKey ^= mCastleHashKeys[chessPosition->castlePrivileges];
	zobristKey ^= mEnpassantHashKeys[chessPosition->enPassantSquare];
	if (!chessPosition->sideToMove)
		zobristKey ^= mSideToPlayHashKey;

	return zobristKey;
}

ZobristKey ZobristKeyGenerator::updateKey(ZobristKey zobristKey, ChessPosition* chessPosition, MoveData* moveData)
{
	zobristKey ^= getPieceHashKeyForUpdate(chessPosition, moveData->pieceBB, moveData->originSquare);
	zobristKey ^= getPieceHashKeyForUpdate(chessPosition, moveData->pieceBB, moveData->targetSquare);
	zobristKey ^= getPieceHashKeyForUpdate(chessPosition, moveData->capturedPieceBB, moveData->targetSquare);

	zobristKey ^= mCastleHashKeys[chessPosition->castlePrivileges];
	zobristKey ^= mEnpassantHashKeys[chessPosition->enPassantSquare];
	if (!chessPosition->sideToMove)
		zobristKey ^= mSideToPlayHashKey;

	return zobristKey;
}