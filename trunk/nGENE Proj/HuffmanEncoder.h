/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		HuffmanEncoder.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_HUFFMANENCODER_H_
#define __INC_HUFFMANENCODER_H_


#include "IEncoder.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Huffman encoder.
		@par
			Huffman algorithm is an entropy encoder.
	*/
	class nGENEDLL HuffmanEncoder
	{
	protected:
		string m_Codes[256];			///< Codes for all 256 ASCII characters

		/// Node of a Huffman codes tree.
		struct SHuffmanNode
		{
			uint frequency;				///< Frequency of the character
			char character;				///< Character with which this node is associated
			SHuffmanNode* leftChild;	///< Pointer to the left child
			SHuffmanNode* rightChild;	///< Pointer to the right child
			SHuffmanNode* parent;		///< Pointer to the parent node
			string* codes;				///< Pointer to Huffman codes table
			bool isLeaf;				///< True if the node is a leaf
			
			SHuffmanNode();

			bool operator>(const SHuffmanNode& _node) const;

			void cleanup();
			
			void calculateCodes(string _code="") const;
			void printTree(ostream& _result, string _code="") const;

			/// Returns total number of tree nodes.
			void getSize(uint& _size);

			void write(ostream& _result, uint& _bits, byte& _currByte, uint& _bytes);
			void write(char* _result, uint& _bits, byte& _currByte, uint& _bytes, uint& _index);
			/// Writes single bit to the output.
			void writeBit(ostream& _result, uint& _bits, byte& _currByte, uint& _bytes, bool _value);
			/// Writes single bit to the output.
			void writeBit(char* _result, uint& _bits, byte& _currByte, uint& _bytes, bool _value, uint& _index);
		};


		priority_queue <SHuffmanNode, vector <SHuffmanNode>, greater<SHuffmanNode> > m_Heap;


		/// Reads single bit from the data.
		bool readBit(const char** _data, byte& _byte, uint& _bits, uint& _bitsLeft);
		/// Reads whole byte from the data.
		byte readByte(const char** _data, byte& _byte, uint& _bits, uint& _bitsLeft);

		/// Builds Huffman codes tree.
		void buildHuffmanTree(const char* _data, uint _size);
		/// Reconstructs tree from the input data.
		SHuffmanNode* reconstructTreeFromData(const char** _data, byte& _byte, uint& _bits, uint& _bitsLeft);

	public:
		uint encode(char* _result, const char* _data, uint _size);
		void encode(ostream& _result, const char* _data, uint _size);

		uint decode(char* _result, const char* _data, uint _size);
		void decode(ostream& _result, const char* _data, uint _size);
	};
}


#endif