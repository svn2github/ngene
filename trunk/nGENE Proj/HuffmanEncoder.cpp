/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		HuffmanEncoder.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "HuffmanEncoder.h"


namespace nGENE
{
	void HuffmanEncoder::buildHuffmanTree(const char* _data, uint _size)
	{
		// Calculate frequencies
		SHuffmanNode nodes[256];
		for(uint i = 0; i < _size; ++i)
		{
			char index = _data[i];
			nodes[(byte)index].character = index;
			nodes[(byte)index].codes = m_Codes;
			++nodes[(byte)index].frequency;
		}

		for(uint i = 0; i < 256; ++i)
		{
			if(nodes[i].frequency)
				m_Heap.push(nodes[i]);
		}


		// Build heap
		while(m_Heap.size() > 1)
		{
			SHuffmanNode* leftChild = new SHuffmanNode(m_Heap.top());
			m_Heap.pop();
			SHuffmanNode* rightChild = new SHuffmanNode(m_Heap.top());
			m_Heap.pop();

			SHuffmanNode newNode;
			newNode.leftChild = leftChild;
			newNode.rightChild = rightChild;
			newNode.character = 0;
			newNode.codes = m_Codes;
			newNode.frequency = leftChild->frequency + rightChild->frequency;

			m_Heap.push(newNode);
		}
	}
//----------------------------------------------------------------------
	uint HuffmanEncoder::encode(char* _result, const char* _data, uint _size)
	{
		if(!_size)
			return 0;

		uint nBits = 0;
		byte nCurrByte = 0;
		uint nBytes = 0;
		uint nIndex = 0;

		uint nSize = 0;

		memcpy(&_result[nIndex], &_size, sizeof(uint));
		nIndex += sizeof(uint);
		memcpy(&_result[nIndex], &_size, sizeof(uint));
		nIndex += sizeof(uint);

		// Build tree
		buildHuffmanTree(_data, _size);

		// Write number of nodes in the tree
		SHuffmanNode rootNode = m_Heap.top();
		rootNode.getSize(nSize);
		memcpy(&_result[nIndex], &nSize, sizeof(uint));
		nIndex += sizeof(uint);

		// Write tree to stream
		rootNode.calculateCodes();
		rootNode.write(_result, nBits, nCurrByte, nBytes, nIndex);
		SHuffmanNode top = m_Heap.top();
		m_Heap.pop();
		top.cleanup();

		const char* pointer = _data;
		char curr = 0;
		while(pointer < _data + _size)
		{
			curr = *(pointer++);
			string& code = m_Codes[(byte)curr];

			char c = 0;
			for(uint i = 0; i < code.length(); ++i)
			{
				c = code[i];

				nCurrByte <<= 1;
				if(c == '1')
					nCurrByte |= 1;
				++nBits;
				if(nBits == 8)
				{
					_result[nIndex++] = (char)nCurrByte;
					++nBytes;
					nBits = 0;
					nCurrByte = 0;
				}
			}
		}

		// Write any remaining bits
		while(nBits)
		{
			nCurrByte <<= 1;
			++nBits;
			if(nBits == 8)
			{
				_result[nIndex++] = (char)nCurrByte;
				++nBytes;
				nBits = 0;
				nCurrByte = 0;
			}
		}


		return nIndex;
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::encode(ostream& _result, const char* _data, uint _size)
	{
		if(!_size)
			return;

		_result.write((char*)&_size, sizeof(uint));

		// Build tree
		buildHuffmanTree(_data, _size);

		uint nBits = 0;
		byte nCurrByte = 0;
		uint nBytes = 0;

		uint nSize = 0;

		// Write number of nodes in the tree
		SHuffmanNode rootNode = m_Heap.top();
		rootNode.getSize(nSize);
		_result.write((char*)&nSize, sizeof(uint));

		// Write tree to stream
		rootNode.calculateCodes();
		rootNode.write(_result, nBits, nCurrByte, nBytes);
		SHuffmanNode top = m_Heap.top();
		m_Heap.pop();
		top.cleanup();

		const char* pointer = _data;
		char curr = 0;
		while(pointer < _data + _size)
		{
			curr = *(pointer++);
			string& code = m_Codes[(byte)curr];

			char c = 0;
			for(uint i = 0; i < code.length(); ++i)
			{
				c = code[i];

				nCurrByte <<= 1;
				if(c == '1')
					nCurrByte |= 1;
				++nBits;
				if(nBits == 8)
				{
					_result.write((char*)&nCurrByte, sizeof(byte));
					++nBytes;
					nBits = 0;
					nCurrByte = 0;
				}
			}
		}

		// Write any remaining bits
		while(nBits)
		{
			nCurrByte <<= 1;
			++nBits;
			if(nBits == 8)
			{
				_result.write((char*)&nCurrByte, sizeof(byte));
				++nBytes;
				nBits = 0;
				nCurrByte = 0;
			}
		}
	}
//----------------------------------------------------------------------
	uint HuffmanEncoder::decode(char* _result, const char* _data, uint _size)
	{
		uint nIndex = 0;
		byte nByte = 0;
		uint nBits = 0;
		uint nBitsLeft = 0;
		int count = 0;

		const char* pointer = _data;

		memcpy(&count, pointer, sizeof(uint));
		pointer += sizeof(uint);

		SHuffmanNode* pRoot = reconstructTreeFromData(&pointer, nByte, nBits, nBitsLeft);

		while(pointer < _data + _size || count > 0)
		{
			SHuffmanNode* pNode = pRoot;
			while(!pNode->isLeaf)
			{
				bool bit = readBit(&pointer, nByte, nBits, nBitsLeft);
				if(bit)
					pNode = pNode->rightChild;
				else
					pNode = pNode->leftChild;
			}

			_result[nIndex++] = pNode->character;
			--count;
		}

		pRoot->cleanup();
		NGENE_DELETE(pRoot);

		return nIndex;
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::decode(ostream& _result, const char* _data, uint _size)
	{
		byte nByte = 0;
		uint nBits = 0;
		uint nBitsLeft = 0;
		int count = 0;

		const char* pointer = _data;

		memcpy(&count, pointer, sizeof(uint));
		pointer += sizeof(uint);

		SHuffmanNode* pRoot = reconstructTreeFromData(&pointer, nByte, nBits, nBitsLeft);

		while(pointer < _data + _size || count > 0)
		{
			SHuffmanNode* pNode = pRoot;
			while(!pNode->isLeaf)
			{
				bool bit = readBit(&pointer, nByte, nBits, nBitsLeft);
				if(bit)
					pNode = pNode->rightChild;
				else
					pNode = pNode->leftChild;
			}

			_result << pNode->character;
			--count;
		}

		pRoot->cleanup();
		NGENE_DELETE(pRoot);
	}
//----------------------------------------------------------------------
	HuffmanEncoder::SHuffmanNode* HuffmanEncoder::reconstructTreeFromData(const char** _data, byte& _byte, uint& _bits, uint& _bitsLeft)
	{
		uint nSize = 0;

		const char* pointer = *_data;

		// Read number of nodes in the tree
		memcpy(&nSize, pointer, sizeof(uint));
		pointer += sizeof(uint);

		vector <SHuffmanNode> vNodes;
		vector <char> vLeaves;

		while(vNodes.size() < nSize)
		{
			bool bLeaf = readBit(&pointer, _byte, _bits, _bitsLeft);
			if(bLeaf)
			{
				SHuffmanNode newNode;
				newNode.character = (char)readByte(&pointer, _byte, _bits, _bitsLeft);
				newNode.codes = m_Codes;

				vNodes.push_back(newNode);
				vLeaves.push_back(1);
			}
			else
			{
				SHuffmanNode newNode;

				newNode.codes = m_Codes;

				vNodes.push_back(newNode);
				vLeaves.push_back(0);
			}
		}

		// Now reconstruct the tree
		SHuffmanNode* pRoot = NULL;
		SHuffmanNode* pCurrent = NULL;
		for(uint i = 0; i < nSize; ++i)
		{
			if(vLeaves[i])
			{
				SHuffmanNode* pNewNode = new SHuffmanNode(vNodes[i]);
				pNewNode->parent = pCurrent;
				pNewNode->isLeaf = true;

				if(!pCurrent->leftChild)
					pCurrent->leftChild = pNewNode;
				else
				{
					pCurrent->rightChild = pNewNode;
					if(i != nSize - 1)
					{
						while(pCurrent->rightChild)
							pCurrent = pCurrent->parent;
					}
				}
			}
			else
			{
				SHuffmanNode* pNewNode = new SHuffmanNode(vNodes[i]);
				pNewNode->parent = pCurrent;

				if(!pRoot)
				{
					pRoot = pNewNode;
				}
				else
				{
					if(!pCurrent->leftChild)
						pCurrent->leftChild = pNewNode;
					else
						pCurrent->rightChild = pNewNode;
				}

				pCurrent = pNewNode;
			}
		}

		pRoot->calculateCodes();

		(*_data) = pointer;


		return pRoot;
	}
//----------------------------------------------------------------------
	bool HuffmanEncoder::readBit(const char** _data, byte& _byte, uint& _bits, uint& _bitsLeft) 
	{
		if(!_bitsLeft)
		{
			_byte = (byte)*((*(_data))++);
			_bitsLeft = 8;
		}
		++_bits;

		return (bool)((_byte >> --_bitsLeft) & 1);
	}
//----------------------------------------------------------------------
	byte HuffmanEncoder::readByte(const char** _data, byte& _byte, uint& _bits, uint& _bitsLeft)
	{
		byte result = 0;
		for(uint i = 0; i < 8; ++i)
		{
			result <<= 1;
			if(readBit(_data, _byte, _bits, _bitsLeft))
				result |= 1;
		}

		return result;
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	HuffmanEncoder::SHuffmanNode::SHuffmanNode()
	{
		frequency = 0;
		character = 0;
		leftChild = NULL;
		rightChild = NULL;
		parent = NULL;
		codes = NULL;
		isLeaf = false;
	}
//----------------------------------------------------------------------
	bool HuffmanEncoder::SHuffmanNode::operator>(const SHuffmanNode& _node) const
	{
		return frequency > _node.frequency;
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::SHuffmanNode::cleanup()
	{
		if(leftChild)
		{
			leftChild->cleanup();
			rightChild->cleanup();
		}

		NGENE_DELETE(leftChild);
		NGENE_DELETE(rightChild);
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::SHuffmanNode::calculateCodes(string _code) const
	{
		if(leftChild)
		{
			leftChild->calculateCodes(_code + '0');
			rightChild->calculateCodes(_code + '1');
		}
		else
			codes[(byte)character] = _code;
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::SHuffmanNode::printTree(ostream& _result, string _code) const
	{
		if(leftChild)
		{
			leftChild->printTree(_result, _code + '0');
			rightChild->printTree(_result, _code + '1');
		}
		else
		{
			_result << character << " ";
			_result << frequency << " ";
			_result << _code << endl;
		}
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::SHuffmanNode::getSize(uint& _size)
	{
		++_size;

		if(leftChild)
		{
			leftChild->getSize(_size);
			rightChild->getSize(_size);
		}
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::SHuffmanNode::write(ostream& _result, uint& _bits, byte& _currByte, uint& _bytes)
	{
		if(leftChild)
		{
			writeBit(_result, _bits, _currByte, _bytes, false);

			leftChild->write(_result, _bits, _currByte, _bytes);
			rightChild->write(_result, _bits, _currByte, _bytes);
		}
		else
		{
			writeBit(_result, _bits, _currByte, _bytes, true);

			int c = (int)character;
			for(uint i = 0; i < 8; ++i)
			{
				bool value = ((1 << (7 - i)) & c) != 0;
				writeBit(_result, _bits, _currByte, _bytes, value);
			}
		}
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::SHuffmanNode::write(char* _result, uint& _bits, byte& _currByte, uint& _bytes, uint& _index)
	{
		if(leftChild)
		{
			writeBit(_result, _bits, _currByte, _bytes, false, _index);

			leftChild->write(_result, _bits, _currByte, _bytes, _index);
			rightChild->write(_result, _bits, _currByte, _bytes, _index);
		}
		else
		{
			writeBit(_result, _bits, _currByte, _bytes, true, _index);

			int c = (int)character;
			for(uint i = 0; i < 8; ++i)
			{
				bool value = ((1 << (7 - i)) & c) != 0;
				writeBit(_result, _bits, _currByte, _bytes, value, _index);
			}
		}
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::SHuffmanNode::writeBit(ostream& _result, uint& _bits, byte& _currByte, uint& _bytes, bool _value)
	{
		_currByte <<= 1;
		if(_value)
			_currByte |= 1;
		++_bits;
		if(_bits == 8)
		{
			_result.write((char*)&_currByte, sizeof(byte));
			++_bytes;
			_bits = 0;
			_currByte = 0;
		}
	}
//----------------------------------------------------------------------
	void HuffmanEncoder::SHuffmanNode::writeBit(char* _result, uint& _bits, byte& _currByte, uint& _bytes, bool _value, uint& _index)
	{
		_currByte <<= 1;
		if(_value)
			_currByte |= 1;
		++_bits;
		if(_bits == 8)
		{
			_result[_index++] = _currByte;
			++_bytes;
			_bits = 0;
			_currByte = 0;
		}
	}
//----------------------------------------------------------------------
}