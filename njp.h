
#ifndef NJP_H
#define NJP_H

#include <chrono>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <set>
#include <list>
#include <vector>
#include <thread>
#include <deque>


#include <cstdlib>
#include <cstring>

#define VECTOR std::vector


namespace wiz {
	template <typename T> /// x is 10吏꾩닔..
	inline static T pos_1(const T x, const int base = 10) // 1먮━ 媛怨꾩궛
	{
		if (x >= 0) { return x % base; }// x - ( x / 10 ) * 10; }
		else { return (x / base) * base - x; }
		// -( x - ( (x/10) * 10 ) )
	}
	template <typename T> /// T <- char, int, long, long long...
	std::string toStr(const T x, const int base = 10);

	/// chk.... need more thinking..., ToDo...
	template <typename T> /// T <- char, int, long, long long...
	static std::string toStr2(const T x, const int str_space, const int base = 10);

	inline std::string str(const int x)
	{
		return toStr<int>(x, 10);
	}

	template <typename T> /// 몄텧좊븣 뚯븘泥댄겕쒕떎
	inline std::string _toString(const T x)
	{
		std::stringstream strs;
		strs << x;
		return strs.str();
	}
	template <>
	inline std::string _toString(const long double x)
	{
		std::stringstream strs;
		strs << std::fixed << x;
		std::string temp = strs.str();
		std::size_t idx = temp.find('.');
		if (idx == temp.size() - 1) {
			temp.push_back('0');
		}
		else if (idx == std::string::npos) {
			temp.push_back('.');
			temp.push_back('0');
		}
		return temp;
	}

	template <>
	inline std::string _toString(const long long x)
	{
		return toStr(x, 10);
	}
	template <>
	inline std::string _toString(const int x)
	{
		return str(x);
	}
	template <>
	inline std::string _toString(const bool x)
	{
		if (x) { return "true"; }
		return "false";
	}

	inline static bool isWhitespace(const char ch)
	{
		return isspace(ch);
		/*
		switch (ch)
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case '\0':
			return true;
		}
		return false;
		*/
	}


	class Token2
	{
	public:
		char* str = nullptr;
		int len = 0;
		bool isComment = false;
	public:
		Token2() { }
		Token2(char* str, int len, bool isComment) :
			str(str), len(len), isComment(isComment) { }
	};

	class LoadDataOption
	{
	public:
		std::vector<char> Left, Right;	// { } , [ ] <- json
		std::vector<char> Assignment;	// = , :
		std::vector<char> Removal;		// ',', empty. 
	};

	inline static int Equal(const std::vector<char>& option, const char ch)
	{
		for (int i = 0; i < option.size(); ++i) {
			if (ch == option[i]) {
				return i;
			}
		}
		return -1;
	}

	class DoThread3 // need to rename!
	{
	private:
		char* start;
		char* last;
	public:
		VECTOR<Token2>* aq;
		const wiz::LoadDataOption* option;
		//int strVecStart;
		//int strVecEnd;
	public:
		DoThread3(char* start, char* last, VECTOR<Token2>* aq, const wiz::LoadDataOption* option) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
			: start(start), last(last), aq(aq), option(option) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
		{
			//
		}

	public:
		void operator() () {
			{
				char* token_first = start;
				char* token_last = start; // idx of token in statement.
				int state = 0;
				std::string token;

				for (char* x = start; x < last; ++x) {
					int idx;

					/*if (0 == state && '\'' == *x) {
						//token_last = x - 1;
						//if (token_last >= 0 && token_last - token_first + 1 > 0) {
						//	aq->push_back(statement.substr(token_first, token_last - token_first + 1));
						//}
						state = 2;
						//token_first = i; 
						token_last = x;

						//token.push_back(*x);
					}
					else if (2 == state && '\\' == *(x - 1) && '\'' == *x) {
						token_last = x;
						//token.push_back(*x);
					}
					else if (2 == state && '\'' == *x) {
						state = 0; token_last = x;
						//token.push_back(*x);
					}
					else 
					*/
					if (0 == state && '\"' == *x) {
						//token_last = x - 1;
						//if (token_last >= 0 && token_last - token_first + 1 > 0) {
						//	aq->push_back(statement.substr(token_first, token_last - token_first + 1));
						//}
						state = 1;
						//token_first = i; 
						token_last = x;
						//token.push_back(*x);
					}
					else if (1 == state && '\\' == *(x - 1) && '\"' == *x) {
						token_last = x;
						//token.push_back(*x);
					}
					else if (1 == state && '\"' == *x) {
						state = 0; token_last = x;
						//token.push_back(*x);
					}
					else if (0 == state && -1 != (idx = Equal(option->Removal, *x)))
					{
						token_last = x - 1;

						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							aq->push_back(Token2(token_first, token_last - token_first + 1, false));
							token.clear();

							token_first = x + 1;
						}
						else {
							token.clear();

							token_first = token_first + 1;
						}
						continue;
					}
					else if (0 == state && -1 != (idx = Equal(option->Assignment, *x))) {
						token_last = x - 1;

						if (token_last >= 0 && token_last - token_first + 1 > 0) {

							

							aq->push_back(Token2(token_first, token_last - token_first + 1, false));

							token.clear();

							token_first = x + 1;

							aq->push_back(Token2(x, 1, false));
						}
						else {
							aq->push_back(Token2(x, 1, false));
							token_first = token_first + 1;

							token.clear();
						}
					}
					else if (0 == state && isWhitespace(*x)) { // isspace ' ' \t \r \n , etc... ?
						token_last = x - 1;
						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							
							aq->push_back(Token2(token_first, token_last - token_first + 1, false));

							token_first = x + 1;
							token.clear();
						}
						else
						{
							token_first = token_first + 1;
							token.clear();
						}
					}
					else if (0 == state && -1 != (idx = Equal(option->Left, *x))) {
						token_last = x - 1;
						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							
							aq->push_back(Token2(token_first, token_last - token_first + 1, false));
							token_first = x + 1;

							aq->push_back(Token2(x, 1, false));
							token.clear();
						}
						else {
							aq->push_back(Token2(x, 1, false));
							token_first = token_first + 1;
							token.clear();
						}
					}
					else if (0 == state && -1 != (idx = Equal(option->Right, *x))) {
						token_last = x - 1;
						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							
							aq->push_back(Token2(token_first, token_last - token_first + 1, false));
							token_first = x + 1;

							aq->push_back(Token2(x, 1, false));

							token.clear();
						}
						else {
							aq->push_back(Token2(x, 1, false));

							token_first = token_first + 1;

							token.clear();
						}
					}
					else {
						//token.push_back(*x);
					}

					token_last = x;
				}

				if (token_first < last)
				{
					aq->push_back(Token2(token_first, token_last - token_first + 1, false));
				}

				//std::cout << tokenNum << " " << aq->size() << std::endl;
				//aq->push_back("", false);
			}
		}
		//
		//DoThread(DoThread& other, tbb::split) : strVec(other.strVec) // , aq(other.aq)
		//{
		//
		//}
		//void join(DoThread& other) 
		//{
		//	aqpush(std::move(other.aq));
		//}
	};

	std::pair<bool, int> Reserve2_3(std::ifstream& inFile, VECTOR<Token2>* aq, const int num, bool* isFirst, 
											const wiz::LoadDataOption& option, int thr_num, char*& _buffer);

	class Type {
	private:
		std::string name;
	public:
		explicit Type(const std::string& name = "", const bool valid = true) : name(name) { }//chk();  }
		explicit Type(std::string&& name, const bool valid = true) : name(move(name)) { }//chk(); }
		Type(const Type& type)
			: name(type.name)
		{
			//chk();
		}
		virtual ~Type() { }
		bool IsFail() const { // change body
			return "" == name;
		}
		const std::string& GetName()const {
			return name;
		}
		std::string& GetName() {
			return name;
		}
		void SetName(const std::string& name)
		{
			this->name = name;

			//chk();
		}
		void SetName(std::string&& name)
		{
			this->name = name;

			//chk();
		}
		bool operator==(const Type& t) const {
			return name == t.name;
		}
		bool operator<(const Type& t) const {
			return name < t.name;
		}
		Type& operator=(const Type& type)
		{
			name = type.name;
			return *this;
		}
		void operator=(Type&& type)
		{
			name = std::move(type.name);
			type.name.clear();
		}

		virtual bool IsItemType()const = 0;
		virtual bool IsUserType()const = 0;
	};

	template < class T >
	class ItemType : public Type {
	public:
		typedef T item_type; //
	private:
		//std::vector<T> arr;
		T data;
		bool inited;
	public:
		ItemType(const ItemType<T>& ta) : Type(ta), data(ta.data), inited(ta.inited)
		{

		}
		ItemType(ItemType<T>&& ta) : Type(std::move(ta))
		{
			data = std::move(ta.data);
			inited = ta.inited;
		}
	public:
		explicit ItemType()
			: Type("", true), inited(false) { }
		explicit ItemType(const std::string& name, const std::string& value, const bool valid = true)
			:Type(name, valid), data(value), inited(true)
		{

		}
		explicit ItemType(std::string&& name, std::string&& value, const bool valid = true)
			:Type(move(name), valid), data(move(value)), inited(true)
		{

		}
		virtual ~ItemType() { }
	public:
		void Remove(const int idx = 0)
		{
			data = T();
			inited = false;
		}
		bool Push(const T& val) { /// do not change..!!
			if (inited) { throw "ItemType already inited"; }
			data = val;
			inited = true;

			return true;
		}
		bool Push(T&& val) {
			if (inited) { throw "ItemType already inited"; }
			data = std::move(val);
			inited = true;

			return true;
		}
		T& Get(const int index) {
			if (!inited) { throw "ItemType, not inited"; }
			return data;
		}
		const T& Get(const int index) const {
			if (!inited) { throw "ItemType, not inited"; }
			return data;
		}
		void Set(const int index, const T& val) {
			if (!inited) { throw "ItemType, not inited"; } // removal?
			data = val;
		}
		void Set(const int index, T&& val) {
			if (!inited) { throw "ItemType, not inited"; } // removal?
			data = std::move(val);
		}
		int size()const {
			return inited ? 1 : 0;
		}
		bool empty()const { return !inited; }
		std::string ToString()const
		{
			if (Type::GetName().empty()) {
				return Get(0);
			}
			return Type::GetName() + " = " + Get(0);
		}
	public:
		ItemType<T>& operator=(const ItemType<T>& ta)
		{
			Type::operator=(ta);
			ItemType<T> temp = ta;

			data = std::move(temp.data);
			inited = temp.inited;
			return *this;
		}
		ItemType<T>& operator=(ItemType<T>&& ta)
		{
			Type::operator=(ta);
			if (data == ta.data) { return *this; }

			data = std::move(ta.data);
			inited = ta.inited;
			return *this;
		}
	public:
		virtual bool IsItemType()const {
			return true;
		}
		virtual bool IsUserType()const {
			return false;
		}
	};

	class UserType : public Type {
	public:
		virtual bool IsItemType()const {
			return false;
		}
		virtual bool IsUserType()const {
			return true;
		}
	public:
		void PushComment(const std::string& comment)
		{
			commentList.push_back(comment);
		}
		void PushComment(std::string&& comment)
		{
			commentList.push_back(move(comment));
		}
		int GetCommentListSize()const { return commentList.size(); }
		const std::string& GetCommentList(const int idx) const { return commentList[idx]; }
		std::string& GetCommentList(const int idx) {
			return commentList[idx];
		}
	public:
		int GetIListSize()const { return ilist.size(); }
		int GetItemListSize()const { return itemList.size(); }
		int GetUserTypeListSize()const { return userTypeList.size(); }
		ItemType<std::string>& GetItemList(const int idx) { return itemList[idx]; }
		const ItemType<std::string>& GetItemList(const int idx) const { return itemList[idx]; }
		UserType*& GetUserTypeList(const int idx) { return userTypeList[idx]; }
		const UserType*& GetUserTypeList(const int idx) const { return const_cast<const UserType*&>(userTypeList[idx]); }

		bool IsItemList(const int idx) const
		{
			return ilist[idx] == 1;
		}
		bool IsUserTypeList(const int idx) const
		{
			return ilist[idx] == 2;
		}

		void AddItemList(const ItemType<std::string>& strTa)
		{
			for (int i = 0; i < strTa.size(); ++i) {
				this->AddItem(strTa.GetName(), strTa.Get(i));
			}
		}
		void AddItemList(ItemType<std::string>&& strTa)
		{
			for (int i = 0; i < strTa.size(); ++i) {
				this->AddItem(std::move(strTa.GetName()), std::move(strTa.Get(i)));
			}
		}
	public:
		void SetParent(UserType* other)
		{
			parent = other;
		}
		UserType* GetParent() { return parent; }
		const UserType* GetParent()const { return parent; }

		void LinkUserType(UserType* ut)
		{
			userTypeList.push_back(ut);
			ilist.push_back(2);
			ut->parent = this;
		}
	private:
		UserType * parent = nullptr;
		std::vector<std::string> commentList;
		std::vector<int> ilist;
		std::vector< ItemType<std::string> > itemList;
		std::vector< UserType* > userTypeList;
		bool noRemove = false;
		bool reservedA = false;
		//	bool userTypeList_sortFlagA; // A : sorted < , B ; sorted > , false : not sorted!
		//	bool userTypeList_sortFlagB;
	public:
		explicit UserType(std::string&& name, bool noRemove = false) : Type(move(name)), parent(nullptr), noRemove(noRemove) { }
		explicit UserType(const std::string& name = "", bool noRemove = false) : Type(name), parent(nullptr), noRemove(noRemove) { } //, userTypeList_sortFlagA(true), userTypeList_sortFlagB(true) { }
		UserType(const UserType& ut) : Type(ut.GetName()) {
			Reset(ut);  // Initial
		}
		UserType(UserType&& ut) : Type(move(ut.GetName())) {
			Reset2(std::move(ut));
		}
		virtual ~UserType() {
			if (false == noRemove) {
				_Remove();
			}
		}
		UserType& operator=(const UserType& ut) {
			if (this == &ut) { return *this; }
			Type::operator=(ut);

			RemoveUserTypeList();
			Reset(ut);
			return *this;
		}
		UserType& operator=(UserType&& ut) {
			if (this == &ut) { return *this; }

			Type::operator=(std::move(ut));
			RemoveUserTypeList();
			Reset2(std::move(ut));
			return *this;
		}
	private:
		void Reset(const UserType& ut) { /// UT 占쏙옙체占쏙옙 占쏙옙占쏙옙占싼댐옙.
										 //	userTypeList_sortFlagA = ut.userTypeList_sortFlagA;
										 //userTypeList_sortFlagB = ut.userTypeList_sortFlagB;

			ilist = ut.ilist;
			itemList = ut.itemList;
			//parent = ut.parent;
			commentList = ut.commentList;

			userTypeList.reserve(ut.userTypeList.size());

			for (int i = 0; i < ut.userTypeList.size(); i++) {
				userTypeList.push_back(new UserType(*ut.userTypeList[i]));
				userTypeList.back()->parent = this;
			}
		}
		void Reset2(UserType&& ut) {
			//std::swap( userTypeList_sortFlagA, ut.userTypeList_sortFlagA );
			//std::swap( userTypeList_sortFlagB, ut.userTypeList_sortFlagB );

			//no use - //parent = ut.parent;
			//no use - //ut.parent = nullptr; /// chk..
			ilist = std::move(ut.ilist);
			itemList = std::move(ut.itemList);
			commentList = std::move(ut.commentList);

			userTypeList.reserve(ut.userTypeList.size());

			for (int i = 0; i < ut.userTypeList.size(); ++i) {
				userTypeList.push_back(std::move(ut.userTypeList[i]));
				ut.userTypeList[i] = nullptr;
				userTypeList.back()->parent = this;
			}
			ut.userTypeList.clear();
		}

		void _Remove()
		{
			//parent = nullptr;
			ilist = std::vector<int>();
			itemList = std::vector< ItemType<std::string> >();
			RemoveUserTypeList();

			commentList.clear();
		}
		// static ??
	public:
		int GetIlistIndex(const int index, const int type)
		{
			return _GetIlistIndex(ilist, index, type);
		}
		int GetUserTypeIndexFromIlistIndex(const int ilist_idx)
		{
			return _GetUserTypeIndexFromIlistIndex(ilist, ilist_idx);
		}
		int GetItemIndexFromIlistIndex(const int ilist_idx)
		{
			return _GetItemIndexFromIlistIndex(ilist, ilist_idx);
		}
	private:
		/// val : 1 or 2
		int _GetIndex(const std::vector<int>& ilist, const int val, const int start = 0) {
			for (int i = start; i < ilist.size(); ++i) {
				if (ilist[i] == val) { return i; }
			}
			return -1;
		}
		// test? - need more thinking!
		int _GetItemIndexFromIlistIndex(const std::vector<int>& ilist, const int ilist_idx) {
			if (ilist.size() == ilist_idx) { return ilist.size(); }
			int idx = _GetIndex(ilist, 1, 0);
			int item_idx = -1;

			while (idx != -1) {
				item_idx++;
				if (ilist_idx == idx) { return item_idx; }
				idx = _GetIndex(ilist, 1, idx + 1);
			}

			return -1;
		}
		int _GetUserTypeIndexFromIlistIndex(const std::vector<int>& ilist, const int ilist_idx) {
			if (ilist.size() == ilist_idx) { return ilist.size(); }
			int idx = _GetIndex(ilist, 2, 0);
			int usertype_idx = -1;

			while (idx != -1) {
				usertype_idx++;
				if (ilist_idx == idx) { return usertype_idx; }
				idx = _GetIndex(ilist, 2, idx + 1);
			}

			return -1;
		}
		/// type : 1 or 2
		int _GetIlistIndex(const std::vector<int>& ilist, const int index, const int type) {
			int count = -1;

			for (int i = 0; i < ilist.size(); ++i) {
				if (ilist[i] == type) {
					count++;
					if (index == count) {
						return i;
					}
				}
			}
			return -1;
		}
	public:
		void RemoveItemList(const int idx)
		{
			// left shift start idx, to end, at itemList. and resize!
			for (int i = idx + 1; i < GetItemListSize(); ++i) {
				itemList[i - 1] = std::move(itemList[i]);
			}
			itemList.resize(itemList.size() - 1);
			//  ilist left shift and resize - count itemType!
			int count = 0;
			for (int i = 0; i < ilist.size(); ++i) {
				if (ilist[i] == 1) { count++; }
				if (count == idx + 1) {
					// i占쏙옙占쏙옙 left shift!and resize!
					for (int k = i + 1; k < ilist.size(); ++k) {
						ilist[k - 1] = std::move(ilist[k]);
					}
					ilist.resize(ilist.size() - 1);
					break;
				}
			}
		}
		void RemoveUserTypeList(const int idx, const bool chk = true)
		{
			if (chk && userTypeList[idx]) {
				delete userTypeList[idx];
			}
			// left shift start idx, to end, at itemList. and resize!
			for (int i = idx + 1; i < GetUserTypeListSize(); ++i) {
				userTypeList[i - 1] = std::move(userTypeList[i]);
			}
			userTypeList.resize(userTypeList.size() - 1);
			//  ilist left shift and resize - count itemType!
			int count = 0;
			for (int i = 0; i < ilist.size(); ++i) {
				if (ilist[i] == 2) { count++; }
				if (count == idx + 1) {
					// i占쏙옙占쏙옙 left shift!and resize!
					for (int k = i + 1; k < ilist.size(); ++k) {
						ilist[k - 1] = std::move(ilist[k]);
					}
					ilist.resize(ilist.size() - 1);
					break;
				}
			}
		}
		void RemoveItemList(const std::string& varName)
		{
			int k = _GetIndex(ilist, 1, 0);
			std::vector<ItemType<std::string>> tempDic;
			for (int i = 0; i < itemList.size(); ++i) {
				if (varName != itemList[i].GetName()) {
					tempDic.push_back(itemList[i]);
					k = _GetIndex(ilist, 1, k + 1);
				}
				else {
					// remove item, ilist left shift 1.
					for (int j = k + 1; j < ilist.size(); ++j) {
						ilist[j - 1] = ilist[j];
					}
					ilist.resize(ilist.size() - 1);
					k = _GetIndex(ilist, 1, k);
				}
			}
			itemList = (tempDic);
		}
		void RemoveItemList() /// ALL
		{
			itemList = std::vector<ItemType<std::string>>();
			//
			std::vector<int> temp;
			for (int i = 0; i < ilist.size(); ++i) {
				if (ilist[i] == 2)
				{
					temp.push_back(2);
				}
			}
			ilist = move(temp);
		}
		void RemoveEmptyItem() // fixed..
		{
			int k = _GetIndex(ilist, 1, 0);
			std::vector<ItemType<std::string>> tempDic;
			for (int i = 0; i < itemList.size(); ++i) {
				if (itemList[i].size() > 0) {
					tempDic.push_back(itemList[i]);
					k = _GetIndex(ilist, 1, k + 1);
				}
				else {
					// remove item, ilist left shift 1.
					for (int j = k + 1; j < ilist.size(); ++j) {
						ilist[j - 1] = ilist[j];
					}
					ilist.resize(ilist.size() - 1);
					k = _GetIndex(ilist, 1, k);
				}
			}
			itemList = move(tempDic);
		}
		void Remove()
		{
			/// parent->removeUserType(name); - ToDo - X
			ilist = std::vector<int>();
			itemList = std::vector< ItemType<std::string> >();

			RemoveUserTypeList();

			commentList.clear();

			//parent = nullptr;
		}
		void RemoveUserTypeList() { /// chk memory leak test!!
			for (int i = 0; i < userTypeList.size(); i++) {
				if (nullptr != userTypeList[i]) {
					delete userTypeList[i]; //
					userTypeList[i] = nullptr;
				}
			}
			// DO Empty..
			userTypeList.clear();

			std::vector<int> temp;
			for (int i = 0; i < ilist.size(); ++i) {
				if (ilist[i] == 1)
				{
					temp.push_back(1);
				}
			}
			ilist = move(temp);
		}
		void RemoveUserTypeList(const std::string& varName, const bool chk = true)
		{
			int k = _GetIndex(ilist, 2, 0);
			std::vector<UserType*> tempDic;
			for (int i = 0; i < userTypeList.size(); ++i) {
				if (varName != userTypeList[i]->GetName()) {
					tempDic.push_back(userTypeList[i]);
					k = _GetIndex(ilist, 2, k + 1);
				}
				else {
					if (chk && userTypeList[i]) {
						delete userTypeList[i];
					}
					// remove usertypeitem, ilist left shift 1.
					for (int j = k + 1; j < ilist.size(); ++j) {
						ilist[j - 1] = ilist[j];
					}
					ilist.resize(ilist.size() - 1);
					k = _GetIndex(ilist, 2, k);
				}
			}
			userTypeList = move(tempDic);
		}
		//			
		void RemoveList(const int idx) // ilist_idx!
		{
			// chk whether item or usertype.
			// find item_idx or usertype_idx.
			// remove item or remove usertype.
			if (ilist[idx] == 1) {
				int item_idx = -1;

				for (int i = 0; i < ilist.size() && i <= idx; ++i) {
					if (ilist[i] == 1) { item_idx++; }
				}

				RemoveItemList(item_idx);
			}
			else {
				int usertype_idx = -1;

				for (int i = 0; i < ilist.size() && i <= idx; ++i) {
					if (ilist[i] == 2) { usertype_idx++; }
				}

				RemoveUserTypeList(usertype_idx);
			}
		}
	public:
		bool empty()const { return ilist.empty(); }

		// chk
		void InsertItemByIlist(const int ilist_idx, const std::string& name, const std::string& item) {
			ilist.push_back(1);
			for (int i = ilist.size() - 1; i > ilist_idx; --i) {
				ilist[i] = ilist[i - 1];
			}
			ilist[ilist_idx] = 1;


			int itemIndex = _GetItemIndexFromIlistIndex(ilist, ilist_idx);

			itemList.emplace_back("", "");

			if (itemIndex != -1) {
				for (int i = itemList.size() - 1; i > itemIndex; --i) {
					itemList[i] = move(itemList[i - 1]);
				}
				itemList[itemIndex] = ItemType<std::string>(name, item);
			}
			else {
				itemList[0] = ItemType<std::string>(name, item); // chk!!
			}

		}
		void InsertItemByIlist(const int ilist_idx, std::string&& name, std::string&& item) {
			ilist.push_back(1);


			for (int i = ilist.size() - 1; i > ilist_idx; --i) {
				ilist[i] = ilist[i - 1];
			}
			ilist[ilist_idx] = 1;

			int itemIndex = _GetItemIndexFromIlistIndex(ilist, ilist_idx);

			itemList.emplace_back("", "");
			if (itemIndex != -1) {
				for (int i = itemList.size() - 1; i > itemIndex; --i) {
					itemList[i] = move(itemList[i - 1]);
				}
				itemList[itemIndex] = ItemType<std::string>(move(name), move(item));
			}
			else {
				itemList[0] = ItemType<std::string>(move(name), move(item));
			}


		}
		// chk
		void InsertUserTypeByIlist(const int ilist_idx, UserType&& item) {
			ilist.push_back(2);
			UserType* temp = new UserType(std::move(item));

			temp->parent = this;

			for (int i = ilist.size() - 1; i > ilist_idx; --i) {
				ilist[i] = ilist[i - 1];
			}
			ilist[ilist_idx] = 2;

			int userTypeIndex = _GetUserTypeIndexFromIlistIndex(ilist, ilist_idx);
			userTypeList.push_back(nullptr);
			if (userTypeIndex != -1) {
				for (int i = userTypeList.size() - 1; i > userTypeIndex; --i) {
					userTypeList[i] = std::move(userTypeList[i - 1]);
				}
				userTypeList[userTypeIndex] = temp;
			}
			else {
				userTypeList[0] = temp;
			}

		}
		void InsertUserTypeByIlist(const int ilist_idx, const UserType& item) {
			ilist.push_back(2);
			UserType* temp = new UserType(item);

			temp->parent = this;


			for (int i = ilist.size() - 1; i > ilist_idx; --i) {
				ilist[i] = ilist[i - 1];
			}
			ilist[ilist_idx] = 2;

			int userTypeIndex = _GetUserTypeIndexFromIlistIndex(ilist, ilist_idx);
			userTypeList.push_back(nullptr);
			if (userTypeIndex != -1) {
				for (int i = userTypeList.size() - 1; i > userTypeIndex; --i) {
					userTypeList[i] = std::move(userTypeList[i - 1]);
				}
				userTypeList[userTypeIndex] = temp;
			}
			else {
				userTypeList[0] = temp;
			}
		}

		/*
		// chk
		void InsertItem(const int item_idx, const std::string& name, const std::string& item) {
		int ilist_idx = _GetIlistIndex(ilist, item_idx, 1);

		ilist.push_back(0);
		for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
		ilist[i] = ilist[i - 1];
		}
		ilist[ilist_idx] = 1;

		itemList.emplace_back("", "");
		for (int i = item_idx + 1; i < itemList.size(); ++i) {
		itemList[i] = move(itemList[i - 1]);
		}
		itemList[item_idx] = ItemType<std::string>(name, item);
		}
		void InsertItem(const int item_idx, std::string&& name, std::string&& item) {
		int ilist_idx = _GetIlistIndex(ilist, item_idx, 1);

		ilist.push_back(0);
		for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
		ilist[i] = ilist[i - 1];
		}
		ilist[ilist_idx] = 1;

		itemList.emplace_back("", "");
		for (int i = item_idx + 1; i < itemList.size(); ++i) {
		itemList[i] = move(itemList[i - 1]);
		}
		itemList[item_idx] = ItemType<std::string>(move(name), move(item));
		}
		// chk
		void InsertUserType(const int ut_idx, UserType&& item) {
		int ilist_idx = _GetIlistIndex(ilist, ut_idx, 2);
		UserType* temp = new UserType(std::move(item));

		temp->parent = this;

		ilist.push_back(0);
		for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
		ilist[i] = ilist[i - 1];
		}
		ilist[ilist_idx] = 2;

		userTypeList.push_back(nullptr);
		for (int i = ut_idx + 1; i < userTypeList.size(); ++i) {
		userTypeList[i] = userTypeList[i - 1];
		}
		userTypeList[ut_idx] = temp;
		}
		void InsertUserType(const int ut_idx, const UserType& item) {
		int ilist_idx = _GetIlistIndex(ilist, ut_idx, 2);
		UserType* temp = new UserType(item);

		temp->parent = this;

		ilist.push_back(0);
		for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
		ilist[i] = ilist[i - 1];
		}
		ilist[ilist_idx] = 2;

		userTypeList.push_back(nullptr);
		for (int i = ut_idx + 1; i < userTypeList.size(); ++i) {
		userTypeList[i] = userTypeList[i - 1];
		}
		userTypeList[ut_idx] = temp;
		}
		*/
		//
		void ReserveIList(int offset)
		{
			if (offset > 0) {
				ilist.reserve(offset);
			}
		}
		void ReserveItemList(int offset)
		{
			if (offset > 0) {
				itemList.reserve(offset);
			}
		}
		void ReserveUserTypeList(int offset)
		{
			if (offset > 0) {
				userTypeList.reserve(offset);
			}
		}
		void AddItem(std::string&& name, std::string&& item) {
			itemList.push_back(ItemType<std::string>(std::move(name), std::move(item)));
			ilist.push_back(1);
		}
		void AddItem(const std::string& name, const std::string& item) {
			itemList.push_back(ItemType<std::string>(name, item));
			ilist.push_back(1);
		}
		void AddUserTypeItem(UserType&& item) {
			UserType* temp = new UserType(std::move(item));
			temp->parent = this;
			//temp->SetName("");

			ilist.push_back(2);

			userTypeList.push_back(temp);
		}
		void AddUserTypeItem(const UserType& item) {
			UserType* temp = new UserType(item);
			temp->parent = this;

			ilist.push_back(2);

			userTypeList.push_back(temp);
		}
		void AddItemAtFront(std::string&& name, std::string&& item) {
			itemList.emplace(itemList.begin(), name, item);

			ilist.insert(ilist.begin(), 1);
		}
		void AddItemAtFront(const std::string& name, const std::string& item) {
			itemList.emplace(itemList.begin(), name, item);

			ilist.insert(ilist.begin(), 1);
		}
		void AddUserTypeItemAtFront(const UserType& item) {
			UserType* temp = new UserType(item);
			temp->parent = this;

			ilist.insert(ilist.begin(), 2);

			userTypeList.insert(userTypeList.begin(), temp);
		}
		void AddUserTypeItemAtFront(UserType&& item) {
			UserType* temp = new UserType(item);
			temp->parent = this;

			ilist.insert(ilist.begin(), 2);

			userTypeList.insert(userTypeList.begin(), temp);
		}

		/// add set Data
		bool SetItem(const int var_idx, const std::string& value) {
			itemList[var_idx].Set(0, value);
			return true;
		}

		std::vector<UserType*> GetUserTypeItem(const std::string& name) const { /// chk...
			std::vector<UserType*> temp;

			for (int i = 0; i < userTypeList.size(); ++i) {
				if (userTypeList[i]->GetName() == name) {
					temp.push_back(userTypeList[i]);
				}
			}

			return temp;
		}

		// deep copy.
		std::vector<UserType*> GetCopyUserTypeItem(const std::string& name) const { /// chk...
			std::vector<UserType*> temp;

			for (int i = 0; i < userTypeList.size(); ++i) {
				if (userTypeList[i]->GetName() == name) {
					temp.push_back(new UserType(*userTypeList[i]));
				}
			}

			return temp;
		}
	public:
		bool GetUserTypeItemRef(const int idx, UserType*& ref)
		{
			ref = userTypeList[idx];
			return true;
		}
		bool GetLastUserTypeItemRef(const std::string& name, UserType*& ref) {
			int idx = -1;

			for (int i = userTypeList.size() - 1; i >= 0; --i)
			{
				if (name == userTypeList[i]->GetName()) {
					idx = i;
					break;
				}
			}
			if (idx > -1) {
				ref = userTypeList[idx];
			}
			return idx > -1;
		}
	private:
		/// save1 - like EU4 savefiles.
		void Save1(std::ostream& stream, const UserType* ut, const int depth = 0) const {
			int itemListCount = 0;
			int userTypeListCount = 0;

			const bool existUserType = ut->GetUserTypeListSize() > 0;

			for (int i = 0; i < ut->commentList.size(); ++i) {
				for (int k = 0; k < depth; ++k) {
					stream << "\t";
				}
				stream << (ut->commentList[i]);

				if (i < ut->commentList.size() - 1 || false == ut->ilist.empty()) {
					stream << "\n";
				}
			}

			for (int i = 0; i < ut->ilist.size(); ++i) {
				//std::cout << "ItemList" << endl;
				if (ut->ilist[i] == 1) {
					for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
						std::string temp;
						if (existUserType) {
							for (int k = 0; k < depth; ++k) {
								temp += "\t";
							}
						}
						if (ut->itemList[itemListCount].GetName() != "") {
							temp += ut->itemList[itemListCount].GetName();
							temp += "=";
						}
						temp += ut->itemList[itemListCount].Get(j);
						if (j != ut->itemList[itemListCount].size() - 1) {
							temp += " ";
						}
						stream << temp;
					}
					if (i != ut->ilist.size() - 1) {
						if (existUserType) {
							stream << "\n";
						}
						else {
							stream << " "; // \n
						}
					}
					itemListCount++;
				}
				else if (ut->ilist[i] == 2) {
					// std::cout << "UserTypeList" << endl;
					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}

					if (ut->userTypeList[userTypeListCount]->GetName() != "") {
						stream << ut->userTypeList[userTypeListCount]->GetName() << "=";
					}

					stream << "{\n";

					Save1(stream, ut->userTypeList[userTypeListCount], depth + 1);
					stream << "\n";

					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}
					stream << "}";
					if (i != ut->ilist.size() - 1) {
						stream << "\n";
					}

					userTypeListCount++;
				}
			}
		}
		/// save2 - for more speed loading data!
		void Save2(std::ostream& stream, const UserType* ut, const int depth = 0) const {
			int itemListCount = 0;
			int userTypeListCount = 0;

			for (int i = 0; i < ut->commentList.size(); ++i) {
				for (int k = 0; k < depth; ++k) {
					stream << "\t";
				}
				stream << (ut->commentList[i]);

				if (i < ut->commentList.size() - 1 || false == ut->ilist.empty()) {
					stream << "\n";
				}

			}
			for (int i = 0; i < ut->ilist.size(); ++i) {
				//std::cout << "ItemList" << endl;
				if (ut->ilist[i] == 1) {
					for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
						//for (int k = 0; k < depth; ++k) {
						//	stream << "\t";
						//}
						if (ut->itemList[itemListCount].GetName() != "")
							stream << ut->itemList[itemListCount].GetName() << " = ";
						stream << ut->itemList[itemListCount].Get(j);
						if (j != ut->itemList[itemListCount].size() - 1)
							stream << " ";
					}
					if (i != ut->ilist.size() - 1) {
						stream << " ";//"\n";
					}
					itemListCount++;
				}
				else if (ut->ilist[i] == 2) {
					// std::cout << "UserTypeList" << endl;
					if (ut->userTypeList[userTypeListCount]->GetName() != "")
					{
						stream << ut->userTypeList[userTypeListCount]->GetName() << " = ";
					}
					stream << "{\n";

					Save2(stream, ut->userTypeList[userTypeListCount], depth + 1);
					stream << "\n";

					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}
					stream << "}";
					if (i != ut->ilist.size() - 1) {
						stream << "\n";
					}
					userTypeListCount++;
				}
			}
		}

		bool IsArrayWithJson(const UserType* ut) const
		{
			if (ut->GetIListSize() == 0) {
				return false;
			}

			bool isArray = true;

			for (int i = 0; i < ut->GetItemListSize(); ++i) {
				if (ut->GetItemList(i).GetName().empty()) {
					isArray = true;
				}
				else {
					return false;
				}
			}

			for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
				if (ut->GetUserTypeList(i)->GetName().empty()) {
					isArray = true;
				}
				else {
					return false;
				}
			}

			return isArray; // && 0 == ut->GetUserTypeListSize();
		}

		// todo - fix bug?
		void SaveWithJson(std::ostream& stream, const UserType* ut, const int depth = 0) const {
			int itemListCount = 0;
			int userTypeListCount = 0;

			/*
			for (int i = 0; i < ut->commentList.size(); ++i) {
			for (int k = 0; k < depth; ++k) {
			stream << "\t";
			}

			stream << "// ";
			stream << (ut->commentList[i]);

			if (i < ut->commentList.size() - 1 || false == ut->ilist.empty()) {
			stream << "\n";
			}

			}
			*/

			for (int i = 0; i < ut->ilist.size(); ++i) {
				//std::cout << "ItemList" << endl;
				if (ut->ilist[i] == 1) {
					//if (userTypeListCount > 0 && 0 == ut->userTypeList[userTypeListCount - 1]->GetIListSize() && ut->userTypeList[userTypeListCount - 1]->GetName().empty())
					//{
					//
					//}
					//else if (i != 0) {
					//	stream << " ,\n";
					//}

					for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
						for (int k = 0; k < depth; ++k) {
							stream << "\t";
						}
						if (ut->itemList[itemListCount].GetName() != "") {
							////if (ut->itemList[itemListCount].GetName()[0] == '\"') {
							stream << ut->itemList[itemListCount].GetName() << " : ";
							////}
							////else {
							////	stream << "\"" + ut->itemList[itemListCount].GetName() + "\"" << " : ";
							////}
						}
						else {
							//stream << "\"_\" : ";
						}
						////if (ut->itemList[itemListCount].Get(j)[0] == '\"') {
						////	if (ut->itemList[itemListCount].Get(j) == "\"\"") {
						////		stream << "\" \"";
						////	}
						////	else {
						stream << ut->itemList[itemListCount].Get(j);
						////	}
					}
					////	else {
					////		stream << "\"" + ut->itemList[itemListCount].Get(j) + "\"";
					////	}
					////}
					if (i != ut->ilist.size() - 1) {
						stream << " ,\n";
					}
					itemListCount++;
				}
				else if (ut->ilist[i] == 2) {
					//if (0 == ut->userTypeList[userTypeListCount]->GetIListSize() && ut->userTypeList[userTypeListCount]->GetName().empty())
					//{
					//	userTypeListCount++;
					//
					//	continue;
					//}
					//else 
					//if (i != 0) {
					//	stream << " ,\n";
					//}

					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}
					// std::cout << "UserTypeList" << endl;

					if (ut->userTypeList[userTypeListCount]->GetName() != "")
					{
						////	if (ut->userTypeList[userTypeListCount]->GetName()[0] == '\"') {
						stream << ut->userTypeList[userTypeListCount]->GetName() << " : ";
						////	}
						////	else {
						////		stream << "\"" + ut->userTypeList[userTypeListCount]->GetName() + "\"" << " : ";
						////	}
						////	}
						////else {
						//	stream << "\"_\"" << " : ";
					}

					if (IsArrayWithJson(ut->userTypeList[userTypeListCount])) {
						stream << "[\n";
					}
					else {
						stream << "{\n";
					}

					SaveWithJson(stream, ut->userTypeList[userTypeListCount], depth + 1);
					stream << "\n";

					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}

					if (IsArrayWithJson(ut->userTypeList[userTypeListCount])) {
						stream << "]";
					}
					else {
						stream << "}";
					}

					if (i != ut->ilist.size() - 1) {
						stream << " ,\n";
					}
					userTypeListCount++;
				}
			}
		}
		void SaveWithJson2(std::ostream& stream, const UserType* ut, const int depth = 0) const {
			int itemListCount = 0;
			int userTypeListCount = 0;

			/*
			for (int i = 0; i < ut->commentList.size(); ++i) {
			for (int k = 0; k < depth; ++k) {
			stream << "\t";
			}

			stream << "// ";
			stream << (ut->commentList[i]);

			if (i < ut->commentList.size() - 1 || false == ut->ilist.empty()) {
			stream << "\n";
			}

			}
			*/

			for (int i = 0; i < ut->ilist.size(); ++i) {
				//std::cout << "ItemList" << endl;
				if (ut->ilist[i] == 1) {
					//if (userTypeListCount > 0 && 0 == ut->userTypeList[userTypeListCount - 1]->GetIListSize() && ut->userTypeList[userTypeListCount - 1]->GetName().empty())
					//{
					//
					//}
					//else if (i != 0) {
					//	stream << " ,\n";
					//}

					for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
						for (int k = 0; k < depth; ++k) {
							stream << "\t";
						}
						if (ut->itemList[itemListCount].GetName() != "") {
							if (ut->itemList[itemListCount].GetName()[0] == '\"') {
								stream << ut->itemList[itemListCount].GetName() << " : ";
							}
							else {
								stream << "\"" + ut->itemList[itemListCount].GetName() + "\"" << " : ";
							}
						}
						else {
							//stream << "\"_\" : ";
						}
						if (ut->itemList[itemListCount].Get(j)[0] == '\"') {
							////	if (ut->itemList[itemListCount].Get(j) == "\"\"") {
							////		stream << "\" \"";
							////	}
							////	else {
							stream << ut->itemList[itemListCount].Get(j);
							////	}
						}
						else {
							stream << "\"" + ut->itemList[itemListCount].Get(j) + "\"";
						}
					}
					////}
					if (i != ut->ilist.size() - 1) {
						stream << " ,\n";
					}
					itemListCount++;
					//}
				}
				else if (ut->ilist[i] == 2) {
					//if (0 == ut->userTypeList[userTypeListCount]->GetIListSize() && ut->userTypeList[userTypeListCount]->GetName().empty())
					//{
					//	userTypeListCount++;
					//
					//	continue;
					//}
					//else 
					//if (i != 0) {
					//	stream << " ,\n";
					//}

					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}
					// std::cout << "UserTypeList" << endl;
					if (ut->userTypeList[userTypeListCount]->GetIListSize() > 0) {

						if (ut->userTypeList[userTypeListCount]->GetName() != "")
						{
							if (ut->userTypeList[userTypeListCount]->GetName()[0] == '\"') {
								stream << ut->userTypeList[userTypeListCount]->GetName() << " : ";
							}
							else {
								stream << "\"" + ut->userTypeList[userTypeListCount]->GetName() + "\"" << " : ";
							}
							////	}
							////else {
							//	stream << "\"_\"" << " : ";
						}


						if (IsArrayWithJson(ut->userTypeList[userTypeListCount])) {
							stream << "[\n";
						}
						else {
							stream << "{\n";
						}

						SaveWithJson2(stream, ut->userTypeList[userTypeListCount], depth + 1);
						stream << "\n";

						for (int k = 0; k < depth; ++k) {
							stream << "\t";
						}

						if (IsArrayWithJson(ut->userTypeList[userTypeListCount])) {
							stream << "]";
						}
						else {
							stream << "}";
						}
						if (i != ut->ilist.size() - 1) {
							stream << " ,\n";
						}
					}

					userTypeListCount++;
				}
			}
		}
	
	public:
		void Save1(std::ostream& stream, int depth = 0) const {
			Save1(stream, this, depth);
		}
		void Save2(std::ostream& stream, int depth = 0) const {
			Save2(stream, this, depth);
		}
		void SaveWithJson(std::ostream& stream, int depth = 0) const
		{
			//stream << "{\n";
			SaveWithJson(stream, this, depth + 1);
			//stream << "\n}";
		}
		void SaveWithJson2(std::ostream& stream, int depth = 0) const
		{
			stream << "{\n";
			SaveWithJson2(stream, this, depth + 1);
			stream << "\n}";
		}

		std::string ItemListToString()const
		{
			std::string temp;
			int itemListCount = 0;

			for (int i = 0; i < itemList.size(); ++i) {
				for (int j = 0; j < itemList[itemListCount].size(); j++) {
					if (itemList[itemListCount].GetName() != "")
						temp = temp + itemList[itemListCount].GetName() + " = ";
					temp = temp + itemList[itemListCount].Get(j);
					if (j != itemList[itemListCount].size() - 1) {
						temp = temp + "/";
					}
				}
				if (i != itemList.size() - 1)
				{
					temp = temp + "/";
				}
				itemListCount++;
			}
			return temp;
		}
		std::string ItemListNamesToString()const
		{
			std::string temp;
			int itemListCount = 0;

			for (int i = 0; i < itemList.size(); ++i) {
				for (int j = 0; j < itemList[itemListCount].size(); j++) {
					if (itemList[itemListCount].GetName() != "")
						temp = temp + itemList[itemListCount].GetName();
					else
						temp = temp + " ";

					if (j != itemList[itemListCount].size() - 1) {
						temp = temp + "/";
					}
				}
				if (i != itemList.size() - 1)
				{
					temp = temp + "/";
				}
				itemListCount++;
			}
			return temp;
		}
		std::vector<std::string> userTypeListNamesToStringArray()const
		{
			std::vector<std::string> temp;
			int userTypeListCount = 0;

			for (int i = 0; i < userTypeList.size(); ++i) {
				if (userTypeList[userTypeListCount]->GetName() != "") {
					temp.push_back(userTypeList[userTypeListCount]->GetName());
				}
				else {
					temp.push_back(" "); // chk!! cf) wiz::load_data::Utility::Find function...
				}
				userTypeListCount++;
			}
			return temp;
		}
		std::string UserTypeListNamesToString()const
		{
			std::string temp;
			int userTypeListCount = 0;

			for (int i = 0; i < userTypeList.size(); ++i) {
				if (userTypeList[userTypeListCount]->GetName() != "") {
					temp = temp + userTypeList[userTypeListCount]->GetName();
				}
				else {
					temp = temp + " "; // chk!! cf) wiz::load_data::Utility::Find function...
				}

				if (i != itemList.size() - 1)
				{
					temp = temp + "/";
				}
				userTypeListCount++;
			}
			return temp;
		}
		std::string ToString()const
		{
			std::string temp;
			int itemListCount = 0;
			int userTypeListCount = 0;

			for (int i = 0; i < ilist.size(); ++i) {
				//std::cout << "ItemList" << endl;
				if (ilist[i] == 1) {
					for (int j = 0; j < itemList[itemListCount].size(); j++) {
						if (itemList[itemListCount].GetName() != "") {
							temp.append(itemList[itemListCount].GetName());
							temp.append(" = ");
						}
						temp.append(itemList[itemListCount].Get(j));
						if (j != itemList[itemListCount].size() - 1)
						{
							temp.append(" ");
						}
					}
					if (i != ilist.size() - 1) {
						temp.append(" ");
					}
					itemListCount++;
				}
				else if (ilist[i] == 2) {
					// std::cout << "UserTypeList" << endl;
					if (userTypeList[userTypeListCount]->GetName() != "") {
						temp.append(userTypeList[userTypeListCount]->GetName());
						temp.append(" = ");
					}
					temp.append(" { ");
					temp.append(userTypeList[userTypeListCount]->ToString());
					temp.append(" ");
					temp.append(" }");
					if (i != ilist.size() - 1) {
						temp.append(" ");
					}

					userTypeListCount++;
				}
			}
			return temp;
		}
	};

	bool __LoadData3(VECTOR<Token2>* strVec, int start_idx, int last_idx, UserType* _global, const wiz::LoadDataOption* _option,
		int start_state, int last_state, UserType** next);

	int FindRight(VECTOR<Token2>& strVec, int start, int last, const wiz::LoadDataOption& option);

	void Merge(UserType* next, UserType* ut, UserType** ut_next);

	//// ToDo : # => option.~?
	template <class Reserver>
	bool _LoadData3(VECTOR<Token2>& strVec, Reserver& reserver, UserType& global, 
									const wiz::LoadDataOption& option, const int lex_thr_num = 4, const int parse_num = 4);
	class InFileReserver3
	{
	private:
		std::ifstream* pInFile;
		bool isFirst;
	public:
		int Num;
	public:
		explicit InFileReserver3(std::ifstream& inFile)
		{
			pInFile = &inFile;
			Num = 1;
			isFirst = true;
		}
		bool end()const { return pInFile->eof(); } //
	public:
		bool operator() (VECTOR<Token2>* strVec, const wiz::LoadDataOption& option, int thr_num, char*& buffer)
		{
			bool x = Reserve2_3(*pInFile, strVec, Num, &isFirst, option, thr_num, buffer).second > 0;
			return x;
		}
	};

	bool LoadDataFromFileFastJson(const std::string& fileName, UserType& global, int lex_thr_num, int parse_num);

	bool SaveWizDB(const UserType& global, const std::string& fileName, const std::string& option = "0", const std::string& option2 = "");

	bool LoadDataFromFile3(const std::string& fileName, UserType& global, int pivot_num, int lex_thr_num);
}

#endif
