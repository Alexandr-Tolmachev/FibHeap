#include <memory>

using std::shared_ptr;

const int SIZE = 60;
template <typename Key>

class FibbonachiHeap {
private:
	class Node;

public:
	class Pointer {
	public:
		friend class FibbonachiHeap<Key>;
		Pointer() : node(nullptr) {}
		~Pointer() {
			node.reset();
		}
		Key value() {
			return node->key;
		}

	private:
		shared_ptr<Node> node;
		Pointer(shared_ptr<Node> _node) : node(_node) {}

	};

	FibbonachiHeap() : heap(nullptr), real_size(0) {}
	FibbonachiHeap(shared_ptr<Node> node) {
		heap = node;
		if (node != nullptr) {
			auto cur = node;
			while (cur->right != node) {
				cur->parent.reset();
				cur->mark = false;
				cur = cur->right;
			}
		}
	}

	~FibbonachiHeap() {
		if (real_size == 0)
			return;
		shared_ptr<Node> curNode = heap;
		destruct(curNode);
		heap.reset();
		curNode.reset();
	}

	bool is_empty() const {
		return heap == nullptr;
	}

	Key get_min() const {
		if (is_empty()) {
			throw std::logic_error("There is no minimum in an empty heap");
		}
		return heap->key;
	}

	int size() {
		return real_size;
	}

	Pointer insert(Key value) {
		Node* new_node = new Node(value);
		shared_ptr<Node> cur(new_node);
		cur->left = cur;
		cur->right = cur;
		if (is_empty()) {
			heap = cur;
			real_size++;
			Pointer ptr(cur);
			return ptr;
		}
		FibbonachiHeap<Key> otherHeap(cur);
		merge(otherHeap);
		++real_size;
		if (cur->key < heap->key) {
			heap = cur;
		}
		Pointer ptr(cur);
		return ptr;
	}

	Key extract_min() {
		if (is_empty()) {
			throw std::logic_error("There is no minimum in the empty heap");
		}
		auto min = heap;
		if (heap->child != nullptr) {
			FibbonachiHeap<Key> otherHeap(heap->child);
			merge(otherHeap);
			heap->child.reset();
		}

		if (heap->left == heap) {
			heap.reset();
			return min->key;
		}

		heap->left->right = heap->right;
		heap->right->left = heap->left;
		heap = heap->right;
		min->left.reset();
		min->right.reset();
		min->deleted = true;
		Key value = min->key;

		--real_size;
		consolidate();
	
		return value;
	}

	void decrease_key(Pointer ptr, Key key) {
		auto node = ptr.node;
		if (ptr.node == nullptr) {
			throw std::invalid_argument("Your ManipulationPointer is empty");
		}

		if (ptr.node->deleted) {
			throw std::invalid_argument("This element has already been deleted");
		}

		if (key > node->key) {
			throw std::invalid_argument("You can not decrease the key");
		}

		if (node->parent == nullptr) {
			node->key = key;

			if (key < heap->key) {
				heap = node;
			}
			return;
		}

		if (node->parent->key < key) {
			node->key = key;
			return;
		}

		node->key = key;
		auto parent = node->parent;
		cut(node);
		cascadingCut(parent);
	}

	void merge(FibbonachiHeap<Key>& otherHeap) {
		if (heap == nullptr) {
			heap = otherHeap.heap;
			return;
		}

		if (otherHeap.heap == nullptr) {
			return;
		}

		merge_node(heap, otherHeap.heap);
		real_size += otherHeap.real_size;
		if (otherHeap.heap->key < heap->key) {
			heap = otherHeap.heap;
		}
		otherHeap.real_size = 0;
		otherHeap.heap.reset();
	}
	
	Key Value(Pointer ptr) {
		if (ptr.node == nullptr) {
			throw std::invalid_argument("Your ManipulationPointer is empty");
		}

		if (ptr.node->deleted) {
			throw std::invalid_argument("This element has already been deleted");
		}
		return ptr.node->key;
	}

private:
	int real_size = 0;

	class Node {
	private:
		Key key;
		shared_ptr<Node> parent, child, left, right;
		int degree;
		bool mark, deleted;
		Node(Key value) : key(value), parent(nullptr), child(nullptr), left(nullptr), right(nullptr),
			degree(0), mark(false), deleted(false) {}

	public:
		friend class FibbonachiHeap<Key>;
		~Node() {
			parent.reset();
			child.reset();
			left.reset();
			right.reset();
		}
	};

	shared_ptr<Node> heap;
	
	void merge_node(shared_ptr<Node> first, shared_ptr<Node> second) {
		if (first == nullptr) {
			first = second;
			return;
		}
		else if (second == nullptr) {
			return;
		}

		shared_ptr<Node> left1 = first->left;
		shared_ptr<Node> right1 = second->right;
		second->right = first;
		first->left = second;
		left1->right = right1;
		right1->left = left1;
		left1.reset();
		right1.reset();
	}

	void consolidate() {
		shared_ptr<Node> nodes[SIZE];
		while (!is_empty()) {
			shared_ptr<Node> now = nullptr;
			if (heap == heap->right) {
				now = heap;
				heap.reset();
			}
			else {
				shared_ptr<Node> toTheRight = heap->right;
				now = heap;
				heap->left->right = heap->right;
				heap->right->left = heap->left;
				heap->left = heap;
				heap->right = heap;
				heap = toTheRight;
				toTheRight.reset();
			}

			bool go = true;
			while (go) {
				if (nodes[now->degree] == nullptr) {
					nodes[now->degree] = now;
					go = false;
				}
				else {
					int deg = now->degree;
					if (now->key <= nodes[now->degree]->key) {
						now->degree++;
						merge_node(now->child, nodes[deg]);
						now->child = nodes[deg];
						nodes[deg]->parent = now;
						nodes[deg].reset();
					}
					else {
						nodes[deg]->degree++;
						merge_node(now, nodes[deg]->child);
						now->parent = nodes[deg];
						nodes[deg]->child = now;
						now = nodes[deg];
						nodes[deg].reset();
					}
				}
			}
		}
		int ind = 0;
		while (nodes[ind] == nullptr) {
			++ind;
		}
		heap = nodes[ind];
		++ind;
		while (ind < SIZE) {
			if (nodes[ind] != nullptr) {
				FibbonachiHeap<Key> otherHeap(nodes[ind]);
				merge(otherHeap);
			}
			++ind;
		}
	}

	void cut(shared_ptr<Node> node) {
		if (node->right == node) {
			node->parent->child = nullptr;
		}
		else {
			node->parent = node->right;
			node->right->left = node->left;
			node->left->right = node->right;
		}
		node->parent->degree--; 
		node->left = node;
		node->right = node;
		FibbonachiHeap<Key> otherHeap(node);
		merge(otherHeap);
	}

	void cascadingCut(shared_ptr<Node> node) {
		while (node->mark && node->parent != nullptr) {
			cut(node);
			node = node->parent;
		}
		node->mark = true;
	}

	void destruct(shared_ptr<Node> node) {
		if (node == nullptr) {
			return;
		}
		bool go = true;
		auto curNode = node;
		while (go) {
			if (curNode->parent != nullptr) {
				curNode->parent.reset();
			}
			if (curNode->right == curNode) {
				auto children = curNode->child;
				curNode->left.reset();
				curNode->right.reset();
				destruct(children);
				curNode->child.reset();
				curNode.reset();
				go = false;
			}
			else {
				auto toTheLeft = curNode->left;
				auto toTheRight = curNode->right;
				auto children = curNode->child;

				toTheLeft->right = toTheRight;
				toTheRight->left = toTheLeft;

				destruct(children);
				curNode->child.reset();
				curNode->left.reset();
				curNode->right.reset();

				curNode = toTheRight;
			}
		}
	}
};