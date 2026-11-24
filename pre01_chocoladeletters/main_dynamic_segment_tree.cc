
#include <bits/stdc++.h>
using namespace std;

const long long INF = LLONG_MAX;

class DynamicSegmentTree {
private:
    struct Node {
        long long count_;
        Node *left = nullptr;
        Node *right = nullptr;

        explicit Node(const long long count) : count_(count) {}
    };

    long long maxIndex_;
    Node *root;

    Node *getOrCreate(Node *&child, const long long l, const long long r) {
        if (!child)
            child = new Node(r - l + 1);

        return child;
    }

    long long findKth(Node *node, const long long l, const long long r, long long k) {
        if (l == r) {
            return l;
        }

        const long long mid = l + (r - l) / 2;

        const long long leftCount = node->left ? node->left->count_ : mid - l + 1;

        if (k <= leftCount) {
            if (!node->left)
                return l + k - 1;

            return findKth(node->left, l, mid, k);
        } else {
            k -= leftCount;

            if (!node->right)
                return mid + k;

            return findKth(node->right, mid + 1, r, k);
        }
    }

    void remove(Node *node, const long long l, const long long r, const long long idx) {
        if (l == r) {
            node->count_ = 0;
            return;
        }

        const long long mid = l + (r - l) / 2;

        if (idx <= mid) {
            Node *left = getOrCreate(node->left, l, mid);
            remove(left, l, mid, idx);
        } else {
            Node *right = getOrCreate(node->right, mid + 1, r);
            remove(right, mid + 1, r, idx);
        }

        node->count_--;
    }

    void destroy(Node *node) {
        if (!node)
            return;

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    explicit DynamicSegmentTree(const long long maxIndex)
        : maxIndex_(maxIndex), root(new Node(maxIndex)) {}

    ~DynamicSegmentTree() { destroy(root); }

    long long findKth(const long long k) { return findKth(root, 1, maxIndex_, k); }

    void remove(const long long idx) { remove(root, 1, maxIndex_, idx); }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("in.txt");

    string collectedLetters;

    long long santaRank = 1;
    long long steps;

    DynamicSegmentTree tree(INF);

    const auto start = chrono::steady_clock::now();

    while (fin >> steps) {
        santaRank += steps;

        long long originalIdx = tree.findKth(santaRank);

        collectedLetters.push_back((originalIdx - 1) % 26 + 'A');

        tree.remove(originalIdx);
    }

    const auto end = chrono::steady_clock::now();

    const double elapsedMs = chrono::duration<double, milli>(end - start).count();

    cout << collectedLetters << '\n';
    cerr << "Execution time: " << elapsedMs << " ms\n";

    return 0;
}
