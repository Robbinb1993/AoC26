#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using Range = pair<long long, long long>;
using RangeIt = list<Range>::iterator;

const long long INF = LONG_LONG_MAX;

list<Range> remainingRanges;

RangeIt removeLetter(RangeIt it, const long long index) {
    const auto [l, r] = *it;

    if (l == r) {
        return remainingRanges.erase(it);
    }

    if (index == l) {
        it->first++;
        return it;
    }

    if (index == r) {
        it->second--;
        return next(it);
    }

    it->second = index - 1;

    return remainingRanges.insert(next(it), {index + 1, r});
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("in.txt");

    remainingRanges.push_back({1, INF});

    string collectedLetters;

    auto santaPos = remainingRanges.begin();
    long long steps;

    const auto start = chrono::steady_clock::now();

    while (fin >> steps) {
        long long currentIndex = santaPos->first;

        if (steps > 0) {
            while (steps > 0) {
                long long lettersInRange = santaPos->second - santaPos->first + 1;
                if (lettersInRange <= steps) {
                    steps -= lettersInRange;
                    ++santaPos;

                    assert(santaPos != remainingRanges.end());
                    currentIndex = santaPos->first;
                } else {
                    currentIndex = santaPos->first + steps;
                    break;
                }
            }
        } else if (steps < 0) {
            steps = -steps;
            while (steps > 0) {
                assert(santaPos != remainingRanges.begin());
                --santaPos;

                long long lettersInRange = santaPos->second - santaPos->first + 1;
                if (steps <= lettersInRange) {
                    currentIndex = santaPos->second - steps + 1;
                    break;
                } else {
                    steps -= lettersInRange;
                }
            }
        }

        collectedLetters.push_back('A' + (currentIndex - 1) % 26);

        santaPos = removeLetter(santaPos, currentIndex);
    }

    const auto end = chrono::steady_clock::now();

    const double elapsedMs = chrono::duration<double, milli>(end - start).count();

    cout << collectedLetters << '\n';
    cout << "Execution time: " << elapsedMs << " ms\n";

    return 0;
}
