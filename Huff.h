#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <memory>


template <typename I>
struct Element {
	bool isSetFlag = false;
	virtual ~Element() = default;
	virtual std::vector<I> GetItemList() = 0;
	virtual I GetItem(std::deque<bool>&) = 0;
	virtual int GetFreq() = 0;
	virtual bool isNode() = 0;
};

template <typename I>
class Item : public Element<I> {
public:
	Item() : i(I()), freq(0) {}
	Item(I in, int f) : i(in), freq(f) {}

	I GetItem(std::deque<bool> &nothing) override {
		return i;
	}
	std::vector<I> GetItemList() override {
		return std::vector<I> {i};
	}
	int GetFreq() override {
		return freq;
	}
	bool isNode() override {
		return false;
	}
private:
	int freq;
	I i;
};

template <typename I>
struct Node : public Element<I> {
	Node() : _left(nullptr), _right(nullptr) {}
	Node(std::unique_ptr<Element<I>> left, std::unique_ptr<Element<I>> right) {
		_left = std::move(left);
		_right = std::move(right);
	}
	std::vector<I> GetItemList() override {
		std::vector<I> out = _right->GetItemList();
		out.push_back(_left->GetItemList());
		return out;
	}
	I GetItem(std::deque<bool> &index) override {
		bool casheBool = index.front();
		index.pop_front();
		if (casheBool) return _right->GetChar(index);
		else		   return _left ->GetChar(index);
	}
	int GetFreq() override {
		int i = 0;
		if (_left != nullptr) i += _left->GetFreq();
		if (_right != nullptr) i += _right->GetFreq();
		return i;
	}
	bool isNode() override {
		return true;
	}
	void operator = (const Node& n) {
		_left = std::make_unique<Node<I>>(n._left);
		_right = std::make_unique<Node<I>>(n._right);
	}

	std::unique_ptr<Element<I>> _left, _right;
};

template <typename I, class Hasher = std::hash<I>>
class Tree {
	Tree(std::vector<I> orderedKey, std::unordered_map<I, int, Hasher> freq) {

		// Initializing Char deque using Ordered key & paired frequency | basically just converting data types
		std::deque<Item<I>> initCharList;
		for (I i : orderedKey) initCharList.push_back(Item<I>(i, freq[i]));


		// Initializing tree -- initial construction always includes a node with the two least frequent items
		Item<I> cashedChar = initCharList.front();
		initCharList.pop_front();
		std::vector<Node<I>> nodes = { Node<I>(std::make_unique<Item<I>>(cashedChar), std::make_unique<Item<I>>(initCharList.front())) };
		itemBoolIndex_[initCharList.front().GetItemList()[0]].push_back(true);
		itemBoolIndex_[cashedChar.GetItemList()[0]].push_back(false);
		initCharList.pop_front();

		// Initializing cashes
		Node<I> cashedNode;
		cashedChar.isSetFlag = false;
		cashedChar.isSetFlag = false;

		// Constructing tree
		while (!nodes.empty()) {

			// Setting cashes
			if (!cashedChar.isSetFlag) {
				cashedChar = initCharList.front();
				initCharList.pop_front();
				cashedChar.isSetFlag = true;
			}
			if (!cashedNode.isSetFlag) {
				cashedNode = nodes[0];
				int tempIndex = 0;
				for (int i = 1; i < nodes.size(); i++) {
					if (nodes[i].GetFreq() < cashedNode.GetFreq()) {
						cashedNode = nodes[i];
						tempIndex = i;
					}
				}
				nodes.erase(nodes.begin() + tempIndex);
				cashedNode.isSetFlag = true;
			}

			{	// Combiner logic -- artificial scope so tempNode and tempIndex always get re-initialized
				Node<I> tempNode = nodes[0];
				int tempIndex = 0;
				for (int i = 1; i < nodes.size(); i++) {
					if (nodes[i].GetFreq() < tempNode.GetFreq()) {
						tempNode = nodes[i];
						tempIndex = i;
					}
				}
				if (tempNode.GetFreq() < cashedChar.GetFreq() || initCharList.empty()) {
					nodes.erase(nodes.begin() + tempIndex);
					nodes.emplace_back(Node<I>(std::make_unique<Node<I>>(tempNode), std::make_unique<Node<I>>(cashedNode)));
					cashedNode.isSetFlag = false;
					for (I i : tempNode.GetItemList()) itemBoolIndex_[i].push_back(false);
					for (I i : cashedNode.GetItemList()) itemBoolIndex_[i].push_back(true);
				}
				else if (cashedNode.GetFreq() < initCharList.front().GetFreq()) {
					nodes.emplace_back(Node<I>(std::make_unique<Node<I>>(cashedNode), std::make_unique<Item<I>>(cashedChar)));
					for (I i : cashedNode.GetItemList()) itemBoolIndex_[i].push_back(false);
					itemBoolIndex_[cashedChar.GetItemList()[0]].push_back(true);
					cashedNode.isSetFlag = false;
					cashedChar.isSetFlag = false;
				}
				else {
					tempNode = Node<I>(std::make_unique<Item<I>>(cashedChar), std::make_unique<Item<I>>(initCharList.front()));
					itemBoolIndex_[cashedChar.GetItemList()[0]].push_back(false);
					itemBoolIndex_[initCharList.front().GetItemList()[0]].push_back(true);
					initCharList.pop_front();

					// Check is new node is less frequent than current least frequent
					if (tempNode.GetFreq() < cashedNode.GetFreq()) {
						cashedNode.isSetFlag = false;				// Maintining false isSetFlag initialization for all Nodes in nodes
						nodes.emplace_back(cashedNode);
						cashedNode = tempNode;
						cashedNode.isSetFlag = true;
					}
					else {
						nodes.emplace_back(tempNode);
					}
				}
			}
		}
		tree_ = cashedNode;
	}
	std::vector<std::deque<bool>> EncodeItemVector(std::vector<I> list) {
		std::vector<std::deque<bool>> out;
		for (I i : list) out.push_back(itemBoolIndex_[i]);
		return out;
	}

	std::vector<I> DecodeBoolVector(std::deque<bool> index) {
		std::vector<I> out;
		while (!index.empty()) out.push_back(tree_.GetItem(index));
		return out;
	}

	// Bool index
	std::unordered_map<I, std::deque<bool>, Hasher> itemBoolIndex_;

	// Tree
	Node<I> tree_;
};